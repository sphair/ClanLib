/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include <memory>
#include "../api_display.h"
#include "../Render/graphic_context.h"
#include "../Render/rasterizer_state_description.h"
#include "../Render/blend_state_description.h"
#include "../Render/depth_stencil_state_description.h"
#include "../Render/storage_buffer.h"
#include "../Render/uniform_buffer.h"
#include "../Render/texture.h"
#include "../Render/render_buffer.h"

namespace clan
{

/// \brief Texture coordinate wrapping modes.
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum UVQuadType
{
	uv_zero_one
};

class ShaderEffectDescription_Impl;

/// \brief Shader Effect Description
///
///    <p>...</p> 
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY ShaderEffectDescription
{
/// \name Construction
/// \{
public:
	ShaderEffectDescription();
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	void set_glsl_version(int version = 330);

	void set_define(std::string name, std::string value = "1");

	void set_vertex_shader(std::string code, ShaderLanguage language = shader_glsl);
	void set_fragment_shader(std::string code, ShaderLanguage language = shader_glsl);
	void set_compute_shader(std::string code, ShaderLanguage language = shader_glsl);

	RasterizerStateDescription &rasterizer();
	BlendStateDescription &blend();
	DepthStencilStateDescription &depth_stencil();

	void set_frag_data(std::string name, RenderBuffer buffer);
	void set_frag_data(std::string name, Texture texture);
	void set_frag_data_back_buffer(std::string name);

	void set_depth_data(RenderBuffer buffer);
	void set_depth_data(Texture texture);

	void set_stencil_data(RenderBuffer buffer);
	void set_stencil_data(Texture texture);

	void set_texture(std::string name, Texture texture);
	void set_image(std::string name, Texture texture);

	void set_storage(std::string name, StorageBuffer values);
	void set_empty_storage(std::string name, int size);

	void set_uniform_block(std::string name, UniformBuffer values);

	void set_attribute(std::string name, VertexArrayBuffer &buffer, int size, VertexAttributeDataType type, size_t offset = 0, int stride = 0, bool normalize = false);

	template<typename Type>
	void set_attribute(std::string name, VertexArrayVector<Type> values);
	void set_attribute_screen_quad(std::string name);
	void set_attribute_uv_quad(std::string name, UVQuadType type = uv_zero_one);

	template<typename Type>
	void set_elements(ElementArrayVector<Type> elements);
/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<ShaderEffectDescription_Impl> impl;
/// \}
};

template<typename Type>
inline void ShaderEffectDescription::set_attribute(std::string name, VertexArrayVector<Type> values)
{
}

template<typename Type>
inline void ShaderEffectDescription::set_elements(ElementArrayVector<Type> elements)
{
}

}

/// \}
