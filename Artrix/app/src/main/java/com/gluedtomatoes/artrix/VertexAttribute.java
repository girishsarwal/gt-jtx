package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 10/30/2015.
 */
public class VertexAttribute {

    public int mElementSize;
    public int mType;
    public boolean mNormalized;
    public int mOffsetInStream;

    public VertexAttribute(int mElementSize, int mType, boolean mNormalized, int mOffsetInStream) {
        this.mElementSize = mElementSize;
        this.mType = mType;
        this.mNormalized = mNormalized;
        this.mOffsetInStream = mOffsetInStream;
    }

}
