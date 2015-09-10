package com.gluedtomatoes.artrix;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.Matrix;
import android.opengl.GLES20;
import android.support.v7.internal.view.menu.ActionMenuItemView;
import android.util.Log;

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

    public static void init(Context context){

        AssetManager am = context.getAssets();
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
        String vsPath = String.format("%s/%s/%s.vs", SHADERS_ROOT, name, name);
        String psPath = String.format("%s/%s/%s.ps", SHADERS_ROOT, name, name);
        String vsSource = readShaderFile(am, vsPath);
        String psSource = readShaderFile(am, psPath);
        return compileProgram(
                loadShader(GLES20.GL_VERTEX_SHADER, vsSource),
                loadShader(GLES20.GL_FRAGMENT_SHADER, psSource)
        );
    }

    private static int loadShader(int shaderType, String resource){
        int shader = GLES20.glCreateShader(shaderType);
        if (shader != 0) {
            GLES20.glShaderSource(shader, resource);
            GLES20.glCompileShader(shader);
            int[] compiled = new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) {
                Log.e("Shader Load Error", "Could not compile shader " + shaderType + ":");
                Log.e("Shader Load Error", GLES20.glGetShaderInfoLog(shader));
                GLES20.glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }

    public static int compileProgram (int vertexShader, int fragmentShader) {
    int program = GLES20.glCreateProgram();
        if (program != 0) {
            GLES20.glAttachShader(program, vertexShader);
            GLES20.glAttachShader(program, fragmentShader);
            GLES20.glLinkProgram(program);
            int[] linkStatus = new int[1];
            GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
            if (linkStatus[0] != GLES20.GL_TRUE) {
                Log.e("Shader Error", "Could not link program: ");
                Log.e("Shader Error", GLES20.glGetProgramInfoLog(program));
                GLES20.glDeleteProgram(program);
                program = 0;
            }
        }
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
            shaderSource = buffer.toString();
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
