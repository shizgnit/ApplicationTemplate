package com.android.ApplicationTemplate;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;

import android.util.AttributeSet;

import android.view.GestureDetector;
import android.view.MotionEvent;

import android.view.ScaleGestureDetector;


public class ApplicationTemplateActivity extends Activity {
	private GLSurfaceView glSurfaceView;
	private boolean rendererSet;
	
    private ScaleGestureDetector mScaleGestureDetector;
    private GestureDetector mGestureDetector;
	
	public ApplicationTemplateInterop renderer;
	
    /**
     * The scale listener, used for handling multi-finger scale gestures.
     */
    private final ScaleGestureDetector.OnScaleGestureListener mScaleGestureListener
            = new ScaleGestureDetector.SimpleOnScaleGestureListener() {
        /**
         * This is the active focal point in terms of the viewport. Could be a local
         * variable but kept here to minimize per-frame allocations.
         */
        //private PointF viewportFocus = new PointF();
        private float lastSpanX;
        private float lastSpanY;

   	    //final ApplicationTemplateInterop rendererWrapper = new ApplicationTemplateInterop(this);
		
        @Override
        public boolean onScaleBegin(ScaleGestureDetector scaleGestureDetector) {
            lastSpanX = scaleGestureDetector.getCurrentSpanX();//ScaleGestureDetectorCompat.getCurrentSpanX(scaleGestureDetector);
            lastSpanY = scaleGestureDetector.getCurrentSpanY();//ScaleGestureDetectorCompat.getCurrentSpanY(scaleGestureDetector);
            return true;
        }

        @Override
        public boolean onScale(ScaleGestureDetector scaleGestureDetector) {
            float spanX = scaleGestureDetector.getCurrentSpanX();//ScaleGestureDetectorCompat.getCurrentSpanX(scaleGestureDetector);
            float spanY = scaleGestureDetector.getCurrentSpanY();//ScaleGestureDetectorCompat.getCurrentSpanY(scaleGestureDetector);

            //float newWidth = lastSpanX / spanX * mCurrentViewport.width();
            //float newHeight = lastSpanY / spanY * mCurrentViewport.height();

            //float focusX = scaleGestureDetector.getFocusX();
            //float focusY = scaleGestureDetector.getFocusY();
            //hitTest(focusX, focusY, viewportFocus);
/*
            mCurrentViewport.set(
                    viewportFocus.x
                            - newWidth * (focusX - mContentRect.left)
                            / mContentRect.width(),
                    viewportFocus.y
                            - newHeight * (mContentRect.bottom - focusY)
                            / mContentRect.height(),
                    0,
                    0);
            mCurrentViewport.right = mCurrentViewport.left + newWidth;
            mCurrentViewport.bottom = mCurrentViewport.top + newHeight;
            constrainViewport();
            ViewCompat.postInvalidateOnAnimation(InteractiveLineGraphView.this);
*/
			ApplicationTemplateInterop.getInstance().onTouchScale(0.0f, 0.0f, lastSpanX - spanX);

            lastSpanX = spanX;
            lastSpanY = spanY;
            return true;
        }
    };
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		ActivityManager activityManager
			= (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

		final boolean supportsEs2 =
			configurationInfo.reqGlEsVersion >= 0x20000 || isProbablyEmulator();

		//if (supportsEs2) {
			glSurfaceView = new GLSurfaceView(this);

			if (isProbablyEmulator()) {
				// Avoids crashes on startup with some emulator images.
				glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
			}

			//final ApplicationTemplateInterop rendererWrapper = new ApplicationTemplateInterop(this);
			glSurfaceView.setEGLContextClientVersion(2);
			
			ApplicationTemplateInterop.createInstance(this);
			
			glSurfaceView.setRenderer(ApplicationTemplateInterop.getInstance());
			rendererSet = true;
			setContentView(glSurfaceView);

			glSurfaceView.setOnTouchListener(new OnTouchListener() {
	            @Override
	            public boolean onTouch(View v, MotionEvent event) {
	                if(event == null) {
						return false;
					}
					final int action = event.getActionMasked();
					// Get the index of the pointer associated with the action.
					final int index = event.getActionIndex();
					
					int xPos = -1;
					int yPos = -1;
					
					final float normalizedX = ((event.getX() / (float) v.getWidth()) * 2 - 1);
					final float normalizedY = ((event.getY() / (float) v.getHeight()) * 2 - 1) * -1;

					if (event.getAction() == MotionEvent.ACTION_DOWN) {
						glSurfaceView.queueEvent(new Runnable() {
							@Override
							public void run() {
								ApplicationTemplateInterop.getInstance().onTouchPress(normalizedX, normalizedY);
							}
						});
					} 
					if (event.getAction() == MotionEvent.ACTION_MOVE) {
						glSurfaceView.queueEvent(new Runnable() {
							@Override
							public void run() {
								ApplicationTemplateInterop.getInstance().onTouchDrag(normalizedX, normalizedY);
							}
						});
					}

					return true;
	            }
	        });
			
            mScaleGestureDetector = new ScaleGestureDetector(this, mScaleGestureListener);
	
			
		//} else {
			// Should never be seen in production, since the manifest filters
			// unsupported devices.
		//	Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
		//			Toast.LENGTH_LONG).show();
		//	return;
		//}
	}

	private boolean isProbablyEmulator() {
//		return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
//				&& (Build.FINGERPRINT.startsWith("generic")
//						|| Build.FINGERPRINT.startsWith("unknown")
//						|| Build.MODEL.contains("google_sdk")
//						|| Build.MODEL.contains("Emulator")
//						|| Build.MODEL.contains("Android SDK built for x86"));
    return(false);
	}

	@Override
	protected void onPause() {
		super.onPause();

		if (rendererSet) {
			glSurfaceView.onPause();
		}
	}

	@Override
	protected void onResume() {
		super.onResume();

		if (rendererSet) {
			glSurfaceView.onResume();
		}
	}
	
}
