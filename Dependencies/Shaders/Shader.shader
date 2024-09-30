#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec3 mcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(position, 1.0);
   mcolor = color;
};

#shader fragment
#version 330 core

out vec4 color;
in vec3 mcolor;

void main()
{
	color = vec4(mcolor, 1.0);
};