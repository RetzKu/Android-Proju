//varying vec2 texCoord0;

precision highp float;
varying vec2 texCoord0;

uniform sampler2D diffuse;

void main() {
  //gl_FragColor = vec4(0.5, 0.0, 0.5, 1.0);
  gl_FragColor = texture2D(diffuse,texCoord0);
}