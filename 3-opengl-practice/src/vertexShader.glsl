#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 aTextureProjection;

out vec2 textureProjection;

uniform mat4 resultMatrix;
uniform mat4 transform;

void main() {
	gl_Position = resultMatrix * transform * vec4(vertexPos, 1);
	textureProjection = aTextureProjection;
}