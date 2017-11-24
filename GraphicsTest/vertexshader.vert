precision highp float;
attribute vec3 vp;
attribute vec2 texCoord;

varying vec2 texCoord0;

void main() {
  gl_Position = vec4(vp, 1.0);
  texCoord0 = texCoord;
}