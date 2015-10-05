set curr_dir=%cd%
cd jni & ndk-build & cd %curr_dir% & copy /y obj\local\armeabi\libApplicationTemplate.so libs\armeabi\ & ant debug & adb install -r bin/ApplicationTemplate-debug.apk