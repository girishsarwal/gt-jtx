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
        State previousState = stateMachine.getPreviousState();
        if(previousState!= null){
            previousState.exit(stateMachine);
        }
        stateMachine.setPreviousState(stateMachine.getCurrentState());
        stateMachine.setCurrentState(state);
        stateMachine.getCurrentState().enter(stateMachine);
    }
}
