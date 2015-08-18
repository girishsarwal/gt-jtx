package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 5/18/2015.
 */
public class SplashState implements State{
    public static SplashState state = new SplashState();
    private Sprite background = new Sprite();
    @Override
    public void enter(StateMachine sm) {
        background.init();
    }

    @Override
    public void update(StateMachine sm) {

        background.update();
    }

    @Override
    public void exit(StateMachine sm) {

    }
}