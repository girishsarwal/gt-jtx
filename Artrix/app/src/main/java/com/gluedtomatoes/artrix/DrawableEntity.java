package com.gluedtomatoes.artrix;

import android.graphics.Point;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

/**
 * Created by gsarwal on 5/18/2015.
 */
public class DrawableEntity extends Entity implements Renderable{
    protected FloatBuffer vertexBuffer;
    protected FloatBuffer textureBuffer;
    protected ShortBuffer indexBuffer;

    private boolean mVisible;
    private Vector2 mPosition;
    private Vector2 mSize;
    private float mRotation;


    public float getRotation() {
        return mRotation;
    }
    public void setRotation(float mRotation) {
        this.mRotation = mRotation;
    }

    public Vector2 getPosition() {
        return mPosition;
    }
    public void setPosition(Vector2 mPosition) {
        this.mPosition = mPosition;
    }
    public void setPosition(Point mPosition) {
        setPosition(Transform.PixelsToCoords(mPosition));
    }


    public Vector2 getSize() {
        return mSize;
    }
    public void setSize(Vector2 mSize) {
        this.mSize = mSize;
    }
    public void setSize(Point mSize) {
        setSize(Transform.PixelsToCoords(mSize));
    }


    DrawableEntity(){
        mPosition = new Vector2();
        mRotation = 0.0f;
        mSize = new Vector2();
        mVisible = true;
    }
    @Override
    public void init() {
        mIsInitialized = true;
    }

    @Override
    public void setVisible(Boolean visible) {
        mVisible = visible;
    }

    @Override
    public Boolean getVisible() {
        return null;
    }

    @Override
    public void update() {
        if(!mIsInitialized){
            init();
        }
        /* is this visible this frame, if yes push it **/
        if(mVisible){
            ArtrixView.RenderQueue.Enque(this);
        } else {
            ArtrixView.RenderQueue.Remove(this);
        }
    }

    @Override
    public void render() {

    }
}
