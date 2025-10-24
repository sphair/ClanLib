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
*/

#pragma once


#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Display/TargetProviders/program_object_provider.h"
#include "API/Core/System/disposable_object.h"

class CL_OpenGLGraphicContextProvider;

class CL_OpenGLProgramObjectProvider : public CL_ProgramObjectProvider, CL_DisposableObject
{
/// \name Construction
/// \{

public:
	CL_OpenGLProgramObjectProvider();
	virtual ~CL_OpenGLProgramObjectProvider();

	virtual void destroy();


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
	virtual CL_String get_info_log() const;

	/// \brief Returns the count of active attributes.
	virtual int get_attribute_count() const;

	/// \brief Returns the shaders used in this program.
	virtual std::vector<CL_ShaderObject> get_shaders() const;

	/// \brief Returns the active attributes.
	virtual std::vector<CL_ProgramAttribute> get_attributes() const;

	/// \brief Returns the location of a named active attribute.
	virtual int get_attribute_location(const CL_StringRef &name) const;

	/// \brief Returns the count of active uniform variables.
	virtual int get_uniform_count() const;

	/// \brief Returns the active uniforms.
	virtual std::vector<CL_ProgramUniform> get_uniforms() const;

	/// \brief Returns the location of a named uniform variable.
	virtual int get_uniform_location(const CL_StringRef &name) const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Add shader to program object.
	virtual void attach(const CL_ShaderObject &obj);

	/// \brief Remove shader from program object.
	virtual void detach(const CL_ShaderObject &obj);

	/// \brief Bind attribute to specific location.
	/** <p>This function must be called before linking.</p>*/
	virtual void bind_attribute_location(int index, const CL_StringRef &name);

	/// \brief Bind shader out variable a specific color buffer location.
	/** <p>This function must be called before linking.</p>*/
	virtual void bind_frag_data_location(int color_number, const CL_StringRef &name);

	/// \brief Link program.
	/** <p>If the linking fails, get_link_status() will return false and
	    get_info_log() will return the link log.</p>*/
	virtual void link();

	/// \brief Validate program.
	/** <p>If the validation fails, get_validate_status() will return
	    false and get_info_log() will return the validation log.</p>*/
	virtual void validate();

	/// \brief Set uniform variable(s).
	virtual void set_uniform1i(const CL_StringRef &name, int);
	virtual void set_uniform2i(const CL_StringRef &name, int, int);
	virtual void set_uniform3i(const CL_StringRef &name, int, int, int);
	virtual void set_uniform4i(const CL_StringRef &name, int, int, int, int);
	virtual void set_uniformiv(const CL_StringRef &name, int size, int count, int *data);
	virtual void set_uniform1f(const CL_StringRef &name, float);
	virtual void set_uniform2f(const CL_StringRef &name, float, float);
	virtual void set_uniform3f(const CL_StringRef &name, float, float, float);
	virtual void set_uniform4f(const CL_StringRef &name, float, float, float, float);
	virtual void set_uniformfv(const CL_StringRef &name, int size, int count, float *data);

	/// \brief Set uniform matrices.
	virtual void set_uniform_matrix(const CL_StringRef &name, int size, int count, bool transpose, float *data);

/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	void fetch_attributes() const;

	void fetch_uniforms() const;

	GLuint handle;

	std::vector<CL_ShaderObject> shaders;

	mutable std::vector<CL_ProgramAttribute> cached_attribs;

	mutable std::vector<CL_ProgramUniform> cached_uniforms;

/// \}
};

class CL_ProgramObjectStateTracker
{
public:
	CL_ProgramObjectStateTracker(GLuint handle);
	~CL_ProgramObjectStateTracker();

private:
	GLuint last_program_object;
	bool program_set;
};


