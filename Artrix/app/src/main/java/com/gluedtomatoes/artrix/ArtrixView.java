package com.gluedtomatoes.artrix;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class ArtrixView extends GLSurfaceView implements GLSurfaceView.Renderer{
    public static RenderQueue RenderQueue;
    private StateMachine mGame;
    private Context mContext;
    public ArtrixView(Context context) {
        super(context);
        mContext = context;
        setEGLContextClientVersion(2);
        RenderQueue = new RenderQueue();
        setRenderer(this);
    }

    public void setMachine(StateMachine sm){
        mGame = sm;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        ShaderProgram.init(mContext);
        ((MainActivity)mContext).init();
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        mGame.update();
        RenderQueue.Process();
    }
}
