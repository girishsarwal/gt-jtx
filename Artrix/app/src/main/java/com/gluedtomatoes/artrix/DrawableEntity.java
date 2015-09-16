package com.gluedtomatoes.artrix;

import android.graphics.Point;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

/**
 * Created by gsarwal on 5/18/2015.
 */
public class DrawableEntity extends Entity implements Renderable{
    protected FloatBuffer vertexBuffer;
    protected FloatBuffer textureBuffer;
    protected ShortBuffer indexBuffer;

    protected int mVertexCount = 0;
    protected int mTriangleCount = 0;
    protected int mVertexStride = 0;

    protected boolean mVisible;
    protected Vector3 mPosition;
    protected Vector3 mSize;
    protected float mRotation;
    protected ShaderProgram mShadingProgram;

    private VertexDescriptor mVertexDescriptor;

    DrawableEntity(){
        this("simple");

        mPosition = new Vector3();
        mRotation = 0.0f;
        mSize = new Vector3();
        mVisible = true;

        mVertexDescriptor = new VertexDescriptor(VertexDescriptor.POSITION_XYZW | VertexDescriptor.COLOR_RGBA);
    }
    public DrawableEntity(String shader){
        setShadingProgram(shader);
    }


    public float getRotation() {
        return mRotation;
    }
    public void setRotation(float mRotation) {
        this.mRotation = mRotation;
    }

    public Vector3 getPosition() {
        return mPosition;
    }
    public void setPosition(Vector3 mPosition) {
        this.mPosition = mPosition;
    }
    public void setPosition(Point mPosition) {
        setPosition(Transform.PixelsToCoords(mPosition));
    }


    public Vector3 getSize() {
        return mSize;
    }
    public void setSize(Vector3 mSize) {
        this.mSize = mSize;
    }
    public void setSize(Point mSize) {
        setSize(Transform.PixelsToCoords(mSize));
    }

    public ShaderProgram getShadingProgram(){
        return mShadingProgram;
    }
    public void setShadingProgram(String shadingProgram){
        mShadingProgram = ShaderProgram.get(shadingProgram);
    }

    public VertexDescriptor getVertexDescriptor(){
        return mVertexDescriptor;
    }
    public void setVertexDescriptor(VertexDescriptor vertexFormat) {
        mVertexDescriptor = vertexFormat;
    }


    @Override
    public void init()
    {
        mVertexCount = vertexBuffer.remaining();
        mIsInitialized = true;

        /** the idea here is to read the shader and see whatever components are available in the shader
         * once we have all the components available, we determine the vertex format that the drawable should be using
         */
        mShadingProgram.use();
        mVertexDescriptor =  mShadingProgram.createSuitableVertexFormat();


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
