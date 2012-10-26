attribute vec4 Position;
uniform mat4 cl_ModelViewProjectionMatrix;

void main(void) 
{ 
	gl_Position = cl_ModelViewProjectionMatrix*Position; 
}
