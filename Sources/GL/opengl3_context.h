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
*/

#pragma once

class CL_OpenGL3Context
{
public:
	CL_OpenGL3Context(HDC dc, HGLRC context);

	typedef int Enum;
	typedef unsigned int Sizei;
	typedef void *Intptr;
	typedef unsigned __int64 Sizeiptr;
	typedef unsigned int Bitfield;
	typedef double Clampd;
	typedef float Clampf;
	typedef void *Sync;
	typedef unsigned __int64 uint64;
	typedef __int64 int64;

	// 2.5 GL Errors

	typedef Enum ErrorCode;
	ErrorCode GetError();

	// 2.7 Vertex Specification

	void VertexAttrib1s(unsigned int index, short v);
	void VertexAttrib1f(unsigned int index, float v);
	void VertexAttrib1d(unsigned int index, double v);
	void VertexAttrib2s(unsigned int index, short v0, short v1);
	void VertexAttrib2f(unsigned int index, float v0, float v1);
	void VertexAttrib2d(unsigned int index, double v0, double v1);
	void VertexAttrib3s(unsigned int index, short v0, short v1, short v2);
	void VertexAttrib3f(unsigned int index, float v0, float v1, float v2);
	void VertexAttrib3d(unsigned int index, double v0, double v1, double v2);
	void VertexAttrib4s(unsigned int index, short v0, short v1, short v2, short v3);
	void VertexAttrib4f(unsigned int index, float v0, float v1, float v2, short v3);
	void VertexAttrib4d(unsigned int index, double v0, double v1, double v2, double v3);
	void VertexAttrib1sv(unsigned int index, short *v);
	void VertexAttrib1fv(unsigned int index, float *v);
	void VertexAttrib1dv(unsigned int index, double *v);
	void VertexAttrib2sv(unsigned int index, short *v);
	void VertexAttrib2fv(unsigned int index, float *v);
	void VertexAttrib2dv(unsigned int index, double *v);
	void VertexAttrib3sv(unsigned int index, short *v);
	void VertexAttrib3fv(unsigned int index, float *v);
	void VertexAttrib3dv(unsigned int index, double *v);
	void VertexAttrib4bv(unsigned int index, char *v);
	void VertexAttrib4sv(unsigned int index, short *v);
	void VertexAttrib4iv(unsigned int index, int *v);
	void VertexAttrib4fv(unsigned int index, float *v);
	void VertexAttrib4dv(unsigned int index, double *v);
	void VertexAttrib4ubv(unsigned int index, unsigned char *v);
	void VertexAttrib4usv(unsigned int index, unsigned short *v);
	void VertexAttrib4uiv(unsigned int index, unsigned int *v);
	void VertexAttrib4Nub(unsigned int index, unsigned char v);
	void VertexAttrib4Nbv(unsigned int index, char *v);
	void VertexAttrib4Nsv(unsigned int index, short *v);
	void VertexAttrib4Niv(unsigned int index, int *v);
	void VertexAttrib4Nubv(unsigned int index, unsigned char *v);
	void VertexAttrib4Nusv(unsigned int index, unsigned short *v);
	void VertexAttrib4Nuiv(unsigned int index, unsigned int *v);

	void VertexAttribI1i(unsigned int index, int v);
	void VertexAttribI1ui(unsigned int index, unsigned int v);
	void VertexAttribI2i(unsigned int index, int v0, int v1);
	void VertexAttribI2ui(unsigned int index, unsigned int v0, unsigned int v1);
	void VertexAttribI3i(unsigned int index, int v0, int v1, int v2);
	void VertexAttribI3ui(unsigned int index, unsigned int v0, unsigned int v1, unsigned int v2);
	void VertexAttribI4i(unsigned int index, int v0, int v1, int v2, int v3);
	void VertexAttribI4ui(unsigned int index, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);

	void VertexAttribI1sv(unsigned int index, short *v);
	void VertexAttribI2sv(unsigned int index, short *v);
	void VertexAttribI3sv(unsigned int index, short *v);
	void VertexAttribI4bv(unsigned int index, char *v);
	void VertexAttribI4sv(unsigned int index, short *v);
	void VertexAttribI4iv(unsigned int index, int *v);
	void VertexAttribI4ubv(unsigned int index, unsigned char *v);
	void VertexAttribI4usv(unsigned int index, unsigned short *v);
	void VertexAttribI4uiv(unsigned int index, unsigned int *v);

	// Enum type = BYTE, SHORT, INT, FLOAT, HALF_FLOAT, DOUBLE, UNSIGNED_BYTE, UNSIGNED_SHORT, UNSIGNED_INT
	void VertexAttribPointer(unsigned int index, int size, Enum type, bool normalized, Sizei stride, const void *pointer);
	void VertexAttribIPointer(unsigned int index, int size, Enum type, Sizei stride, const void *pointer);

	// 2.8 Vertex Arrays

	void EnableVertexAttribArray(unsigned int index);
	void DisableVertexAttribArray(unsigned int index);

	// 2.8.1 Transferring Array Elements

	// Enum target = PRIMITIVE_RESTART
	void Enable(Enum target);
	void Disable(Enum target);
	void PrimitiveRestartIndex(unsigned int index);

	// 2.8.2 Drawing Commands

	// Enum mode = POINTS, LINE_STRIP, LINE_LOOP, LINES, TRIANGLE_STRIP, TRIANGLE_FAN, TRIANGLES, LINES_ADJACENCY, LINE_STRIP_ADJACENCY, TRIANGLES_ADJACENCY, TRIANGLE_STRIP_ADJACENCY
	// Enum type = UNSIGNED_BYTE, UNSIGNED_SHORT, UNSIGNED_INT
	void DrawArrays(Enum mode, int first, Sizei count);
	void MultiDrawArrays(Enum mode, int *first, Sizei *count, Sizei primcount);
	void DrawElements(Enum mode, Sizei count, Enum type, void *indices);
	void MultiDrawElements(Enum mode, Sizei *count, Enum type, void **indices, Sizei primcount);
	void DrawRangeElements(Enum mode, unsigned int start, unsigned int end, Sizei count, Enum type, void *indices);
	void DrawArraysInstanced(Enum mode, int first, Sizei count, Sizei primcount);
	void DrawElementsInstanced(Enum mode, Sizei count, Enum type, const void *indices, Sizei primcount);
	void DrawElementsBaseVertex(Enum mode, Sizei count, Enum type, void *indices, int basevertex);
	void DrawRangeElementsBaseVertex(Enum mode, unsigned int start, unsigned int end, Sizei count, Enum type, void *indices, int basevertex);
	void DrawElementsInstancedBaseVertex(Enum mode, Sizei count, Enum type, const void *indices, Sizei primcount, int basevertex);
	void MultiDrawElementsBaseVertex(Enum mode, Sizei *count, Enum type, void **indices, Sizei primcount, int *basevertex);

	// 2.9 Buffer Objects

	// Enum target = ARRAY_BUFFER, COPY_READ_BUFFER, COPY_WRITE_BUFFER, ELEMENT_ARRAY_BUFFER, PIXEL_PACK_BUFFER, PIXEL_UNPACK_BUFFER, TEXTURE_BUFFER, TRANSFORM_FEEDBACK_BUFFER, UNIFORM_BUFFER
	void GenBuffers(Sizei n, unsigned int *buffers);
	void DeleteBuffers(Sizei n, const unsigned int *buffers);
	void BindBuffer(Enum target, unsigned int buffer);
	void BindBufferRange(Enum target, unsigned int index, unsigned int buffer, Intptr offset, Sizeiptr size);
	void BindBufferBase(Enum target, unsigned int index, unsigned int buffer);

	// 2.9.2 Creating Buffer Object Data Stores

	// Enum usage = STREAM_DRAW, STREAM_READ, STREAM_COPY, STATIC_DRAW, STATIC_READ, STATIC_COPY, DYNAMIC_DRAW, DYNAMIC_READ, DYNAMIC_COPY
	void BufferData(Enum target, Sizeiptr size, const void *data, Enum usage);
	void BufferSubData(Enum target, Intptr offset, Sizeiptr size, const void *data);

	// 2.9.3 Mapping and Unmapping Buffer Data

	// Bitfield access = MAP_READ_BIT, MAP_WRITE_BIT, MAP_INVALIDATE_RANGE_BIT, MAP_INVALIDATE_BUFFER_BIT, MAP_FLUSH_EXPLICIT_BIT, MAP_UNSYNCHRONIZED_BIT
	// Enum access = READ_ONLY, WRITE_ONLY, READ_WRITE
	void *MapBufferRange(Enum target, Intptr offset, Sizeiptr length, Bitfield access);
	void *MapBuffer(Enum target, Enum access);
	bool UnmapBuffer(Enum target);

	// 2.9.5 Copying Between Buffers

	// Enum readtarget = target
	// Enum writetarget = target
	void *CopyBufferSubData(Enum readtarget, Enum writetarget, Intptr readoffset, Intptr writeoffset, Sizeiptr size);

	// 2.10 Vertex Array Objects

	void GenVertexArrays(Sizei n, unsigned int *arrays);
	void DeleteVertexArrays(Sizei n, const unsigned int *arrays);
	void BindVertexArray(unsigned int array);

	// 2.11.1 Shader Objects

	// Enum type = VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER
	unsigned int CreateShader(Enum type);
	void ShaderSource(unsigned int shader, Sizei count, const char **string, const int *length);
	void CompileShader(unsigned int shader);
	void DeleteShader(unsigned int shader);

	// 2.11.2 Program Objects

	unsigned int CreateProgram();
	void AttachShader(unsigned int program, unsigned int shader);
	void DetachShader(unsigned int program, unsigned int shader);
	void LinkProgram(unsigned int program);
	void UseProgram(unsigned int program);
	void DeleteProgram(unsigned int program);

	// 2.11.3 Vertex Attributes

	// Enum type = FLOAT, FLOAT_VEC2, FLOAT_VEC3, FLOAT_VEC4, FLOAT_MAT2, FLOAT_MAT3, FLOAT_MAT4, FLOAT_MAT2x3, FLOAT_MAT2x4, FLOAT_MAT3x2, FLOAT_MAT3x4, FLOAT_MAT4x2, FLOAT_MAT4x3, INT, INT_VEC2, INT_VEC3, INT_VEC4, UNSIGNED_INT, UNSIGNED_INT_VEC2, UNSIGNED_INT_VEC3, UNSIGNED_INT_VEC4
	void GetActiveAttrib(unsigned int program, unsigned int index, Sizei bufSize, Sizei *length, int *size, Enum *type, char *name);
	int GetAttribLocation(unsigned int program, const char *name);
	void BindAttribLocation(unsigned int program, unsigned int index, const char *name);

	// 2.11.4 Uniform Variables

	// Enum pname = UNIFORM_BLOCK_BINDING, UNIFORM_BLOCK_DATA_SIZE, UNIFORM_BLOCK_NAME_LENGTH, UNIFORM_BLOCK_ACTIVE_UNIFORMS, UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER, UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER
	// Enum typetoken = Table 2.10 (page 61-62)
	// Enum paramname = UNIFORM_TYPE, UNIFORM_SIZE, UNIFORM_NAME_LENGTH, UNIFORM_BLOCK_INDEX, UNIFORM_OFFSET, UNIFORM_ARRAY_STRIDE, UNIFORM_MATRIX_STRIDE, UNIFORM_IS_ROW_MAJOR
	int GetUniformLocation(unsigned int program, const char *name);
	unsigned int GetUniformBlockIndex(unsigned int program, const char *uniformBlockName);
	void GetActiveUniformBlockName(unsigned int program, unsigned int uniformBlockIndex, Sizei bufSize, Sizei *length, char *uniformBlockName);
	void GetActiveUniformBlockiv(unsigned int program, unsigned int uniformBlockIndex, Enum pname, int *params);
	void GetUniformIndices(unsigned int program, Sizei uniformCount, const char **uniformNames, unsigned int *uniformIndices);
	void GetActiveUniformName(unsigned int program, unsigned int uniformIndex, Sizei bufSize, Sizei *length, char *uniformName);
	void GetActiveUniform(unsigned int program, unsigned int index, Sizei bufSize, Sizei *length, int *size, Enum *typetoken, char *name);
	void GetActiveUniformsiv(unsigned int program, Sizei uniformCount, const unsigned int *uniformIndices, Enum paramname, int *params);

	void Uniform1i(int location, int v);
	void Uniform1f(int location, float v);
	void Uniform2i(int location, int v0, int v1);
	void Uniform2f(int location, float v0, float v1);
	void Uniform3i(int location, int v0, int v1, int v2);
	void Uniform3f(int location, float v0, float v1, float v2);
	void Uniform4i(int location, int v0, int v1, int v2, int v3);
	void Uniform4f(int location, float v0, float v1, float v2, float v3);

	void Uniform1iv(int location, Sizei count, int *v);
	void Uniform1fv(int location, Sizei count, float *v);
	void Uniform2iv(int location, Sizei count, int *v);
	void Uniform2fv(int location, Sizei count, float *v);
	void Uniform3iv(int location, Sizei count, int *v);
	void Uniform3fv(int location, Sizei count, float *v);
	void Uniform4iv(int location, Sizei count, int *v);
	void Uniform4fv(int location, Sizei count, float *v);

	void Uniform1ui(int location, unsigned int v);
	void Uniform2ui(int location, unsigned int v0, unsigned int v1);
	void Uniform3ui(int location, unsigned int v0, unsigned int v1, unsigned int v2);
	void Uniform4ui(int location, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);

	void Uniform1uiv(int location, Sizei count, unsigned int *v);
	void Uniform2uiv(int location, Sizei count, unsigned int *v);
	void Uniform3uiv(int location, Sizei count, unsigned int *v);
	void Uniform4uiv(int location, Sizei count, unsigned int *v);

	void UniformMatrix2fv(int location, Sizei count, bool transpose, const float *value);
	void UniformMatrix3fv(int location, Sizei count, bool transpose, const float *value);
	void UniformMatrix4fv(int location, Sizei count, bool transpose, const float *value);

	void UniformMatrix2x3fv(int location, Sizei count, bool transpose, const float *value);
	void UniformMatrix3x2fv(int location, Sizei count, bool transpose, const float *value);
	void UniformMatrix2x4fv(int location, Sizei count, bool transpose, const float *value);
	void UniformMatrix4x2fv(int location, Sizei count, bool transpose, const float *value);
	void UniformMatrix3x4fv(int location, Sizei count, bool transpose, const float *value);
	void UniformMatrix4x3fv(int location, Sizei count, bool transpose, const float *value);

	void UniformBlockBinding(unsigned int program, unsigned int uniformBlockIndex, unsigned int uniformBlockBinding);

	// 2.11.6 Varying Variables

	// Enum bufferMode = INTERLEAVED_ATTRIBS, SEPARATE_ATTRIBS
	// Enum type = FLOAT, FLOAT_VEC2, FLOAT_VEC3, FLOAT_VEC4, FLOAT_MAT2, FLOAT_MAT3, FLOAT_MAT4, FLOAT_MAT2x3, FLOAT_MAT2x4, FLOAT_MAT3x2, FLOAT_MAT3x4, FLOAT_MAT4x2, FLOAT_MAT4x3, INT, INT_VEC2, INT_VEC3, INT_VEC4, UNSIGNED_INT, UNSIGNED_INT_VEC2, UNSIGNED_INT_VEC3, UNSIGNED_INT_VEC4
	void TransformFeedbackVaryings(unsigned int program, Sizei count, const char **varyings, Enum bufferMode);
	void GetTransformFeedbackVarying(unsigned int program, unsigned int index, Sizei bufSize, Sizei *length, Sizei *size, Enum *type, char *name);

	// 2.11.7 Shader Execution

	void ValidateProgram(unsigned int program);

	// 2.13.1 Controlling the Viewport

	void DepthRange(Clampd n, Clampd f);
	void Viewport(int x, int y, Sizei w, Sizei h);

	// 2.14 Asynchronous Queries

	// Enum target = PRIMITIVES_GENERATED, TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, SAMPLES_PASSED, INTERLEAVED_ATTRIBS, SEPARATE_ATTRIBS???
	void BeginQuery(Enum target, unsigned int id);
	void EndQuery(Enum target);
	void GenQueries(Sizei n, unsigned int *ids);
	void DeleteQueries(Sizei n, const unsigned int *ids);

	// 2.15 Conditional Rendering

	// Enum mode = QUERY_WAIT, QUERY_NO_WAIT, QUERY_BY_REGION_WAIT
	void BeginConditionalRender(unsigned int id, Enum mode);
	void EndConditionalRender();

	// 2.16 Transform Feedback

	// Enum mode = TRIANGLES, LINES, POINTS
	void BeginTransformFeedback(Enum primitiveMode);
	void EndTransformFeedback();

	// 2.18 Flatshading

	// Enum provokeMode = FIRST_VERTEX_CONVENTION, LAST_VERTEX_CONVENTION
	void ProvokingVertex(Enum provokeMode);

	// 3.3.1 Multisampling

	// Enum pname = SAMPLE_POSITION
	void GetMultisamplefv(Enum pname, unsigned int index, float *val);

	// 3.4 Points

	// Enable/Disable: PROGRAM_POINT_SIZE
	// Enum pname = POINT_FADE_THRESHOLD_SIZE, POINT_SPRITE_COORD_ORIGIN
	// param for POINT_SPRITE_COORD_ORIGIN: LOWER_LEFT, UPPER_LEFT
	void PointSize(float size);
	void PointParameteri(Enum pname, int param);
	void PointParameterf(Enum pname, float param);
	void PointParameteriv(Enum pname, int *param);
	void PointParameterfv(Enum pname, float *param);

	// 3.5 Line Segments

	void LineWidth(float width);

	// 3.6.1 Basic Polygon Rasterization

	// Enable/Disable: CULL_FACE
	// Enum dir = CCW, CW
	// Enum mode = FRONT, BACK, FRONT_AND_BACK
	void FrontFace(Enum dir);
	void CullFace(Enum mode);

	// 3.6.3 Options Controlling Polygon Rasterization

	// Enum face = FRONT_AND_BACK
	// Enum mode = POINT, LINE, FILL
	void PolygonMode(Enum face, Enum mode);

	// 3.6.4 Depth Offset

	void PolygonOffset(float factor, float units);

	// 3.7.1 Pixel Storage Modes and Pixel Buffer Objects

	// Enum pname = UNPACK_SWAP_BYTES, UNPACK_LSB_FIRST, UNPACK_ROW_LENGTH, UNPACK_SKIP_ROWS, UNPACK_SKIP_PIXELS, UNPACK_ALIGNMENT, UNPACK_IMAGE_HEIGHT, UNPACK_SKIP_IMAGES
	void PixelStorei(Enum pname, int param);
	void PixelStoref(Enum pname, float param);

	// 3.8 Texturing

	// Enum texture = TEXTURE0+i (i is between 0 and MAX_COMBINED_TEXTURE_IMAGE_UNITS)
	void ActiveTexture(Enum texture);

	// 3.8.1 Texture Image Specification

	// Enum target3D = TEXTURE_3D, TEXTURE_2D_ARRAY, PROXY_TEXTURE_3D, PROXY_TEXTURE_2D_ARRAY
	// Enum format = section 3.7.2 (STENCIL_INDEX is not allowed), table 3.3
	// Enum type = section 3.7.2
	// int internalformat = DEPTH_COMPONENT, DEPTH_STENCIL, RED, RG, RGB, RGBA, table 3.12, table 3.14, page 135
	// Enum target2D = TEXTURE_2D, TEXTURE_1D_ARRAY, TEXTURE_RECTANGLE, TEXTURE_CUBE_MAP_POSITIVE_X ..., PROXY_TEXTURE_2D, PROXY_TEXTURE_1D_ARRAY, PROXY_TEXTURE_RECTANGLE, PROXY_TEXTURE_CUBE_MAP
	// Enum target1D = TEXTURE_1D, PROXY_TEXTURE_1D
	void TexImage3D(Enum target3D, int level, int internalformat, Sizei width, Sizei height, Sizei depth, int border, Enum format, Enum type, void *data);
	void TexImage2D(Enum target2D, int level, int internalformat, Sizei width, Sizei height, int border, Enum format, Enum type, void *data);
	void TexImage1D(Enum target1D, int level, int internalformat, Sizei width, int border, Enum format, Enum type, void *data);

	// 3.8.2 Alternate Texture Image Specification Commands

	// Enum target3D = TEXTURE_3D, TEXTURE_2D_ARRAY
	// Enum target2D = TEXTURE_2D, TEXTURE_1D_ARRAY, TEXTURE_RECTANGLE, TEXTURE_CUBE_MAP_POSITIVE_X ...
	// Enum target1D = TEXTURE_1D
	void CopyTexImage2D(Enum target2D, int level, Enum internalformat, int x, int y, Sizei width, Sizei height, int border);
	void CopyTexImage1D(Enum target1D, int level, Enum internalformat, int x, int y, Sizei width, int border);
	void TexSubImage3D(Enum target3D, int level, int xoffset, int yoffset, int zoffset, Sizei width, Sizei height, Sizei depth, Enum format, Enum type, void *data);
	void TexSubImage2D(Enum target2D, int level, int xoffset, int yoffset, Sizei width, Sizei height, Enum format, Enum type, void *data);
	void TexSubImage1D(Enum target1D, int level, int xoffset, Sizei width, Enum format, Enum type, void *data);
	void CopyTexSubImage3D(Enum target3D, int level, int xoffset, int yoffset, int zoffset, int x, int y, Sizei width, Sizei height);
	void CopyTexSubImage2D(Enum target2D, int level, int xoffset, int yoffset, int x, int y, Sizei width, Sizei height);
	void CopyTexSubImage1D(Enum target1D, int level, int xoffset, int x, int y, Sizei width);

	// 3.8.3 Compressed Texture Images

	// Enum target = same meaning as in 3.8.1
	void CompressedTexImage1D(Enum target, int level, Enum internalformat, Sizei width, int border, Sizei imageSize, void *data);
	void CompressedTexImage2D(Enum target, int level, Enum internalformat, Sizei width, Sizei height, int border, Sizei imageSize, void *data);
	void CompressedTexImage3D(Enum target, int level, Enum internalformat, Sizei width, Sizei height, Sizei depth, int border, Sizei imageSize, void *data);
	void CompressedTexSubImage1D(Enum target, int level, int xoffset, Sizei width, Enum format, Sizei imageSize, void *data);
	void CompressedTexSubImage2D(Enum target, int level, int xoffset, int yoffset, Sizei width, Sizei height, Enum format, Sizei imageSize, void *data); 
	void CompressedTexSubImage3D(Enum target, int level, int xoffset, int yoffset, int zoffset, Sizei width, Sizei height, Sizei depth, Enum format, Sizei imageSize, void *data);

	// 3.8.4 Multisample Textures

	// Enum target2D = TEXTURE_2D_MULTISAMPLE, PROXY_TEXTURE_2D_MULTISAMPLE
	// Enum target3D = TEXTURE_2D_MULTISAMPLE_ARRAY, PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY
	void TexImage2DMultisample(Enum target2D, Sizei samples, int internalformat, Sizei width, Sizei height, bool fixedsamplelocations);
	void TexImage3DMultisample(Enum target3D, Sizei samples, int internalformat, Sizei width, Sizei height, Sizei depth, bool fixedsamplelocations);

	// 3.8.5 Buffer Textures

	// Enum target = all targets except proxy targets
	// Enum internalformat = table 3.15
	void TexBuffer(Enum target, Enum internalformat, unsigned int buffer);

	// 3.8.6 Texture Parameters

	// Enum target = TEXTURE_1D, TEXTURE_2D, TEXTURE_3D, TEXTURE_1D_ARRAY, TEXTURE_2D_ARRAY, TEXTURE_RECTANGLE, TEXTURE_CUBE_MAP
	// Enum pname = TEXTURE_WRAP_S, TEXTURE_WRAP_T, TEXTURE_WRAP_R, TEXTURE_MIN_FILTER, TEXTURE_MAG_FILTER, TEXTURE_BORDER_COLOR, TEXTURE_MIN_LOD, TEXTURE_MAX_LOD, TEXTURE_BASE_LEVEL, TEXTURE_MAX_LEVEL, TEXTURE_LOD_BIAS, TEXTURE_COMPARE_MODE, TEXTURE_COMPARE_FUNC
	// params values: see table 3.16 page 159
	void TexParameteri(Enum target, Enum pname, int param);
	void TexParameterf(Enum target, Enum pname, float param);
	void TexParameteriv(Enum target, Enum pname, int *params);
	void TexParameterfv(Enum target, Enum pname, float *params);
	void TexParameterIiv(Enum target, Enum pname, int *params);
	void TexParameterIuiv(Enum target, Enum pname, unsigned int *params);

	// 3.8.9 Texture Minification

	// Enum target = TEXTURE_1D, TEXTURE_2D, TEXTURE_3D, TEXTURE_1D_ARRAY, TEXTURE_2D_ARRAY, TEXTURE_CUBE_MAP
	void GenerateMipmap(Enum target);

	// 3.8.14 Texture Objects

	// Enum target = all targets except proxy targets
	void BindTexture(Enum target, unsigned int texture);
	void DeleteTextures(Sizei n, unsigned int *textures);
	void GenTextures(Sizei n, unsigned int *textures);

	// 3.9.2 Shader Execution

	void BindFragDataLocation(unsigned int program, unsigned int colorNumber, const char *name);
	int GetFragDataLocation(unsigned int program, const char *name);

	// 4.1.2 Scissor Test

	// Enable/Disable: SCISSOR_TEST
	void Scissor(int left, int bottom, Sizei width, Sizei height);

	// 4.1.3 Multisample Fragment Operations

	// Enable/Disable: SAMPLE_ALPHA_TO_COVERAGE, SAMPLE_ALPHA_TO_ONE, SAMPLE_COVERAGE
	void SampleCoverage(Clampf value, bool invert);
	void SampleMaski(unsigned int maskNumber, Bitfield mask);

	// 4.1.4 Stencil Test

	// Enable/Disable: STENCIL_TEST
	// Enum face = FRONT, BACK, FRONT_AND_BACK
	// Enum func = NEVER, ALWAYS, LESS, LEQUAL, EQUAL, GEQUAL, GREATER, NOTEQUAL
	// Enum sfail, dpfail, dppass = KEEP, ZERO, REPLACE, INCR, DECR, INVERT, INCR_WRAP, DECR_WRAP
	void StencilFunc(Enum func, int ref, unsigned int mask);
	void StencilFuncSeparate(Enum face, Enum func, int ref, unsigned int mask);
	void StencilOp(Enum sfail, Enum dpfail, Enum dppass);
	void StencilOpSeparate(Enum face, Enum sfail, Enum dpfail, Enum dppass);

	// 4.1.5 Depth Buffer Test

	// Enable/Disable: DEPTH_TEST
	// Enum func = NEVER, ALWAYS, LESS, LEQUAL, EQUAL, GEQUAL, GREATER, NOTEQUAL
	void DepthFunc(Enum func);

	// 4.1.7 Blending

	// Enable/Disable: BLEND (Enablei enables it for a single draw buffer, while Enable does it for all)
	// Enum target = BLEND
	// Enum mode/modeRGB/modeAlpha: FUNC_ADD, FUNC_SUBTRACT, FUNC_REVERSE_SUBTRACT, MIN, MAX
	// Enum src/dst/srcRGB/dstRGB/srcAlpha/dstAlpha: ZERO, ONE, SRC_COLOR, ONE_MINUS_SRC_COLOR, DST_COLOR, ONE_MINUS_DST_COLOR, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR, ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA, SRC_ALPHA_SATURATE
	void Enablei(Enum target, unsigned int index);
	void Disablei(Enum target, unsigned int index);
	void BlendEquation(Enum mode);
	void BlendEquationSeparate(Enum modeRGB, Enum modeAlpha);
	void BlendFuncSeparate(Enum srcRGB, Enum dstRGB, Enum srcAlpha, Enum dstAlpha);
	void BlendFunc(Enum src, Enum dst);
	void BlendColor(Clampf red, Clampf green, Clampf blue, Clampf alpha);

	// 4.1.10 Logical Operation

	// Enable/Disable: COLOR_LOGIC_OP
	// Enum op: CLEAR, AND, AND_REVERSE, COPY, AND_INVERTED, NOOP, XOR, OR, NOR, EQUIV, INVERT, OR_REVERSE, COPY_INVERTED, OR_INVERTED, NAND, SET
	void LogicOp(Enum op);

	// 4.2.1 Selecting a Buffer for Writing

	// Enum buf/bufs: NONE, COLOR_ATTACHMENTi, FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT
	void DrawBuffer(Enum buf);
	void DrawBuffers(Sizei n, const Enum *bufs);

	// 4.2.2 Fine Control of Buffer Updates

	// Enum face: FRONT, BACK, FRONT_AND_BACK
	void ColorMask(bool r, bool g, bool b, bool a);
	void ColorMaski(unsigned int buf, bool r, bool g, bool b, bool a);
	void DepthMask(bool mask);
	void StencilMask(unsigned int mask);
	void StencilMaskSeparate(Enum face, unsigned int mask);

	// 4.2.3 Clearing the Buffers

	// Enum buffer: COLOR, FRONT, BACK, LEFT, RIGHT, FRONT_AND_BACK
	// Enum depthstencil: DEPTH_STENCIL
	// Bitfield buf: COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT, STENCIL_BUFFER_BIT
	void Clear(Bitfield buf);
	void ClearColor(Clampf r, Clampf g, Clampf b, Clampf a);
	void ClearDepth(Clampd d);
	void ClearStencil(int s);
	void ClearBufferiv(Enum buffer, int drawbuffer, const int *value);
	void ClearBufferfv(Enum buffer, int drawbuffer, const float *value);
	void ClearBufferuiv(Enum buffer, int drawbuffer, const unsigned int *value);
	void ClearBufferfi(Enum depthstencil, int drawbuffer, float depth, int stencil);

	// 4.3.1 Reading Pixels

	// Enum format: DEPTH_COMPONENT, DEPTH_STENCIL, STENCIL_INDEX, any color buffer format
	// Enum src: COLOR, FRONT, BACK, LEFT, RIGHT, FRONT_AND_BACK
	// Enum target: CLAMP_READ_COLOR
	// Enum clamp: TRUE, FALSE, FIXED_ONLY
	void ReadPixels(int x, int y, Sizei width, Sizei height, Enum format, Enum type, void *data);
	void ReadBuffer(Enum src);
	void ClampColor(Enum target, Enum clamp);

	// 4.3.2 Copying Pixels

	// Enum filter: LINEAR, NEAREST
	// Bitfield mask: COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT, STENCIL_BUFFER_BIT
	void BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, Bitfield mask, Enum filter);

	// 4.4.1 Binding and Managing Framebuffer Objects

	void BindFramebuffer(Enum target, unsigned int framebuffer);
	void GenFramebuffers(Sizei n, unsigned int *ids);
	void BindRenderbuffer(Enum target, unsigned int renderbuffer);
	void DeleteRenderbuffers(Sizei n, const unsigned int *renderbuffers);
	void GenRenderbuffers(Sizei n, unsigned int *renderbuffers);
	void RenderbufferStorageMultisample(Enum target, Sizei samples, Enum internalformat, Sizei width, Sizei height);
	void RenderbufferStorage(Enum target, Enum internalformat, Sizei width, Sizei height);
	void FramebufferRenderbuffer(Enum target, Enum attachment, Enum renderbuffertarget, unsigned int renderbuffer);
	void FramebufferTexture(Enum target, Enum attachment, unsigned int texture, int level);
	void FramebufferTexture1D(Enum target, Enum attachment, Enum textarget, unsigned int texture, int level);
	void FramebufferTexture2D(Enum target, Enum attachment, Enum textarget, unsigned int texture, int level);
	void FramebufferTexture3D(Enum target, Enum attachment, Enum textarget, unsigned int texture, int level, int layer);
	void FramebufferTextureLayer(Enum target, Enum attachment, unsigned int texture, int level, int layer);
	Enum CheckFramebufferStatus(Enum target);

	// 5.1 Flush and Finish

	void Flush();
	void Finish();

	// 5.2 Sync Objects and Fences

	// Enum condition: SYNC_GPU_COMMANDS_COMPLETE
	// Bitfield flags: 0
	Sync FenceSync(Enum condition, Bitfield flags);
	void DeleteSync(Sync sync);

	// 5.2.1 Waiting for Sync Objects

	// Enum return value: ALREADY_SIGNALED, TIMEOUT_EXPIRED, CONDITION_SATISFIED, WAIT_FAILED
	// Bitfield clientflags: SYNC_FLUSH_COMMANDS_BIT
	// Bitfield serverflags: 0
	Enum ClientWaitSync(Sync sync, Bitfield clientflags, uint64 timeout);
	void WaitSync(Sync sync, Bitfield serverflags, uint64 timeout);

	// 5.3 Hints

	// Enum target: LINE_SMOOTH_HINT, POLYGON_SMOOTH_HINT, TEXTURE_COMPRESSION_HINT, FRAGMENT_SHADER_DERIVATIVE_HINT
	// Enum hint: FASTEST, NICEST, DONT_CARE
	void Hint(Enum target, Enum hint);

	// 6.1.1 Simple Queries

	void GetBooleanv(Enum value, bool *data);
	void GetIntegerv(Enum value, int *data);
	void GetInteger64v(Enum value, int64 *data);
	void GetFloatv(Enum value, float *data);
	void GetDoublev(Enum value, double *data);
	void GetBooleani_v(Enum target, unsigned int index, bool *data);
	void GetIntegeri_v(Enum target, unsigned int index, int *data);
	void GetInteger64i_v(Enum target, unsigned int index, int64 *data);
	bool IsEnabled(Enum value);
	bool IsEnabledi(Enum target, unsigned int index);

	// 6.1.3 Enumerated Queries

	void GetTexParameteriv(Enum target, Enum value, int *data);
	void GetTexParameterfv(Enum target, Enum value, float *data);
	void GetTexParameterIiv(Enum target, Enum value, int *data);
	void GetTexParameterIuiv(Enum target, Enum value, unsigned int *data);
	void GetTexLevelParameteriv(Enum target, int lod, Enum value, int *data);
	void GetTexLevelParameterfv(Enum target, int lod, Enum value, float *data);

	// 6.1.4 Texture Queries

	void GetTexImage(Enum tex, int lod, Enum format, Enum type, void *img);
	void GetCompressedTexImage(Enum target, int lod, void *img);
	bool IsTexture(unsigned int texture);

	// 6.1.5 String Queries (UTF-8 strings)

	unsigned char *GetString(Enum name);
	unsigned char *GetStringi(Enum name, unsigned int index);

	// 6.1.6 Asynchronous Queries

	bool IsQuery(unsigned int id);
	void GetQueryiv(Enum target, Enum pname, int *params);
	void GetQueryObjectiv(unsigned int id, Enum pname, int *params );
	void GetQueryObjectuiv(unsigned int id, Enum pname, unsigned int *params);

	// 6.1.7 Sync Object Queries

	void GetSynciv(Sync sync, Enum pname, Sizei bufSize, Sizei *length, int *values);
	bool IsSync(Sync sync);

	// 6.1.8 Buffer Object Queries

	bool IsBuffer(unsigned int buffer);
	void GetBufferParameteriv(Enum target, Enum pname, int *data);
	void GetBufferParameteri64v(Enum target, Enum pname, int64 *data);
	void GetBufferSubData(Enum target, Intptr offset, Sizeiptr size, void *data);
	void GetBufferPointerv(Enum target, Enum pname, void **params);

	// 6.1.9 Vertex Array Object Queries

	bool IsVertexArray(unsigned int array);

	// 6.1.10 Shader and Program Queries

	bool IsShader(unsigned int shader);
	void GetShaderiv(unsigned int shader, Enum pname, int *params);
	bool IsProgram(unsigned int program);
	void GetProgramiv(unsigned int program, Enum pname, int *params);
	void GetAttachedShaders(unsigned int program, Sizei maxCount, Sizei *count, unsigned int *shaders);
	void GetShaderInfoLog(unsigned int shader, Sizei bufSize, Sizei *length, char *infoLog);
	void GetProgramInfoLog(unsigned int program, Sizei bufSize, Sizei *length, char *infoLog);
	void GetShaderSource(unsigned int shader, Sizei bufSize, Sizei *length, char *source);
	void GetVertexAttribdv(unsigned int index, Enum pname, double *params);
	void GetVertexAttribfv(unsigned int index, Enum pname, float *params);
	void GetVertexAttribiv(unsigned int index, Enum pname, int *params);
	void GetVertexAttribIiv(unsigned int index, Enum pname, int *params);
	void GetVertexAttribIuiv(unsigned int index, Enum pname, unsigned int *params);
	void GetVertexAttribPointerv(unsigned int index, Enum pname, void **pointer);
	void GetUniformfv(unsigned int program, int location, float *params);
	void GetUniformiv(unsigned int program, int location, int *params);
	void GetUniformuiv(unsigned int program, int location, unsigned int *params);

	// 6.1.11 Framebuffer Object Queries

	bool IsFramebuffer(unsigned int framebuffer);
	void GetFramebufferAttachmentParameteriv(Enum target, Enum attachment, Enum pname, int *params);

	// 6.1.12 Renderbuffer Object Queries

	bool IsRenderbuffer(unsigned int renderbuffer);
	void GetRenderbufferParameteriv(Enum target, Enum pname, int *params);

private:
	HDC dc;
	HGLRC context;
};
