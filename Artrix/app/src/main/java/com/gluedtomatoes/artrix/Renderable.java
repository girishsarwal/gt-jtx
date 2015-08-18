package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 5/8/2015.
 */
public interface Renderable {
    public Vector3 getPosition();
    public void setPosition(Vector3 position);
    public Vector3 getSize();
    public void setSize(Vector3 size);
    public float getRotation();
    public void setRotation(float rotation);
    public void setVisible(Boolean visible);
    public Boolean getVisible();

    public void render();
}
