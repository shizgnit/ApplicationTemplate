package com.android.ApplicationTemplate;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView.Renderer;

import android.util.Log;

import java.io.IOException;

public class ApplicationTemplateInterop implements Renderer {
	static {
		System.loadLibrary("ApplicationTemplate");
	}

	private final Context context;

	public ApplicationTemplateInterop(Context context) {
		this.context = context;
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		/*
		try {
			Log.i("ApplicationTemplate", "attempting to list assets");
			String[] fileNames = context.getAssets().list("");
			for(String name:fileNames) {
			   Log.i("ApplicationTemplate", name);
			}
		} catch(IOException e) {
			Log.i("ApplicationTemplate", "failed to get asset list");
		}
		*/

		on_startup(context.getAssets());
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		on_resize(width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		on_draw();
	}

	public void handleTouchPress(float normalizedX, float normalizedY) {
		on_touch_press(normalizedX, normalizedY);
	}

	public void handleTouchDrag(float normalizedX, float normalizedY) {
		on_touch_drag(normalizedX, normalizedY);
	}

	private static native void on_startup(AssetManager asset_manager);

	private static native void on_resize(int width, int height);

	private static native void on_draw();

	private static native void on_touch_press(float normalized_x, float normalized_y);

	private static native void on_touch_drag(float normalized_x, float normalized_y);
}
