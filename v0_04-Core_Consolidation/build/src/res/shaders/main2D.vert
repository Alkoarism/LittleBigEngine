#version 330 core

layout(location = 0) in vec4 aVertText;

out vec2 textCoord;

uniform mat4 projection;
uniform mat4 model;

void main() {
	textCoord = aVertText.zw;
	gl_Position = projection * model * vec4(aVertText.xy, 0.0, 1.0);
}