#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texturePosition;

out vec2 textureCoords;

uniform mat4 resultMatrix;

void main() {
    gl_Position = resultMatrix * vec4(vertex, 1.0);
    textureCoords = texturePosition;
}  
