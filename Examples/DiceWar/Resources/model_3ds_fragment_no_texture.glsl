/*
	Phong Shader (for one Point-Light)
*/

varying vec3 N;
varying vec3 v;

uniform vec4 lightSourcePosition;       // gl_LightSource[0].position
uniform vec4 frontLightProductAmbient;  // gl_FrontLightProduct[0].ambient
uniform vec4 frontLightProductDiffuse;  // gl_FrontLightProduct[0].diffuse
uniform vec4 frontLightProductSpecular; // gl_FrontLightProduct[0].specular
uniform float frontMaterialShininess;   // gl_FrontMaterial.shininess
uniform vec4 frontLightModelProductSceneColor; // gl_FrontLightModelProduct.sceneColor

void main (void)  
{
   vec3 L = normalize(lightSourcePosition.xyz - v);   
   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 R = normalize(-reflect(L,N));  

   //calculate Ambient Term:  
   vec4 Iamb = frontLightProductAmbient;    

   //calculate Diffuse Term:  
   vec4 Idiff = frontLightProductDiffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = frontLightProductSpecular * pow(max(dot(R,E),0.0),0.3*frontMaterialShininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 

   // write Total Color:  
   vec4 c = frontLightModelProductSceneColor + Iamb + Idiff + Ispec;
   
   gl_FragColor = c;
   gl_FragColor.a = 1.0; // needed by ATI, why?
}
