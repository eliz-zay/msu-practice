#version 330 core

in vec2 textureProjection; // takes data from vertex shader => same name

out vec4 fragColor;

uniform sampler2D fragTexture;

void main() {
	fragColor = texture(fragTexture, textureProjection);
}