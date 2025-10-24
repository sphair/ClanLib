#include <ClanLib/display.h>

#include "mask_object_extractor.h"

// Process a mask, and creates mask objects
MaskObjectExtractor::MaskObjectExtractor(CL_PixelBuffer &background, CL_PixelBuffer &mask)
{
	background.lock();
	mask.lock();

	std::vector<int> colors;
	unsigned int c;

	for(int y = 0; y < mask.get_height(); ++y)
	{
		int *data = (int *) ((char *)mask.get_data() + y * mask.get_pitch());

		for(int x = 0; x < mask.get_width(); ++x)
		{
			int col = data[x];
			if(col > 255)	// Found a mask object
			{
				// Check if we're already processed this color
				for(c = 0; c < colors.size(); ++c)
					if(colors[c] == col)
						break;	// Already processed, skip this one

				if (c == colors.size())
				{
					// Find out where the mask object is located and how big it is
					CL_Rect rect = isolate_mask_object(mask, col);
					
					// Extract the mask object and create a surface from it
					Object *object = create_object_from_mask(rect, background, mask, col);

					// Store object in our list
					objects.push_back(object);
					
					// Save the colour so we don't process it more than once.
					colors.push_back(col);
				}
			}
		}
	}
	
	mask.unlock();
	background.unlock();
}

// Find boundaries for a mask object
CL_Rect MaskObjectExtractor::isolate_mask_object(CL_PixelBuffer &mask, int object_col)
{
	unsigned int min_x = static_cast<unsigned int>(-1), min_y = static_cast<unsigned int>(-1), max_x = 0, max_y = 0;
	
	for(unsigned int y = 0; y < (unsigned int)mask.get_height(); ++y)
	{
		int *data = (int *) ((char *)mask.get_data() + y * mask.get_pitch());

		for(unsigned int x = 0; x < (unsigned int)mask.get_width(); ++x)
		{
			if(object_col == data[x])	// We found a pixel with the color we're looking for
			{
				if (x < min_x) min_x = x;
				if (x > max_x) max_x = x;
				if (y < min_y) min_y = y;
				if (y > max_y) max_y = y;
			}
		}
	}

	return CL_Rect(min_x, min_y, max_x + 1, max_y + 1);
}

// Extract image data from background using mask and create a surface/object from it
Object *MaskObjectExtractor::create_object_from_mask(const CL_Rect &rect, CL_PixelBuffer &background, CL_PixelBuffer &mask, int color)
{	
	// Create a pixelbuffer which will store our extracted mask object
	CL_PixelBuffer buffer(rect.get_width(), rect.get_height(), 32 * rect.get_width(), background.get_format());
	buffer.lock();

	for(int by = 0, y = rect.top; y < (int)rect.bottom; ++y, ++by)
	{
		// Locate the positions in our pixelbuffers
		int *background_data = (int *) ((char *)background.get_data() + y * background.get_pitch());
		int *mask_data = (int *) ((char *)mask.get_data() + y * mask.get_pitch());
		int *buffer_data = (int *) ((char *)buffer.get_data() + by * buffer.get_pitch());

		for(int bx = 0, x = rect.left; x < (int)rect.right; ++x, ++bx)
		{
			// Extract alpha fram current mask position
			unsigned int current_alpha = mask_data[x] & 0xff;
			
			if(current_alpha != 0)	// Skip where mask is alpha'ed ?
				buffer_data[bx] = background_data[x];
			else
				buffer_data[bx] = 0;
		}
	}

	buffer.unlock();
	
	// Create a surface from our new buffer
	CL_Surface surface(buffer);

	// Store surface and position in an Object
	Object *object = new Object;
	object->surface = surface;
	object->pos = CL_Point(rect.left, rect.top);
	object->z_value = color >> 16;	// Use R and G colours to specify z-value
	if(object->z_value == 0)
		object->z_value = static_cast<unsigned int>(-1);

	return object;
}
