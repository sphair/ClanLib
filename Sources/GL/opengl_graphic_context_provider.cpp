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
**    Ken Hirsch
**    Kenneth Gangstoe
**    Mark Page
**    Kevin J Bluck
*/

#include "GL/precomp.h"
#include "opengl_graphic_context_provider.h"
#include "opengl_occlusion_query_provider.h"
#include "opengl_texture_provider.h"
#include "opengl_program_object_provider.h"
#include "opengl_shader_object_provider.h"
#include "opengl_frame_buffer_provider.h"
#include "opengl_render_buffer_provider.h"
#include "opengl_vertex_array_buffer_provider.h"
#include "opengl_element_array_buffer_provider.h"
#include "opengl_pixel_buffer_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Math/cl_math.h"
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
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/2D/image.h"

#ifndef WIN32
#include "GLX/opengl_window_provider_glx.h"
#endif

const CL_String::char_type *cl_glsl_vertex_color_only = 
	"attribute vec4 Position, Color0; "
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"varying vec4 Color; "
	"void main(void) { gl_Position = cl_ModelViewProjectionMatrix*Position; Color = Color0; }";

const CL_String::char_type *cl_glsl_fragment_color_only =
	"varying vec4 Color; "
	"void main(void) { gl_FragColor = Color; }";

const CL_String::char_type *cl_glsl_vertex_single_texture =
	"attribute vec4 Position, Color0; "
	"attribute vec2 TexCoord0; "
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"varying vec4 Color; "
	"varying vec2 TexCoord; "
	"void main(void) { gl_Position = cl_ModelViewProjectionMatrix*Position; Color = Color0; TexCoord = TexCoord0; }";

const CL_String::char_type *cl_glsl_fragment_single_texture =
	"uniform sampler2D Texture0; "
	"varying vec4 Color; "
	"varying vec2 TexCoord; "
	"void main(void) { gl_FragColor = Color*texture2D(Texture0, TexCoord); }";

const CL_String::char_type *cl_glsl_vertex_sprite =
	"attribute vec4 Position, Color0; "
	"attribute vec2 TexCoord0; "
	"attribute float TexIndex0; "
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"varying vec4 Color; "
	"varying vec2 TexCoord; "
	"varying float TexIndex; "
	"void main(void) { gl_Position = cl_ModelViewProjectionMatrix*Position; Color = Color0; TexCoord = TexCoord0; TexIndex = TexIndex0; }";

const CL_String::char_type *cl_glsl_fragment_sprite =
	"uniform sampler2D Texture0; "
	"uniform sampler2D Texture1; "
	"uniform sampler2D Texture2; "
	"uniform sampler2D Texture3; "
	"varying vec4 Color; "
	"varying vec2 TexCoord; "
	"varying float TexIndex; "
	"vec4 sampleTexture(int index, vec2 pos) { if (index == 0) return texture2D(Texture0, TexCoord); else if (index == 1) return texture2D(Texture1, TexCoord); else if (index == 2) return texture2D(Texture2, TexCoord); else if (index == 3) return texture2D(Texture3, TexCoord); else return vec4(1.0,1.0,1.0,1.0); }"
	"void main(void) { gl_FragColor = Color*sampleTexture(int(TexIndex), TexCoord); } ";


/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLGraphicContextProvider Construction:

CL_OpenGLGraphicContextProvider::CL_OpenGLGraphicContextProvider(const CL_RenderWindowProvider * const render_window)
: render_window(render_window), map_mode(cl_map_2d_upper_left), projection(CL_Mat4f::identity()), modelview(CL_Mat4f::identity()),
  framebuffer_bound(false), prim_arrays_set(false), num_set_program_attribute_arrays(0), cur_prim_array(0),
  modelview_projection_matrix_valid(false), normal_matrix_valid(false)
{
	check_opengl_version();

	CL_ShaderObject vertex_color_only_shader(this, cl_shadertype_vertex, cl_glsl_vertex_color_only);
	if(!vertex_color_only_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'vertex color only' Error:" + vertex_color_only_shader.get_info_log());

	CL_ShaderObject fragment_color_only_shader(this, cl_shadertype_fragment, cl_glsl_fragment_color_only);
	if(!fragment_color_only_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'fragment color only' Error:" + fragment_color_only_shader.get_info_log());

	CL_ShaderObject vertex_single_texture_shader(this, cl_shadertype_vertex, cl_glsl_vertex_single_texture);
	if(!vertex_single_texture_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'vertex single texture' Error:" + vertex_single_texture_shader.get_info_log());

	CL_ShaderObject fragment_single_texture_shader(this, cl_shadertype_fragment, cl_glsl_fragment_single_texture);
	if(!fragment_single_texture_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'fragment single texture' Error:" + fragment_single_texture_shader.get_info_log());

	CL_ShaderObject vertex_sprite_shader(this, cl_shadertype_vertex, cl_glsl_vertex_sprite);
	if(!vertex_sprite_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'vertex sprite' Error:" + vertex_sprite_shader.get_info_log());

	CL_ShaderObject fragment_sprite_shader(this, cl_shadertype_fragment, cl_glsl_fragment_sprite);
	if(!fragment_sprite_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'fragment sprite' Error:" + fragment_sprite_shader.get_info_log());

	CL_ProgramObject color_only_program(this);
	color_only_program.attach(vertex_color_only_shader);
	color_only_program.attach(fragment_color_only_shader);
	color_only_program.bind_attribute_location(0, "Position");
	color_only_program.bind_attribute_location(1, "Color0");
	if (!color_only_program.link())
		throw CL_Exception("Unable to link the standard shader program: 'color only' Error:" + color_only_program.get_info_log());

	CL_ProgramObject single_texture_program(this);
	single_texture_program.attach(vertex_single_texture_shader);
	single_texture_program.attach(fragment_single_texture_shader);
	single_texture_program.bind_attribute_location(0, "Position");
	single_texture_program.bind_attribute_location(1, "Color0");
	single_texture_program.bind_attribute_location(2, "TexCoord0");
	if (!single_texture_program.link())
		throw CL_Exception("Unable to link the standard shader program: 'single texture' Error:" + single_texture_program.get_info_log());
	single_texture_program.set_uniform1i("Texture0", 0);

	CL_ProgramObject sprite_program(this);
	sprite_program.attach(vertex_sprite_shader);
	sprite_program.attach(fragment_sprite_shader);
	sprite_program.bind_attribute_location(0, "Position");
	sprite_program.bind_attribute_location(1, "Color0");
	sprite_program.bind_attribute_location(2, "TexCoord0");
	sprite_program.bind_attribute_location(3, "TexIndex0");
	if (!sprite_program.link())
		throw CL_Exception("Unable to link the standard shader program: 'sprite' Error:" + sprite_program.get_info_log());

	sprite_program.set_uniform1i("Texture0", 0);
	sprite_program.set_uniform1i("Texture1", 1);
	sprite_program.set_uniform1i("Texture2", 2);
	sprite_program.set_uniform1i("Texture3", 3);

	standard_programs.push_back(color_only_program);
	standard_programs.push_back(single_texture_program);
	standard_programs.push_back(sprite_program);

	reset_program_object();
}

CL_OpenGLGraphicContextProvider::~CL_OpenGLGraphicContextProvider()
{
	current_program_object = CL_ProgramObject();
	standard_programs.clear();

	CL_OpenGL::remove_active(this);
	delete render_window;

}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLGraphicContextProvider Attributes:

void CL_OpenGLGraphicContextProvider::check_opengl_version()
{
	int version_major = 0;
	int version_minor = 0;
	int version_release = 0;

	get_opengl_version(version_major, version_minor, version_release);
	if(version_major < 2)
		throw CL_Exception(cl_format("This application requires OpenGL 2.0 or above. Your hardware only supports OpenGL %1.%2.%3. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor, version_release));
}

void CL_OpenGLGraphicContextProvider::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
/*	The GL_VERSION string begins with a version number. The version number uses one of these forms: 
	major_number.minor_number 
	major_number.minor_number.release_number 
	Vendor-specific information may follow the version number. Its format depends on the implementation, but a space always separates the version number and the vendor-specific information. 
	All strings are null-terminated. 
	If an error is generated, glGetString returns zero.
*/
	CL_OpenGL::set_active(this);

	CL_String version = (char*)clGetString(CL_VERSION);

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

void CL_OpenGLGraphicContextProvider::get_opengl_shading_language_version(int &version_major, int &version_minor, int &version_release)
{
	CL_OpenGL::set_active(this);

	CL_String version = (char*)clGetString(CL_SHADING_LANGUAGE_VERSION);

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


CL_String CL_OpenGLGraphicContextProvider::get_renderer_string()
{
	CL_OpenGL::set_active(this);
	CL_String renderer = (char*)clGetString(CL_RENDERER);
	return renderer;
}

CL_String CL_OpenGLGraphicContextProvider::get_vendor_string()
{
	CL_OpenGL::set_active(this);
	CL_String vendor = (char*)clGetString(CL_VENDOR);
	return vendor;
}

std::vector<CL_String> CL_OpenGLGraphicContextProvider::get_extensions()
{
	CL_OpenGL::set_active(this);
	CL_StringRef extension_string = (char*)clGetString(CL_EXTENSIONS);
	std::vector<CL_String> tmp = CL_StringHelp::split_text(extension_string, " ");
	std::vector<CL_String> extensions;
	for (std::vector<CL_String>::size_type i=0; i<tmp.size(); i++)
		extensions.push_back(tmp[i]);
	return extensions;
}

int CL_OpenGLGraphicContextProvider::get_max_attributes()
{
	CL_OpenGL::set_active(this);
	CLint max_attributes = 0;
	clGetIntegerv(CL_MAX_VERTEX_ATTRIBS, &max_attributes);
	return max_attributes;
}

CL_Size CL_OpenGLGraphicContextProvider::get_max_texture_size() const
{
	CL_OpenGL::set_active(this);
	CLint max_size = 0;
	clGetIntegerv(CL_MAX_TEXTURE_SIZE, &max_size);
	return CL_Size(max_size, max_size);
}

int CL_OpenGLGraphicContextProvider::get_width() const
{
	if (framebuffer_bound)
		return draw_buffer_provider->get_size().width;
	else
		return render_window->get_viewport_width();
}

int CL_OpenGLGraphicContextProvider::get_height() const
{
	if (framebuffer_bound)
		return draw_buffer_provider->get_size().height;
	else
		return render_window->get_viewport_height();
}


#ifdef __APPLE__
static CFBundleRef cl_gBundleRefOpenGL = 0;
#endif

CL_ProcAddress *CL_OpenGLGraphicContextProvider::get_proc_address(const CL_String8& function_name) const
{

#ifdef WIN32
	return (void (*)())wglGetProcAddress(function_name.c_str());
#else
#ifdef __APPLE__
	// Mac OS X doesn't have an OpenGL extension fetch function. Isn't that silly?
	if (cl_gBundleRefOpenGL == 0)
	{
		cl_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
		if (cl_gBundleRefOpenGL == 0)
			throw CL_Exception("Unable to find com.apple.opengl bundle");
	}

	return (CL_ProcAddress *) CFBundleGetFunctionPointerForName(
		cl_gBundleRefOpenGL,
		CFStringCreateWithCStringNoCopy(
			0,
			function_name.c_str(),
			CFStringGetSystemEncoding(),
			0));
#else

	const CL_GL_RenderWindowProvider_GLX *wptr = dynamic_cast<const CL_GL_RenderWindowProvider_GLX *> (render_window);
	if (wptr == NULL)
		return NULL;

	return wptr->get_proc_address(function_name);
#endif
#endif

}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLGraphicContextProvider Operations:

void CL_OpenGLGraphicContextProvider::destroy()
{
	delete this;
}

CL_GraphicContext CL_OpenGLGraphicContextProvider::create_worker_gc()
{
	return CL_GraphicContext(new CL_OpenGLGraphicContextProvider(render_window->new_worker_context()));
}

CL_OcclusionQueryProvider *CL_OpenGLGraphicContextProvider::alloc_occlusion_query()
{
	return new CL_OpenGLOcclusionQueryProvider(this);
}

CL_ProgramObjectProvider *CL_OpenGLGraphicContextProvider::alloc_program_object()
{
	return new CL_OpenGLProgramObjectProvider(this);
}

CL_ShaderObjectProvider *CL_OpenGLGraphicContextProvider::alloc_shader_object()
{
	return new CL_OpenGLShaderObjectProvider(this);
}

CL_TextureProvider *CL_OpenGLGraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return new CL_OpenGLTextureProvider(this, texture_dimensions);
}

CL_FrameBufferProvider *CL_OpenGLGraphicContextProvider::alloc_frame_buffer()
{
	return new CL_OpenGLFrameBufferProvider(this);
}

CL_RenderBufferProvider *CL_OpenGLGraphicContextProvider::alloc_render_buffer()
{
	return new CL_OpenGLRenderBufferProvider(this);
}

CL_VertexArrayBufferProvider *CL_OpenGLGraphicContextProvider::alloc_vertex_array_buffer()
{
	return new CL_OpenGLVertexArrayBufferProvider(this);
}

CL_ElementArrayBufferProvider *CL_OpenGLGraphicContextProvider::alloc_element_array_buffer()
{
	return new CL_OpenGLElementArrayBufferProvider(this);
}

CL_PixelBufferProvider *CL_OpenGLGraphicContextProvider::alloc_pixel_buffer()
{
	return new CL_OpenGLPixelBufferProvider(this);
}

CL_PixelBuffer CL_OpenGLGraphicContextProvider::get_pixeldata(const CL_Rect& rect) const 
{
	CL_OpenGL::set_active(this);

	if( rect.left != rect.right )
	{
		CL_PixelBuffer pbuf(rect.get_width(), rect.get_height(), cl_abgr8);
		clReadPixels(rect.left, rect.top, rect.get_width(), rect.get_height(), CL_RGBA, CL_UNSIGNED_BYTE, pbuf.get_data());
		pbuf.flip_vertical();
		return pbuf;
	}

	CL_PixelBuffer pbuf( get_width(), get_height(), cl_abgr8);
	clReadPixels(0, 0, get_width(), get_height(), CL_RGBA, CL_UNSIGNED_BYTE, pbuf.get_data());
	pbuf.flip_vertical();
	return pbuf;
}

void CL_OpenGLGraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
	CL_OpenGL::set_active(this);

	if (clActiveTexture != 0)
	{
		clActiveTexture( CL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}

	if (texture.is_null())
	{
		// Perhaps call clBindTexture(texture_type, 0) ?
	}
	else
	{
		CL_OpenGLTextureProvider *provider = static_cast<CL_OpenGLTextureProvider *>(texture.get_provider());
		clEnable(provider->get_texture_type());
		clBindTexture(provider->get_texture_type(), provider->get_handle());
	}
}

void CL_OpenGLGraphicContextProvider::reset_texture(int unit_index, const CL_Texture &texture)
{
	CL_OpenGL::set_active(this);

	if (clActiveTexture != 0)
	{
		clActiveTexture( CL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}
	// Set the texture to the default state
	clBindTexture(CL_TEXTURE_2D, 0);
}

void CL_OpenGLGraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &draw_buffer, const CL_FrameBuffer &read_buffer)
{
	CL_OpenGL::set_active(this);

	draw_buffer_provider = dynamic_cast<CL_OpenGLFrameBufferProvider *>(draw_buffer.get_provider());
	read_buffer_provider = dynamic_cast<CL_OpenGLFrameBufferProvider *>(read_buffer.get_provider());

	// Check for framebuffer completeness
	// (Ideally this should be before rendering)
	draw_buffer_provider->check_framebuffer_complete();
	if (draw_buffer_provider != read_buffer_provider)
		read_buffer_provider->check_framebuffer_complete();

	// Bind the framebuffers
	CLuint draw_handle = draw_buffer_provider->get_handle();
	clBindFramebuffer(CL_FRAMEBUFFER, draw_handle);

	if (draw_buffer_provider != read_buffer_provider)
	{
		CLuint read_handle = read_buffer_provider->get_handle();
		clBindFramebuffer(CL_READ_FRAMEBUFFER, read_handle);
	}

	// Save the map mode before when the framebuffer was bound
	if (!framebuffer_bound)	
	{
		map_mode_before_framebuffer = map_mode;

		framebuffer_bound = true;
		if (map_mode != cl_user_projection)
			set_map_mode(map_mode);
	}
}

void CL_OpenGLGraphicContextProvider::reset_frame_buffer()
{
	CL_OpenGL::set_active(this);
	clBindFramebuffer(CL_FRAMEBUFFER, 0);
	clBindFramebuffer(CL_READ_FRAMEBUFFER, 0);

	if (framebuffer_bound)
	{
		framebuffer_bound = false;

		if (map_mode != cl_user_projection)
			set_map_mode(map_mode_before_framebuffer);
	}
}

void CL_OpenGLGraphicContextProvider::set_program_object(CL_StandardProgram standard_program)
{
	set_program_object(standard_programs[(int)standard_program], cl_program_matrix_modelview_projection);
}

void CL_OpenGLGraphicContextProvider::set_program_object(const CL_ProgramObject &program, int program_matrix_flags)
{
	current_program_matrix_flags = program_matrix_flags;
	current_program_object = CL_ProgramObject();

	CL_OpenGL::set_active(this);
	if (clUseProgram == 0)
		return;

	if (program.is_null())
		clUseProgram(0);
	else
	{
		clUseProgram(program.get_handle());
		current_program_object = program;
		set_current_program_object_matricies();
	}
}

void CL_OpenGLGraphicContextProvider::reset_program_object()
{
	CL_OpenGL::set_active(this);
	clUseProgram(0);
	current_program_object = CL_ProgramObject();
}

void CL_OpenGLGraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
/*	if (cur_prim_array != prim_array) 
	{
		cur_prim_array = prim_array;
		if (prim_arrays_set)
			reset_primitives_array();
		set_primitives_array(prim_array);
	}*/

	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void CL_OpenGLGraphicContextProvider::primitives_array_freed(const CL_PrimitivesArrayData * const prim_array)
{
	if (cur_prim_array == prim_array)
	{
		reset_primitives_array();
		cur_prim_array = 0;
	}
}

void CL_OpenGLGraphicContextProvider::set_primitives_array(const CL_PrimitivesArrayData * const prim_array)
{
	if (prim_arrays_set)
		reset_primitives_array();
	CL_OpenGL::set_active(this);
	prim_arrays_set = true;
	num_set_program_attribute_arrays = 0;
	for (int i = 0; i < prim_array->num_attributes; i++)
		num_set_program_attribute_arrays = cl_max(num_set_program_attribute_arrays, prim_array->attribute_indexes[i]+1);

	for (int i = 0; i < prim_array->num_attributes; i++)
	{
		const CL_PrimitivesArrayData::VertexData &attribute = prim_array->attributes[i];
		if (attribute.single_value)
		{
			int attrib_index = prim_array->attribute_indexes[i];
			switch (attribute.size)
			{
			case 1:
				switch (attribute.type)
				{
				default: break;
				case cl_type_short: clVertexAttrib1sv(attrib_index, attribute.value_short); break;
				case cl_type_float: clVertexAttrib1fv(attrib_index, attribute.value_float); break;
				}
				break;
			case 2:
				switch (attribute.type)
				{
				default: break;
				case cl_type_short: clVertexAttrib2sv(attrib_index, attribute.value_short); break;
				case cl_type_float: clVertexAttrib2fv(attrib_index, attribute.value_float); break;
				}
				break;
			case 3:
				switch (attribute.type)
				{
				default: break;
				case cl_type_short: clVertexAttrib3sv(attrib_index, attribute.value_short); break;
				case cl_type_float: clVertexAttrib3fv(attrib_index, attribute.value_float); break;
				}
				break;
			case 4:
				if (prim_array->normalize_attributes[i])
				{
					switch (attribute.type)
					{
					default: break;
					case cl_type_unsigned_byte: clVertexAttrib4Nubv(attrib_index, attribute.value_ubyte); break;
					case cl_type_unsigned_short: clVertexAttrib4Nusv(attrib_index, attribute.value_ushort); break;
					case cl_type_unsigned_int: clVertexAttrib4Nuiv(attrib_index, attribute.value_uint); break;
					case cl_type_byte: clVertexAttrib4Nbv(attrib_index, (CLbyte*) attribute.value_byte); break;
					case cl_type_short: clVertexAttrib4Nsv(attrib_index, attribute.value_short); break;
					case cl_type_int: clVertexAttrib4Niv(attrib_index, attribute.value_int); break;
					}
				}
				else
				{
					switch (attribute.type)
					{
					default: break;
					case cl_type_unsigned_byte: clVertexAttrib4ubv(attrib_index, attribute.value_ubyte); break;
					case cl_type_unsigned_short: clVertexAttrib4usv(attrib_index, attribute.value_ushort); break;
					case cl_type_unsigned_int: clVertexAttrib4uiv(attrib_index, attribute.value_uint); break;
					case cl_type_byte: clVertexAttrib4bv(attrib_index, (CLbyte*) attribute.value_byte); break;
					case cl_type_short: clVertexAttrib4sv(attrib_index, attribute.value_short); break;
					case cl_type_int: clVertexAttrib4iv(attrib_index, attribute.value_int); break;
					case cl_type_float: clVertexAttrib4fv(attrib_index, attribute.value_float); break;
					}
				}
				break;
			}
		}
		else if (attribute.array_provider)
		{
			clBindBuffer(CL_ARRAY_BUFFER, static_cast<CL_OpenGLVertexArrayBufferProvider *>(attribute.array_provider)->get_handle());
			clEnableVertexAttribArray(prim_array->attribute_indexes[i]);
			clVertexAttribPointer(
				prim_array->attribute_indexes[i], attribute.size, to_enum(attribute.type),
				prim_array->normalize_attributes[i], attribute.stride, attribute.data);
			clBindBuffer(CL_ARRAY_BUFFER, 0);
		}
		else if (attribute.data)
		{
			clEnableVertexAttribArray(prim_array->attribute_indexes[i]);
			clVertexAttribPointer(
				prim_array->attribute_indexes[i], attribute.size, to_enum(attribute.type),
				prim_array->normalize_attributes[i], attribute.stride, attribute.data);
		}
	}
}

void CL_OpenGLGraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
	CL_OpenGL::set_active(this);
/*	if (num_vertices < 64) // For small arrays it is faster to use ArrayElement instead of DrawArrays
	{
		clBegin(to_enum(type));
		for (int i = 0; i < num_vertices; i++)
			clArrayElement(offset + i);
		clEnd();
	}
	else
	{*/
		clDrawArrays(to_enum(type), offset, num_vertices);
//	}
}

void CL_OpenGLGraphicContextProvider::draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count)
{
	CL_OpenGL::set_active(this);
	clDrawArraysInstanced(to_enum(type), offset, num_vertices, instance_count);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
	CL_OpenGL::set_active(this);
	clDrawElements(to_enum(type), count, CL_UNSIGNED_INT, indices);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
	CL_OpenGL::set_active(this);
	clDrawElements(to_enum(type), count, CL_UNSIGNED_SHORT, indices);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
	CL_OpenGL::set_active(this);
	clDrawElements(to_enum(type), count, CL_UNSIGNED_BYTE, indices);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(
	CL_PrimitivesType type,
	int count,
	CL_ElementArrayBufferProvider *array_provider,
	CL_VertexAttributeDataType indices_type,
	void *offset)
{
	CL_OpenGL::set_active(this);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, static_cast<CL_OpenGLElementArrayBufferProvider *>(array_provider)->get_handle());
	clDrawElements(to_enum(type), count, to_enum(indices_type), offset);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, 0);
}

void CL_OpenGLGraphicContextProvider::reset_primitives_array()
{
	CL_OpenGL::set_active(this);

	/* Removed in OpenGL 3 ...
	clDisableClientState(CL_VERTEX_ARRAY);
	clDisableClientState(CL_COLOR_ARRAY);
	if (clSecondaryColor3i)
		clDisableClientState(CL_SECONDARY_COLOR_ARRAY);
	clDisableClientState(CL_NORMAL_ARRAY);
	clDisableClientState(CL_EDGE_FLAG_ARRAY);
	clDisableClientState(CL_TEXTURE_COORD_ARRAY);
	*/

	for (int i=0; i<num_set_program_attribute_arrays; ++i)
	{
		clDisableVertexAttribArray(i);
	}

	prim_arrays_set = false;
}

void CL_OpenGLGraphicContextProvider::draw_pixels(CL_GraphicContext &gc,
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
}

void CL_OpenGLGraphicContextProvider::set_clip_rect(const CL_Rect &rect)
{
	CL_OpenGL::set_active(this);
	clEnable(CL_SCISSOR_TEST);

	if (map_mode == cl_map_2d_upper_left)
		clScissor(
			rect.left,
			get_height() - rect.top - rect.get_height(),
			rect.get_width(),
			rect.get_height());
	else
		clScissor(
			rect.left,
			rect.top,
			rect.get_width(),
			rect.get_height());

	last_clip_rect = rect;
}

void CL_OpenGLGraphicContextProvider::reset_clip_rect()
{
	CL_OpenGL::set_active(this);
	clDisable(CL_SCISSOR_TEST);
}

void CL_OpenGLGraphicContextProvider::clear(const CL_Colorf &color)
{
	CL_OpenGL::set_active(this);
	clClearColor(
		(CLclampf) color.r,
		(CLclampf) color.g,
		(CLclampf) color.b,
		(CLclampf) color.a);
	clClear(CL_COLOR_BUFFER_BIT);
}

void CL_OpenGLGraphicContextProvider::clear_stencil(int value)
{
	CL_OpenGL::set_active(this);
	clClearStencil(value);
	clClear(CL_STENCIL_BUFFER_BIT);
}

void CL_OpenGLGraphicContextProvider::clear_depth(float value)
{
	CL_OpenGL::set_active(this);
	clClearDepth(value);
	clClear(CL_DEPTH_BUFFER_BIT);
}

void CL_OpenGLGraphicContextProvider::set_map_mode(CL_MapMode mode)
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

void CL_OpenGLGraphicContextProvider::on_window_resized()
{
	int width = get_width();
	int height = get_height();

	switch (map_mode)
	{
	default:
		break;
	case cl_map_2d_upper_left:
		CL_OpenGL::set_active(this);
		set_viewport(CL_Rectf(0.0f, 0.0f, width, height));
		set_projection(CL_Mat4f::ortho_2d(0.0f, (float)width, (float)height, 0.0f));
		set_modelview(CL_Mat4f::identity());

		if (clIsEnabled(CL_SCISSOR_TEST))
			clScissor(
				last_clip_rect.left,
				get_height() - last_clip_rect.top - last_clip_rect.get_height(),
				last_clip_rect.get_width(),
				last_clip_rect.get_height());
		break;
	case cl_map_2d_lower_left:
		CL_OpenGL::set_active(this);
		set_viewport(CL_Rectf(0.0f, 0.0f, width, height));
		set_projection(CL_Mat4f::ortho_2d(0.0f, (float)width, 0.0f, (float)height));
		set_modelview(CL_Mat4f::identity());

		if (clIsEnabled(CL_SCISSOR_TEST))
			clScissor(
				last_clip_rect.left,
				last_clip_rect.top,
				last_clip_rect.get_width(),
				last_clip_rect.get_height());
		break;
	case cl_user_projection:
		CL_OpenGL::set_active(this);
		set_viewport(CL_Rectf(0.0f, 0.0f, width, height));
		if (clIsEnabled(CL_SCISSOR_TEST))
			clScissor(
				last_clip_rect.left,
				last_clip_rect.top,
				last_clip_rect.get_width(),
				last_clip_rect.get_height());
		break;
	}
}

void CL_OpenGLGraphicContextProvider::set_viewport(const CL_Rectf &viewport)
{
	int height = get_height();
	CL_OpenGL::set_active(this);
	clViewport(
		CLsizei(viewport.left),
		CLsizei(height - viewport.bottom),
		CLsizei(viewport.right - viewport.left),
		CLsizei(viewport.bottom - viewport.top));
}

void CL_OpenGLGraphicContextProvider::set_projection(const CL_Mat4f &matrix)
{
	if (projection != matrix)
	{
		projection = matrix;
		modelview_projection_matrix_valid = false;
		set_current_program_object_matricies();
	}
}

void CL_OpenGLGraphicContextProvider::set_modelview(const CL_Mat4f &matrix)
{
	CL_Mat4f matrix_copy(matrix);

	if (modelview != matrix_copy)
	{
		modelview = matrix_copy;
		modelview_projection_matrix_valid = false;
		normal_matrix_valid = false;
		set_current_program_object_matricies();
	}
}

void CL_OpenGLGraphicContextProvider::set_blend_mode(const CL_BlendMode &mode)
{
	CL_OpenGL::set_active(this);

	if( mode.is_blend_enabled() )
		clEnable(CL_BLEND);
	else
		clDisable(CL_BLEND);

	const CL_Colorf &col = mode.get_blend_color();

	if (clBlendColor)
	{
		clBlendColor(
			CLclampf(col.get_red()),
			CLclampf(col.get_green()),
			CLclampf(col.get_blue()),
			CLclampf(col.get_alpha()));
	}

	if (mode.get_blend_equation() == mode.get_blend_equation_alpha())
	{
		if (clBlendEquation)
			clBlendEquation(to_enum(mode.get_blend_equation()));
	}
	else
	{
		if (clBlendEquationSeparate)
			clBlendEquationSeparate(
				to_enum(mode.get_blend_equation()),
				to_enum(mode.get_blend_equation_alpha()) );
	}


	if( mode.get_blend_function_src() == mode.get_blend_function_src_alpha() &&
		mode.get_blend_function_dest() == mode.get_blend_function_dest_alpha() )
	{
		if (clBlendFunc)
			clBlendFunc(to_enum(mode.get_blend_function_src()), to_enum(mode.get_blend_function_dest()));
	}
	else
	{
		if (clBlendFuncSeparate)
			clBlendFuncSeparate( 
				to_enum(mode.get_blend_function_src()),
				to_enum(mode.get_blend_function_dest()),
				to_enum(mode.get_blend_function_src_alpha()),
				to_enum(mode.get_blend_function_dest_alpha()) );
	}
}

void CL_OpenGLGraphicContextProvider::set_pen(const CL_Pen &pen)
{
	CL_OpenGL::set_active(this);

	if (clPointParameterf)
	{
		clPointParameterf(CL_POINT_FADE_THRESHOLD_SIZE, (CLfloat)pen.get_point_fade_treshold_size());
	}
	clPointSize((CLfloat)pen.get_point_size());
	clLineWidth((CLfloat)pen.get_line_width());

	if (pen.is_line_antialiased())
		clEnable(CL_LINE_SMOOTH);
	else
		clDisable(CL_LINE_SMOOTH);

	if (pen.is_using_vertex_program_point_sizes())
		clEnable(CL_VERTEX_PROGRAM_POINT_SIZE);
	else
		clDisable(CL_VERTEX_PROGRAM_POINT_SIZE);

	if(clPointParameterf)
	{
		switch (pen.get_point_sprite_origin())
		{
		case cl_point_sprite_origin_upper_left:
			clPointParameterf(CL_POINT_SPRITE_COORD_ORIGIN, CL_UPPER_LEFT);
			break;
		case cl_point_sprite_origin_lower_left:
			clPointParameterf(CL_POINT_SPRITE_COORD_ORIGIN, CL_LOWER_LEFT);
			break;
		}
	}
}

void CL_OpenGLGraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
	CL_OpenGL::set_active(this);

	if (raster.is_antialiased())
		clEnable(CL_POLYGON_SMOOTH);
	else
		clDisable(CL_POLYGON_SMOOTH);

	if (raster.is_culled())
		clEnable(CL_CULL_FACE);
	else
		clDisable(CL_CULL_FACE);

	if (raster.is_line_offset())
		clEnable(CL_POLYGON_OFFSET_LINE);
	else
		clDisable(CL_POLYGON_OFFSET_LINE);

	if (raster.is_point_offset())
		clEnable(CL_POLYGON_OFFSET_POINT);
	else
		clDisable(CL_POLYGON_OFFSET_POINT);

	if (raster.is_polygon_offset())
		clEnable(CL_POLYGON_OFFSET_FILL);
	else
		clDisable(CL_POLYGON_OFFSET_FILL);

	clPolygonMode(CL_FRONT, to_enum(raster.get_face_fill_mode_front()));
	clPolygonMode(CL_BACK, to_enum(raster.get_face_fill_mode_back()));

	switch (raster.get_front_face())
	{
	case cl_face_side_counter_clockwise:
		clFrontFace(CL_CCW);
		break;
	case cl_face_side_clockwise:
		clFrontFace(CL_CW);
		break;
	}

	switch (raster.get_face_cull_mode())
	{
	case cl_cull_front:
		clCullFace(CL_FRONT);
		break;
	case cl_cull_back:
		clCullFace(CL_BACK);
		break;
	case cl_cull_front_and_back:
		clCullFace(CL_FRONT_AND_BACK);
		break;
	}

	clPolygonOffset(raster.get_offset_factor(), raster.get_offset_units());
}

void CL_OpenGLGraphicContextProvider::set_buffer_control(const CL_BufferControl &bc)
{
	CL_OpenGL::set_active(this);

	bool r, g, b, a;
	bc.is_color_write_enabled(r, g, b, a);
	clColorMask(r,g,b,a);

	if( bc.is_depth_test_enabled() )
		clEnable(CL_DEPTH_TEST);
	else
		clDisable(CL_DEPTH_TEST);

	clDepthFunc(to_enum(bc.get_depth_compare_function()));
	clDepthMask(bc.is_depth_write_enabled() ? 1 : 0);

	if (bc.is_stencil_test_enabled())
	{
		clEnable(CL_STENCIL_TEST);

		if (clStencilFuncSeparate)
		{
			clStencilFuncSeparate(CL_FRONT,
				to_enum(bc.get_stencil_compare_func_front()),
				bc.get_stencil_compare_reference_front(),
				bc.get_stencil_compare_mask_front());

			clStencilFuncSeparate(CL_BACK,
				to_enum(bc.get_stencil_compare_func_back()),
				bc.get_stencil_compare_reference_back(),
				bc.get_stencil_compare_mask_back());
		}

		if (clStencilOpSeparate)
		{
			clStencilOpSeparate(CL_FRONT,
				to_enum(bc.get_stencil_fail_front()),
				to_enum(bc.get_stencil_pass_depth_fail_front()),
				to_enum(bc.get_stencil_pass_depth_pass_front()));

			clStencilOpSeparate(CL_BACK,
				to_enum(bc.get_stencil_fail_back()),
				to_enum(bc.get_stencil_pass_depth_fail_back()),
				to_enum(bc.get_stencil_pass_depth_pass_back()));
		}

		if (clStencilMaskSeparate)
		{
			clStencilMaskSeparate( CL_FRONT, bc.get_stencil_write_mask_front() );
			clStencilMaskSeparate( CL_BACK, bc.get_stencil_write_mask_back() );
		}
	}
	else
	{
		clDisable(CL_STENCIL_TEST);
	}

	clDrawBuffer( to_enum(bc.get_draw_buffer()) );

	if (bc.is_logic_op_enabled())
	{
		clEnable(CL_COLOR_LOGIC_OP);
		clLogicOp(to_enum(bc.get_logic_op()));
	}
	else
	{
		clDisable(CL_COLOR_LOGIC_OP);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLGraphicContextProvider Implementation:

CLenum CL_OpenGLGraphicContextProvider::to_enum(CL_DrawBuffer buffer)
{
	switch(buffer)
	{
	case cl_buffer_back: return CL_BACK;
	case cl_buffer_back_left: return CL_BACK_LEFT;
	case cl_buffer_back_right: return CL_BACK_RIGHT;
	case cl_buffer_front: return CL_FRONT;
	case cl_buffer_front_and_back: return CL_FRONT_AND_BACK;
	case cl_buffer_front_left: return CL_FRONT_LEFT;
	case cl_buffer_front_right: return CL_FRONT_RIGHT;
	case cl_buffer_left: return CL_LEFT;
	case cl_buffer_none: return CL_NONE;
	case cl_buffer_right: return CL_RIGHT;
	default: return CL_BACK;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(CL_CompareFunction func)
{
	switch( func )
	{
	case cl_comparefunc_never: return CL_NEVER;
	case cl_comparefunc_less: return CL_LESS;
	case cl_comparefunc_lequal: return CL_LEQUAL; 
	case cl_comparefunc_greater: return CL_GREATER; 
	case cl_comparefunc_gequal: return CL_GEQUAL; 
	case cl_comparefunc_equal: return CL_EQUAL; 
	case cl_comparefunc_notequal: return CL_NOTEQUAL; 
	case cl_comparefunc_always: return CL_ALWAYS; 
	default: return CL_LEQUAL;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(CL_StencilOp op)
{
	switch( op )
	{
	case cl_stencil_decr: return CL_DECR;
	case cl_stencil_decr_wrap: return CL_DECR_WRAP;
	case cl_stencil_incr: return CL_INCR;
	case cl_stencil_incr_wrap: return CL_INCR_WRAP;
	case cl_stencil_invert: return CL_INVERT;
	case cl_stencil_keep: return CL_KEEP;
	case cl_stencil_replace: return CL_REPLACE;
	case cl_stencil_zero: return CL_ZERO;	
	default: return CL_KEEP;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(CL_CullMode mode)
{
	switch( mode )
	{
	case cl_cull_front: return CL_FRONT;
	case cl_cull_back: return CL_BACK;
	case cl_cull_front_and_back: return CL_FRONT_AND_BACK;
	default: return CL_BACK;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(CL_FillMode mode)
{
	switch( mode )
	{
	case cl_fill_point: return CL_POINT;
	case cl_fill_line: return CL_LINE;
	case cl_fill_polygon: return CL_FILL;
	default: return CL_FILL;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(CL_BlendFunc func)
{
	switch( func )
	{
	case cl_blend_zero: return CL_ZERO;
	case cl_blend_one: return CL_ONE;
	case cl_blend_dest_color: return CL_DST_COLOR;
	case cl_blend_src_color: return CL_SRC_COLOR;
	case cl_blend_one_minus_dest_color: return CL_ONE_MINUS_DST_COLOR;
	case cl_blend_one_minus_src_color: return CL_ONE_MINUS_SRC_COLOR;
	case cl_blend_src_alpha: return CL_SRC_ALPHA;
	case cl_blend_one_minus_src_alpha: return CL_ONE_MINUS_SRC_ALPHA;
	case cl_blend_dest_alpha: return CL_DST_ALPHA;
	case cl_blend_one_minus_dest_alpha: return CL_ONE_MINUS_DST_ALPHA;
	case cl_blend_src_alpha_saturate: return CL_SRC_ALPHA_SATURATE;
	case cl_blend_constant_color: return CL_CONSTANT_COLOR;
	case cl_blend_one_minus_constant_color: return CL_ONE_MINUS_CONSTANT_COLOR;
	case cl_blend_constant_alpha: return CL_CONSTANT_ALPHA;
	case cl_blend_one_minus_constant_alpha: return CL_ONE_MINUS_CONSTANT_ALPHA;
	default: return CL_BLEND_SRC;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(CL_BlendEquation eq)
{
	switch( eq )
	{
	case cl_blend_equation_add: return CL_FUNC_ADD;
	case cl_blend_equation_subtract: return CL_FUNC_SUBTRACT;
	case cl_blend_equation_reverse_subtract: return CL_FUNC_REVERSE_SUBTRACT;
	case cl_blend_equation_min: return CL_MIN;
	case cl_blend_equation_max: return CL_MAX;
	default: return CL_FUNC_ADD;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(enum CL_VertexAttributeDataType value)
{
	switch(value)
	{
	case cl_type_unsigned_byte:
		return CL_UNSIGNED_BYTE;
	case cl_type_unsigned_short:
		return CL_UNSIGNED_SHORT;
	case cl_type_unsigned_int:
		return CL_UNSIGNED_INT;
	case cl_type_byte:
		return CL_BYTE;
	case cl_type_short:
		return CL_SHORT;
	case cl_type_int:
		return CL_INT;
	case cl_type_float:
		return CL_FLOAT;
	default:
		return 0;
	}
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(enum CL_PrimitivesType value)
{
	CLenum gl_mode = 0;
	switch (value)
	{
	case cl_points: gl_mode = CL_POINTS; break;
	case cl_line_strip: gl_mode = CL_LINE_STRIP; break;
	case cl_line_loop: gl_mode = CL_LINE_LOOP; break;
	case cl_lines: gl_mode = CL_LINES; break;
	case cl_triangle_strip: gl_mode = CL_TRIANGLE_STRIP; break;
	case cl_triangle_fan: gl_mode = CL_TRIANGLE_FAN; break;
	case cl_triangles: gl_mode = CL_TRIANGLES; break;
	}
	return gl_mode;
}

CLenum CL_OpenGLGraphicContextProvider::to_enum(enum CL_LogicOp op)
{
	CLenum gl_op = 0;
	switch (op)
	{
		case cl_logic_op_clear: gl_op = CL_CLEAR; break;  
		case cl_logic_op_and: gl_op = CL_AND; break;
		case cl_logic_op_and_reverse: gl_op = CL_AND_REVERSE; break;
		case cl_logic_op_copy: gl_op = CL_COPY; break;
		case cl_logic_op_and_inverted: gl_op = CL_AND_INVERTED; break;
		case cl_logic_op_noop: gl_op = CL_NOOP; break;
		case cl_logic_op_xor: gl_op = CL_XOR; break;
		case cl_logic_op_or: gl_op = CL_OR; break;
		case cl_logic_op_nor: gl_op = CL_NOR; break;
		case cl_logic_op_equiv: gl_op = CL_EQUIV; break;
		case cl_logic_op_invert: gl_op = CL_INVERT; break;
		case cl_logic_op_or_reverse: gl_op = CL_OR_REVERSE; break;
		case cl_logic_op_copy_inverted: gl_op = CL_COPY_INVERTED; break;
		case cl_logic_op_or_inverted: gl_op = CL_OR_INVERTED; break;
		case cl_logic_op_nand: gl_op = CL_NAND; break;
		case cl_logic_op_set: gl_op = CL_SET; break;
		default: break;
	}
	return gl_op;
}

void CL_OpenGLGraphicContextProvider::set_current_program_object_matricies()
{
	if (current_program_object.is_null())	// Exit now if the program object is not yet set
		return;

	if (current_program_matrix_flags & cl_program_matrix_modelview_projection)
	{
		if (!modelview_projection_matrix_valid)
		{
			modelview_projection_matrix = modelview * projection;
			modelview_projection_matrix_valid = true;
		}
		current_program_object.set_uniform_matrix("cl_ModelViewProjectionMatrix", modelview_projection_matrix);

	}

	if (current_program_matrix_flags & cl_program_matrix_normal)
	{
		if (!normal_matrix_valid)
		{
			normal_matrix = modelview;
			normal_matrix.inverse();
			normal_matrix.transpose();
			normal_matrix_valid = true;
		}
		current_program_object.set_uniform_matrix("cl_NormalMatrix", normal_matrix);
	}

	if (current_program_matrix_flags & cl_program_matrix_modelview)
	{
		current_program_object.set_uniform_matrix("cl_ModelViewMatrix", modelview);
	}

	if (current_program_matrix_flags & cl_program_matrix_projection)
	{
		current_program_object.set_uniform_matrix("cl_ProjectionMatrix", projection);
	}
}
