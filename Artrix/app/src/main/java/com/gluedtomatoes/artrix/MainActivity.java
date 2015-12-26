package com.gluedtomatoes.artrix;

import android.app.Activity;
import android.content.Context;
import android.hardware.usb.UsbManager;
import android.opengl.GLSurfaceView;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialProber;

import java.io.IOException;


public class MainActivity extends Activity implements StateMachine{
    private ArtrixView surface;

    private State mCurrentState;
    private State mPreviousState;
    private State mGlobalState;


    private RenderQueue renderQueue;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        // Get UsbManager from Android.
        UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);

        // Find the first available driver.
        UsbSerialDriver driver = UsbSerialProber.acquire(manager);

        if (driver != null) {
            try {
                driver.open();
                driver.setBaudRate(115200);
                byte buffer[] = new byte[16];
                int numBytesRead = driver.read(buffer, 1000);
                Log.d("", "Read " + numBytesRead + " bytes.");
            } catch (IOException e) {
                // Deal with error.
            } finally {
                try {
                    driver.close();
                }catch (IOException e){

                }
            }

        }

        super.onCreate(savedInstanceState);

        surface = new ArtrixView(this);
        surface.setMachine(this);
        renderQueue = new RenderQueue();
        setContentView(surface);
    }

    @Override
    public void onBackPressed(){
        //Do nothing
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
        changeState(SplashState.state);
    }

    @Override
    public void update() {
        mCurrentState.update(this);
    }

    @Override
    public void shutdown() {

    }

    @Override
    public void changeState(State state) {
        StateManager.changeState(this, state);
    }
}
