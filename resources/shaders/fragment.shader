#version 330 core

in vec2 v_textureCoords;

out vec4 color;
uniform sampler2D textureAtlas;

void main() {
    color = texture(textureAtlas, v_textureCoords);
}