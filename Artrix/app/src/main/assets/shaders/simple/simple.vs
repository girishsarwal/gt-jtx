 attribute vec4 inPosition;
 attribute vec4 inColor;

 varying vec4 vColor;

 void main() {
    gl_Position = inPosition;
    vColor = inColor;
 }