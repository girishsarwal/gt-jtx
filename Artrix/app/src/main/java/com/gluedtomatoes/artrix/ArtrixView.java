package com.gluedtomatoes.artrix;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class ArtrixView extends GLSurfaceView{
    private ArtrixRenderer mRenderer;
    public ArtrixView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        mRenderer = new ArtrixRenderer();
        setRenderer(mRenderer);
    }
}
