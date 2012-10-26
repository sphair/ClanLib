
#pragma once

class Material
{
public:
	Material()
	: emission(0.0f, 0.0f, 0.0f, 1.0f), ambient(0.2f, 0.2f, 0.2f, 1.0f), diffuse(0.8f, 0.8f, 0.8f, 1.0f),
	  specular(0.0f, 0.0f, 0.0f, 1.0f), shininess(0.0f)
	{
	}

	Vec4f emission;
	Vec4f ambient;
	Vec4f diffuse;
	Vec4f specular;
	float shininess;
};

class LightSource
{
public:
	LightSource()
	: ambient(0.0f, 0.0f, 0.0f, 1.0f), diffuse(1.0f, 1.0f, 1.0f, 1.0f), specular(1.0f, 1.0f, 1.0f, 1.0f),
	  position(0.0f, 0.0f, 1.0f, 0.0f), spot_direction(0.0f, 0.0f, -1.0f), spot_exponent(0.0f),
	  spot_cutoff(180.0f), constant_attenuation(1.0f), linear_attenuation(0.0f), quadratic_attenuation(0.0f)
	{
	}

	Vec4f ambient;
	Vec4f diffuse;
	Vec4f specular;
	Vec4f position;
	Vec3f spot_direction;
	float spot_exponent;
	float spot_cutoff;
	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
	
	Vec4f get_halfvector() const { Vec4f p = Vec4f(0.0f, 0.0f, 1.0f, 0.0f) + position; p.normalize3(); return p; }
	float get_spot_cos_cutoff() const { return (float) cos(spot_cutoff * 3.14159265f / 180.0f); }

	void set_position(const Mat4f &modelview, const Vec4f &p)
	{
		// Convert point to eye coordinates
		position = modelview * p;
	}
};

class LightModel
{
public:
	LightModel()
	: scene_ambient(0.2f, 0.2f, 0.2f, 1.0f)
	{
	}

	/** gl_LightSource[] */
	std::vector<LightSource> light_sources;
	
	/** gl_LightModel.ambient */
	Vec4f scene_ambient;
	
	/** gl_FrontLightModelProduct.sceneColor and gl_BackLightModelProduct.sceneColor*/
	Vec4f get_scene_color(const Material &material) const { return material.emission + material.ambient * scene_ambient; }
	
	/** gl_FrontLightProduct[i].ambient and gl_BackLightProduct[i].ambient */
	Vec4f get_light_ambient(const Material &material, const LightSource &light) const { return material.ambient * light.ambient; }

	/** gl_FrontLightProduct[i].diffuse and gl_BackLightProduct[i].diffuse */
	Vec4f get_light_diffuse(const Material &material, const LightSource &light) const { return material.diffuse * light.diffuse; }

	/** gl_FrontLightProduct[i].specular and gl_BackLightProduct[i].specular */
	Vec4f get_light_specular(const Material &material, const LightSource &light) const { return material.specular * light.specular; }
};
