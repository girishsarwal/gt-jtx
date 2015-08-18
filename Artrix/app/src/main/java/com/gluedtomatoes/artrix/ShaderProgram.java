package com.gluedtomatoes.artrix;

import android.app.Activity;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.Matrix;
import android.opengl.GLES20;
import android.support.v7.internal.view.menu.ActionMenuItemView;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Hashtable;

/**
 * Created by gsarwal on 5/8/2015.
 */
public class ShaderProgram {
    public static String SHADERS_ROOT = "shaders";
    private static HashMap<String, Integer> mShaders = new HashMap<>();
    public static void use(String name){
        GLES20.glUseProgram(mShaders.get(name));
    }

    public static void setUniformVector(String shader, String uniform, Vector3 value){

    }

    public static void setUniformMatrix(String shader, String uniform, Matrix matrix){

    }

    public static void setUniformFloat(String shader, String uniform, float value){

    }

    public static void init(Activity activity){

        AssetManager am = activity.getAssets();
        String[] list;

        try {
            list = am.list(SHADERS_ROOT);
            for(String s: list) {
                mShaders.put(s, createShadingProgram(am, s));
            }
        } catch (IOException e) {

            e.printStackTrace();
        }
    }
    private static int createShadingProgram(AssetManager am, String name) {

        int vertexShader = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
        GLES20.glShaderSource(vertexShader, readShaderFile(am, String.format("%s/%s/%s.vs", SHADERS_ROOT, name, name)));
        GLES20.glCompileShader(vertexShader);

        int fragmentShader = GLES20.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
        GLES20.glShaderSource(vertexShader, readShaderFile(am, String.format("%s/%s/%s.ps", SHADERS_ROOT, name, name)));
        GLES20.glCompileShader(fragmentShader);

        int program = GLES20.glCreateProgram();
        GLES20.glAttachShader(program, vertexShader);
        GLES20.glAttachShader(program, fragmentShader);
        GLES20.glLinkProgram(program);
        return program;
    }

    private static String readShaderFile(AssetManager am, String name){
        String shaderSource = "";
        try {

            byte[] buffer = new byte[1024];
            ByteArrayOutputStream baos = new ByteArrayOutputStream();

            InputStream in = am.open(name);
            int read = 0;
            while ((read = in.read(buffer)) != -1) {
                baos.write(buffer, 0, read);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return shaderSource;
    }

    public static int useShaderProgram(String name){
        int program = mShaders.get(name);
        GLES20.glUseProgram(program);
        return program;
    }

}
