#version 450

in vec3 position;
in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform float size;

uniform mat4 treeTransformation;

uniform mat4 gridScale;
uniform vec3 nodeColor;
uniform mat4 nodeTransformation;

out vec4 fColor;

void main(void)
{
		fColor = vec4(nodeColor, 1.0f);
	  vec4 localPosition = treeTransformation * nodeTransformation * gridScale * vec4(position, 1.0);

		/*
		float positionLength = length(localPosition);
		localPosition.xyz /= positionLength;
		localPosition.xyz *= size;
		*/

    vec4 newPosition = view * localPosition;
    gl_Position = projection * newPosition;
}
