package com.gluedtomatoes.artrix;

import android.graphics.Point;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class Entity implements StateMachine, Renderable{
    protected  State mCurrentState;
    protected  State mPreviousState;
    protected  State mGlobalState;

    private Vector2 mPosition;
    private Vector2 mSize;

    private Boolean mIsVisible;

    public float getRotation() {
        return mRotation;
    }

    public void setRotation(float mRotation) {
        this.mRotation = mRotation;
    }

    private float mRotation;

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

    @Override
    public State getCurrentState() {
        return mCurrentState;
    }

    @Override
    public State getPreviousState() {
        return mPreviousState;
    }

    @Override
    public State getGlobalState() {
        return mGlobalState;
    }

    @Override
    public void setCurrentState(State state) {
        if(state == null){
            state = NopState.state;
        }
        mCurrentState = state;
    }

    @Override
    public void setPreviousState(State state) {
        if(state == null){
            state = NopState.state;
        }
        mPreviousState = state;
    }

    @Override
    public void setGlobalState(State state) {
        if(state == null){
            state = NopState.state;
        }
        mGlobalState = state;
    }

    @Override
    public void changeState(State state) {
        StateManager.changeState(this, state);
    }
    @Override
    public void init(){
        mCurrentState = NopState.state;
    }
    @Override
    public void update(){
        mCurrentState.update(this);
    }
    @Override
    public void shutdown(){

    }

    @Override
    public void setVisible(Boolean visible) {
        mIsVisible = visible;
        if(mIsVisible){
            //enque in renderqueue
        } else {
            //deque from renderqueue
        }
    }

    @Override
    public Boolean getVisible() {
        return mIsVisible;
    }
}
