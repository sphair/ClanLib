/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Harry Storbacka
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "../CollisionAPI/outline_accuracy.h"
#include "../CollisionAPI/outline_circle.h"
#include "outline_provider_bitmap_impl.h"
#include "collision_outline_impl.h"

using namespace clan;

OutlineProviderBitmap_Impl::OutlineProviderBitmap_Impl(
	const PixelBuffer &pbuf,
	int alpha_limit,
	bool get_insides)
:
	data(0),
	get_insides(get_insides),
	alpha_limit(alpha_limit),
	//double_precision(false),
	//consecutive_left_turns(0),
	//consecutive_right_turns(0),
	alpha_pixel(3),
	pb(pbuf),
	last_point(0,0),
	last_dir(DIR_LEFT)
{
	if( pbuf.get_format() != TextureFormat::rgba8 )
	{
		// the image contains no alpha - add only a rectangle
		Contour contour;
		
		contour.get_points().push_back( Pointf(0.0f, 0.0f) );
		contour.get_points().push_back( Pointf(0.0f, float(height)) );
		contour.get_points().push_back( Pointf(float(width), float(height)) );
		contour.get_points().push_back( Pointf(float(width), 0.0f) );
		
		contours.push_back(contour);
		
		return;
	}

	height = pbuf.get_height();
	width = pbuf.get_width();
	
	// allocate a grid of unsigned chars, this represents the corners between pixels.
	// We will only use the first 4 bits of each char:
	//  (1 << 0) 0x1 : the pixel to the upper left
	//  (1 << 1) 0x2 : the pixel to the upper right
	//  (1 << 2) 0x4 : the pixel to the lower left
	//  (1 << 3) 0x8 : the pixel to the lower right
	data = new unsigned char[(height+1)*(width+1)];

	// The image part
	for(int y = 0; y <= height; y++)
	{
		for(int x = 0; x <= width; x++)
		{
			get_corner(x,y) = 0x0;
			if(is_opaque(x-1,y-1))
				get_corner(x,y) |= 0x1;
			if(is_opaque(x,y-1))
				get_corner(x,y) |= 0x2;
			if(is_opaque(x-1,y))
				get_corner(x,y) |= 0x4;
			if(is_opaque(x,y))
				get_corner(x,y) |= 0x8;
		}
	}

	find_contours();
}

OutlineProviderBitmap_Impl::~OutlineProviderBitmap_Impl()
{
	delete[] data;
}

void OutlineProviderBitmap_Impl::find_contours()
{
	Pointf start_point(find_next_contour_start(0,0));
		
	while( start_point.x != -1.0f && start_point.y != -1.0f )
	{
		Contour contour;
		
		contour.get_points().push_back(start_point);

		// initialize outline finder algorithm (get_next_point)
		last_point = clan::Pointf(start_point);
		last_corner = get_corner(last_point.x,last_point.y); // can be 0x8 or 0xe
		if(last_corner == 0x8)
		{
			// This is the top left corner of an outside contour
			last_dir = DIR_LEFT; // Since we are now going down, last time we was going left.
			contour.set_inside_contour(false);
		}
		else if(last_corner == 0x7)
		{
			// This is the top left corner of an inside contour
			last_dir = DIR_UP; // Since we are now going right, last time was up
			contour.set_inside_contour(true);
		}
		else
			throw Exception("Arg, the start corner is not of any of the two known types: 0x8, 0x7\n");
			
		while( true )
		{
			get_next_point(contour.get_points());
			
			if( contour.get_points().front().x == contour.get_points().back().x
				&& contour.get_points().front().y == contour.get_points().back().y )
			{
				if( contour.get_points().size() > 3 )
				{
					// line loop closed, remove last point (its the same as the first)
					contour.get_points().pop_back();
					break;
				}
				else
					throw Exception(string_format("Error: front() == back(), but only %1 points in list", (int)contour.get_points().size()));
			}

			if( contour.get_points().size() > 10000 ) break; // Sanity ?
		}
		// Now we have found the points for the contour, but this contour might be an
		// inside of some other contour.
		//   (Image a big 'O' would give two contours one on the outside, one on the inside
		//    but the inside one is inside-out, very bad !)
		// If this is the case we should "connect" it to the outer contour, that way they
		// become one concave contour, and all collision-tests will work.
		if(!contour.is_inside_contour())
		{
			// It's a normal (outside) contour
			contour.get_sub_circles().clear();
			contours.push_back(contour);
		}
		else
		{
			if(get_insides)
			{
				// Reverse the points (and shift by one, to get the same point as the front)
				std::vector<Pointf> rpoints;
				rpoints.push_back(contour.get_points().front());
				contour.get_points().erase(contour.get_points().begin());
				std::vector<Pointf>::reverse_iterator it = contour.get_points().rend();
				do
				{
					it--;
					rpoints.push_back(*it);
				}
				while(it != contour.get_points().rbegin());
				// Since we will be going through our starting point twice, we add it to the end.
				rpoints.push_back(rpoints.front());
				
				Contour *outside_contour = 0;
				Pointf tmp_point = rpoints.front();
				// Look for the outside contour to the left of our starting point (that was the upper left corner, remember)
				while(!outside_contour)
				{
					tmp_point.x--;
					outside_contour = point_in_outline((int)tmp_point.x, (int)tmp_point.y);
				}
				// Now we have the outside contour, and we know at what point we can splice our selves in.
				// then all we need to do is combine the two vectors, into one great list of points.
				std::vector<Pointf> combined_points;
				// First add from the outer contour untill we reach the point where we should splice.
				while(outside_contour->get_points().size() > 0)
				{
					Pointf outside_point = outside_contour->get_points().front();
					combined_points.push_back(outside_point);
					outside_contour->get_points().erase(outside_contour->get_points().begin());
					if(outside_point.x == tmp_point.x && outside_point.y == tmp_point.y)
					{
						Pointf bridge_point(outside_point);
						// Build the bridge from the outside to the inside
						for(bridge_point.x++; bridge_point.x != rpoints.front().x; bridge_point.x++)
						{
							combined_points.push_back(bridge_point);
						}
							
						// Now its time to add our reversed points
						for(std::vector<Pointf>::iterator it = rpoints.begin(); it != rpoints.end(); ++it)
						{
							combined_points.push_back(*it);
						}

						bridge_point = combined_points.back();
						// Build the bridge back from the inside to the outside
						// Build the bridge from the outside to the inside
						for(bridge_point.x--; bridge_point.x != outside_point.x; bridge_point.x--)
						{
							combined_points.push_back(bridge_point);
						}
						
						// And add the outside point again to complete the link
						combined_points.push_back(outside_point);
					}
				}
				// Now we have combined the points, then we just add them to the other contour.
				outside_contour->get_points() = combined_points;
			}
		}
	
		int next_search_start_x = (int)start_point.x + 1;
		int next_search_start_y = (int)start_point.y;

		if( next_search_start_x >= width+1 )
		{
			next_search_start_x = 0;
			next_search_start_y++;
			if( next_search_start_y >= height+1 )
				break;
		}

		start_point = find_next_contour_start(
			next_search_start_x,
			next_search_start_y);
	}
}


Pointf OutlineProviderBitmap_Impl::find_next_contour_start(unsigned int sx, unsigned int sy)
{
	// we start where we last stopped
	for( int y=sy; y < height+1; y++ )
	{
		// first time we start where we last stopped
		for( int x=sx; x < width+1; x++ )
		{
			// We can only start on a corner facing up and left, unless we also look for insides
			if( get_corner(x,y) == 0x8 || (get_insides && get_corner(x,y) == 0x7) )
			{
				// Lets check that it has not already been used
				if( !point_in_outline(x,y) )
					return Pointf(float(x),float(y));
			}
		}
		sx = 0;
	}
	
	return Pointf(-1.0f,-1.0f);
}

void OutlineProviderBitmap_Impl::get_next_point(std::vector<Pointf> &points)
{
	/*
	 See what type of corner we are, this gives us our counter-clockwise direction
	  in case of interesting places with connected corners, we use the last direction to figure it out.
	  above all cases in the switch, you can se what pixels are opaque if the corner in question is the middle
	  corner.
	*/
	MoveDir next_dir = DIR_UP;
	switch(last_corner)
	{
	// X0
	// 00
	case 0x1:
		next_dir = DIR_UP;
		break;
	// 0X
	// 00
	case 0x2:
		next_dir = DIR_RIGHT;
		break;
	// XX
	// 00
	case 0x3:
		next_dir = DIR_RIGHT;
		break;
	// 00
	// X0
	case 0x4:
		next_dir = DIR_LEFT;
		break;
	// X0
	// X0
	case 0x5:
		next_dir = DIR_UP;
		break;
	// 0X
	// X0
	case 0x6:
		if(last_dir == DIR_UP)
			next_dir = DIR_RIGHT;
		else if(last_dir == DIR_DOWN)
			next_dir = DIR_LEFT;
		else
			throw Exception(string_format("Came to a corner-type: 0x6, but last direction was: %1",last_dir));
		break;
	// XX
	// X0
	case 0x7:
		next_dir = DIR_RIGHT;
		break;
	// 00
	// 0X
	case 0x8:
		next_dir = DIR_DOWN;
		break;
	// X0
	// 0X
	case 0x9:
		if(last_dir == DIR_RIGHT)
			next_dir = DIR_DOWN;
		else if(last_dir == DIR_LEFT)
			next_dir = DIR_UP;
		else
			throw Exception(string_format("Came to a corner-type: 0x9, but last direction was: %1", last_dir));
		break;
	// 0X
	// 0X
	case 0xa:
		next_dir = DIR_DOWN;
		break;
	// XX
	// 0X
	case 0xb:
		next_dir = DIR_DOWN;
		break;
	// 00
	// XX
	case 0xc:
		next_dir = DIR_LEFT;
		break;
	// X0
	// XX
	case 0xd:
		next_dir = DIR_UP;
		break;
	// 0X
	// XX
	case 0xe:
		next_dir = DIR_LEFT;
		break;
	
	default:
		throw Exception(string_format("Unknown corner-type: %1", last_corner));
		break;
	}
	
	Pointf next_point(0.0f,0.0f);
	switch (next_dir)
	{
	case DIR_UP:
		//printf("up (%x)\n", last_corner);
		next_point = Pointf(last_point.x, last_point.y-1);
		break;
	case DIR_DOWN:
		//printf("down (%x)\n", last_corner);
		next_point = Pointf(last_point.x, last_point.y+1);
		break;
	case DIR_LEFT:
		//printf("left (%x)\n", last_corner);
		next_point = Pointf(last_point.x-1.0f, last_point.y);
		break;
	case DIR_RIGHT:
		//printf("right (%x)\n", last_corner);
		next_point = Pointf(last_point.x+1.0f, last_point.y);
		break;
	}
	last_point = next_point;
	last_dir = next_dir;
	last_corner = get_corner(next_point.x, next_point.y);
	points.push_back(next_point);
}

Contour *OutlineProviderBitmap_Impl::point_in_outline(unsigned int x, unsigned int y)
{
	std::vector<Contour>::iterator it;
	for( it = contours.begin(); it != contours.end(); ++it )
	{
		std::vector<Pointf>::iterator ita;
		for( ita = (*it).get_points().begin(); ita != (*it).get_points().end(); ++ita )
		{
			if( (*ita).x == x && (*ita).y == y ) 
			{
				return &(*it);
			}
		}
	}

	return 0;
}

bool OutlineProviderBitmap_Impl::is_opaque(int x, int y)
{
	if(x < 0 || y < 0 || x >= width || y >= height)
	{
		return false;
	}
	
	unsigned char *pbdata = static_cast<unsigned char *>(pb.get_data());
	unsigned int pitch = pb.get_pitch();
	unsigned char bpp = pb.get_bytes_per_pixel();

	return pbdata[y*pitch+x*bpp+alpha_pixel] >= alpha_limit;
}

bool OutlineProviderBitmap_Impl::is_edge(int x, int y)
{
	// These are the only (non-edges)
	// 0x0: empty
	// 0xf: full
	return (get_corner(x,y) != 0x0 && get_corner(x,y) != 0xf);
}

