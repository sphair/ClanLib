attribute vec2 Position;
uniform mat4 cl_ModelViewProjectionMatrix;

void main(void) 
{
	gl_Position = cl_ModelViewProjectionMatrix* vec4(Position.xy, 0,1); 
}
