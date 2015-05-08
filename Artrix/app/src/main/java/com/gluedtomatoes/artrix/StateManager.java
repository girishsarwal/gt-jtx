package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class StateManager {
    public static void changeState(StateMachine stateMachine, State state){
        if(state == null){
            return;
        }
        if(stateMachine.getPreviousState() == state){
            return;
        }
        stateMachine.getPreviousState().exit(stateMachine);
        stateMachine.setPreviousState(stateMachine.getCurrentState());
        stateMachine.getCurrentState().enter(stateMachine);
    }
}
