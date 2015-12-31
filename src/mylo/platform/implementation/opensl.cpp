/*
================================================================================

Copyright (c) 2012, Dee E. Abbott
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
* Neither the name of the organization nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================
*/

#include "opensl.hpp"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

// engine interfaces
static SLObjectItf engineObject = NULL;
static SLEngineItf engineEngine;

// output mix interfaces
static SLObjectItf outputMixObject = NULL;
static SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;

// buffer queue player interfaces
static SLObjectItf bqPlayerObject = NULL;
static SLPlayItf bqPlayerPlay;
static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
static SLEffectSendItf bqPlayerEffectSend;
static SLMuteSoloItf bqPlayerMuteSolo;
static SLVolumeItf bqPlayerVolume;

// aux effect on the output mix, used by the buffer queue player
static const SLEnvironmentalReverbSettings reverbSettings =
    SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

// URI player interfaces
static SLObjectItf uriPlayerObject = NULL;
static SLPlayItf uriPlayerPlay;
static SLSeekItf uriPlayerSeek;
static SLMuteSoloItf uriPlayerMuteSolo;
static SLVolumeItf uriPlayerVolume;

// file descriptor player interfaces
static SLObjectItf fdPlayerObject = NULL;
static SLPlayItf fdPlayerPlay;
static SLSeekItf fdPlayerSeek;
static SLMuteSoloItf fdPlayerMuteSolo;
static SLVolumeItf fdPlayerVolume;

// recorder interfaces
static SLObjectItf recorderObject = NULL;
static SLRecordItf recorderRecord;
static SLAndroidSimpleBufferQueueItf recorderBufferQueue;

// synthesized sawtooth clip
#define SAWTOOTH_FRAMES 8000
static short sawtoothBuffer[SAWTOOTH_FRAMES];

// 5 seconds of recorded audio at 16 kHz mono, 16-bit signed little endian
#define RECORDER_FRAMES (16000 * 5)
static short recorderBuffer[RECORDER_FRAMES];
static unsigned recorderSize = 0;
static SLmilliHertz recorderSR;

// pointer and size of the next player buffer to enqueue, and number of remaining buffers
static short *nextBuffer;
static unsigned nextSize;
static int nextCount;


// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) { DEBUG_SCOPE;
	DEBUG_TRACE << "player callback" << my::endl;
    //assert(bq == bqPlayerBufferQueue);
    //assert(NULL == context);
    // for streaming playback, replace this test by logic to find and fill the next buffer
    if (--nextCount > 0 && NULL != nextBuffer && 0 != nextSize) {
        SLresult result;
        // enqueue another buffer
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        //assert(SL_RESULT_SUCCESS == result);
    }
}


// this callback handler is called every time a buffer finishes recording
void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context) { DEBUG_SCOPE;
	DEBUG_TRACE << "recorder callback" << my::endl;
    //assert(bq == bqRecorderBufferQueue);
    //assert(NULL == context);
    // for streaming recording, here we would call Enqueue to give recorder the next buffer to fill
    // but instead, this is a one-time buffer so we stop recording
    SLresult result;
    result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_STOPPED);
    if (SL_RESULT_SUCCESS == result) {
        recorderSize = RECORDER_FRAMES * sizeof(short);
        recorderSR = SL_SAMPLINGRATE_16;
    }
}

__PLATFORM_NAMESPACE_BEGIN

void opensl::audio::init(int players) { DEBUG_SCOPE;
	DEBUG_TRACE << "audio init" << my::endl;
	SLresult result;

	// create engine
	DEBUG_TRACE << "create engine" << my::endl;
	result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}
	
	//assert(SL_RESULT_SUCCESS == result);

	// realize the engine
	DEBUG_TRACE << "realize engine" << my::endl;
	result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

	// get the engine interface, which is needed in order to create other objects
	DEBUG_TRACE << "get engine interface" << my::endl;
	result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

	// create output mix, with environmental reverb specified as a non-required interface
	DEBUG_TRACE << "create mixer" << my::endl;
	const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
	const SLboolean req[1] = {SL_BOOLEAN_FALSE};
	result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

	// realize the output mix
	DEBUG_TRACE << "realize mixer" << my::endl;
	result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

	// get the environmental reverb interface
	// this could fail if the environmental reverb effect is not available,
	// either because the feature is not present, excessive CPU load, or
	// the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
	DEBUG_TRACE << "get reverb interface" << my::endl;
	result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
			&outputMixEnvironmentalReverb);
	if (SL_RESULT_SUCCESS == result) {
		result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
				outputMixEnvironmentalReverb, &reverbSettings);
	}
	else {
		DEBUG_TRACE << "failed" << my::endl;
	}
	// ignore unsuccessful result codes for environmental reverb, as it is optional for this example
}

void opensl::audio::compile(my::audio &sound) { DEBUG_SCOPE;
	DEBUG_TRACE << "audio compile" << my::endl;
	SLresult result;
	
    //SLresult result;

    // configure audio source
	DEBUG_TRACE << "buffer queue" << my::endl;
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm;
	format_pcm.formatType = SL_DATAFORMAT_PCM;
	//format_pcm.numChannels = 1;
	//format_pcm.samplesPerSec = SL_SAMPLINGRATE_8;
	//format_pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_8;
	//format_pcm.containerSize = SL_PCMSAMPLEFORMAT_FIXED_8;
	format_pcm.numChannels = sound.header.channels;
	format_pcm.samplesPerSec = sound.header.sample_rate * 1000;
	format_pcm.bitsPerSample = sound.header.bits_per_sample;
	format_pcm.containerSize = sound.header.bits_per_sample;
	
	format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
	format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
	DEBUG_TRACE << "create player" << my::endl;
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 3, ids, req);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

    // realize the player
	DEBUG_TRACE << "realize player" << my::endl;
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

    // get the play interface
	DEBUG_TRACE << "get player interface" << my::endl;
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

    // get the buffer queue interface
	DEBUG_TRACE << "get buffer queue interface" << my::endl;
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

    // register callback on the buffer queue
	DEBUG_TRACE << "register callback" << my::endl;
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

    // get the effect send interface
	DEBUG_TRACE << "get effect interface" << my::endl;
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_EFFECTSEND, &bqPlayerEffectSend);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}

#if 0   
    // mute/solo is not supported for sources that are known to be mono, as this is
    // get the mute/solo interface
	DEBUG_TRACE << "get mono/solo interface" << my::endl;
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_MUTESOLO, &bqPlayerMuteSolo);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}
#endif

    // get the volume interface
	DEBUG_TRACE << "get volume interface" << my::endl;
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}
	
    int header = sizeof(sound.header);
	
	DEBUG_TRACE << "buffer: " << sound.size - header << " bytes, " << sound.size << " bytes total" << my::endl;
	DEBUG_TRACE << "channels: " << sound.header.channels << my::endl;
	DEBUG_TRACE << "bits_per_sample: " << sound.header.bits_per_sample << my::endl;
	DEBUG_TRACE << "sample_rate: " << sound.header.sample_rate << my::endl;
	
	result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, reinterpret_cast<short *>(&sound.data + header), sound.size - header);
	//result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, reinterpret_cast<short *>(*sound.data + header), sound.size - header);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}
	
}

void opensl::audio::play(my::audio &sound) { DEBUG_SCOPE;
    SLresult result;
	
	DEBUG_TRACE << "audio play" << my::endl;
    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
	if(result != SL_RESULT_SUCCESS) {
		DEBUG_TRACE << "failed" << my::endl;
	}
    //assert(SL_RESULT_SUCCESS == result);
}

void opensl::audio::shutdown(void) { DEBUG_SCOPE;
	DEBUG_TRACE << "audio shutdown" << my::endl;

    // destroy buffer queue audio player object, and invalidate all associated interfaces
    if (bqPlayerObject != NULL) {
        (*bqPlayerObject)->Destroy(bqPlayerObject);
        bqPlayerObject = NULL;
        bqPlayerPlay = NULL;
        bqPlayerBufferQueue = NULL;
        bqPlayerEffectSend = NULL;
        bqPlayerMuteSolo = NULL;
        bqPlayerVolume = NULL;
    }

    // destroy file descriptor audio player object, and invalidate all associated interfaces
    if (fdPlayerObject != NULL) {
        (*fdPlayerObject)->Destroy(fdPlayerObject);
        fdPlayerObject = NULL;
        fdPlayerPlay = NULL;
        fdPlayerSeek = NULL;
        fdPlayerMuteSolo = NULL;
        fdPlayerVolume = NULL;
    }

    // destroy URI audio player object, and invalidate all associated interfaces
    if (uriPlayerObject != NULL) {
        (*uriPlayerObject)->Destroy(uriPlayerObject);
        uriPlayerObject = NULL;
        uriPlayerPlay = NULL;
        uriPlayerSeek = NULL;
        uriPlayerMuteSolo = NULL;
        uriPlayerVolume = NULL;
    }

    // destroy audio recorder object, and invalidate all associated interfaces
    if (recorderObject != NULL) {
        (*recorderObject)->Destroy(recorderObject);
        recorderObject = NULL;
        recorderRecord = NULL;
        recorderBufferQueue = NULL;
    }

    // destroy output mix object, and invalidate all associated interfaces
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
        outputMixEnvironmentalReverb = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }

}

/*

void engine_init () {
// create OpenSL ES engine
  SLEngineOption EngineOption[] = {(SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE};
  const SLInterfaceID lEngineMixIIDs[] = {SL_IID_ENGINE};
  const SLboolean lEngineMixReqs[] = {SL_BOOLEAN_TRUE};
  SLresult res = slCreateEngine(&mEngineObj, 1, EngineOption, 1, lEngineMixIIDs, lEngineMixReqs);
  res = (*mEngineObj)->Realize(mEngineObj, SL_BOOLEAN_FALSE);
  res = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE, &mEngine);   // get 'engine' interface
  // create output mix (AKA playback; this represents speakers, headset etc.)
  res = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj, 0,NULL, NULL);
  res = (*mOutputMixObj)->Realize(mOutputMixObj, SL_BOOLEAN_FALSE);
}

void player_init() {
SLresult lRes;
  // Set-up sound audio source.
  SLDataLocator_AndroidSimpleBufferQueue lDataLocatorIn;
  lDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
  lDataLocatorIn.numBuffers = 1;   // 1 buffer for a one-time load
  // analyze and set correct PCM format
  SLDataFormat_PCM lDataFormat;
  lDataFormat.formatType = SL_DATAFORMAT_PCM;

  lDataFormat.numChannels = audio->wav.channels;     // etc. 1,2
  lDataFormat.samplesPerSec = audio->wav.sampleRate * 1000;   // etc. 44100 * 1000
  lDataFormat.bitsPerSample = audio->wav.bitsPerSample;   // etc. 16
  lDataFormat.containerSize = audio->wav.bitsPerSample;
  lDataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
  lDataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

  SLDataSource lDataSource;
  lDataSource.pLocator = &lDataLocatorIn;
  lDataSource.pFormat = &lDataFormat;

  SLDataLocator_OutputMix lDataLocatorOut;
  lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
  lDataLocatorOut.outputMix = mOutputMixObj;

  SLDataSink lDataSink;
  lDataSink.pLocator = &lDataLocatorOut;
  lDataSink.pFormat = NULL;

  const SLInterfaceID lSoundPlayerIIDs[] = { SL_IID_PLAY, SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
  const SLboolean lSoundPlayerReqs[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
  lRes = (*mEngine)->CreateAudioPlayer(mEngine, &mPlayerObj, &lDataSource, &lDataSink, 2, lSoundPlayerIIDs, lSoundPlayerReqs);
  if (lRes != SL_RESULT_SUCCESS) { return; }
  lRes = (*mPlayerObj)->Realize(mPlayerObj, SL_BOOLEAN_FALSE);
  if (lRes != SL_RESULT_SUCCESS) { return; }
  lRes = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_PLAY, &mPlayer);
  if (lRes != SL_RESULT_SUCCESS) { return; }

  lRes = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &mPlayerQueue);
  if (lRes != SL_RESULT_SUCCESS) { return; }
  // register callback on the buffer queue
  lRes = (*mPlayerQueue)->RegisterCallback(mPlayerQueue, bqPlayerQueueCallback, NULL);
  if (lRes != SL_RESULT_SUCCESS) { return; }
  lRes = (*mPlayer)->SetCallbackEventsMask(mPlayer, SL_PLAYEVENT_HEADATEND);
  if (lRes != SL_RESULT_SUCCESS) { return; }

  // ..fetch the data in 'audio->data' from opened FILE* stream and set 'datasize'

  // feed the buffer with data
  lRes = (*mPlayerQueue)->Clear(mPlayerQueue);   // remove any sound from buffer
  lRes = (*mPlayerQueue)->Enqueue(mPlayerQueue, audio->data, datasize);}

}

*/

__PLATFORM_NAMESPACE_END

// Local Variables:
// mode:C++
// End:

