package com.gluedtomatoes.artrix;

import android.opengl.GLES20;

/**
 * Created by gsarwal on 10/30/2015.
 */
public class PredefinedVertexDescriptors {
    /** create some known vertex formats **/
    public static VertexDescriptor VF_POSITION_COLOR = new VertexDescriptor(VertexDescriptor.POSITION | VertexDescriptor.COLOR_RGB){{
        addVertexAttribute("inPosition", new VertexAttribute(3, GLES20.GL_FLOAT, false, 0));
        addVertexAttribute("inColor", new VertexAttribute(3, GLES20.GL_FLOAT, false, 12));
        calculateStride();
    }};

    public static VertexDescriptor VF_SPRITE = new VertexDescriptor(VertexDescriptor.POSITION | VertexDescriptor.TEX0){{

        addVertexAttribute("inPosition", new VertexAttribute(2, GLES20.GL_FLOAT, false, 0));
        addVertexAttribute("inTex0", new VertexAttribute(2, GLES20.GL_FLOAT, false, 8));
        calculateStride();
    }};
}
