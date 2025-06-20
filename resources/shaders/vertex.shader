#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoords;

out vec2 v_textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 translationMatrix;
uniform mat4 rotationThetaMatrix;
uniform mat4 rotationPhiMatrix;

void main() {
  gl_Position = projectionMatrix * rotationPhiMatrix * rotationThetaMatrix * translationMatrix * position;
  v_textureCoords = textureCoords;
}