package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class NopState implements State{
    public static NopState state = new NopState();
    @Override
    public void enter(StateMachine sm) {

    }

    @Override
    public void update(StateMachine sm) {

    }

    @Override
    public void exit(StateMachine sm) {

    }
}
