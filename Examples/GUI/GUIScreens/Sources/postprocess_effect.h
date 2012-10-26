
#pragma once

class PostProcessScene;

class PostProcessEffect
{
public:
	PostProcessEffect(PostProcessScene *scene) : scene(scene) {}
	virtual ~PostProcessEffect() {}
	virtual void render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry) = 0;

	PostProcessScene *scene;
};

class PostProcessEffectDarken : public PostProcessEffect
{
public:
	PostProcessEffectDarken(PostProcessScene *scene) : PostProcessEffect(scene) {}
	void render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry);
};

class PostProcessEffectTransparency : public PostProcessEffect
{
public:
	PostProcessEffectTransparency(PostProcessScene *scene) : PostProcessEffect(scene) { alpha = 1.0f; }
	void render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry);

	float get_transparency() const { return alpha; }
	void set_transparency(float alpha);

private:
	float alpha;
};
