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
**    Harry Storbacka
*/

#pragma once


#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Display/TargetProviders/program_object_provider.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{

class GL3GraphicContextProvider;

class GL3ProgramObjectProvider : public ProgramObjectProvider, DisposableObject
{
/// \name Construction
/// \{
public:
	GL3ProgramObjectProvider();
	virtual ~GL3ProgramObjectProvider();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the OpenGL program object handle.
	virtual unsigned int get_handle() const;

	/// \brief Returns true if the link succeeded.
	virtual bool get_link_status() const;

	/// \brief Returns true if validation succeeded.
	virtual bool get_validate_status() const;

	/// \brief Returns the current info log for the program object.
	virtual std::string get_info_log() const;

	/// \brief Returns the count of active attributes.
	virtual int get_attribute_count() const;

	/// \brief Returns the shaders used in this program.
	virtual std::vector<ShaderObject> get_shaders() const;

	/// \brief Returns the active attributes.
	virtual std::vector<ProgramAttribute> get_attributes() const;

	/// \brief Returns the location of a named active attribute.
	virtual int get_attribute_location(const std::string &name) const;

	/// \brief Returns the count of active uniform variables.
	virtual int get_uniform_count() const;

	/// \brief Returns the active uniforms.
	virtual std::vector<ProgramUniform> get_uniforms() const;

	/// \brief Returns the location of a named uniform variable.
	virtual int get_uniform_location(const std::string &name) const;

	int get_uniform_buffer_size(int block_index) const;
	int get_uniform_buffer_index(const std::string &block_name) const;
	int get_storage_buffer_index(const std::string &name) const;
/// \}

/// \name Operations
/// \{
public:
	/// \brief Add shader to program object.
	virtual void attach(const ShaderObject &obj);

	/// \brief Remove shader from program object.
	virtual void detach(const ShaderObject &obj);

	/// \brief Bind attribute to specific location.
	/** <p>This function must be called before linking.</p>*/
	virtual void bind_attribute_location(int index, const std::string &name);

	/// \brief Bind shader out variable a specific color buffer location.
	/** <p>This function must be called before linking.</p>*/
	virtual void bind_frag_data_location(int color_number, const std::string &name);

	/// \brief Link program.
	/** <p>If the linking fails, get_link_status() will return false and
	    get_info_log() will return the link log.</p>*/
	virtual void link();

	/// \brief Validate program.
	/** <p>If the validation fails, get_validate_status() will return
	    false and get_info_log() will return the validation log.</p>*/
	virtual void validate();

	/// \brief Set uniform variable(s).
	void set_uniform1i(int location, int);

	void set_uniform_buffer_index(int block_index, int bind_index);
	void set_storage_buffer_index(int buffer_index, int bind_unit_index);
/// \}

/// \name Implementation
/// \{
private:
	void on_dispose();
	void fetch_attributes() const;

	void fetch_uniforms() const;

	GLuint handle;
	std::vector<ShaderObject> shaders;
	mutable std::vector<ProgramAttribute> cached_attribs;
	mutable std::vector<ProgramUniform> cached_uniforms;
/// \}
};

class ProgramObjectStateTracker
{
public:
	ProgramObjectStateTracker(GLuint handle);
	~ProgramObjectStateTracker();

private:
	GLuint last_program_object;
	bool program_set;
};

}
