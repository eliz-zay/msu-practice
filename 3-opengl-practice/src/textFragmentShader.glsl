#version 330 core

in vec2 textureCoords;

out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main() {    
    vec4 sampled = vec4(1., 1., 1., texture(text, textureCoords).r);
    color = textColor * sampled;
} 