package com.gluedtomatoes.artrix;

import android.opengl.GLES20;

import java.util.HashMap;

/**
 * Created by gsarwal on 7/2/2015.
 */
public class VertexDescriptor {


    public static final int POSITION = 0x0001;
    public static final int POSITION_H = 0x0002;
    public static final int COLOR_RGB = 0x0004;
    public static final int COLOR_RGBA = 0x0008;
    public static final int NORMAL_XYZ = 0x0010;
    public static final int TANGENT = 0x0020;
    public static final int INDEX_SHORT = 0x0040;
    public static final int TEX0 = 0x0080;
    public static final int TEX1 = 0x0100;
    public static final int TEX2 = 0x0200;
    public static final int TEX3 = 0x0400;


    public static final int SIZE_OF_FLOAT = 4;
    public static final int SIZE_OF_SHORT = 2;

    private HashMap<String, VertexAttribute> mVertexAttributes;

    private int mFormat;
    private int mStride;

    public int getStride(){
        return mStride;
    }
    public int getFormat(){return mFormat;}

    public void setFormat(int format){
        mFormat = format;
        calculateStride();
    }

    public void calculateStride(){
        mStride = 0;
        addStrideElement(POSITION, "inPosition");
        addStrideElement(POSITION_H, "inPositionH");
        addStrideElement(COLOR_RGB, "inColor");
        addStrideElement(COLOR_RGBA, "inColorA");
        addStrideElement(NORMAL_XYZ, "inNormal");
        addStrideElement(TEX0, "inTex0");
        addStrideElement(TEX1, "inTex1");
        addStrideElement(TEX2, "inTex2");
        addStrideElement(TEX3, "inTex3");
    }

    private void addStrideElement(int semantic, String shaderElement){
        if((mFormat & semantic) == semantic) {
            VertexAttribute attr =mVertexAttributes.get(shaderElement);
            if(attr != null) {
                mStride += attr.mElementSize * SIZE_OF_FLOAT;
            }
        }
    }

    public void addVertexAttribute(String name, VertexAttribute vattr){
        mVertexAttributes.put(name, vattr);
    }

    public VertexDescriptor(int format) {
        this();
        mFormat = format;
        init();
    }

    public VertexDescriptor(){
        mVertexAttributes = new HashMap<>();
    }


    void init(){
        calculateStride();
    }

    public VertexAttribute getVertexAttribute(String value){
        return mVertexAttributes.get(value);
    }



}
