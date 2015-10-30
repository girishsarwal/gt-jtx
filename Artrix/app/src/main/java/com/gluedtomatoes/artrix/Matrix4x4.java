package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 10/26/2015.
 */
public class Matrix4x4 {
    private float
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44;


    public  Matrix4x4(){
        _11 = _22 = _33 = _44 = 1.0f;
        _12 = _13 = _14 = 0.0f;
        _21 = _23 = _24 = 0.0f;
        _31 = _32 = _34 = 0.0f;
        _41 = _42 = _43 = 0.0f;
    }

    public void identity(Matrix4x4 matrix){
        matrix._11 = matrix._22 = matrix._33 = matrix._44 = 1.0f;
        matrix._12 = matrix._13 = matrix._14 = 0.0f;
        matrix._21 = matrix._23 = matrix._24 = 0.0f;
        matrix._31 = matrix._32 = matrix._34 = 0.0f;
        matrix._41 = matrix._42 = matrix._43 = 0.0f;
    }
}
