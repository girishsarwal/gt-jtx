package com.gluedtomatoes.artrix;
import android.app.Notification;
import android.graphics.Point;
import android.location.SettingInjectorService;
import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class Sprite extends DrawableEntity implements Renderable {

    private static final int COORDS_PER_VERTEX = 3;
    private static final int SIZE_OF_FLOAT = 4;
    private static final int SIZE_OF_SHORT = 2;
    private static float vertices[]={
            -0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
    };

    private static float texcoords[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };

    private static short indices[] = {
            0, 1, 2,
            0, 2, 3,
    };

    @Override
    public void init() {
        ByteBuffer bbv = ByteBuffer.allocateDirect(vertices.length * SIZE_OF_FLOAT);
        bbv.order(ByteOrder.nativeOrder());
        vertexBuffer = bbv.asFloatBuffer();
        vertexBuffer.put(vertices);
        vertexBuffer.position(0);

        ByteBuffer bbi = ByteBuffer.allocateDirect(indices.length * SIZE_OF_SHORT);
        bbi.order(ByteOrder.nativeOrder());
        indexBuffer = bbi.asShortBuffer();
        indexBuffer.put(indices);
        indexBuffer.position(0);
        super.init();
    }
}
