package com.gluedtomatoes.artrix;

import android.graphics.Point;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class Vector3 {
    public float mX;
    public float mY;
    public float mZ;

    public Vector3(float mX, float mY, float mZ) {
        this.mX = mX;
        this.mY = mY;
        this.mZ = mZ;
    }

    public Vector3() {
        this.mX = 0.0f;
        this.mY = 0.0f;
        this.mZ = 0.0f;
    }
}


