package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 7/2/2015.
 */
public class VertexDescriptor {
    public static final int POSITION_XYZ = 0x0001;
    public static final int POSITION_XYZW = 0x0002;
    public static final int COLOR_RGB = 0x0004;
    public static final int COLOR_RGBA = 0x0008;
    public static final int NORMAL_XYZ = 0x0010;
    public static final int TANGENT = 0x0020;
    public static final int INDEX_SHORT = 0x0040;
    public static final int UV1 = 0x0080;
    public static final int UV2 = 0x0100;


    public static final int SIZE_OF_FLOAT = 4;
    public static final int SIZE_OF_SHORT = 2;


    public static final VertexDescriptor VF_POSITION_COLOR= new VertexDescriptor(POSITION_XYZW | COLOR_RGBA);
    public static final VertexDescriptor VF_POSITION_COLOR_UV1 = new VertexDescriptor(POSITION_XYZW | COLOR_RGBA | UV1);
    public static final VertexDescriptor VF_POSITION_COLOR_UV1_UV2 = new VertexDescriptor(POSITION_XYZW | COLOR_RGBA | UV1 | UV2);

    private int mFormat;
    private int mStride;

    public int getStride(){
        return mStride;
    }
    public int getFormat(){return mFormat;}
    public void setFormat(int format){
        mFormat = format;
        mStride = 0;
        if((mFormat & POSITION_XYZ) == POSITION_XYZ) mStride += 3 * SIZE_OF_FLOAT;
        if((mFormat & POSITION_XYZW) == POSITION_XYZW) mStride += 4 * SIZE_OF_FLOAT;
        if((mFormat & COLOR_RGB) == COLOR_RGB) mStride += 3 * SIZE_OF_FLOAT;
        if((mFormat & COLOR_RGBA) == COLOR_RGBA) mStride += 4 * SIZE_OF_FLOAT;
        if((mFormat & NORMAL_XYZ) == NORMAL_XYZ) mStride += 3 * SIZE_OF_FLOAT;
        if((mFormat & UV1) == UV1) mStride += 2 * SIZE_OF_FLOAT;
        if((mFormat & UV2) == UV2) mStride += 2 * SIZE_OF_FLOAT;
    }

    public VertexDescriptor(int format) {
        setFormat(POSITION_XYZ | COLOR_RGB);
    }
}
