#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 vertexColor;

uniform vec4 horizontalOffset;

void main()
{
	gl_Position = vec4(-1.0*aPos+horizontalOffset.x, 1.0);
	vertexColor = vec4(aColor, 1.0);
}
