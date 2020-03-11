#version 150

in vec4 vPosition;
out vec4 f_colour;

uniform mat4 ModelView;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * ModelView * vPosition;
<<<<<<< HEAD
	f_colour = vec4(1,0,0,1); 	
=======
	f_colour = vColor; 	
>>>>>>> 2c5509930b418b34792faf59af91255a2dce8519
}