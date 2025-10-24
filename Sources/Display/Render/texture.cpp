/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/Render/texture.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_format.h"
#include "API/Display/2D/color.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_format.h"
#include "graphic_context_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Texture_Impl Class:

class CL_Texture_Impl
{
public:
	CL_Texture_Impl()
	: provider(0),
	width(0),
	height(0),
	min_lod(0),
	max_lod(0),
	lod_bias(0),
	base_level(0),
	max_level(0),
	wrap_mode_s(cl_wrap_clamp_to_edge),
	wrap_mode_t(cl_wrap_clamp_to_edge),
	wrap_mode_r(cl_wrap_clamp_to_edge),
	min_filter(cl_filter_linear),
	mag_filter(cl_filter_linear),
	max_anisotropy(1.0f),
	resident(0),
	compare_mode(cl_comparemode_none),
	compare_function(cl_comparefunc_lequal)
	{
	}

	~CL_Texture_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_TextureProvider *provider;
	int width, height;
	float min_lod;
	float max_lod;
	float lod_bias;
	int base_level;
	int max_level;
	CL_TextureWrapMode wrap_mode_s;
	CL_TextureWrapMode wrap_mode_t;
	CL_TextureWrapMode wrap_mode_r;
	CL_TextureFilter min_filter;
	CL_TextureFilter mag_filter;
	float max_anisotropy;
	bool resident;
	CL_TextureCompareMode compare_mode;
	CL_CompareFunction compare_function;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Construction:

CL_Texture::CL_Texture()
{
}

CL_Texture::CL_Texture(CL_GraphicContext &context, CL_TextureDimensions texture_dimensions)
 : impl(new CL_Texture_Impl)
{
	CL_GraphicContextProvider *gc_provider = context.get_provider();
	if (gc_provider == NULL)
	{
		throw CL_Exception("todo: if gc_provider is null, use CL_Display::get_current_gc()->get_provider()");
	}
	impl->provider = gc_provider->alloc_texture(texture_dimensions);

	impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
}

CL_Texture::CL_Texture(CL_GraphicContext &context, const CL_Size &size, CL_TextureFormat internal_format)
: impl(new CL_Texture_Impl)
{
	if ( (size.width<=0) || (size.height<=0) )
	{
		throw CL_Exception("An attempt was made to create a CL_Texture with an invalid size");
	}

	CL_GraphicContextProvider *gc_provider = context.get_provider();
	if (gc_provider == NULL)
	{
		throw CL_Exception("todo: if gc_provider is null, use CL_Display::get_current_gc()->get_provider()");
	}

	impl->provider = gc_provider->alloc_texture(cl_texture_2d);
	impl->provider->create(size.width, size.height, internal_format, 1);
	impl->width = size.width;
	impl->height = size.height;

	impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
}

CL_Texture::CL_Texture(CL_GraphicContext &context, int width, int height, CL_TextureFormat internal_format)
: impl(new CL_Texture_Impl)
{
	if ( (width<=0) || (height<=0) )
	{
		throw CL_Exception("An attempt was made to create a CL_Texture with an invalid size");
	}

	CL_GraphicContextProvider *gc_provider = context.get_provider();
	if (gc_provider == NULL)
	{
		throw CL_Exception("todo: if gc_provider is null, use CL_Display::get_current_gc()->get_provider()");
	}

	impl->provider = gc_provider->alloc_texture(cl_texture_2d);
	impl->provider->create(width, height, internal_format, 1);
	impl->width = width;
	impl->height = height;

	impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
}

CL_Texture::CL_Texture(CL_GraphicContext &context, int width, int height, int depth, CL_TextureFormat internal_format)
: impl(new CL_Texture_Impl)
{
	if ( (width<=0) || (height<=0) || (depth<=0) )
	{
		throw CL_Exception("An attempt was made to create a CL_Texture with an invalid size");
	}

	CL_GraphicContextProvider *gc_provider = context.get_provider();
	if (gc_provider == NULL)
	{
		throw CL_Exception("todo: if gc_provider is null, use CL_Display::get_current_gc()->get_provider()");
	}

	impl->provider = gc_provider->alloc_texture(cl_texture_3d);
	impl->provider->create(width, height, internal_format, depth);
	impl->width = width;
	impl->height = height;
//	impl->depth = depth;

	impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
}

CL_Texture::CL_Texture(
	CL_GraphicContext &context,
	const CL_StringRef &filename,
	const CL_VirtualDirectory &directory, const CL_ImageImportDescription &import_desc)
{
	CL_PixelBuffer pb = CL_ImageProviderFactory::load(filename, directory, CL_String());
	pb = import_desc.process(pb);
	
	*this = CL_Texture(context, pb.get_width(), pb.get_height());

	set_subimage(CL_Point(0, 0), pb, CL_Rect(pb.get_size()), 0);

	impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
}

CL_Texture::CL_Texture(
	CL_GraphicContext &context,
	const CL_StringRef &fullname, const CL_ImageImportDescription &import_desc)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	*this = CL_Texture(context, filename, vfs.get_root_directory(), import_desc );
}

CL_Texture::CL_Texture(
		CL_GraphicContext &context,
		CL_IODevice &file, const CL_String &image_type, const CL_ImageImportDescription &import_desc)
{
	CL_PixelBuffer pb = CL_ImageProviderFactory::load(file, image_type);
	pb = import_desc.process(pb);
	*this = CL_Texture(context, pb.get_width(), pb.get_height());

	set_subimage(CL_Point(0, 0), pb, CL_Rect(pb.get_size()), 0);

	impl->provider->set_wrap_mode(impl->wrap_mode_s, impl->wrap_mode_t, impl->wrap_mode_r);
}

CL_Texture::CL_Texture(
	const CL_StringRef &resource_id,
	CL_ResourceManager *resources,
	CL_GraphicContext &gc, const CL_ImageImportDescription &import_desc)
{
	CL_Resource resource = resources->get_resource(resource_id);
	CL_String type = resource.get_element().get_tag_name();
	
	if (type != "texture")
		throw CL_Exception(cl_format("Resource '%1' is not of type 'texture'", resource_id));

	CL_String filename = resource.get_element().get_attribute("file");
	CL_VirtualDirectory directory = resource.get_manager().get_directory(resource);
	*this = CL_Texture(gc, filename, directory, import_desc);
}

CL_Texture::CL_Texture(CL_SharedPtr<CL_Texture_Impl> &impl) : impl(impl)
{
}


CL_Texture::~CL_Texture()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Attributes:

void CL_Texture::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_Texture is null");
}

int CL_Texture::get_width() const
{
	return impl->width;
}

int CL_Texture::get_height() const
{
	return impl->height;
}

CL_Size CL_Texture::get_size() const
{
	return CL_Size(impl->width, impl->height);
}

int CL_Texture::get_depth() const
{
	throw CL_Exception("CL_Texture::get_depth() is not implemented");
}

CL_PixelBuffer CL_Texture::get_pixeldata(int level) const 
{
	// todo: pixel format rgba8888?
	return impl->provider->get_pixeldata(cl_rgba8, level); 
}

CL_PixelBuffer CL_Texture::get_pixeldata(CL_TextureFormat sized_format, int level) const 
{
	return impl->provider->get_pixeldata(sized_format, level);
}

float CL_Texture::get_min_lod() const
{
	return impl->min_lod;
}

float CL_Texture::get_max_lod() const
{
	return impl->max_lod;
}

float CL_Texture::get_lod_bias() const
{
	return impl->lod_bias;
}

int CL_Texture::get_base_level() const
{
	return impl->base_level;
}

int CL_Texture::get_max_level() const
{
	return impl->max_level;
}

CL_TextureWrapMode CL_Texture::get_wrap_mode_s() const
{
	return impl->wrap_mode_s;
}

CL_TextureWrapMode CL_Texture::get_wrap_mode_t() const
{
	return impl->wrap_mode_t;
}

CL_TextureWrapMode CL_Texture::get_wrap_mode_r() const
{
	return impl->wrap_mode_r;
}

CL_TextureFilter CL_Texture::get_min_filter() const
{
	return impl->min_filter;
}

CL_TextureFilter CL_Texture::get_mag_filter() const
{
	return impl->mag_filter;
}

bool CL_Texture::is_resident() const
{
	return impl->resident;
}

CL_TextureCompareMode CL_Texture::get_compare_mode() const
{
	return impl->compare_mode;
}

CL_CompareFunction CL_Texture::get_compare_function() const
{
	return impl->compare_function;
}

CL_TextureProvider *CL_Texture::get_provider() const
{
	if (impl.is_null())
		return 0;
	else
		return impl->provider;
}

CL_WeakPtr<CL_Texture_Impl> CL_Texture::get_impl() const
{
	return CL_WeakPtr<CL_Texture_Impl>(impl);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Operations:

void CL_Texture::generate_mipmap()
{
	impl->provider->generate_mipmap();
}

void CL_Texture::set_image(
	CL_PixelBuffer &image,
	int level)
{
	impl->provider->set_image(image, level);
	impl->width = image.get_width();
	impl->height = image.get_height();
}

void CL_Texture::set_cube_map(
	CL_PixelBuffer &cube_map_positive_x,
	CL_PixelBuffer &cube_map_negative_x,
	CL_PixelBuffer &cube_map_positive_y,
	CL_PixelBuffer &cube_map_negative_y,
	CL_PixelBuffer &cube_map_positive_z,
	CL_PixelBuffer &cube_map_negative_z,
	int level)
{
	impl->provider->set_cube_map(
		cube_map_positive_x,
		cube_map_negative_x,
		cube_map_positive_y,
		cube_map_negative_y,
		cube_map_positive_z,
		cube_map_negative_z,
		level);
	impl->width = cube_map_positive_x.get_width();
	impl->height = cube_map_positive_x.get_height();
}

void CL_Texture::set_compressed_image(
	int level,
	CL_TextureFormat internal_format,
	int width,
	int height,
	CL_DataBuffer &image)
{
	impl->provider->set_compressed_image(level, internal_format, width, height, image);
	impl->width = width;
	impl->height = height;
}

void CL_Texture::set_subimage(
	int x,
	int y,
	const CL_PixelBuffer &image,
	const CL_Rect &src_rect,

	int level)
{
	impl->provider->set_subimage(x, y, image, src_rect, level);
}

void CL_Texture::set_subimage(
	const CL_Point &point,
	const CL_PixelBuffer &image,
	const CL_Rect &src_rect,
	int level)
{
	impl->provider->set_subimage(point.x, point.y, image, src_rect, level);
}

void CL_Texture::copy_image_from(
	CL_GraphicContext &context,
	int level,
	CL_TextureFormat internal_format)
{
	impl->provider->copy_image_from(0,0,impl->width,impl->height,level,internal_format,context.get_provider());
}

void CL_Texture::copy_image_from(
	CL_GraphicContext &context,
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_TextureFormat internal_format)
{
	impl->provider->copy_image_from(x,y,width,height,level,internal_format,context.get_provider());
}
	
void CL_Texture::copy_image_from(
	CL_GraphicContext &context,
	const CL_Rect &pos,
	int level,
	CL_TextureFormat internal_format)
{
	impl->provider->copy_image_from(pos.left,pos.top,pos.get_width(),pos.get_height(),level, internal_format,context.get_provider());
}

void CL_Texture::copy_subimage_from(
	CL_GraphicContext &context,
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level)
{
	impl->provider->copy_subimage_from(offset_x, offset_y, x, y, width, height, level, context.get_provider() );
}

void CL_Texture::copy_subimage_from(
	CL_GraphicContext &context,
	const CL_Point &offset,
	const CL_Rect &pos,
	int level)
{
	impl->provider->copy_subimage_from(offset.x, offset.y, pos.left, pos.top, pos.get_width(), pos.get_height(), level, context.get_provider() );
}

void CL_Texture::set_min_lod(float min_lod)
{
	if( impl->min_lod != min_lod )
	{
		impl->provider->set_min_lod(min_lod);
		impl->min_lod = min_lod;
	}
}

void CL_Texture::set_max_lod(float max_lod)
{
	if( impl->max_lod != max_lod )
	{
		impl->provider->set_max_lod(max_lod);
		impl->max_lod = max_lod;
	}
}

void CL_Texture::set_lod_bias(float lod_bias)
{
	if( impl->lod_bias != lod_bias )
	{
		impl->provider->set_lod_bias(lod_bias);
		impl->lod_bias = lod_bias;
	}
}

void CL_Texture::set_base_level(int base_level)
{
	if( impl->base_level != base_level )
	{
		impl->provider->set_base_level(base_level);
		impl->base_level = base_level;
	}
}

void CL_Texture::set_max_level(int max_level)
{
	if( impl->max_level != max_level )
	{
		impl->provider->set_max_level(max_level);
		impl->max_level = max_level;
	}
}

void CL_Texture::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t,
	CL_TextureWrapMode wrap_r)
{
	if( impl->wrap_mode_s != wrap_s || impl->wrap_mode_t != wrap_t || impl->wrap_mode_r != wrap_r )
	{
		impl->provider->set_wrap_mode(wrap_s, wrap_t, wrap_r);
		impl->wrap_mode_s = wrap_s;
		impl->wrap_mode_t = wrap_t;
		impl->wrap_mode_r = wrap_r;
	}
}

void CL_Texture::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t)
{
	if( impl->wrap_mode_s != wrap_s || impl->wrap_mode_t != wrap_t  )
	{
		impl->provider->set_wrap_mode(wrap_s, wrap_t);
		impl->wrap_mode_s = wrap_s;
		impl->wrap_mode_t = wrap_t;
	}
}

void CL_Texture::set_wrap_mode(
	CL_TextureWrapMode wrap_s)
{
	if( impl->wrap_mode_s != wrap_s )
	{
		impl->provider->set_wrap_mode(wrap_s);
		impl->wrap_mode_s = wrap_s;
	}
}

void CL_Texture::set_min_filter(CL_TextureFilter min_filter)
{
	if( impl->min_filter != min_filter )
	{
		impl->provider->set_min_filter(min_filter);
		impl->min_filter = min_filter;
	}
}

void CL_Texture::set_mag_filter(CL_TextureFilter mag_filter)
{
	if( impl->mag_filter != mag_filter )
	{
		impl->provider->set_mag_filter(mag_filter);
		impl->mag_filter = mag_filter;
	}
}

void CL_Texture::set_max_anisotropy(float max_anisotropy)
{
	if( impl->max_anisotropy != max_anisotropy )
	{
		impl->provider->set_max_anisotropy(max_anisotropy);
		impl->max_anisotropy = max_anisotropy;
	}
}

void CL_Texture::set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func)
{
	if( impl->compare_mode != mode || impl->compare_function != func )
	{
		impl->compare_mode = mode;
		impl->compare_function = func;
		impl->provider->set_texture_compare(mode,func);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Texture Implementation:
