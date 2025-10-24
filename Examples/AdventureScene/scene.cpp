#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <algorithm>

#include "scene.h"
#include "mask_object_extractor.h"

Scene::Scene(const std::string &background_file, const std::string &mask_file, const std::string &walk_file)
{
	// Load all images using the factory, this way we support any fileformat
	CL_PixelBuffer background_buffer_org = CL_ProviderFactory::load(background_file);
	CL_PixelBuffer mask_buffer_org = CL_ProviderFactory::load(mask_file);
	CL_PixelBuffer walk_buffer_org = CL_ProviderFactory::load(walk_file);

	// Convert the walk buffer to a known format so we know where the various colours are located
	CL_PixelBuffer background_buffer = background_buffer_org.to_format(CL_PixelFormat::rgba8888);

	// Convert the walk buffer to a known format so we know where the various colours are located
	CL_PixelBuffer mask_buffer = mask_buffer_org.to_format(CL_PixelFormat::rgba8888);

	// Convert the walk buffer to a known format so we know where the various colours are located
	walk_buffer = walk_buffer_org.to_format(CL_PixelFormat::rgba8888);

	// We lock it, so we can access the pixeldata later
	walk_buffer.lock();

	// Extract all objects from the background using the mask buffer
	MaskObjectExtractor extractor(background_buffer, mask_buffer);
	objects = extractor.objects;
	
	// Create a surface to display as the background
	background = CL_Surface(background_buffer);

	// These are only for debugging (showing how the mask and walkarea look like)
	mask = CL_Surface(mask_buffer);
	walk = CL_Surface(walk_buffer);
}

Scene::~Scene()
{
	walk_buffer.unlock();

	for(unsigned int i=0; i<objects.size(); ++i)
		delete objects[i];
}

void Scene::add_object(Object *object)
{
	objects.push_back(object);
	update_objects();
}

void Scene::update_objects()
{
	// Make sure the objects are z-sorted
	std::sort(objects.begin(), objects.end(), object_compare());
}

void Scene::draw()
{
	// Draw background (but skip if 1 is pressed)
	if(!CL_Keyboard::get_keycode(CL_KEY_1))
		background.draw();
	else
		CL_Display::clear();

	// Draw walk if 3 is pressed
	if(CL_Keyboard::get_keycode(CL_KEY_3))
		walk.draw();

	// Draw all extracted mask objects (but skip if 4 is pressed)
	if(!CL_Keyboard::get_keycode(CL_KEY_4))
		for(unsigned int i=0; i<objects.size(); ++i)
			objects[i]->draw();

	// Draw mask if 2 is pressed
	if(CL_Keyboard::get_keycode(CL_KEY_2))
		mask.draw();
}
