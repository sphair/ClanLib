
#version 330

layout(std140) uniform Uniforms
{
	mat3 ObjectNormalToEye;
	mat4 ObjectToWorld;
	mat4 WorldToEye;
	mat4 EyeToProjection;
};

in vec4 AttrPositionInObject;
in vec3 AttrNormalInObject;
in vec2 AttrTexCoord;

out vec4 PositionInWorld;
out vec4 PositionInEye;
out vec3 NormalInEye;
out vec2 TexCoord;
out vec4 LightInEye;

void main()
{
	TexCoord = AttrTexCoord;
	NormalInEye = normalize(ObjectNormalToEye * AttrNormalInObject);
	LightInEye = normalize(WorldToEye * vec4(0,10,1,1));
	PositionInWorld = ObjectToWorld * AttrPositionInObject;
	PositionInEye = WorldToEye * PositionInWorld;
	gl_Position = EyeToProjection * PositionInEye;
}
