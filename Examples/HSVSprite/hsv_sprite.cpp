
#include "precomp.h"
#include "hsv_sprite.h"
#include "hsv_sprite_batch.h"

HSVSprite::HSVSprite(CL_GraphicContext &gc, HSVSpriteBatch *batcher, const CL_String &image_filename)
: batcher(batcher)
{
	CL_PixelBuffer image = CL_ImageProviderFactory::load(image_filename);
	CL_Subtexture subtexture = batcher->alloc_sprite(gc, image.get_size());
	geometry = subtexture.get_geometry();
	texture = subtexture.get_texture();
	texture.set_subimage(geometry.get_top_left(), image);
}

HSVSprite::~HSVSprite()
{
}

void HSVSprite::draw(CL_GraphicContext &gc, int x, int y, float hue_offset)
{
	batcher->draw_sprite(gc, CL_Rect(CL_Point(x, y), geometry.get_size()), geometry, texture, hue_offset);
}
