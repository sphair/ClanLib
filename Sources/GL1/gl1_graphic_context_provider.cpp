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
**    Harry Storbacka
**    Ken Hirsch
**    Claudio Zanella
*/

#include "GL1/precomp.h"
#include "gl1_graphic_context_provider.h"
#include "gl1_texture_provider.h"
#include "gl1_frame_buffer_provider.h"
#include "gl1_render_buffer_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/pen.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/TargetProviders/render_window_provider.h"
#include "API/GL1/light_model.h"
#include "API/GL1/light_source.h"
#include "API/GL1/material.h"
#include "API/GL1/texture_unit.h"
#include "API/GL1/opengl1.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/Display/2D/image.h"

#ifdef WIN32
#include "WGL/gl1_window_provider_wgl.h"
#else
#include "GLX/gl1_window_provider_glx.h"
#include "GLX/pbuffer_impl.h"
#endif
#include "pbuffer.h"

#include "../Display/2D/render_batch2d.h"
#include "../Display/2D/render_batch3d.h"

class CL_GL1SelectedTexture
{
public:
	CL_GL1SelectedTexture() : texture(NULL) {};

	CL_GL1TextureProvider *texture;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GL1GraphicContextProvider Construction:

CL_GL1GraphicContextProvider::CL_GL1GraphicContextProvider(const CL_RenderWindowProvider * const render_window)
: render_window(render_window), map_mode(cl_map_2d_upper_left), projection(CL_Mat4f::identity()), modelview(CL_Mat4f::identity()),
  framebuffer_bound(false), prim_arrays_set(false), cur_prim_array(0), num_set_tex_arrays(0),
  primitives_array_texture_set(false), primitives_array_texindex_set(false)
{
	check_opengl_version();
	max_texture_coords = get_max_texture_coords();
	// Hack, so the sprite render batcher does not exceed the allowed number of textures
	if (max_texture_coords < CL_RenderBatch2D::max_textures)
	{
		if (max_texture_coords > 0)
		{
			CL_RenderBatch2D::max_textures = max_texture_coords;
			CL_RenderBatch3D::max_textures = max_texture_coords;
		}
		else
		{
			CL_RenderBatch2D::max_textures = 1;
			CL_RenderBatch3D::max_textures = 1;
		}
	}
	// Limit the internal texture coords, to avoid situations where the opengl driver says there are unlimited texture coords
	if (max_texture_coords > 255)
		max_texture_coords = 255;

	selected_textures.resize(max_texture_coords);
}

CL_GL1GraphicContextProvider::~CL_GL1GraphicContextProvider()
{
	while (!disposable_objects.empty())
		disposable_objects.front()->dispose();

	CL_GL1::remove_active(this);
	delete render_window;
}

void CL_GL1GraphicContextProvider::add_disposable(CL_DisposableObject *disposable)
{
	disposable_objects.push_back(disposable);
}

void CL_GL1GraphicContextProvider::remove_disposable(CL_DisposableObject *disposable)
{
	for (size_t i = 0; i < disposable_objects.size(); i++)
	{
		if (disposable_objects[i] == disposable)
		{
			disposable_objects.erase(disposable_objects.begin() + i);
			return;
		}
	}
}

void CL_GL1GraphicContextProvider::check_opengl_version()
{
	int version_major = 0;
	int version_minor = 0;
	int version_release = 0;

	get_opengl_version(version_major, version_minor, version_release);
	bool valid_version = false;
	if (version_major > 1)
		valid_version = true;
	if ( (version_major == 1) && (version_minor >= 3) )
		valid_version = true;

	if(!valid_version)
		throw CL_Exception(cl_format("This application requires OpenGL 1.3. Your hardware only supports OpenGL %1.%2.%3. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor, version_release));
}

void CL_GL1GraphicContextProvider::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
/*	The GL_VERSION string begins with a version number. The version number uses one of these forms: 
	major_number.minor_number 
	major_number.minor_number.release_number 
	Vendor-specific information may follow the version number. Its format depends on the implementation, but a space always separates the version number and the vendor-specific information. 
	All strings are null-terminated. 
	If an error is generated, glGetString returns zero.
*/
	CL_GL1::set_active(this);
	CL_String version = (char*)cl1GetString(GL_VERSION);
	version_major = 0;
	version_minor = 0;
	version_release = 0;

	std::vector<CL_String> split_version = CL_StringHelp::split_text(version, ".");
	if(split_version.size() > 0)
		version_major = CL_StringHelp::text_to_int(split_version[0]);
	if(split_version.size() > 1)
		version_minor = CL_StringHelp::text_to_int(split_version[1]);
	if(split_version.size() > 2)
		version_release = CL_StringHelp::text_to_int(split_version[2]);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1GraphicContextProvider Attributes:

int CL_GL1GraphicContextProvider::get_max_texture_coords()
{
	set_active();
	GLint max_textures = 0;
	cl1GetIntegerv(GL_MAX_TEXTURE_COORDS, &max_textures);

	if(cl1GetError() == GL_INVALID_ENUM)
		cl1GetIntegerv(GL_MAX_TEXTURE_UNITS, &max_textures);

	return max_textures;
}

int CL_GL1GraphicContextProvider::get_max_attributes()
{
	set_active();
	GLint max_attributes = 0;
	cl1GetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
    if(max_attributes < 16)
        max_attributes = 16;
	return max_attributes;
}

int CL_GL1GraphicContextProvider::get_light_count()
{
	set_active();
	GLint max_lights = 0;
	cl1GetIntegerv(GL_MAX_LIGHTS, &max_lights);
	return max_lights;
}

CL_Size CL_GL1GraphicContextProvider::get_max_texture_size() const
{
	set_active();
	GLint max_size = 0;
	cl1GetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
	return CL_Size(max_size, max_size);
}

int CL_GL1GraphicContextProvider::get_width() const
{
	if (framebuffer_bound)
		return framebuffer_provider->get_size().width;
	else
		return render_window->get_viewport_width();

}

CL_PBuffer_GL1 CL_GL1GraphicContextProvider::create_pbuffer(CL_Size size)
{
#ifdef WIN32
	const CL_RenderWindowProvider_WGL *wptr = dynamic_cast<const CL_RenderWindowProvider_WGL *> (render_window);
	if (wptr == NULL)
		throw CL_Exception("Render window type is not known");

	return ((CL_RenderWindowProvider_WGL *) wptr)->get_window().create_pbuffer(this, size);
#else
	const CL_GL1_RenderWindowProvider_GLX *wptr = dynamic_cast<const CL_GL1_RenderWindowProvider_GLX *> (render_window);
	if (wptr == NULL)
		throw CL_Exception("Render window type is not known");

	return ((CL_GL1_RenderWindowProvider_GLX *) wptr)->get_window().create_pbuffer(this, size);
#endif

}

int CL_GL1GraphicContextProvider::get_height() const
{
	if (framebuffer_bound)
		return framebuffer_provider->get_size().height;
	else
		return render_window->get_viewport_height();
}

#ifdef __APPLE__
static CFBundleRef cl_gl1_gBundleRefOpenGL = 0;
#endif

CL_GL1ProcAddress *CL_GL1GraphicContextProvider::get_proc_address(const CL_String8& function_name) const
{

#ifdef WIN32
	return (void (*)())wglGetProcAddress(function_name.c_str());
#else
#ifdef __APPLE__
	// Mac OS X doesn't have an OpenGL extension fetch function. Isn't that silly?
	if (cl_gl1_gBundleRefOpenGL == 0)
	{
		cl_gl1_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
		if (cl_gl1_gBundleRefOpenGL == 0)
			throw CL_Exception("Unable to find com.apple.opengl bundle");
	}

	return (CL_GL1ProcAddress *) CFBundleGetFunctionPointerForName(
		cl_gl1_gBundleRefOpenGL,
		CFStringCreateWithCStringNoCopy(
			0,
			function_name.c_str(),
			CFStringGetSystemEncoding(),
			0));
#else
	// FIXME: This is very messy
	const CL_GL1_RenderWindowProvider_GLX *wptr = dynamic_cast<const CL_GL1_RenderWindowProvider_GLX *> (render_window);
	if (wptr)
		return wptr->get_proc_address(function_name);

	const CL_RenderWindowProvider_GLX_PBuffer *pptr = dynamic_cast<const CL_RenderWindowProvider_GLX_PBuffer *> (render_window);
	if (pptr)
		return pptr->get_proc_address(function_name);

	return NULL;

#endif
#endif

}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1GraphicContextProvider Operations:

void CL_GL1GraphicContextProvider::set_active() const
{
	if (framebuffer_bound)
	{
		framebuffer_provider->set_active();
	}
	else
	{
		CL_GL1::set_active(this);
	}
}

void CL_GL1GraphicContextProvider::destroy()
{
	delete this;
}

CL_OcclusionQueryProvider *CL_GL1GraphicContextProvider::alloc_occlusion_query()
{
	throw CL_Exception("Occlusion Queries are not supported for OpenGL 1.3");
	return NULL;
}

CL_ProgramObjectProvider *CL_GL1GraphicContextProvider::alloc_program_object()
{
	throw CL_Exception("Program Objects are not supported for OpenGL 1.3");
	return NULL;
}

CL_ShaderObjectProvider *CL_GL1GraphicContextProvider::alloc_shader_object()
{
	throw CL_Exception("Shader Objects are not supported for OpenGL 1.3");
	return NULL;
}

CL_TextureProvider *CL_GL1GraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return new CL_GL1TextureProvider(texture_dimensions);
}

CL_FrameBufferProvider *CL_GL1GraphicContextProvider::alloc_frame_buffer()
{
	return new CL_GL1FrameBufferProvider(this);
}

CL_RenderBufferProvider *CL_GL1GraphicContextProvider::alloc_render_buffer()
{
	return new CL_GL1RenderBufferProvider(this);
}

CL_VertexArrayBufferProvider *CL_GL1GraphicContextProvider::alloc_vertex_array_buffer()
{
	throw CL_Exception("Vertex Array Buffers are not supported for OpenGL 1.3");
	return NULL;
}

CL_ElementArrayBufferProvider *CL_GL1GraphicContextProvider::alloc_element_array_buffer()
{
	throw CL_Exception("Element Array Buffers are not supported for Open GL 1.3");
	return NULL;
}


CL_PixelBufferProvider *CL_GL1GraphicContextProvider::alloc_pixel_buffer()
{
	throw CL_Exception("Pixel Buffers Objects are not supported for Open GL 1.3");
	return NULL;
}

CL_PixelBuffer CL_GL1GraphicContextProvider::get_pixeldata(const CL_Rect& rect2, CL_TextureFormat pixel_format, bool clamp) const
{
	CL_Rect rect = rect2;
	if (rect == CL_Rect())
		rect = CL_Rect(0, 0, get_width(), get_height());

	GLenum format;
	GLenum type;
	bool found = CL_GL1::to_opengl_pixelformat(pixel_format, format, type);
	if (!found)
		throw CL_Exception("Unsupported pixel format passed to CL_GraphicContext::get_pixeldata");

	CL_PixelBuffer pbuf(rect.get_width(), rect.get_height(), pixel_format);
	set_active();

	if (!framebuffer_bound)
		glReadBuffer(GL_BACK);
	//cl1ClampColor(GL_CLAMP_READ_COLOR, clamp ? GL_TRUE : GL_FALSE);
	cl1ReadPixels(rect.left, rect.top, rect.get_width(), rect.get_height(), format, type, pbuf.get_data());
	pbuf.flip_vertical();
	return pbuf;
}

void CL_GL1GraphicContextProvider::set_texture_unit(int unit_index, const CL_TextureUnit_GL1 &unit)
{
	set_active();

	cl1ActiveTexture( GL_TEXTURE0 + unit_index );
	
	cl1TexEnvf( GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, (float)unit.get_lod_bias() );

	float color_array[4];
	CL_Colorf color = unit.get_color();
	color_array[0] = color.r;
	color_array[1] = color.g;
	color_array[2] = color.b;
	color_array[3] = color.a;

	cl1TexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color_array);
	cl1TexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB, to_enum(unit.get_combine_rgb_mode()) );
	cl1TexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA, to_enum(unit.get_combine_alpha_mode()) );
	cl1TexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, to_enum(unit.get_mode()));

	cl1TexEnvi( GL_TEXTURE_ENV, GL_SRC0_RGB, to_enum(unit.get_combine_arg_src_rgb(0)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_SRC1_RGB, to_enum(unit.get_combine_arg_src_rgb(1)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_SRC2_RGB, to_enum(unit.get_combine_arg_src_rgb(2)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_SRC0_ALPHA, to_enum(unit.get_combine_arg_src_alpha(0)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_SRC1_ALPHA, to_enum(unit.get_combine_arg_src_alpha(1)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_SRC2_ALPHA, to_enum(unit.get_combine_arg_src_alpha(2)));

	cl1TexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB, to_enum(unit.get_combine_arg_operand_rgb(0)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB, to_enum(unit.get_combine_arg_operand_rgb(1)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB, to_enum(unit.get_combine_arg_operand_rgb(2)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, to_enum(unit.get_combine_arg_operand_alpha(0)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, to_enum(unit.get_combine_arg_operand_alpha(1)));
	cl1TexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, to_enum(unit.get_combine_arg_operand_alpha(2)));

	cl1TexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, unit.get_point_coord_replace() ? GL_TRUE : GL_FALSE);
}


void CL_GL1GraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
	set_active();

	if ( (unit_index <0) || (unit_index >= max_texture_coords) )
	{
		throw CL_Exception("Invalid texture unit index in GL1 target");
	}
	selected_textures[unit_index].texture = NULL;

	if (cl1ActiveTexture != 0)
	{
		cl1ActiveTexture( GL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}

	if (texture.is_null())
	{
		cl1Disable(GL_TEXTURE_1D);
		cl1Disable(GL_TEXTURE_2D);
		cl1Disable(GL_TEXTURE_3D);
		cl1Disable(GL_TEXTURE_CUBE_MAP);
	}
	else
	{
		CL_GL1TextureProvider *provider = static_cast<CL_GL1TextureProvider *>(texture.get_provider());
		selected_textures[unit_index].texture = provider;

		cl1Enable(provider->get_texture_type());
		cl1BindTexture(provider->get_texture_type(), provider->get_handle());
	}
}

void CL_GL1GraphicContextProvider::reset_texture(int unit_index, const CL_Texture &texture)
{
	set_active();

	if ( (unit_index >=0) && (unit_index < max_texture_coords) )
	{
		selected_textures[unit_index].texture = NULL;
	}

	if (cl1ActiveTexture != 0)
	{
		cl1ActiveTexture( GL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}

	cl1Disable(GL_TEXTURE_1D);
	cl1Disable(GL_TEXTURE_2D);
	cl1Disable(GL_TEXTURE_3D);
	cl1Disable(GL_TEXTURE_CUBE_MAP);
	cl1BindTexture(GL_TEXTURE_2D, 0);

}

void CL_GL1GraphicContextProvider::push_texture_matrix(int unit_index)
{
	set_active();
	cl1MatrixMode(GL_TEXTURE);
	cl1ActiveTexture( GL_TEXTURE0 + unit_index );
	cl1PushMatrix();
	cl1MatrixMode(GL_MODELVIEW);
}

void CL_GL1GraphicContextProvider::set_texture_matrix(int unit_index, const CL_Mat4f &matrix)
{
	set_active();
	cl1MatrixMode(GL_TEXTURE);
	cl1ActiveTexture( GL_TEXTURE0 + unit_index );
	cl1LoadMatrixf(matrix);
	cl1MatrixMode(GL_MODELVIEW);
}

void CL_GL1GraphicContextProvider::pop_texture_matrix(int unit_index)
{
	set_active();
	cl1MatrixMode(GL_TEXTURE);
	cl1ActiveTexture( GL_TEXTURE0 + unit_index );
	cl1PopMatrix();
	cl1MatrixMode(GL_MODELVIEW);
}

void CL_GL1GraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &w_buffer, const CL_FrameBuffer &r_buffer)
{
	framebuffer_provider = dynamic_cast<CL_GL1FrameBufferProvider *>(w_buffer.get_provider());
	framebuffer_provider->set_active();
	framebuffer_provider->start();

	// Save the map mode before when the framebuffer was bound
	if (!framebuffer_bound)	
	{
		map_mode_before_framebuffer = map_mode;

		framebuffer_bound = true;
		if (map_mode != cl_user_projection)
			set_map_mode(map_mode);
	}
}

void CL_GL1GraphicContextProvider::reset_frame_buffer()
{

	if (framebuffer_bound)
	{
		framebuffer_bound = false;

		framebuffer_provider->set_active();
		framebuffer_provider->stop();
		CL_GL1::set_active(this);

		if (map_mode != cl_user_projection)
			set_map_mode(map_mode_before_framebuffer);
	}
}

void CL_GL1GraphicContextProvider::set_program_object(CL_StandardProgram standard_program)
{
}

void CL_GL1GraphicContextProvider::set_program_object(const CL_ProgramObject &program, int program_matrix_flags)
{
	throw CL_Exception("GLSL program objects are not supported on OpenGL 1.3");
}

void CL_GL1GraphicContextProvider::reset_program_object()
{
}

void CL_GL1GraphicContextProvider::set_light_model(const CL_LightModel_GL1 &light_model)
{
	set_active();

	if (light_model.is_lighting_enabled())
		cl1Enable(GL_LIGHTING);
	else
		cl1Disable(GL_LIGHTING);

	cl1LightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, light_model.is_local_viewer() ? GL_TRUE : GL_FALSE);
	cl1LightModeli(GL_LIGHT_MODEL_TWO_SIDE, light_model.get_two_sided_materials() ? GL_TRUE : GL_FALSE);

	switch (light_model.get_color_control())
	{
	case cl_color_control_single_color:
		cl1LightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
		break;
	case cl_color_control_separate_specular_color:
		cl1LightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		break;
	}

	if (light_model.get_flat_shading())
		cl1Disable(GL_SMOOTH);
	else
		cl1Enable(GL_SMOOTH);

	CL_Colorf color = light_model.get_scene_ambient_light();
	GLfloat color_array[4] = { color.r, color.g, color.b, color.a };
	cl1LightModelfv(GL_LIGHT_MODEL_AMBIENT, color_array);
}

void CL_GL1GraphicContextProvider::reset_light_model()
{
	set_active();
	cl1Disable(GL_LIGHTING);
}

void CL_GL1GraphicContextProvider::set_material(const CL_Material_GL1 &material)
{
	set_material(GL_FRONT_AND_BACK, material);
}

void CL_GL1GraphicContextProvider::set_front_material(const CL_Material_GL1 &material)
{
	set_material(GL_FRONT, material);
}

void CL_GL1GraphicContextProvider::set_back_material(const CL_Material_GL1 &material)
{
	set_material(GL_BACK, material);
}

void CL_GL1GraphicContextProvider::reset_front_material()
{
	CL_Material_GL1 material;
	set_front_material(material);
}

void CL_GL1GraphicContextProvider::reset_back_material()
{
	CL_Material_GL1 material;
	set_back_material(material);
}

void CL_GL1GraphicContextProvider::set_material(GLenum face, const CL_Material_GL1 &material)
{
	set_active();

	CL_Colorf color;
	GLfloat color_array[4];

	color = material.get_ambient();
	color_array[0] = (GLfloat) color.r;
	color_array[1] = (GLfloat) color.g;
	color_array[2] = (GLfloat) color.b;
	color_array[3] = (GLfloat) color.a;
	cl1Materialfv(face, GL_AMBIENT, color_array);

	color = material.get_diffuse();
	color_array[0] = (GLfloat) color.r;
	color_array[1] = (GLfloat) color.g;
	color_array[2] = (GLfloat) color.b;
	color_array[3] = (GLfloat) color.a;
	cl1Materialfv(face, GL_DIFFUSE, color_array);

	color = material.get_specular();
	color_array[0] = (GLfloat) color.r;
	color_array[1] = (GLfloat) color.g;
	color_array[2] = (GLfloat) color.b;
	color_array[3] = (GLfloat) color.a;
	cl1Materialfv(face, GL_SPECULAR, color_array);

	color = material.get_emission();
	color_array[0] = (GLfloat) color.r;
	color_array[1] = (GLfloat) color.g;
	color_array[2] = (GLfloat) color.b;
	color_array[3] = (GLfloat) color.a;
	cl1Materialfv(face, GL_EMISSION, color_array);

	cl1Materialf(face, GL_SHININESS, (GLfloat) material.get_shininess());
}

void CL_GL1GraphicContextProvider::reset_material()
{
	CL_Material_GL1 material;
	set_material(material);
}

void CL_GL1GraphicContextProvider::set_color_material(CL_ColorMaterial_GL1 color)
{
	set_color_material(GL_FRONT_AND_BACK, color);
}

void CL_GL1GraphicContextProvider::set_color_material_front(CL_ColorMaterial_GL1 color)
{
	set_color_material(GL_FRONT_AND_BACK, color);
}

void CL_GL1GraphicContextProvider::set_color_material_back(CL_ColorMaterial_GL1 color)
{
	set_color_material(GL_FRONT_AND_BACK, color);
}

void CL_GL1GraphicContextProvider::reset_color_material()
{
	set_active();
	cl1Disable(GL_COLOR_MATERIAL);
}

void CL_GL1GraphicContextProvider::set_color_material(GLenum face, CL_ColorMaterial_GL1 color)
{
	set_active();
	cl1Enable(GL_COLOR_MATERIAL);
	switch (color)
	{
	case cl_color_material_emission:
		cl1ColorMaterial(face, GL_EMISSION);
		break;
	case cl_color_material_ambient:
		cl1ColorMaterial(face, GL_AMBIENT);
		break;
	case cl_color_material_diffuse:
		cl1ColorMaterial(face, GL_DIFFUSE);
		break;
	case cl_color_material_specular:
		cl1ColorMaterial(face, GL_SPECULAR);
		break;
	case cl_color_material_ambient_and_diffuse:
		cl1ColorMaterial(face, GL_AMBIENT_AND_DIFFUSE);
		break;
	}
}

void CL_GL1GraphicContextProvider::set_light(int light_index, const CL_LightSource_GL1 &light)
{
	set_active();
	cl1Enable(GL_LIGHT0 + light_index);

	cl1Lightf(GL_LIGHT0 + light_index, GL_SPOT_EXPONENT, (GLfloat) light.get_spot_exponent());
	cl1Lightf(GL_LIGHT0 + light_index, GL_SPOT_CUTOFF, (GLfloat) light.get_spot_cutoff());
	cl1Lightf(GL_LIGHT0 + light_index, GL_CONSTANT_ATTENUATION, (GLfloat) light.get_constant_attenuation());
	cl1Lightf(GL_LIGHT0 + light_index, GL_LINEAR_ATTENUATION, (GLfloat) light.get_linear_attenuation());
	cl1Lightf(GL_LIGHT0 + light_index, GL_QUADRATIC_ATTENUATION, (GLfloat) light.get_quadratic_attenuation());

	CL_Colorf c;
	GLfloat cf[4];

	c = light.get_ambient_intensity();
	cf[0] = (GLfloat) c.r; cf[1] = (GLfloat) c.g; cf[2] = (GLfloat) c.b; cf[3] = (GLfloat) c.a;
	cl1Lightfv(GL_LIGHT0 + light_index, GL_AMBIENT, cf);

	c = light.get_diffuse_intensity();
	cf[0] = (GLfloat) c.r; cf[1] = (GLfloat) c.g; cf[2] = (GLfloat) c.b; cf[3] = (GLfloat) c.a;
	cl1Lightfv(GL_LIGHT0 + light_index, GL_DIFFUSE, cf);

	c = light.get_specular_intensity();
	cf[0] = (GLfloat) c.r; cf[1] = (GLfloat) c.g; cf[2] = (GLfloat) c.b; cf[3] = (GLfloat) c.a;
	cl1Lightfv(GL_LIGHT0 + light_index, GL_SPECULAR, cf);

	CL_Vec4f v = light.get_position();
	cf[0] = (GLfloat) v.x;
	cf[1] = (GLfloat) v.y;
	cf[2] = (GLfloat) v.z;
	cf[3] = (GLfloat) v.w;
	cl1Lightfv(GL_LIGHT0 + light_index, GL_POSITION, cf);

	v = light.get_spot_direction();
	cf[0] = (GLfloat) v.x;
	cf[1] = (GLfloat) v.y;
	cf[2] = (GLfloat) v.z;
	cl1Lightfv(GL_LIGHT0 + light_index, GL_SPOT_DIRECTION, cf);
}

void CL_GL1GraphicContextProvider::reset_light(int light_index)
{
	set_active();
	cl1Disable(GL_LIGHT0 + light_index);
}

void CL_GL1GraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void CL_GL1GraphicContextProvider::primitives_array_freed(const CL_PrimitivesArrayData * const prim_array)
{
	if (cur_prim_array == prim_array)
	{
		reset_primitives_array();
		cur_prim_array = 0;
	}
}

void CL_GL1GraphicContextProvider::set_primitives_array(const CL_PrimitivesArrayData * const prim_array)
{
	if (prim_arrays_set)
		reset_primitives_array();
	set_active();
	prim_arrays_set = true;

	num_set_tex_arrays = 0;

	for(int attrib_count=0; attrib_count<prim_array->num_attributes; attrib_count++)
	{
		const CL_PrimitivesArrayData::VertexData &attribute = prim_array->attributes[attrib_count];

		int attribute_index = prim_array->attribute_indexes[attrib_count];

		switch(attribute_index)
		{
			case 0: // POSITION
				if (attribute.single_value)
				{
					throw CL_Exception("Implement me!");
				}
				else if (attribute.data)
				{
					cl1EnableClientState(GL_VERTEX_ARRAY);
					cl1VertexPointer(attribute.size, to_enum(attribute.type),  attribute.stride,  attribute.data);
				}
				else if (attribute.array_provider)
				{
					throw CL_Exception("BindBuffer is not available for OpenGL 1.3");
					//cl1BindBuffer(GL_ARRAY_BUFFER, static_cast<CL_GL1VertexArrayBufferProvider *>(attribute.array_provider)->get_handle());
					//cl1EnableClientState(GL_VERTEX_ARRAY);
					//cl1VertexPointer(attribute.size, to_enum(attribute.type),  attribute.stride,  attribute.data);
					//cl1BindBuffer(GL_ARRAY_BUFFER, 0);
				}
				break;
			case 1: // COLOR
				if (attribute.single_value)
				{
					switch (attribute.size)
					{
					case 3:
						switch (attribute.type)
						{
						case cl_type_unsigned_byte: cl1Color3ubv((GLubyte *) attribute.value_ubyte); break;
						case cl_type_unsigned_short: cl1Color3usv(attribute.value_ushort); break;
						case cl_type_unsigned_int: cl1Color3uiv(attribute.value_uint); break;
						case cl_type_byte: cl1Color3bv((GLbyte *) attribute.value_byte); break;
						case cl_type_short: cl1Color3sv(attribute.value_short); break;
						case cl_type_int: cl1Color3iv(attribute.value_int); break;
						case cl_type_float: cl1Color3fv(attribute.value_float); break;
						}
						break;
					case 4:
						switch (attribute.type)
						{
						case cl_type_unsigned_byte: cl1Color4ubv((GLubyte *) attribute.value_ubyte); break;
						case cl_type_unsigned_short: cl1Color4usv(attribute.value_ushort); break;
						case cl_type_unsigned_int: cl1Color4uiv(attribute.value_uint); break;
						case cl_type_byte: cl1Color4bv((GLbyte *) attribute.value_byte); break;
						case cl_type_short: cl1Color4sv(attribute.value_short); break;
						case cl_type_int: cl1Color4iv(attribute.value_int); break;
						case cl_type_float: cl1Color4fv(attribute.value_float); break;
						}
						break;
					}
				}
				else if (attribute.data)
				{
					cl1EnableClientState(GL_COLOR_ARRAY);
					cl1ColorPointer(attribute.size, to_enum(attribute.type),  attribute.stride,  attribute.data);
				}
				else if (attribute.array_provider)
				{
					throw CL_Exception("BindBuffer is not available for OpenGL 1.3");
				}

				break;
			case 2: // TEXTURE
				primitives_array_texture = attribute;
				primitives_array_texture_set = true;
				break;
			case 3: // TEXINDEX
				primitives_array_texindex = attribute;
				primitives_array_texindex_set = true;
				break;
			case 4: // NORMAL
				if (attribute.single_value)
				{
					switch (attribute.type)
					{
						case cl_type_byte: cl1Normal3bv((GLbyte *) attribute.value_byte); break;
						case cl_type_short: cl1Normal3sv(attribute.value_short); break;
						case cl_type_int: cl1Normal3iv(attribute.value_int); break;
						case cl_type_float: cl1Normal3fv(attribute.value_float); break;
					}
				}
				else if (attribute.data)
				{
					cl1EnableClientState(GL_NORMAL_ARRAY);
					cl1NormalPointer(to_enum(attribute.type),  attribute.stride,  attribute.data);
				}
				else if (attribute.array_provider)
				{
					throw CL_Exception("BindBuffer is not available for OpenGL 1.3");
				}
				break;
		}
	}
}

void CL_GL1GraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
	set_active();

	// Simple condition - No textures set
	if (!primitives_array_texture_set)
	{
		cl1DrawArrays(to_enum(type), offset, num_vertices);
		return;
	}

	if (primitives_array_texture.single_value)
	{
		throw CL_Exception("Implement me!");
	}

	if (primitives_array_texture.data == NULL)
	{
		// Must be the unsupported bindbuffer
		throw CL_Exception("BindBuffer is not available for OpenGL 1.3");
	}

	GLenum primitive_type = to_enum(type);

	int total_vertices = offset + num_vertices;

	// Normal condition - No texture index set
	if (!primitives_array_texindex_set)
	{
		set_primitive_texture( 0, primitives_array_texture, offset, num_vertices, total_vertices );
		cl1DrawArrays(primitive_type, offset, num_vertices);
		reset_primitive_texture( 0 );
		return;
	}

	// For code simplicity, disable all textures
	reset_primitive_texture_all();

	// Difficult condition - Draw all texture indexes
	while(num_vertices > 0)
	{
		// First, find these values...
		int texture_index;
		int num_vertices_in_group;

		if (primitives_array_texindex.size <= 0)
		{
			// Invalid texindex array (ignore)
			texture_index = 0;
			num_vertices_in_group = num_vertices;
		}
		else if (primitives_array_texindex.single_value)
		{
			// Contains a single texture
			num_vertices_in_group = num_vertices;
			switch(primitives_array_texindex.type)
			{
			case cl_type_float:
				texture_index = primitives_array_texindex.value_float[0];
				break;
			case cl_type_int:
				texture_index = primitives_array_texindex.value_int[0];
				break;
			case cl_type_unsigned_int:
				texture_index = primitives_array_texindex.value_uint[0];
				break;
			case cl_type_short:
				texture_index = primitives_array_texindex.value_short[0];
				break;
			case cl_type_unsigned_short:
				texture_index = primitives_array_texindex.value_ushort[0];
				break;
			case cl_type_unsigned_byte:
				texture_index = primitives_array_texindex.value_ubyte[0];
				break;
			case cl_type_byte:
				texture_index = primitives_array_texindex.value_byte[0];
				break;
			default:
				texture_index = 0;
				break;
			}
		}
		else if (primitives_array_texindex.data)
		{
			// Multiple textures possible
			switch(primitives_array_texindex.type)
			{
			case cl_type_float:
				{
					int stride_float;
					if (primitives_array_texindex.stride)
					{
						stride_float = (primitives_array_texindex.stride / sizeof(float));
					}
					else
					{
						stride_float = primitives_array_texindex.size;
					}

					float *dptr = (float *) primitives_array_texindex.data;
					dptr += offset * stride_float;
					num_vertices_in_group = 1;
					texture_index = *dptr;
					dptr += stride_float;
					for (int texcnt = 1; texcnt < num_vertices; ++texcnt)
					{
						// Note, we assume all textures in "primitives_array_texindex.size" are identical
						if ( (*dptr) != texture_index )
							break;
						dptr += stride_float;
						num_vertices_in_group++;
					}
				}
				break;
			default:
				throw CL_Exception("Implement me!");
			}
		}
		else
		{
			texture_index = 0;
			num_vertices_in_group = num_vertices;
		}

		set_primitive_texture( texture_index, primitives_array_texture, offset, num_vertices_in_group, total_vertices );
		cl1DrawArrays(primitive_type, offset, num_vertices_in_group);
		reset_primitive_texture( texture_index );

		offset += num_vertices_in_group;
		num_vertices -=	num_vertices_in_group;
	}
}

void CL_GL1GraphicContextProvider::draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count)
{
	throw CL_Exception("Cannot draw instanced for the OpenGL 1.3 target");
}

void CL_GL1GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
	set_active();
	cl1DrawElements(to_enum(type), count, GL_UNSIGNED_INT, indices);
}

void CL_GL1GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
	set_active();
	cl1DrawElements(to_enum(type), count, GL_UNSIGNED_SHORT, indices);
}

void CL_GL1GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
	set_active();
	cl1DrawElements(to_enum(type), count, GL_UNSIGNED_BYTE, indices);
}

void CL_GL1GraphicContextProvider::draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned int *indices, int instance_count)
{
	throw CL_Exception("Cannot draw instanced for the OpenGL 1.3 target");
}

void CL_GL1GraphicContextProvider::draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned short *indices, int instance_count)
{
	throw CL_Exception("Cannot draw instanced for the OpenGL 1.3 target");
}

void CL_GL1GraphicContextProvider::draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned char *indices, int instance_count)
{
	throw CL_Exception("Cannot draw instanced for the OpenGL 1.3 target");
}

void CL_GL1GraphicContextProvider::draw_primitives_elements(
	CL_PrimitivesType type,
	int count,
	CL_ElementArrayBufferProvider *array_provider,
	CL_VertexAttributeDataType indices_type,
	void *offset)
{
	throw CL_Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
}

void CL_GL1GraphicContextProvider::draw_primitives_elements_instanced(
	CL_PrimitivesType type,
	int count,
	CL_ElementArrayBufferProvider *array_provider,
	CL_VertexAttributeDataType indices_type,
	void *offset,
	int instance_count)
{
	throw CL_Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
}

void CL_GL1GraphicContextProvider::reset_primitives_array()
{
	set_active();

	primitives_array_texture_set = false;
	primitives_array_texindex_set = false;

	cl1DisableClientState(GL_VERTEX_ARRAY);
	cl1DisableClientState(GL_COLOR_ARRAY);
	cl1DisableClientState(GL_NORMAL_ARRAY);
	cl1DisableClientState(GL_EDGE_FLAG_ARRAY);
	cl1DisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (cl1ClientActiveTexture)
	{
		for (int i = 0; i < num_set_tex_arrays; ++i)
		{
			cl1ClientActiveTexture(GL_TEXTURE0+i);
			cl1DisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		num_set_tex_arrays = 0;
	}
	else
	{
		cl1DisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	prim_arrays_set = false;
}

void CL_GL1GraphicContextProvider::draw_pixels(CL_GraphicContext &gc, 
	float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &image, const CL_Rect &src_rect, const CL_Colorf &color)
{
	if (x + image.get_width() * zoom_x < 0 || y + image.get_height() * zoom_y < 0 ||
		x + image.get_width() * zoom_x > get_width() || y + image.get_height() * zoom_y > get_height())
	{
		return;
	}

	CL_Image texture_image(gc, image, src_rect);
	texture_image.draw(gc, CL_Rectf(x, y, CL_Sizef( ((float) texture_image.get_width()) * zoom_x, ((float) texture_image.get_height()) * zoom_y) ));
	gc.flush_batcher();

	// This can be speeded up by using the code below, but it requires fixing (to use src_rect)
/*
	CL_PixelBuffer subimage(image);
	if (x < 0 || y < 0)
	{
		if (zoom_x == 0.0f || zoom_y == 0.0f)
			return;
		
		float clip_left = (-1.0f*(x<0))*x;
		float clip_top = (-1.0f*(y<0))*y;

		subimage = image.get_subimage(CL_Rect(int(clip_left/zoom_x), int(clip_top/zoom_y), image.get_width(), image.get_height()));
	}

	set_active();

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	GLenum format;
	GLenum type;
	bool conv_needed = !CL_GL1::to_opengl_pixelformat(image, format, type);

	// also check for the pitch (OpenGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (subimage.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);

		char *data = (char *) subimage.get_data();

		cl1RasterPos2f((x>0)*x, (y>0)*y);
		if (map_mode == cl_map_2d_upper_left)
			cl1PixelZoom((GLfloat) zoom_x, (GLfloat) -zoom_y);
		else
			cl1PixelZoom((GLfloat) zoom_x, (GLfloat) zoom_y);

		cl1PixelTransferf(GL_RED_SCALE, (GLfloat) color.get_red());
		cl1PixelTransferf(GL_GREEN_SCALE, (GLfloat) color.get_green());
		cl1PixelTransferf(GL_BLUE_SCALE, (GLfloat) color.get_blue());

		cl1DrawPixels(
			subimage.get_width(),
			subimage.get_height(),
			format,
			type,
			data);

		cl1PixelTransferf(GL_RED_SCALE, 1.0);
		cl1PixelTransferf(GL_GREEN_SCALE, 1.0);
		cl1PixelTransferf(GL_BLUE_SCALE, 1.0);
	}
	// conversion needed
	else
	{
		bool big_endian = CL_Endian::is_system_big();

		CL_PixelBuffer buffer;
		if (!big_endian)
			buffer = CL_PixelBuffer(
				subimage.get_width(), subimage.get_height(),
				subimage.get_width() * (needs_alpha ? 4 : 3),
				needs_alpha ? cl_abgr8 : cl_bgr8); // OpenGL RGB/RGBA is always big endian
		else
			buffer = CL_PixelBuffer(
				subimage.get_width(), subimage.get_height(),
				subimage.get_width() * (needs_alpha ? 4 : 3),
				needs_alpha ? cl_rgba8 : cl_rgb8);
	
		CL_PixelBuffer(subimage).convert(buffer);

		format = needs_alpha ? GL_RGBA : GL_RGB;

		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);

		cl1RasterPos2d((x>0)*x, (y>0)*y);
		if (map_mode == cl_map_2d_upper_left)
			cl1PixelZoom((GLfloat) zoom_x, (GLfloat) -zoom_y);
		else
			cl1PixelZoom((GLfloat) zoom_x, (GLfloat) zoom_y);

		cl1PixelTransferf(GL_RED_SCALE, (GLfloat) color.get_red());
		cl1PixelTransferf(GL_GREEN_SCALE, (GLfloat) color.get_green());
		cl1PixelTransferf(GL_BLUE_SCALE, (GLfloat) color.get_blue());

		cl1DrawPixels(
			subimage.get_width(),
			subimage.get_height(),
			format,
			GL_UNSIGNED_BYTE,
			buffer.get_data());

		cl1PixelTransferf(GL_RED_SCALE, 1.0);
		cl1PixelTransferf(GL_GREEN_SCALE, 1.0);
		cl1PixelTransferf(GL_BLUE_SCALE, 1.0);
	}
*/
}

void CL_GL1GraphicContextProvider::set_clip_rect(const CL_Rect &rect)
{
	set_active();
	cl1Enable(GL_SCISSOR_TEST);

	if (map_mode == cl_map_2d_upper_left)
		cl1Scissor(
			rect.left,
			get_height() - rect.top - rect.get_height(),
			rect.get_width(),
			rect.get_height());
	else
		cl1Scissor(
			rect.left,
			rect.top,
			rect.get_width(),
			rect.get_height());

	last_clip_rect = rect;
}

void CL_GL1GraphicContextProvider::reset_clip_rect()
{
	set_active();
	cl1Disable(GL_SCISSOR_TEST);
}

void CL_GL1GraphicContextProvider::clear(const CL_Colorf &color)
{
	set_active();
	cl1ClearColor(
		(GLclampf) color.r,
		(GLclampf) color.g,
		(GLclampf) color.b,
		(GLclampf) color.a);
	cl1Clear(GL_COLOR_BUFFER_BIT);
}

void CL_GL1GraphicContextProvider::clear_stencil(int value)
{
	set_active();
	cl1ClearStencil(value);
	cl1Clear(GL_STENCIL_BUFFER_BIT);
}

void CL_GL1GraphicContextProvider::clear_depth(float value)
{
	set_active();
	cl1ClearDepth(value);
	cl1Clear(GL_DEPTH_BUFFER_BIT);
}

void CL_GL1GraphicContextProvider::set_map_mode(CL_MapMode mode)
{
	// Invert the mapping mode for FBO's 
	if (framebuffer_bound)
	{
		if (mode == cl_map_2d_upper_left)
			mode = cl_map_2d_lower_left;
		else if (mode == cl_map_2d_lower_left)
			mode = cl_map_2d_upper_left;
	}

	map_mode = mode;
	on_window_resized();
}

void CL_GL1GraphicContextProvider::on_window_resized()
{
	int width = get_width();
	int height = get_height();

	switch (map_mode)
	{
	default:
		break;
	case cl_map_2d_upper_left:
		set_active();
		set_viewport(CL_Rectf(0.0f, 0.0f, width, height));
		set_projection(CL_Mat4f::ortho_2d(0.0f, (float)width, (float)height, 0.0f));
		set_modelview(CL_Mat4f::identity());

		if (cl1IsEnabled(GL_SCISSOR_TEST))
			cl1Scissor(
				last_clip_rect.left,
				get_height() - last_clip_rect.top - last_clip_rect.get_height(),
				last_clip_rect.get_width(),
				last_clip_rect.get_height());
		break;
	case cl_map_2d_lower_left:
		set_active();
		set_viewport(CL_Rectf(0.0f, 0.0f, width, height));
		set_projection(CL_Mat4f::ortho_2d(0.0f, (float)width, 0.0f, (float)height));
		set_modelview(CL_Mat4f::identity());
		if (cl1IsEnabled(GL_SCISSOR_TEST))
			cl1Scissor(
				last_clip_rect.left,
				last_clip_rect.top,
				last_clip_rect.get_width(),
				last_clip_rect.get_height());
		break;

	case cl_user_projection:
		set_active();
		set_viewport(CL_Rectf(0.0f, 0.0f, width, height));
		if (cl1IsEnabled(GL_SCISSOR_TEST))
			cl1Scissor(
				last_clip_rect.left,
				last_clip_rect.top,
				last_clip_rect.get_width(),
				last_clip_rect.get_height());
		break;
	}
}

void CL_GL1GraphicContextProvider::set_viewport(const CL_Rectf &viewport)
{
	int height = get_height();
	set_active();
	cl1Viewport(
		GLsizei(viewport.left),
		GLsizei(height - viewport.bottom),
		GLsizei(viewport.right - viewport.left),
		GLsizei(viewport.bottom - viewport.top));
}

void CL_GL1GraphicContextProvider::set_projection(const CL_Mat4f &matrix)
{
	if (projection != matrix)
	{
		projection = matrix;
		set_active();
		cl1MatrixMode(GL_PROJECTION);
		cl1LoadMatrixf(projection.matrix);
	}
}

void CL_GL1GraphicContextProvider::set_modelview(const CL_Mat4f &matrix)
{
	if (modelview != matrix)
	{
		modelview = matrix;
		set_active();
		cl1MatrixMode(GL_MODELVIEW);
		cl1LoadMatrixf(modelview);
	}
}

void CL_GL1GraphicContextProvider::set_blend_mode(const CL_BlendMode &mode)
{
	set_active();

	if( mode.is_blend_enabled() )
		cl1Enable(GL_BLEND);
	else
		cl1Disable(GL_BLEND);

	const CL_Colorf &col = mode.get_blend_color();

	if (cl1BlendColor)
	{
		cl1BlendColor(
			GLclampf(col.get_red()),
			GLclampf(col.get_green()),
			GLclampf(col.get_blue()),
			GLclampf(col.get_alpha()));
	}

	if (cl1BlendEquation)
		cl1BlendEquation(to_enum(mode.get_blend_equation()));

	if( mode.get_blend_function_src() == mode.get_blend_function_src_alpha() &&
		mode.get_blend_function_dest() == mode.get_blend_function_dest_alpha() )
	{
		if (cl1BlendFunc)
			cl1BlendFunc(to_enum(mode.get_blend_function_src()), to_enum(mode.get_blend_function_dest()));
	}
	else
	{
		if (cl1BlendFuncSeparate)
		{
			cl1BlendFuncSeparate( 
				to_enum(mode.get_blend_function_src()),
				to_enum(mode.get_blend_function_dest()),
				to_enum(mode.get_blend_function_src_alpha()),
				to_enum(mode.get_blend_function_dest_alpha()) );
		}
		else
		{
			//throw CL_Exception("BlendFuncSeparate is not supported for OpenGL 1.3");
			if (cl1BlendFunc)
				cl1BlendFunc(to_enum(mode.get_blend_function_src()), to_enum(mode.get_blend_function_dest()));
		}
	}
}

void CL_GL1GraphicContextProvider::set_pen(const CL_Pen &pen)
{
	set_active();


// (OpenGL 1.4)
//	if (cl1PointParameterf)
//	{
//		cl1PointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, (GLfloat)pen.get_point_fade_treshold_size());
//	}

	cl1PointSize((GLfloat)pen.get_point_size());
	cl1LineWidth((GLfloat)pen.get_line_width());

	if (pen.is_line_antialiased())
		cl1Enable(GL_LINE_SMOOTH);
	else
		cl1Disable(GL_LINE_SMOOTH);

	if (pen.is_using_vertex_program_point_sizes())
		cl1Enable(GL_VERTEX_PROGRAM_POINT_SIZE);
	else
		cl1Disable(GL_VERTEX_PROGRAM_POINT_SIZE);

// (OpenGL 1.4)
//	if(cl1PointParameterf)
//	{
//		switch (pen.get_point_sprite_origin())
//		{
//		case cl_point_sprite_origin_upper_left:
//			cl1PointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_UPPER_LEFT);
//			break;
//		case cl_point_sprite_origin_lower_left:
//			cl1PointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
//			break;
//		}
//	}
}

void CL_GL1GraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
	set_active();

	if (raster.is_antialiased())
		cl1Enable(GL_POLYGON_SMOOTH);
	else
		cl1Disable(GL_POLYGON_SMOOTH);

	if (raster.is_culled())
		cl1Enable(GL_CULL_FACE);
	else
		cl1Disable(GL_CULL_FACE);

	if (raster.is_line_offset())
		cl1Enable(GL_POLYGON_OFFSET_LINE);
	else
		cl1Disable(GL_POLYGON_OFFSET_LINE);

	if (raster.is_point_offset())
		cl1Enable(GL_POLYGON_OFFSET_POINT);
	else
		cl1Disable(GL_POLYGON_OFFSET_POINT);

	if (raster.is_polygon_offset())
		cl1Enable(GL_POLYGON_OFFSET_FILL);
	else
		cl1Disable(GL_POLYGON_OFFSET_FILL);

	cl1PolygonMode(GL_FRONT, to_enum(raster.get_face_fill_mode()));
	cl1PolygonMode(GL_BACK, to_enum(raster.get_face_fill_mode()));

	switch (raster.get_front_face())
	{
	case cl_face_side_counter_clockwise:
		cl1FrontFace(GL_CCW);
		break;
	case cl_face_side_clockwise:
		cl1FrontFace(GL_CW);
		break;
	}

	switch (raster.get_face_cull_mode())
	{
	case cl_cull_front:
		cl1CullFace(GL_FRONT);
		break;
	case cl_cull_back:
		cl1CullFace(GL_BACK);
		break;
	case cl_cull_front_and_back:
		cl1CullFace(GL_FRONT_AND_BACK);
		break;
	}

	cl1PolygonOffset(raster.get_offset_factor(), raster.get_offset_units());
}

void CL_GL1GraphicContextProvider::set_buffer_control(const CL_BufferControl &bc)
{
	set_active();

	if( bc.is_depth_test_enabled() )
		cl1Enable(GL_DEPTH_TEST);
	else
		cl1Disable(GL_DEPTH_TEST);

	cl1DepthFunc(to_enum(bc.get_depth_compare_function()));
	cl1DepthMask(bc.is_depth_write_enabled() ? 1 : 0);

	if( bc.is_stencil_test_enabled() )
	{
		throw CL_Exception("Stencil buffer is not available for OpenGL 1.3");
	}

	cl1DrawBuffer( to_enum(bc.get_draw_buffer()) );

	if (bc.is_logic_op_enabled())
	{
		cl1Enable(GL_COLOR_LOGIC_OP);
		cl1LogicOp(to_enum(bc.get_logic_op()));
	}
	else
	{
		cl1Disable(GL_COLOR_LOGIC_OP);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CL_GL1GraphicContextProvider Implementation:

GLenum CL_GL1GraphicContextProvider::to_enum(CL_DrawBuffer buffer)
{
	switch(buffer)
	{
	case cl_buffer_back: return GL_BACK;
	case cl_buffer_back_left: return GL_BACK_LEFT;
	case cl_buffer_back_right: return GL_BACK_RIGHT;
	case cl_buffer_front: return GL_FRONT;
	case cl_buffer_front_and_back: return GL_FRONT_AND_BACK;
	case cl_buffer_front_left: return GL_FRONT_LEFT;
	case cl_buffer_front_right: return GL_FRONT_RIGHT;
	case cl_buffer_left: return GL_LEFT;
	case cl_buffer_none: return GL_NONE;
	case cl_buffer_right: return GL_RIGHT;
	default: return GL_BACK;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(CL_CompareFunction func)
{
	switch( func )
	{
	case cl_comparefunc_never: return GL_NEVER;
	case cl_comparefunc_less: return GL_LESS;
	case cl_comparefunc_lequal: return GL_LEQUAL; 
	case cl_comparefunc_greater: return GL_GREATER; 
	case cl_comparefunc_gequal: return GL_GEQUAL; 
	case cl_comparefunc_equal: return GL_EQUAL; 
	case cl_comparefunc_notequal: return GL_NOTEQUAL; 
	case cl_comparefunc_always: return GL_ALWAYS; 
	default: return GL_LEQUAL;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(CL_StencilOp op)
{
	switch( op )
	{
	case cl_stencil_decr: return GL_DECR;
	case cl_stencil_decr_wrap: return GL_DECR_WRAP;
	case cl_stencil_incr: return GL_INCR;
	case cl_stencil_incr_wrap: return GL_INCR_WRAP;
	case cl_stencil_invert: return GL_INVERT;
	case cl_stencil_keep: return GL_KEEP;
	case cl_stencil_replace: return GL_REPLACE;
	case cl_stencil_zero: return GL_ZERO;	
	default: return GL_KEEP;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(CL_CullMode mode)
{
	switch( mode )
	{
	case cl_cull_front: return GL_FRONT;
	case cl_cull_back: return GL_BACK;
	case cl_cull_front_and_back: return GL_FRONT_AND_BACK;
	default: return GL_BACK;
	}
};

GLenum CL_GL1GraphicContextProvider::to_enum(CL_FillMode mode)
{
	switch( mode )
	{
	case cl_fill_point: return GL_POINT;
	case cl_fill_line: return GL_LINE;
	case cl_fill_polygon: return GL_FILL;
	default: return GL_FILL;
	}
};

GLenum CL_GL1GraphicContextProvider::to_enum(CL_BlendFunc func)
{
	switch( func )
	{
	case cl_blend_zero: return GL_ZERO;
	case cl_blend_one: return GL_ONE;
	case cl_blend_dest_color: return GL_DST_COLOR;
	case cl_blend_src_color: return GL_SRC_COLOR;
	case cl_blend_one_minus_dest_color: return GL_ONE_MINUS_DST_COLOR;
	case cl_blend_one_minus_src_color: return GL_ONE_MINUS_SRC_COLOR;
	case cl_blend_src_alpha: return GL_SRC_ALPHA;
	case cl_blend_one_minus_src_alpha: return GL_ONE_MINUS_SRC_ALPHA;
	case cl_blend_dest_alpha: return GL_DST_ALPHA;
	case cl_blend_one_minus_dest_alpha: return GL_ONE_MINUS_DST_ALPHA;
	case cl_blend_src_alpha_saturate: return GL_SRC_ALPHA_SATURATE;
	case cl_blend_constant_color: return GL_CONSTANT_COLOR;
	case cl_blend_one_minus_constant_color: return GL_ONE_MINUS_CONSTANT_COLOR;
	case cl_blend_constant_alpha: return GL_CONSTANT_ALPHA;
	case cl_blend_one_minus_constant_alpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
	default: return GL_BLEND_SRC;
	}
}


GLenum CL_GL1GraphicContextProvider::to_enum(CL_BlendEquation eq)
{
	switch( eq )
	{
	case cl_blend_equation_add: return GL_FUNC_ADD;
	case cl_blend_equation_subtract: return GL_FUNC_SUBTRACT;
	case cl_blend_equation_reverse_subtract: return GL_FUNC_REVERSE_SUBTRACT;
	case cl_blend_equation_min: return GL_MIN;
	case cl_blend_equation_max: return GL_MAX;
	default: return GL_FUNC_ADD;
	}
};

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_TextureUnitSource value)
{
	switch( value )
	{
	case cl_source_texture: return GL_TEXTURE;
	case cl_source_texture0: return GL_TEXTURE0;
	case cl_source_texture1: return GL_TEXTURE1;
	case cl_source_texture2: return GL_TEXTURE2;
	case cl_source_texture3: return GL_TEXTURE3;
	case cl_source_texture4: return GL_TEXTURE4;
	case cl_source_texture5: return GL_TEXTURE5;
	case cl_source_texture6: return GL_TEXTURE6;
	case cl_source_texture7: return GL_TEXTURE7;
	case cl_source_texture8: return GL_TEXTURE8;
	case cl_source_texture9: return GL_TEXTURE9;
	case cl_source_texture10: return GL_TEXTURE10;
	case cl_source_texture11: return GL_TEXTURE11;
	case cl_source_texture12: return GL_TEXTURE12;
	case cl_source_texture13: return GL_TEXTURE13;
	case cl_source_texture14: return GL_TEXTURE14;
	case cl_source_texture15: return GL_TEXTURE15;
	case cl_source_constant: return GL_CONSTANT;
	case cl_source_primary_color: return GL_PRIMARY_COLOR;
	case cl_source_previous: return GL_PREVIOUS;
	default: return GL_TEXTURE;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_TextureUnitRGBOperand value)
{
	switch( value )
	{
	case cl_rgb_operand_src_color: return GL_SRC_COLOR;
	case cl_rgb_operand_one_minus_src_color: return GL_ONE_MINUS_SRC_COLOR;
	case cl_rgb_operand_src_alpha: return GL_SRC_ALPHA;
	case cl_rgb_operand_one_minus_src_alpha: return GL_ONE_MINUS_SRC_ALPHA;
	default: return GL_SRC_COLOR;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_TextureUnitAlphaOperand value)
{
	if( value == cl_alpha_operand_src_alpha )
		return GL_SRC_ALPHA_SATURATE;

	return GL_ONE_MINUS_SRC_ALPHA;
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_TextureUnitAlphaMode value)
{
	switch(value)
	{
	case cl_combinemode_alpha_replace: return GL_REPLACE;
	case cl_combinemode_alpha_modulate: return GL_MODULATE;
	case cl_combinemode_alpha_add: return GL_ADD;
	case cl_combinemode_alpha_add_signed: return GL_ADD_SIGNED;
	case cl_combinemode_alpha_interpolate: return GL_INTERPOLATE;
	case cl_combinemode_alpha_subtract: return GL_SUBTRACT;
	default: return GL_REPLACE;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_TextureUnitRGBMode value)
{
	switch(value)
	{
	case cl_combinemode_rgb_replace: return GL_REPLACE;
	case cl_combinemode_rgb_modulate: return GL_MODULATE;
	case cl_combinemode_rgb_add: return GL_ADD;
	case cl_combinemode_rgb_add_signed: return GL_ADD_SIGNED;
	case cl_combinemode_rgb_interpolate: return GL_INTERPOLATE;
	case cl_combinemode_rgb_subtract: return GL_SUBTRACT;
	case cl_combinemode_rgb_dot3_rgb: return GL_DOT3_RGB;
	case cl_combinemode_rgb_dot3_rgba: return GL_DOT3_RGBA;
	default: return GL_REPLACE;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_TextureUnitMode value)
{
	switch(value)
	{
	case cl_unitmode_replace: return GL_REPLACE;
	case cl_unitmode_modulate: return GL_MODULATE;
	case cl_unitmode_decal: return GL_DECAL;
	case cl_unitmode_blend: return GL_BLEND;
	case cl_unitmode_add: return GL_ADD;
	case cl_unitmode_combine: return GL_COMBINE;
	default: return GL_REPLACE;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_VertexAttributeDataType value)
{
	switch(value)
	{
	case cl_type_unsigned_byte:
		return GL_UNSIGNED_BYTE;
	case cl_type_unsigned_short:
		return GL_UNSIGNED_SHORT;
	case cl_type_unsigned_int:
		return GL_UNSIGNED_INT;
	case cl_type_byte:
		return GL_BYTE;
	case cl_type_short:
		return GL_SHORT;
	case cl_type_int:
		return GL_INT;
	case cl_type_float:
		return GL_FLOAT;
	default:
		return 0;
	}
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_PrimitivesType value)
{
	GLenum gl_mode = 0;
	switch (value)
	{
	case cl_points: gl_mode = GL_POINTS; break;
	case cl_line_strip: gl_mode = GL_LINE_STRIP; break;
	case cl_line_loop: gl_mode = GL_LINE_LOOP; break;
	case cl_lines: gl_mode = GL_LINES; break;
	case cl_triangle_strip: gl_mode = GL_TRIANGLE_STRIP; break;
	case cl_triangle_fan: gl_mode = GL_TRIANGLE_FAN; break;
	case cl_triangles: gl_mode = GL_TRIANGLES; break;
	}
	return gl_mode;
}

GLenum CL_GL1GraphicContextProvider::to_enum(enum CL_LogicOp op)
{
	GLenum gl_op = 0;
	switch (op)
	{
		case cl_logic_op_clear: gl_op = GL_CLEAR; break;  
		case cl_logic_op_and: gl_op = GL_AND; break;
		case cl_logic_op_and_reverse: gl_op = GL_AND_REVERSE; break;
		case cl_logic_op_copy: gl_op = GL_COPY; break;
		case cl_logic_op_and_inverted: gl_op = GL_AND_INVERTED; break;
		case cl_logic_op_noop: gl_op = GL_NOOP; break;
		case cl_logic_op_xor: gl_op = GL_XOR; break;
		case cl_logic_op_or: gl_op = GL_OR; break;
		case cl_logic_op_nor: gl_op = GL_NOR; break;
		case cl_logic_op_equiv: gl_op = GL_EQUIV; break;
		case cl_logic_op_invert: gl_op = GL_INVERT; break;
		case cl_logic_op_or_reverse: gl_op = GL_OR_REVERSE; break;
		case cl_logic_op_copy_inverted: gl_op = GL_COPY_INVERTED; break;
		case cl_logic_op_or_inverted: gl_op = GL_OR_INVERTED; break;
		case cl_logic_op_nand: gl_op = GL_NAND; break;
		case cl_logic_op_set: gl_op = GL_SET; break;
		default: break;
	}
	return gl_op;
}

void CL_GL1GraphicContextProvider::set_primitive_texture( int texture_index, CL_PrimitivesArrayData::VertexData &array_texture, int offset, int num_vertices, int total_vertices)
{
	CL_GL1TextureProvider *texture;
	if ( (texture_index <0) || (texture_index >= max_texture_coords) )
	{
		texture = NULL;		// Ignore invalid texture index's
	}
	else
	{
		texture = selected_textures[texture_index].texture;
	}

	if (texture)
	{
		if (cl1ActiveTexture != 0)
			cl1ActiveTexture( GL_TEXTURE0 + texture_index );

		cl1Enable(texture->get_texture_type());
		cl1BindTexture(texture->get_texture_type(), texture->get_handle());

		if (cl1ClientActiveTexture)
			cl1ClientActiveTexture(GL_TEXTURE0 + texture_index );

		cl1EnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (texture->is_power_of_two_texture() || (num_vertices==0))
		{
			cl1TexCoordPointer(array_texture.size, to_enum(array_texture.type),  array_texture.stride,  array_texture.data);
		}
		else
		{
			// A hack to handle non-power-of-two textures
			texture->transform_coordinate(array_texture, transformed_coords, offset, num_vertices, total_vertices);
			cl1TexCoordPointer(array_texture.size, GL_FLOAT, 0,  &transformed_coords[0]);
		}
	}
	else
	{
		//Disabled this exception because the sprite render batcher does this intentionally
		//throw CL_Exception("Attempt to draw a texture that was not selected in the GL1 target");
	}

}

void CL_GL1GraphicContextProvider::reset_primitive_texture( int texture_index)
{
	if ( (texture_index <0) || (texture_index >= max_texture_coords) )
	{
		return;
	}

	CL_GL1TextureProvider *texture = selected_textures[texture_index].texture;
	if (texture)
	{
		if (cl1ActiveTexture != 0)
			cl1ActiveTexture( GL_TEXTURE0 + texture_index );

		cl1BindTexture(GL_TEXTURE_2D, 0);
		cl1Disable(GL_TEXTURE_2D);
	}

}

void CL_GL1GraphicContextProvider::reset_primitive_texture_all()
{
	for (int cnt=0; cnt < max_texture_coords; cnt++)
	{
		CL_GL1TextureProvider *texture = selected_textures[cnt].texture;
		if (texture)
		{
			if (cl1ActiveTexture != 0)
				cl1ActiveTexture( GL_TEXTURE0 + cnt );
	
			cl1BindTexture(GL_TEXTURE_2D, 0);
			cl1Disable(GL_TEXTURE_2D);
		}
	}
}
