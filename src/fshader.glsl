#version 150

in vec4 f_colour;
out vec4 out_colour;

void main() 
<<<<<<< HEAD
{    
=======
{

	// https://www.desultoryquest.com/blog/drawing-anti-aliased-circular-Vertexs-using-opengl-slash-webgl/
	float r = 0.0, delta = 0.0, alpha = 1.0;
	vec2 cxy = 2.0 * gl_VertexCoord - 1.0;
	r = dot(cxy, cxy);
	#ifdef GL_OES_standard_derivatives
	delta = fwidth(r);
	alpha = 1.0 - smoothstep(1.0 - delta, 1.0 + delta, r);
	#endif
	if (r > 1.0) {
		discard;
	}    
    
>>>>>>> 2c5509930b418b34792faf59af91255a2dce8519
	out_colour = f_colour;
}
