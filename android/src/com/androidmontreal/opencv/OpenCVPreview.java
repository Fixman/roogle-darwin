package com.androidmontreal.opencv;

import com.androidmontreal.arduino.bluetooth.RoogleTankApp;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.Toast;

public class OpenCVPreview extends CameraPreviewViewBase {
	Context mContext;
	private static final String TAG = "OpenCVforHackathons";
	
	public OpenCVPreview(Context context, AttributeSet attrs) {
		super(context, attrs);
		mContext = context;
	}

	@Override
	protected Bitmap processFrame(byte[] data) {
		int frameSize = getFrameWidth() * getFrameHeight();
		int[] rgba = new int[frameSize];

		String resultsForThisFrame = processimage(getFrameWidth(), getFrameHeight(), data, rgba);
		Log.d(TAG, "Got a string back from the cpp: "+resultsForThisFrame);
		((RoogleTankApp) mContext.getApplicationContext()).setLastMessage(resultsForThisFrame); 
		
		Bitmap bmp = Bitmap.createBitmap(getFrameWidth(), getFrameHeight(),
				Bitmap.Config.ARGB_8888);
		bmp.setPixels(rgba, 0/* offset */, getFrameWidth() /* stride */, 0, 0,
				getFrameWidth(), getFrameHeight());
		return bmp;
	}
 
	/*
     * This is the function which will call the main image processing
     * 
     */
    public native String processimage(int width, int height, byte yuv[], int[] rgba);

	static {
		System.loadLibrary("opencv_sample");
	}
}
