/*
	Phong Shader (for one Point-Light)
*/

varying vec3 N;
varying vec3 v;
//varying vec2 gl_TexCoord[1];
uniform sampler2D texture1;

void main (void)  
{  
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 R = normalize(-reflect(L,N));  
 
   //calculate Ambient Term:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient;    

   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 

   // write Total Color:  
   vec4 c = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
   gl_FragColor = c * texture2D(texture1, gl_TexCoord[0].st);
   gl_FragColor.a = 1.0; // needed by ATI, why?
}
