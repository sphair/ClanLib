
varying vec4 diffuse,ambientGlobal, ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;
varying vec2 texcoord;

uniform float lightSourceConstantAttenuation;
uniform float lightSourceLinearAttenuation;
uniform float lightSourceQuadraticAttenuation;
uniform vec4 lightSourceSpecular;
uniform vec4 frontMaterialSpecular;
uniform float frontMaterialShininess;
uniform sampler2D texture1;

void main()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = ambientGlobal;
	float att;
	
	/* a fragment shader can't write a varying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	
	/* compute the dot product between normal and normalized lightdir */
	NdotL = max(dot(n,normalize(lightDir)),0.0);

	if (NdotL > 0.0)
	{
		att = 1.0 / (lightSourceConstantAttenuation +
			lightSourceLinearAttenuation * dist +
			lightSourceQuadraticAttenuation * dist * dist);
		color += att * (diffuse * NdotL + ambient);

		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += att * frontMaterialSpecular * lightSourceSpecular * pow(NdotHV,frontMaterialShininess);
	}

	gl_FragColor = color * texture2D(texture1, texcoord);
	gl_FragColor.a = 1.0; // needed by ATI, why?
}
