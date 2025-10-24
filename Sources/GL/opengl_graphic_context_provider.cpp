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
#include "API/Core/System/uniqueptr.h"
#include "API/GL/opengl_window_description.h"
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include "AGL/opengl_window_provider_agl.h"
#elif !defined(WIN32)
#include "GLX/opengl_window_provider_glx.h"
#endif

const CL_String::char_type *cl_glsl15_vertex_color_only = 
	"#version 150\n"
	"in vec4 Position, Color0; "
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"out vec4 Color; "
	"void main(void) { gl_Position = cl_ModelViewProjectionMatrix*Position; Color = Color0; }";

const CL_String::char_type *cl_glsl15_fragment_color_only =
	"#version 150\n"
	"in vec4 Color; "
	"out vec4 cl_FragColor;"
	"void main(void) { cl_FragColor = Color; }";

const CL_String::char_type *cl_glsl15_vertex_single_texture =
	"#version 150\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"void main(void) { gl_Position = cl_ModelViewProjectionMatrix*Position; Color = Color0; TexCoord = TexCoord0; }";

const CL_String::char_type *cl_glsl15_fragment_single_texture =
	"#version 150\n"
	"uniform sampler2D Texture0; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"out vec4 cl_FragColor;"
	"void main(void) { cl_FragColor = Color*texture2D(Texture0, TexCoord); }";

const CL_String::char_type *cl_glsl15_vertex_sprite =
	"#version 150\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"in float TexIndex0; "
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"out float TexIndex; "
	"void main(void) { gl_Position = cl_ModelViewProjectionMatrix*Position; Color = Color0; TexCoord = TexCoord0; TexIndex = TexIndex0; }";

const CL_String::char_type *cl_glsl15_fragment_sprite =
	"#version 150\n"
	"uniform sampler2D Texture0; "
	"uniform sampler2D Texture1; "
	"uniform sampler2D Texture2; "
	"uniform sampler2D Texture3; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"in float TexIndex; "
	"out vec4 cl_FragColor;"
	"highp vec4 sampleTexture(int index, highp vec2 pos) { if (index == 0) return texture2D(Texture0, TexCoord); else if (index == 1) return texture2D(Texture1, TexCoord); else if (index == 2) return texture2D(Texture2, TexCoord); else if (index == 3) return texture2D(Texture3, TexCoord); else return vec4(1.0,1.0,1.0,1.0); }"
	"void main(void) { cl_FragColor = Color*sampleTexture(int(TexIndex), TexCoord); } ";

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

CL_OpenGLGraphicContextProvider::CL_OpenGLGraphicContextProvider(const CL_RenderWindowProvider * const render_window, const CL_OpenGLWindowDescription &gldesc)
: render_window(render_window), map_mode(cl_map_2d_upper_left), projection(CL_Mat4f::identity()), modelview(CL_Mat4f::identity()),
  framebuffer_bound(false), prim_arrays_set(false), num_set_program_attribute_arrays(0), cur_prim_array(0),
  modelview_projection_matrix_valid(false), normal_matrix_valid(false),
  opengl_version_major(0), shader_version_major(0)
{
	check_opengl_version();

	use_glsl_1_50 = false;
	int glsl_version_major;
	int glsl_version_minor;
	get_opengl_shading_language_version(glsl_version_major, glsl_version_minor);
	if ( glsl_version_major == 1)
	{
		if ( glsl_version_minor >= 50)
			use_glsl_1_50 = true;
	}
	if ( glsl_version_major > 1)
			use_glsl_1_50 = true;

	// Must write here, although CL_OpenGL::SetActive() updates it, because the version number is not available on the initial CL_OpenGL::SetActive() call
	CL_OpenGL::opengl_version_major = opengl_version_major;
	CL_OpenGL::opengl_version_minor = opengl_version_minor;
	CL_OpenGL::glsl_version_major = shader_version_major;
	CL_OpenGL::glsl_version_minor = shader_version_minor;

	// We cannot use opengl_version_major or opengl_version_minor because OpenGL may still create a higher but compatible context
	if (gldesc.get_version_major() < 3)
	{
		allow_vertex_array_without_buffer_object = true;
	}
	else
	{
		if ( (gldesc.get_version_major() == 3) && (gldesc.get_version_minor() == 0) )
		{
			allow_vertex_array_without_buffer_object = true;
		}
		else
		{
			allow_vertex_array_without_buffer_object = false;
		}
	}

#if defined(__APPLE__)
	// Force glsl 1_5 for apple
	use_glsl_1_50 = true;
#endif
	CL_ShaderObject vertex_color_only_shader(this, cl_shadertype_vertex, use_glsl_1_50 ? cl_glsl15_vertex_color_only : cl_glsl_vertex_color_only);
	if(!vertex_color_only_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'vertex color only' Error:" + vertex_color_only_shader.get_info_log());

	CL_ShaderObject fragment_color_only_shader(this, cl_shadertype_fragment, use_glsl_1_50 ? cl_glsl15_fragment_color_only : cl_glsl_fragment_color_only);
	if(!fragment_color_only_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'fragment color only' Error:" + fragment_color_only_shader.get_info_log());

	CL_ShaderObject vertex_single_texture_shader(this, cl_shadertype_vertex, use_glsl_1_50 ? cl_glsl15_vertex_single_texture : cl_glsl_vertex_single_texture);
	if(!vertex_single_texture_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'vertex single texture' Error:" + vertex_single_texture_shader.get_info_log());

	CL_ShaderObject fragment_single_texture_shader(this, cl_shadertype_fragment, use_glsl_1_50 ? cl_glsl15_fragment_single_texture : cl_glsl_fragment_single_texture);
	if(!fragment_single_texture_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'fragment single texture' Error:" + fragment_single_texture_shader.get_info_log());

	CL_ShaderObject vertex_sprite_shader(this, cl_shadertype_vertex, use_glsl_1_50 ? cl_glsl15_vertex_sprite : cl_glsl_vertex_sprite);
	if(!vertex_sprite_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'vertex sprite' Error:" + vertex_sprite_shader.get_info_log());

	CL_ShaderObject fragment_sprite_shader(this, cl_shadertype_fragment, use_glsl_1_50 ? cl_glsl15_fragment_sprite : cl_glsl_fragment_sprite);
	if(!fragment_sprite_shader.compile())
		throw CL_Exception("Unable to compile the standard shader program: 'fragment sprite' Error:" + fragment_sprite_shader.get_info_log());

	CL_ProgramObject color_only_program(this);
	color_only_program.attach(vertex_color_only_shader);
	color_only_program.attach(fragment_color_only_shader);
	color_only_program.bind_attribute_location(0, "Position");
	color_only_program.bind_attribute_location(1, "Color0");
	if (use_glsl_1_50)
		color_only_program.bind_frag_data_location(0, "cl_FragColor");

	if (!color_only_program.link())
		throw CL_Exception("Unable to link the standard shader program: 'color only' Error:" + color_only_program.get_info_log());

	CL_ProgramObject single_texture_program(this);
	single_texture_program.attach(vertex_single_texture_shader);
	single_texture_program.attach(fragment_single_texture_shader);
	single_texture_program.bind_attribute_location(0, "Position");
	single_texture_program.bind_attribute_location(1, "Color0");
	single_texture_program.bind_attribute_location(2, "TexCoord0");
	if (use_glsl_1_50)
		single_texture_program.bind_frag_data_location(0, "cl_FragColor");
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
	if (use_glsl_1_50)
		sprite_program.bind_frag_data_location(0, "cl_FragColor");
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


	// Enable point sprites for legacy opengl
	bool use_opengl_3_2 = false;
	if ( opengl_version_major == 3)
	{
		if ( opengl_version_minor >= 2)
			use_opengl_3_2 = true;
	}
	if ( opengl_version_major > 3)
		use_opengl_3_2 = true;

	if (!use_opengl_3_2)
		glEnable(0x8861);	// GL_POINT_SPRITE
}

CL_OpenGLGraphicContextProvider::~CL_OpenGLGraphicContextProvider()
{
	while (!disposable_objects.empty())
		disposable_objects.front()->dispose();

	current_program_object = CL_ProgramObject();
	standard_programs.clear();

	CL_OpenGL::remove_active(this);
	delete render_window;

}

void CL_OpenGLGraphicContextProvider::add_disposable(CL_DisposableObject *disposable)
{
	disposable_objects.push_back(disposable);
}

void CL_OpenGLGraphicContextProvider::remove_disposable(CL_DisposableObject *disposable)
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

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLGraphicContextProvider Attributes:

void CL_OpenGLGraphicContextProvider::check_opengl_version()
{
	int version_major = 0;
	int version_minor = 0;

	get_opengl_version(version_major, version_minor);
	if(version_major < 2)
		throw CL_Exception(cl_format("This application requires OpenGL 2.0 or above. Your hardware only supports OpenGL %1.%2. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor));
}

void CL_OpenGLGraphicContextProvider::get_opengl_version(int &version_major, int &version_minor)
{
	if (!opengl_version_major)	// Is not cached
	{

	/*	The GL_VERSION string begins with a version number. The version number uses one of these forms: 
		major_number.minor_number 
		major_number.minor_number.release_number 
		Vendor-specific information may follow the version number. Its format depends on the implementation, but a space always separates the version number and the vendor-specific information. 
		All strings are null-terminated. 
		If an error is generated, glGetString returns zero.
	*/
		CL_OpenGL::set_active(this);

		#if defined(__APPLE__)
			opengl_version_major = 0;
			opengl_version_minor = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &opengl_version_major)
			glGetIntegerv(GL_MINOR_VERSION, &opengl_version_minor)
		#else

			CL_String version = (char*)glGetString(GL_VERSION);

			opengl_version_major = 0;
			opengl_version_minor = 0;
    
			std::vector<CL_String> split_version = CL_StringHelp::split_text(version, ".");
			if(split_version.size() > 0)
				opengl_version_major = CL_StringHelp::text_to_int(split_version[0]);
			if(split_version.size() > 1)
				opengl_version_minor = CL_StringHelp::text_to_int(split_version[1]);
		#endif
	}

	version_major = opengl_version_major;
	version_minor = opengl_version_minor;
}

void CL_OpenGLGraphicContextProvider::get_opengl_shading_language_version(int &version_major, int &version_minor)
{
	if (!shader_version_major)	// Is not cached
	{
		// See http://www.opengl.org/wiki/Detecting_the_Shader_Model
		shader_version_major = 0;
		shader_version_minor = 0;

		if ( (opengl_version_major < 2) || ( (opengl_version_major == 2) && (opengl_version_minor < 1) ) )
		{
			CL_OpenGL::set_active(this);
			
			CL_String version = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			std::vector<CL_String> split_version = CL_StringHelp::split_text(version, ".");
			if(split_version.size() > 0)
				shader_version_major = CL_StringHelp::text_to_int(split_version[0]);
			if(split_version.size() > 1)
				shader_version_minor = CL_StringHelp::text_to_int(split_version[1]);
		}
		else
		{
			if (opengl_version_major == 2)
			{
				shader_version_major = 1;
				shader_version_minor = 20;
			}
			else if ( (opengl_version_major == 3) && (opengl_version_minor == 0) )
			{
				shader_version_major = 1;
				shader_version_minor = 30;
			}
			else if ( (opengl_version_major == 3) && (opengl_version_minor == 1) )
			{
				shader_version_major = 1;
				shader_version_minor = 40;
			}
			else if ( (opengl_version_major == 3) && (opengl_version_minor == 2) )
			{
				shader_version_major = 1;
				shader_version_minor = 50;
			}
			else
			{
				shader_version_major = opengl_version_major;
				shader_version_minor = opengl_version_minor * 10;
			}
		}
	}
	version_major = shader_version_major;
	version_minor = shader_version_minor;
}

CL_String CL_OpenGLGraphicContextProvider::get_renderer_string()
{
	CL_OpenGL::set_active(this);
	CL_String renderer = (char*)glGetString(GL_RENDERER);
	return renderer;
}

CL_String CL_OpenGLGraphicContextProvider::get_vendor_string()
{
	CL_OpenGL::set_active(this);
	CL_String vendor = (char*)glGetString(GL_VENDOR);
	return vendor;
}

std::vector<CL_String> CL_OpenGLGraphicContextProvider::get_extensions()
{
	CL_OpenGL::set_active(this);
	CL_StringRef extension_string = (char*)glGetString(GL_EXTENSIONS);
	std::vector<CL_String> tmp = CL_StringHelp::split_text(extension_string, " ");
	std::vector<CL_String> extensions;
	for (std::vector<CL_String>::size_type i=0; i<tmp.size(); i++)
		extensions.push_back(tmp[i]);
	return extensions;
}

int CL_OpenGLGraphicContextProvider::get_max_attributes()
{
	CL_OpenGL::set_active(this);
	GLint max_attributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
    if(max_attributes < 16)
        max_attributes = 16;
	return max_attributes;
}

CL_Size CL_OpenGLGraphicContextProvider::get_max_texture_size() const
{
	CL_OpenGL::set_active(this);
	GLint max_size = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
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
        {
            cl_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengles"));
            if (cl_gBundleRefOpenGL == 0)
                throw CL_Exception("Unable to find opengl bundle");
        }
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

CL_OcclusionQueryProvider *CL_OpenGLGraphicContextProvider::alloc_occlusion_query()
{
	return new CL_OpenGLOcclusionQueryProvider(this);
}

CL_ProgramObjectProvider *CL_OpenGLGraphicContextProvider::alloc_program_object()
{
	return new CL_OpenGLProgramObjectProvider();
}

CL_ShaderObjectProvider *CL_OpenGLGraphicContextProvider::alloc_shader_object()
{
	return new CL_OpenGLShaderObjectProvider();
}

CL_TextureProvider *CL_OpenGLGraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return new CL_OpenGLTextureProvider(texture_dimensions);
}

CL_FrameBufferProvider *CL_OpenGLGraphicContextProvider::alloc_frame_buffer()
{
	return new CL_OpenGLFrameBufferProvider(this);
}

CL_RenderBufferProvider *CL_OpenGLGraphicContextProvider::alloc_render_buffer()
{
	return new CL_OpenGLRenderBufferProvider();
}

CL_VertexArrayBufferProvider *CL_OpenGLGraphicContextProvider::alloc_vertex_array_buffer()
{
	return new CL_OpenGLVertexArrayBufferProvider();
}

CL_ElementArrayBufferProvider *CL_OpenGLGraphicContextProvider::alloc_element_array_buffer()
{
	return new CL_OpenGLElementArrayBufferProvider();
}

CL_PixelBufferProvider *CL_OpenGLGraphicContextProvider::alloc_pixel_buffer()
{
	return new CL_OpenGLPixelBufferProvider();
}

CL_PixelBuffer CL_OpenGLGraphicContextProvider::get_pixeldata(const CL_Rect& rect2, CL_TextureFormat pixel_format, bool clamp) const 
{
	CL_Rect rect = rect2;
	if (rect == CL_Rect())
		rect = CL_Rect(0, 0, get_width(), get_height());

	GLenum format;
	GLenum type;
	bool found = CL_OpenGL::to_opengl_pixelformat(pixel_format, format, type);
	if (!found)
		throw CL_Exception("Unsupported pixel format passed to CL_GraphicContext::get_pixeldata");

	CL_PixelBuffer pbuf(rect.get_width(), rect.get_height(), pixel_format);
	CL_OpenGL::set_active(this);
	if (!framebuffer_bound)
		glReadBuffer(GL_BACK);

	if (glClampColor)
		glClampColor(GL_CLAMP_READ_COLOR, clamp ? GL_TRUE : GL_FALSE);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, pbuf.get_pitch() / pbuf.get_bytes_per_pixel());
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);

	glReadPixels(rect.left, rect.top, rect.get_width(), rect.get_height(), format, type, pbuf.get_data());
	pbuf.flip_vertical();
	return pbuf;
}

void CL_OpenGLGraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
	CL_OpenGL::set_active(this);

	if (glActiveTexture != 0)
	{
		glActiveTexture( GL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}

	if (texture.is_null())
	{
		// Perhaps call glBindTexture(texture_type, 0) ?
	}
	else
	{
		CL_OpenGLTextureProvider *provider = static_cast<CL_OpenGLTextureProvider *>(texture.get_provider());
		if (CL_OpenGL::get_opengl_version_major() < 3)
		{
			glEnable(provider->get_texture_type());
		}
		glBindTexture(provider->get_texture_type(), provider->get_handle());
	}
}

void CL_OpenGLGraphicContextProvider::reset_texture(int unit_index, const CL_Texture &texture)
{
	CL_OpenGL::set_active(this);

	if (glActiveTexture != 0)
	{
		glActiveTexture( GL_TEXTURE0 + unit_index );
	}
	else if (unit_index > 0)
	{
		return;
	}
	// Set the texture to the default state
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CL_OpenGLGraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &draw_buffer, const CL_FrameBuffer &read_buffer)
{
	draw_buffer_provider = dynamic_cast<CL_OpenGLFrameBufferProvider *>(draw_buffer.get_provider());
	read_buffer_provider = dynamic_cast<CL_OpenGLFrameBufferProvider *>(read_buffer.get_provider());

	if (draw_buffer_provider->get_gc_provider() != this || read_buffer_provider->get_gc_provider() != this)
		throw CL_Exception("CL_FrameBuffer objects cannot be shared between multiple CL_GraphicContext objects");

	CL_OpenGL::set_active(this);

	draw_buffer_provider->bind_framebuffer(true);
	if (draw_buffer_provider != read_buffer_provider)		// You cannot read and write to the same framebuffer
		read_buffer_provider->bind_framebuffer(false);

	// Check for framebuffer completeness
	draw_buffer_provider->check_framebuffer_complete();
	if (draw_buffer_provider != read_buffer_provider)
		read_buffer_provider->check_framebuffer_complete();

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

#ifdef __APPLE__
    cl_set_default_frame_buffer((CL_RenderWindowProvider*)render_window);
        
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
#endif
	glDrawBuffer(GL_BACK);	// OpenGL default
	glReadBuffer(GL_BACK);	// OpenGL default

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
	if (glUseProgram == 0)
		return;

	if (program.is_null())
		glUseProgram(0);
	else
	{
		glUseProgram(program.get_handle());
		current_program_object = program;
		set_current_program_object_matricies();
	}
}

void CL_OpenGLGraphicContextProvider::reset_program_object()
{
	CL_OpenGL::set_active(this);
	glUseProgram(0);
	current_program_object = CL_ProgramObject();
}

void CL_OpenGLGraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
	// Client vertex arrays must have a vertex buffer object for opengl 3.1 and above (without compatibility option)
	if (!allow_vertex_array_without_buffer_object)
	{
		for (int i = 0; i < prim_array->num_attributes; i++)
		{
			if ( (!prim_array->attributes[i].single_value) && (prim_array->attributes[i].array_provider == NULL) )
			{
				draw_primitives_legacy(type, num_vertices,  prim_array);
				return;
			}
		}
	}

	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void CL_OpenGLGraphicContextProvider::draw_primitives_legacy(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
	if (num_vertices <= 0)
		return;

	// This function converts vertex arrays without a vertex buffer object into a correct format

	CL_PrimitivesArrayData new_prim_array = *prim_array;

	std::vector< CL_SharedPtr<CL_VertexArrayBufferProvider> > vbo_store;
	std::vector< CL_PrimitivesArrayData::VertexData > vertex_data_store;
	vertex_data_store.resize(prim_array->num_attributes);
	new_prim_array.attributes = &(vertex_data_store[0]);

	bool first_array_found = false;
	bool use_stride_vbo = false;
	unsigned char *lowest_array_start_ptr;
	unsigned char *highest_array_end_ptr;

	for (int i = 0; i < prim_array->num_attributes; i++)
	{
		new_prim_array.attributes[i] = prim_array->attributes[i];

		if ( prim_array->attributes[i].single_value) 
			continue;

		if (prim_array->attributes[i].array_provider)
			throw CL_Exception("Mixed vertex arrays, with and without vertex buffer object's, are not implemented. Use vertex buffer objects");

		if (prim_array->attributes[i].data == NULL)
			throw CL_Exception("Vertex array specified without data. Use vertex buffer objects");

		// Check for single vbo required per vertex
		if (prim_array->attributes[i].stride == 0)
		{
			if (!first_array_found)
			{
				first_array_found = true;
				use_stride_vbo = false;
			}
			else
			{
				if (use_stride_vbo)
					throw CL_Exception("Vertex arrays using stride and non-stride, are not implemented. Use vertex buffer objects");
			}

			CL_VertexArrayBufferProvider *vbo_ptr = alloc_vertex_array_buffer();
			vbo_store.push_back(CL_SharedPtr<CL_VertexArrayBufferProvider>(vbo_ptr));

			int data_length = 0;
			switch (prim_array->attributes[i].type)
			{
				case cl_type_unsigned_byte:   data_length = sizeof(GLubyte); break;
				case cl_type_unsigned_short:  data_length = sizeof(GLushort); break;
				case cl_type_unsigned_int:    data_length = sizeof(GLuint); break;
				case cl_type_byte:            data_length = sizeof(GLbyte); break;
				case cl_type_short:           data_length = sizeof(GLshort); break;
				case cl_type_int:             data_length = sizeof(GLint); break;
				case cl_type_float:           data_length = sizeof(GLfloat); break;
				default:                      break;
			}

			vbo_ptr->create(prim_array->attributes[i].data, prim_array->attributes[i].size * data_length * num_vertices, cl_usage_static_draw);
			new_prim_array.attributes[i].array_provider = vbo_ptr;
			new_prim_array.attributes[i].data = (void *) 0;

		}
		else
		{
			// Multiple vbo required per vertex
			if (!first_array_found)
			{
				first_array_found = true;
				use_stride_vbo = true;
				lowest_array_start_ptr = (unsigned char *) prim_array->attributes[i].data;
				highest_array_end_ptr = lowest_array_start_ptr + prim_array->attributes[i].stride * num_vertices;
			}
			else
			{
				if (!use_stride_vbo)
					throw CL_Exception("Vertex arrays using stride and non-stride, are not implemented. Use vertex buffer objects");

				unsigned char *current_array_start_ptr = (unsigned char *) prim_array->attributes[i].data;
				unsigned char *current_array_end_ptr = current_array_start_ptr + prim_array->attributes[i].stride * num_vertices;

				// Validate belongs to the same array. (This does assume the stride values are identical)
				if ( (current_array_end_ptr <= (lowest_array_start_ptr - prim_array->attributes[i].stride)) || (current_array_start_ptr >= (highest_array_end_ptr + prim_array->attributes[i].stride)) )
					throw CL_Exception("Vertex arrays using stride for different locations, are not implemented. Use vertex buffer objects");

				if (current_array_start_ptr < lowest_array_start_ptr)
					lowest_array_start_ptr = current_array_start_ptr;

				if (current_array_end_ptr > highest_array_end_ptr)
					highest_array_end_ptr = current_array_end_ptr;

			}
		}
	}

	// Create single vbo
	if (use_stride_vbo && first_array_found)
	{
		CL_VertexArrayBufferProvider *vbo_ptr = alloc_vertex_array_buffer();
		vbo_store.push_back(CL_SharedPtr<CL_VertexArrayBufferProvider>(vbo_ptr));
		vbo_ptr->create(lowest_array_start_ptr, highest_array_end_ptr - lowest_array_start_ptr, cl_usage_static_draw);

		for (int i = 0; i < prim_array->num_attributes; i++)
		{
			if ( prim_array->attributes[i].single_value) 
				continue;
			new_prim_array.attributes[i].array_provider = vbo_ptr;
			new_prim_array.attributes[i].data = (void *) (((unsigned char *) new_prim_array.attributes[i].data) - lowest_array_start_ptr);
		}
	}

	set_primitives_array(&new_prim_array);
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
				case cl_type_short: glVertexAttrib1sv(attrib_index, attribute.value_short); break;
				case cl_type_float: glVertexAttrib1fv(attrib_index, attribute.value_float); break;
				}
				break;
			case 2:
				switch (attribute.type)
				{
				default: break;
				case cl_type_short: glVertexAttrib2sv(attrib_index, attribute.value_short); break;
				case cl_type_float: glVertexAttrib2fv(attrib_index, attribute.value_float); break;
				}
				break;
			case 3:
				switch (attribute.type)
				{
				default: break;
				case cl_type_short: glVertexAttrib3sv(attrib_index, attribute.value_short); break;
				case cl_type_float: glVertexAttrib3fv(attrib_index, attribute.value_float); break;
				}
				break;
			case 4:
				if (prim_array->normalize_attributes[i])
				{
					switch (attribute.type)
					{
					default: break;
					case cl_type_unsigned_byte: glVertexAttrib4Nubv(attrib_index, attribute.value_ubyte); break;
					case cl_type_unsigned_short: glVertexAttrib4Nusv(attrib_index, attribute.value_ushort); break;
					case cl_type_unsigned_int: glVertexAttrib4Nuiv(attrib_index, attribute.value_uint); break;
					case cl_type_byte: glVertexAttrib4Nbv(attrib_index, (GLbyte*) attribute.value_byte); break;
					case cl_type_short: glVertexAttrib4Nsv(attrib_index, attribute.value_short); break;
					case cl_type_int: glVertexAttrib4Niv(attrib_index, attribute.value_int); break;
					}
				}
				else
				{
					switch (attribute.type)
					{
					default: break;
					case cl_type_unsigned_byte: glVertexAttrib4ubv(attrib_index, attribute.value_ubyte); break;
					case cl_type_unsigned_short: glVertexAttrib4usv(attrib_index, attribute.value_ushort); break;
					case cl_type_unsigned_int: glVertexAttrib4uiv(attrib_index, attribute.value_uint); break;
					case cl_type_byte: glVertexAttrib4bv(attrib_index, (GLbyte*) attribute.value_byte); break;
					case cl_type_short: glVertexAttrib4sv(attrib_index, attribute.value_short); break;
					case cl_type_int: glVertexAttrib4iv(attrib_index, attribute.value_int); break;
					case cl_type_float: glVertexAttrib4fv(attrib_index, attribute.value_float); break;
					}
				}
				break;
			}
		}
		else if (attribute.array_provider)
		{
			glBindBuffer(GL_ARRAY_BUFFER, static_cast<CL_OpenGLVertexArrayBufferProvider *>(attribute.array_provider)->get_handle());
			glEnableVertexAttribArray(prim_array->attribute_indexes[i]);
			glVertexAttribPointer(
				prim_array->attribute_indexes[i], attribute.size, to_enum(attribute.type),
				prim_array->normalize_attributes[i], attribute.stride, attribute.data);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else if (attribute.data)
		{
			// DEPRECATED FEATURES OF OPENGL 3.0
			// Client vertex arrays - all vertex array attribute pointers must refer to buffer
			// objects (section 2.9.1). The default vertex array object (the name zero) is Bug 3236
			// also deprecated. Calling VertexAttribPointer when no buffer object or no
			// vertex array object is bound will generate an INVALID OPERATION error,
			// as will calling any array drawing command when no vertex array object is
			// bound.

			if (allow_vertex_array_without_buffer_object)
			{
				glEnableVertexAttribArray(prim_array->attribute_indexes[i]);
				glVertexAttribPointer(
					prim_array->attribute_indexes[i], attribute.size, to_enum(attribute.type),
					prim_array->normalize_attributes[i], attribute.stride, attribute.data);
			}
			else
			{
				throw CL_Exception("Since OpenGL 3.0, vertex array attributes must contain a buffer object. Use CL_VertexArrayBuffer");
			}
		}
	}
}

void CL_OpenGLGraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
	CL_OpenGL::set_active(this);
	glDrawArrays(to_enum(type), offset, num_vertices);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count)
{
	CL_OpenGL::set_active(this);
	glDrawArraysInstanced(to_enum(type), offset, num_vertices, instance_count);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
	CL_OpenGL::set_active(this);
	glDrawElements(to_enum(type), count, GL_UNSIGNED_INT, indices);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
	CL_OpenGL::set_active(this);
	glDrawElements(to_enum(type), count, GL_UNSIGNED_SHORT, indices);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
	CL_OpenGL::set_active(this);
	glDrawElements(to_enum(type), count, GL_UNSIGNED_BYTE, indices);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned int *indices, int instance_count)
{
	CL_OpenGL::set_active(this);
	glDrawElementsInstanced(to_enum(type), count, GL_UNSIGNED_INT, indices, instance_count);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned short *indices, int instance_count)
{
	CL_OpenGL::set_active(this);
	glDrawElementsInstanced(to_enum(type), count, GL_UNSIGNED_SHORT, indices, instance_count);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned char *indices, int instance_count)
{
	CL_OpenGL::set_active(this);
	glDrawElementsInstanced(to_enum(type), count, GL_UNSIGNED_BYTE, indices, instance_count);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements(
	CL_PrimitivesType type,
	int count,
	CL_ElementArrayBufferProvider *array_provider,
	CL_VertexAttributeDataType indices_type,
	void *offset)
{
	CL_OpenGL::set_active(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<CL_OpenGLElementArrayBufferProvider *>(array_provider)->get_handle());
	glDrawElements(to_enum(type), count, to_enum(indices_type), offset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CL_OpenGLGraphicContextProvider::draw_primitives_elements_instanced(
	CL_PrimitivesType type,
	int count,
	CL_ElementArrayBufferProvider *array_provider,
	CL_VertexAttributeDataType indices_type,
	void *offset,
	int instance_count)
{
	CL_OpenGL::set_active(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<CL_OpenGLElementArrayBufferProvider *>(array_provider)->get_handle());
	glDrawElementsInstanced(to_enum(type), count, to_enum(indices_type), offset, instance_count);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CL_OpenGLGraphicContextProvider::reset_primitives_array()
{
	CL_OpenGL::set_active(this);

	for (int i=0; i<num_set_program_attribute_arrays; ++i)
	{
		glDisableVertexAttribArray(i);
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
	glEnable(GL_SCISSOR_TEST);

	if (map_mode == cl_map_2d_upper_left)
		glScissor(
			rect.left,
			get_height() - rect.top - rect.get_height(),
			rect.get_width(),
			rect.get_height());
	else
		glScissor(
			rect.left,
			rect.top,
			rect.get_width(),
			rect.get_height());

	last_clip_rect = rect;
}

void CL_OpenGLGraphicContextProvider::reset_clip_rect()
{
	CL_OpenGL::set_active(this);
	glDisable(GL_SCISSOR_TEST);
}

void CL_OpenGLGraphicContextProvider::clear(const CL_Colorf &color)
{
	CL_OpenGL::set_active(this);
	glClearColor(
		(GLclampf) color.r,
		(GLclampf) color.g,
		(GLclampf) color.b,
		(GLclampf) color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CL_OpenGLGraphicContextProvider::clear_stencil(int value)
{
	CL_OpenGL::set_active(this);
	glClearStencil(value);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void CL_OpenGLGraphicContextProvider::clear_depth(float value)
{
	CL_OpenGL::set_active(this);
    if (glClearDepth)
        glClearDepth(value);
    else
        glClearDepthf(value);
	glClear(GL_DEPTH_BUFFER_BIT);
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

		if (glIsEnabled(GL_SCISSOR_TEST))
			glScissor(
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
		if (glIsEnabled(GL_SCISSOR_TEST))
			glScissor(
				last_clip_rect.left,
				last_clip_rect.top,
				last_clip_rect.get_width(),
				last_clip_rect.get_height());
		break;
	case cl_user_projection:
		CL_OpenGL::set_active(this);
		set_viewport(CL_Rectf(0.0f, 0.0f, width, height));
		if (glIsEnabled(GL_SCISSOR_TEST))
			glScissor(
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
	glViewport(
		GLsizei(viewport.left),
		GLsizei(height - viewport.bottom),
		GLsizei(viewport.right - viewport.left),
		GLsizei(viewport.bottom - viewport.top));
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
	if (modelview != matrix)
	{
		modelview = matrix;
		modelview_projection_matrix_valid = false;
		normal_matrix_valid = false;
		set_current_program_object_matricies();
	}
}

void CL_OpenGLGraphicContextProvider::set_blend_mode(const CL_BlendMode &mode)
{
	CL_OpenGL::set_active(this);

	if( mode.is_blend_enabled() )
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	const CL_Colorf &col = mode.get_blend_color();

	if (glBlendColor)
	{
		glBlendColor(
			GLclampf(col.get_red()),
			GLclampf(col.get_green()),
			GLclampf(col.get_blue()),
			GLclampf(col.get_alpha()));
	}

	if (mode.get_blend_equation() == mode.get_blend_equation_alpha())
	{
		if (glBlendEquation)
			glBlendEquation(to_enum(mode.get_blend_equation()));
	}
	else
	{
		if (glBlendEquationSeparate)
			glBlendEquationSeparate(
				to_enum(mode.get_blend_equation()),
				to_enum(mode.get_blend_equation_alpha()) );
	}


	if( mode.get_blend_function_src() == mode.get_blend_function_src_alpha() &&
		mode.get_blend_function_dest() == mode.get_blend_function_dest_alpha() )
	{
		if (glBlendFunc)
			glBlendFunc(to_enum(mode.get_blend_function_src()), to_enum(mode.get_blend_function_dest()));
	}
	else
	{
		if (glBlendFuncSeparate)
			glBlendFuncSeparate( 
				to_enum(mode.get_blend_function_src()),
				to_enum(mode.get_blend_function_dest()),
				to_enum(mode.get_blend_function_src_alpha()),
				to_enum(mode.get_blend_function_dest_alpha()) );
	}
}

void CL_OpenGLGraphicContextProvider::set_pen(const CL_Pen &pen)
{
	CL_OpenGL::set_active(this);

	if (glPointParameterf)
		glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, (GLfloat)pen.get_point_fade_treshold_size());
    if (glPointSize)
        glPointSize((GLfloat)pen.get_point_size());
	glLineWidth((GLfloat)pen.get_line_width());

	if (pen.is_line_antialiased())
		glEnable(GL_LINE_SMOOTH);
	else
		glDisable(GL_LINE_SMOOTH);

	if (pen.is_using_vertex_program_point_sizes())
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	else
		glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);

	if(glPointParameterf)
	{
		switch (pen.get_point_sprite_origin())
		{
		case cl_point_sprite_origin_upper_left:
			glPointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_UPPER_LEFT);
			break;
		case cl_point_sprite_origin_lower_left:
			glPointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
			break;
		}
	}
}

void CL_OpenGLGraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
	CL_OpenGL::set_active(this);

	if (raster.is_antialiased())
		glEnable(GL_POLYGON_SMOOTH);
	else
		glDisable(GL_POLYGON_SMOOTH);

	if (raster.is_culled())
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if (raster.is_line_offset())
		glEnable(GL_POLYGON_OFFSET_LINE);
	else
		glDisable(GL_POLYGON_OFFSET_LINE);

	if (raster.is_point_offset())
		glEnable(GL_POLYGON_OFFSET_POINT);
	else
		glDisable(GL_POLYGON_OFFSET_POINT);

	if (raster.is_polygon_offset())
		glEnable(GL_POLYGON_OFFSET_FILL);
	else
		glDisable(GL_POLYGON_OFFSET_FILL);

    if (glPolygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, to_enum(raster.get_face_fill_mode()));
    }

	switch (raster.get_front_face())
	{
	case cl_face_side_counter_clockwise:
		glFrontFace(GL_CCW);
		break;
	case cl_face_side_clockwise:
		glFrontFace(GL_CW);
		break;
	}

	switch (raster.get_face_cull_mode())
	{
	case cl_cull_front:
		glCullFace(GL_FRONT);
		break;
	case cl_cull_back:
		glCullFace(GL_BACK);
		break;
	case cl_cull_front_and_back:
		glCullFace(GL_FRONT_AND_BACK);
		break;
	}

	glPolygonOffset(raster.get_offset_factor(), raster.get_offset_units());
}

void CL_OpenGLGraphicContextProvider::set_buffer_control(const CL_BufferControl &bc)
{
	CL_OpenGL::set_active(this);

	bool r, g, b, a;
	bc.is_color_write_enabled(r, g, b, a);
	glColorMask(r,g,b,a);

	if( bc.is_depth_test_enabled() )
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glDepthFunc(to_enum(bc.get_depth_compare_function()));
	glDepthMask(bc.is_depth_write_enabled() ? 1 : 0);

	if (bc.is_stencil_test_enabled())
	{
		glEnable(GL_STENCIL_TEST);

		if (glStencilFuncSeparate)
		{
			glStencilFuncSeparate(GL_FRONT,
				to_enum(bc.get_stencil_compare_func_front()),
				bc.get_stencil_compare_reference_front(),
				bc.get_stencil_compare_mask_front());

			glStencilFuncSeparate(GL_BACK,
				to_enum(bc.get_stencil_compare_func_back()),
				bc.get_stencil_compare_reference_back(),
				bc.get_stencil_compare_mask_back());
		}

		if (glStencilOpSeparate)
		{
			glStencilOpSeparate(GL_FRONT,
				to_enum(bc.get_stencil_fail_front()),
				to_enum(bc.get_stencil_pass_depth_fail_front()),
				to_enum(bc.get_stencil_pass_depth_pass_front()));

			glStencilOpSeparate(GL_BACK,
				to_enum(bc.get_stencil_fail_back()),
				to_enum(bc.get_stencil_pass_depth_fail_back()),
				to_enum(bc.get_stencil_pass_depth_pass_back()));
		}

		if (glStencilMaskSeparate)
		{
			glStencilMaskSeparate( GL_FRONT, bc.get_stencil_write_mask_front() );
			glStencilMaskSeparate( GL_BACK, bc.get_stencil_write_mask_back() );
		}
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}

    if (glDrawBuffer)
        glDrawBuffer( to_enum(bc.get_draw_buffer()) );

	if (bc.is_logic_op_enabled())
	{
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(to_enum(bc.get_logic_op()));
	}
	else
	{
		glDisable(GL_COLOR_LOGIC_OP);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLGraphicContextProvider Implementation:

GLenum CL_OpenGLGraphicContextProvider::to_enum(CL_DrawBuffer buffer)
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

GLenum CL_OpenGLGraphicContextProvider::to_enum(CL_CompareFunction func)
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

GLenum CL_OpenGLGraphicContextProvider::to_enum(CL_StencilOp op)
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

GLenum CL_OpenGLGraphicContextProvider::to_enum(CL_CullMode mode)
{
	switch( mode )
	{
	case cl_cull_front: return GL_FRONT;
	case cl_cull_back: return GL_BACK;
	case cl_cull_front_and_back: return GL_FRONT_AND_BACK;
	default: return GL_BACK;
	}
}

GLenum CL_OpenGLGraphicContextProvider::to_enum(CL_FillMode mode)
{
	switch( mode )
	{
	case cl_fill_point: return GL_POINT;
	case cl_fill_line: return GL_LINE;
	case cl_fill_polygon: return GL_FILL;
	default: return GL_FILL;
	}
}

GLenum CL_OpenGLGraphicContextProvider::to_enum(CL_BlendFunc func)
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

GLenum CL_OpenGLGraphicContextProvider::to_enum(CL_BlendEquation eq)
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
}

GLenum CL_OpenGLGraphicContextProvider::to_enum(enum CL_VertexAttributeDataType value)
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

GLenum CL_OpenGLGraphicContextProvider::to_enum(enum CL_PrimitivesType value)
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

GLenum CL_OpenGLGraphicContextProvider::to_enum(enum CL_LogicOp op)
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
