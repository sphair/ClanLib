
#pragma once

class HSVSpriteBatch;

class HSVSprite
{
public:
	HSVSprite(CL_GraphicContext &gc, HSVSpriteBatch *batcher, const CL_String &image_filename);
	~HSVSprite();

	void draw(CL_GraphicContext &gc, int x, int y, float hue_offset);

private:
	HSVSpriteBatch *batcher;
	CL_Rect geometry;
	CL_Texture texture;
};
