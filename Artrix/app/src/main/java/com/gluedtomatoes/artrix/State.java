package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 5/8/2015.
 */
public interface State{
    public void enter(StateMachine sm);
    public void update(StateMachine sm);
    public void exit(StateMachine sm);
}
