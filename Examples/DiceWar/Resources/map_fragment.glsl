// Phong Shader
varying vec3 N;
varying vec3 v;
varying vec2 texcoord;

// Ground texture
uniform sampler2D texture_base;

// Area overlay
uniform sampler2D texture_areas;
uniform sampler2D texture_borders;
uniform sampler2D texture_colors;

// Light stuff
uniform vec4 lightSourcePosition;       // gl_LightSource[0].position
uniform vec4 frontLightProductAmbient;  // gl_FrontLightProduct[0].ambient
uniform vec4 frontLightProductDiffuse;  // gl_FrontLightProduct[0].diffuse
uniform vec4 frontLightProductSpecular; // gl_FrontLightProduct[0].specular
uniform float frontMaterialShininess;   // gl_FrontMaterial.shininess
uniform vec4 frontLightModelProductSceneColor; // gl_FrontLightModelProduct.sceneColor

vec4 calcLight(void);
vec4 calcAreaColor(void);
vec4 blend(vec4 a, vec4 b);

void main(void)  
{  
	vec4 lightColor = calcLight();
	vec4 groundColor = texture2D(texture_base, texcoord);
	vec4 borderColor = texture2D(texture_borders, texcoord);
	vec4 areaColor = calcAreaColor();
	gl_FragColor = lightColor * blend(blend(groundColor, areaColor), vec4(0.0,0.0,0.0,1.0-borderColor.r));
}

vec4 calcLight(void)
{
return vec4(1.0,1.0,1.0,1.0);
/*	// Phong Shader (for one Point-Light)

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
	c.a = 1.0;
	
	return c;*/
}

vec4 calcAreaColor(void)
{
	float index = texture2D(texture_areas, texcoord).r;
	return texture2D(texture_colors, vec2(index, 0.5));
}

vec4 blend(vec4 c1, vec4 c2)
{
	return vec4(c1.r*(1.0-c2.a)+c2.r*c2.a, c1.g*(1.0-c2.a)+c2.g*c2.a, c1.b*(1.0-c2.a)+c2.b*c2.a, c1.a);
}
