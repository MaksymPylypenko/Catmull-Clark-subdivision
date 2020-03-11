#version 150

in vec4 vPosition;
out vec4 f_colour;

uniform mat4 ModelView;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * ModelView * vPosition;

	f_colour = vec4(1,0,0,1); 	
}