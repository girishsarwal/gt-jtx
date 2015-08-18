package com.gluedtomatoes.artrix;

import android.graphics.Point;
import android.opengl.GLES20;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

/**
 * Created by gsarwal on 5/18/2015.
 */
public class DrawableEntity extends Entity implements Renderable{
    protected FloatBuffer vertexBuffer;
    protected FloatBuffer textureBuffer;
    protected ShortBuffer indexBuffer;

    protected int vertexCount = 0;
    protected int triangleCount = 0;
    protected int vertexStride = 0;

    private boolean mVisible;
    private Vector3 mPosition;
    private Vector3 mSize;
    private float mRotation;
    private String mShadingProgram;

    private VertexDescriptor mVertexDescriptor;

    DrawableEntity(){
        this("simple");
        mVertexDescriptor = new VertexDescriptor(VertexDescriptor.POSITION_XYZW | VertexDescriptor.COLOR_RGBA);
        mVisible = true;
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

    public String getShadingProgram(){
        return mShadingProgram;
    }
    public void setShadingProgram(String shadingProgram){
        mShadingProgram = shadingProgram;
    }


    public VertexDescriptor getVertexDescriptor(){
        return mVertexDescriptor;
    }
    public void setVertexDescriptor(VertexDescriptor vertexFormat){
        mVertexDescriptor = vertexFormat;
    }

    @Override
    public void init()
    {
        vertexCount = vertexBuffer.remaining();
        mIsInitialized = true;

        int shader = ShaderProgram.useShaderProgram(mShadingProgram);
        int format = mVertexDescriptor.getFormat();


        /** the idea here is to read the shader and see whatever components are available in the shader
         * once we have all the components available, we determine the vertex format that the drawable should be using
         */

        int hPosition = GLES20.glGetAttribLocation(shader, "vPosition");

        if(hPosition > 0){
            if((mVertexDescriptor.getFormat() & VertexDescriptor.POSITION_XYZ) != VertexDescriptor.POSITION_XYZ){
                format |= VertexDescriptor.POSITION_XYZW;
            }
            GLES20.glEnableVertexAttribArray(hPosition);
        }
        int hColor = GLES20.glGetAttribLocation(shader, "vColor");
        if(hColor > 0){
            if((mVertexDescriptor.getFormat() & VertexDescriptor.COLOR_RGB) != VertexDescriptor.COLOR_RGB){
                format |= VertexDescriptor.COLOR_RGBA;
            }
            GLES20.glEnableVertexAttribArray(hColor);
        }
        mVertexDescriptor.setFormat(format);
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
