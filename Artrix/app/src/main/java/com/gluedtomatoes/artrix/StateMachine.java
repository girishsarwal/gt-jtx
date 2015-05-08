package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 5/8/2015.
 */
public interface StateMachine {
    public State getCurrentState();
    public State getPreviousState();
    public State getGlobalState();
    public void setCurrentState(State state);
    public void setPreviousState(State state);
    public void setGlobalState(State state);

    public void init();
    public void update();
    public void shutdown();

    public void changeState(State state);
}

