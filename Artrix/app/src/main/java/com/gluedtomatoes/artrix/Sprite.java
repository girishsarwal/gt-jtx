package com.gluedtomatoes.artrix;

import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class Sprite extends DrawableEntity  {

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

        ByteBuffer bbv = ByteBuffer.allocateDirect(vertices.length * VertexDescriptor.SIZE_OF_FLOAT);
        bbv.order(ByteOrder.nativeOrder());
        vertexBuffer = bbv.asFloatBuffer();
        vertexBuffer.put(vertices);
        vertexBuffer.position(0);

        ByteBuffer bbi = ByteBuffer.allocateDirect(indices.length * VertexDescriptor.SIZE_OF_SHORT);
        bbi.order(ByteOrder.nativeOrder());
        indexBuffer = bbi.asShortBuffer();
        indexBuffer.put(indices);
        indexBuffer.position(0);

        super.init();
    }

    @Override
    public void render() {
        super.render();
    }
}
