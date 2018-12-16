#version 460

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec4 fColor;

void main(void)
{
		fColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
    vec4 newPosition = view * vec4(position,1.0);
    gl_Position = projection * newPosition;
}
