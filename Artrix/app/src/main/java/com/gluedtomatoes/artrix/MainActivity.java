package com.gluedtomatoes.artrix;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;


public class MainActivity extends Activity implements StateMachine{
    private GLSurfaceView surface;

    private State mCurrentState;
    private State mPreviousState;
    private State mGlobalState;

    private Sprite s;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        init();
        surface = new ArtrixView(this);
        setContentView(surface);
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
    public void init() {
        Transform.init(this);
        ShaderProgram.init(this);
        s = new Sprite();
    }

    @Override
    public void update() {
        s.update();
    }

    @Override
    public void shutdown() {

    }

    @Override
    public void changeState(State state) {
        StateManager.changeState(this, state);
    }
}
