package com.gluedtomatoes.artrix;

import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class Sprite extends DrawableEntity  {

    private static float vertices[]={
            -0.95f, 0.95f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.95f, -0.95f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.95f, -0.95f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.95f, 0.95f, 0.0f, 1.0f, 1.0f, 1.0f,
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


        mVertexCount = vertexBuffer.remaining();
        mIndexCount = indexBuffer.remaining();
        mTriangleCount = mIndexCount/3;

        mShadingProgram.use();
        mVertexDescriptor =  mShadingProgram.createSuitableVertexFormat();

        /** generate the buffers **/
        GLES20.glGenBuffers(2, buffers, 0);

        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, buffers[0]);
        GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, mVertexCount * VertexDescriptor.SIZE_OF_FLOAT, vertexBuffer, GLES20.GL_STATIC_DRAW);

        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
        GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, mIndexCount * VertexDescriptor.SIZE_OF_SHORT, indexBuffer, GLES20.GL_STATIC_DRAW );

        super.init();
    }

    @Override
    public void render() {
        mShadingProgram.enableVertexAttributes();

        GLES20.glVertexAttribPointer(mShadingProgram.getAttributeLocation("inPosition"), 3,
                GLES20.GL_FLOAT, false,
                mVertexDescriptor.getStride(), 0);

        GLES20.glVertexAttribPointer(mShadingProgram.getAttributeLocation("inColor"), 3,
                GLES20.GL_FLOAT, false,
                mVertexDescriptor.getStride(), 12);

        //GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, mVertexCount);
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, mIndexCount, GLES20.GL_UNSIGNED_SHORT, 0);
        super.render();
        mShadingProgram.disableVertexAttributes();
    }
}
