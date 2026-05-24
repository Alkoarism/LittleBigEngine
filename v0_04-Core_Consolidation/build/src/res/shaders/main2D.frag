#version 330 core

in vec2 textCoord;

uniform sampler2D image;
uniform vec3 textColor;

out vec4 fragColor;

void main() {
	fragColor = texture(image, textCoord) * vec4(textColor, 1.0);
}