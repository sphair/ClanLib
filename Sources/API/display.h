/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Magnus Norddahl
**    Kenneth Gangstoe
**    Harry Storbacka
*/

/// \brief <p>ClanLib display and input library.</p>
//! Global=Display

#pragma once

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "Display/display.h"
#include "Display/display_target.h"
#include "Display/setup_display.h"
#include "Display/screen_info.h"
#include "Display/2D/color.h"
#include "Display/2D/color_hsv.h"
#include "Display/2D/color_hsv2.h"
#include "Display/2D/color_hsl.h"
#include "Display/2D/color_hsl2.h"
#include "Display/2D/draw.h"
#include "Display/2D/gradient.h"
#include "Display/2D/image.h"
#include "Display/2D/rounded_rect.h"
#include "Display/2D/sprite.h"
#include "Display/2D/sprite_description.h"
#include "Display/2D/subtexture.h"
#include "Display/2D/texture_group.h"
#include "Display/2D/span_layout.h"
#include "Display/2D/collidable_sprite.h"
#include "Display/Collision/collision_outline.h"
#include "Display/Collision/contour.h"
#include "Display/Collision/outline_accuracy.h"
#include "Display/Collision/outline_circle.h"
#include "Display/Collision/outline_math.h"
#include "Display/Collision/outline_provider.h"
#include "Display/Collision/outline_provider_bitmap.h"
#include "Display/Collision/outline_provider_file.h"
#include "Display/Font/font.h"
#include "Display/Font/font_description.h"
#include "Display/Font/font_system.h"
#include "Display/Font/font_sprite.h"
#include "Display/Font/font_metrics.h"
#include "Display/Font/font_freetype.h"
#include "Display/Font/font_vector.h"
#include "Display/Image/palette.h"
#include "Display/Image/pixel_buffer.h"
#include "Display/Image/pixel_buffer_help.h"
#include "Display/Image/pixel_format.h"
#include "Display/Image/icon_set.h"
#include "Display/Image/perlin_noise.h"
#include "Display/Image/image_import_description.h"
#include "Display/ImageProviders/jpeg_compressor.h"
#include "Display/ImageProviders/jpeg_decompressor.h"
#include "Display/ImageProviders/jpeg_provider.h"
#include "Display/ImageProviders/pcx_provider.h"
#include "Display/ImageProviders/png_provider.h"
#include "Display/ImageProviders/provider_factory.h"
#include "Display/ImageProviders/provider_type.h"
#include "Display/ImageProviders/provider_type_register.h"
#include "Display/ImageProviders/targa_provider.h"
#include "Display/Render/blend_mode.h"
#include "Display/Render/buffer_control.h"
#include "Display/Render/compare_function.h"
#include "Display/Render/element_array_buffer.h"
#include "Display/Render/frame_buffer.h"
#include "Display/Render/graphic_context.h"
#include "Display/Render/occlusion_query.h"
#include "Display/Render/pen.h"
#include "Display/Render/polygon_rasterizer.h"
#include "Display/Render/primitives_array.h"
#include "Display/Render/primitives_array_builder.h"
#include "Display/Render/program_attribute.h"
#include "Display/Render/program_object.h"
#include "Display/Render/program_uniform.h"
#include "Display/Render/render_batcher.h"
#include "Display/Render/render_buffer.h"
#include "Display/Render/shader_object.h"
#include "Display/Render/shared_gc_data.h"
#include "Display/Render/texture.h"
#include "Display/Render/vertex_array_buffer.h"
#include "Display/TargetProviders/cursor_provider.h"
#include "Display/TargetProviders/display_target_provider.h"
#include "Display/TargetProviders/display_window_provider.h"
#include "Display/TargetProviders/element_array_buffer_provider.h"
#include "Display/TargetProviders/pixel_buffer_provider.h"
#include "Display/TargetProviders/font_provider.h"
#include "Display/TargetProviders/frame_buffer_provider.h"
#include "Display/TargetProviders/graphic_context_provider.h"
#include "Display/TargetProviders/input_device_provider.h"
#include "Display/TargetProviders/occlusion_query_provider.h"
#include "Display/TargetProviders/program_object_provider.h"
#include "Display/TargetProviders/render_buffer_provider.h"
#include "Display/TargetProviders/render_window_provider.h"
#include "Display/TargetProviders/shader_object_provider.h"
#include "Display/TargetProviders/texture_provider.h"
#include "Display/TargetProviders/vertex_array_buffer_provider.h"
#include "Display/Window/cursor.h"
#include "Display/Window/display_window.h"
#include "Display/Window/display_window_description.h"
#include "Display/Window/input_context.h"
#include "Display/Window/input_device.h"
#include "Display/Window/input_event.h"
#include "Display/Window/input_state.h"
#include "Display/Window/keys.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(CL_DLL)
			#pragma comment(lib, "clanDisplay-dll.lib")
			#pragma comment(lib, "libjpeg-dll.lib")
			#pragma comment(lib, "libpng-dll.lib")
			#pragma comment(lib, "freetype-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanDisplay-static-mtdll.lib")
			#pragma comment(lib, "libjpeg-static-mtdll.lib")
			#pragma comment(lib, "libpng-static-mtdll.lib")
			#pragma comment(lib, "freetype-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanDisplay-static-mt.lib")
			#pragma comment(lib, "libjpeg-static-mt.lib")
			#pragma comment(lib, "libpng-static-mt.lib")
			#pragma comment(lib, "freetype-static-mt.lib")
		#endif
	#else
		#if defined(CL_DLL)
			#pragma comment(lib, "clanDisplay-dll-debug.lib")
			#pragma comment(lib, "libjpeg-dll-debug.lib")
			#pragma comment(lib, "libpng-dll-debug.lib")
			#pragma comment(lib, "freetype-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanDisplay-static-mtdll-debug.lib")
			#pragma comment(lib, "libjpeg-static-mtdll-debug.lib")
			#pragma comment(lib, "libpng-static-mtdll-debug.lib")
			#pragma comment(lib, "freetype-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanDisplay-static-mt-debug.lib")
			#pragma comment(lib, "libjpeg-static-mt-debug.lib")
			#pragma comment(lib, "libpng-static-mt-debug.lib")
			#pragma comment(lib, "freetype-static-mt-debug.lib")
		#endif
	#endif
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "dinput8.lib")
	#pragma comment(lib, "dxguid.lib")
#endif
