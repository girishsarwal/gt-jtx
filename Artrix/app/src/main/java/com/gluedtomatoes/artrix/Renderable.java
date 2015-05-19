package com.gluedtomatoes.artrix;

/**
 * Created by gsarwal on 5/8/2015.
 */
public interface Renderable {
    public Vector2 getPosition();
    public void setPosition(Vector2 position);
    public Vector2 getSize();
    public void setSize(Vector2 size);
    public float getRotation();
    public void setRotation(float rotation);
    public void setVisible(Boolean visible);
    public Boolean getVisible();

    public void render();
}
