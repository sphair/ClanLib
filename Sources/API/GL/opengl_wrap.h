/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanGL="System"
//! header=gl.h

#ifndef header_opengl_wrap
#define header_opengl_wrap

#ifdef CL_API_DLL
#ifdef CL_GL_EXPORT
#define CL_API_GL __declspec(dllexport)
#else
#define CL_API_GL __declspec(dllimport)
#endif
#else
#define CL_API_GL
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "opengl.h"

#ifdef WIN32
#define CL_GLFUNC WINAPI
#else
#define CL_GLFUNC
#endif

//: OpenGL 2.0 defines
//- !group=GL/System!
//- !header=gl.h!
enum CL_GLDefines
{
	// AccumOp:

	CL_ACCUM                          = 0x0100,
	CL_LOAD                           = 0x0101,
	CL_RETURN                         = 0x0102,
	CL_MULT                           = 0x0103,
	CL_ADD                            = 0x0104,

	// AlphaFunction:

	CL_NEVER                          = 0x0200,
	CL_LESS                           = 0x0201,
	CL_EQUAL                          = 0x0202,
	CL_LEQUAL                         = 0x0203,
	CL_GREATER                        = 0x0204,
	CL_NOTEQUAL                       = 0x0205,
	CL_GEQUAL                         = 0x0206,
	CL_ALWAYS                         = 0x0207,

	// AttribMask:

	CL_CURRENT_BIT                    = 0x00000001,
	CL_POINT_BIT                      = 0x00000002,
	CL_LINE_BIT                       = 0x00000004,
	CL_POLYGON_BIT                    = 0x00000008,
	CL_POLYGON_STIPPLE_BIT            = 0x00000010,
	CL_PIXEL_MODE_BIT                 = 0x00000020,
	CL_LIGHTING_BIT                   = 0x00000040,
	CL_FOG_BIT                        = 0x00000080,
	CL_DEPTH_BUFFER_BIT               = 0x00000100,
	CL_ACCUM_BUFFER_BIT               = 0x00000200,
	CL_STENCIL_BUFFER_BIT             = 0x00000400,
	CL_VIEWPORT_BIT                   = 0x00000800,
	CL_TRANSFORM_BIT                  = 0x00001000,
	CL_ENABLE_BIT                     = 0x00002000,
	CL_COLOR_BUFFER_BIT               = 0x00004000,
	CL_HINT_BIT                       = 0x00008000,
	CL_EVAL_BIT                       = 0x00010000,
	CL_LIST_BIT                       = 0x00020000,
	CL_TEXTURE_BIT                    = 0x00040000,
	CL_SCISSOR_BIT                    = 0x00080000,
	CL_ALL_ATTRIB_BITS                = 0x000fffff,

	// BeginMode:

	CL_POINTS                         = 0x0000,
	CL_LINES                          = 0x0001,
	CL_LINE_LOOP                      = 0x0002,
	CL_LINE_STRIP                     = 0x0003,
	CL_TRIANGLES                      = 0x0004,
	CL_TRIANGLE_STRIP                 = 0x0005,
	CL_TRIANGLE_FAN                   = 0x0006,
	CL_QUADS                          = 0x0007,
	CL_QUAD_STRIP                     = 0x0008,
	CL_POLYGON                        = 0x0009,

	// BlendEquationMode:

	// CL_LOGIC_OP
	// CL_FUNC_ADD
	// CL_MIN
	// CL_MAX
	// CL_FUNC_SUBTRACT
	// CL_FUNC_REVERSE_SUBTRACT

	// BlendingFactorDest:

	CL_ZERO                           = 0,
	CL_ONE                            = 1,
	CL_SRC_COLOR                      = 0x0300,
	CL_ONE_MINUS_SRC_COLOR            = 0x0301,
	CL_SRC_ALPHA                      = 0x0302,
	CL_ONE_MINUS_SRC_ALPHA            = 0x0303,
	CL_DST_ALPHA                      = 0x0304,
	CL_ONE_MINUS_DST_ALPHA            = 0x0305,
	// CL_CONSTANT_COLOR
	// CL_ONE_MINUS_CONSTANT_COLOR
	// CL_CONSTANT_ALPHA
	// CL_ONE_MINUS_CONSTANT_ALPHA

	// BlendingFactorSrc:

	// CL_ZERO
	// CL_ONE
	CL_DST_COLOR                      = 0x0306,
	CL_ONE_MINUS_DST_COLOR            = 0x0307,
	CL_SRC_ALPHA_SATURATE             = 0x0308,
	// CL_SRC_ALPHA
	// CL_ONE_MINUS_SRC_ALPHA
	// CL_DST_ALPHA
	// CL_ONE_MINUS_DST_ALPHA
	// CL_CONSTANT_COLOR
	// CL_ONE_MINUS_CONSTANT_COLOR
	// CL_CONSTANT_ALPHA
	// CL_ONE_MINUS_CONSTANT_ALPHA

	// Boolean:

	CL_TRUE                           = 1,
	CL_FALSE                          = 0,

	// ClearBufferMask:

	// COLOR_BUFFER_BIT
	// ACCUM_BUFFER_BIT
	// STENCIL_BUFFER_BIT
	// DEPTH_BUFFER_BIT

	// ClientArrayType:

	// VERTEX_ARRAY
	// NORMAL_ARRAY
	// COLOR_ARRAY
	// INDEX_ARRAY
	// TEXTURE_COORD_ARRAY
	// EDGE_FLAG_ARRAY

	// ClipPlaneName:

	CL_CLIP_PLANE0                    = 0x3000,
	CL_CLIP_PLANE1                    = 0x3001,
	CL_CLIP_PLANE2                    = 0x3002,
	CL_CLIP_PLANE3                    = 0x3003,
	CL_CLIP_PLANE4                    = 0x3004,
	CL_CLIP_PLANE5                    = 0x3005,

	// ColorMaterialFace:

	// CL_FRONT
	// CL_BACK
	// CL_FRONT_AND_BACK

	// ColorMaterialParameter:

	// CL_AMBIENT
	// CL_DIFFUSE
	// CL_SPECULAR
	// CL_EMISSION
	// CL_AMBIENT_AND_DIFFUSE

	// ColorPointerType:

	// CL_BYTE
	// CL_UNSIGNED_BYTE
	// CL_SHORT
	// CL_UNSIGNED_SHORT
	// CL_INT
	// CL_UNSIGNED_INT
	// CL_FLOAT
	// CL_DOUBLE

	// ColorTableParameterPName:

	// CL_COLOR_TABLE_SCALE
	// CL_COLOR_TABLE_BIAS

	// ColorTableTarget:

	// CL_COLOR_TABLE
	// CL_POST_CONVOLUTION_COLOR_TABLE
	// CL_POST_COLOR_MATRIX_COLOR_TABLE
	// CL_PROXY_COLOR_TABLE
	// CL_PROXY_POST_CONVOLUTION_COLOR_TABLE
	// CL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE

	// ConvolutionBorderMode:

	// CL_REDUCE
	// CL_IGNORE_BORDER
	// CL_CONSTANT_BORDER

	// ConvolutionParameter:

	// CL_CONVOLUTION_BORDER_MODE
	// CL_CONVOLUTION_FILTER_SCALE
	// CL_CONVOLUTION_FILTER_BIAS

	// ConvolutionTarget:

	// CL_CONVOLUTION_1D
	// CL_CONVOLUTION_2D

	// CullFaceMode:

	// CL_FRONT
	// CL_BACK
	// CL_FRONT_AND_BACK

	// DataType:

	CL_BYTE                           = 0x1400,
	CL_UNSIGNED_BYTE                  = 0x1401,
	CL_SHORT                          = 0x1402,
	CL_UNSIGNED_SHORT                 = 0x1403,
	CL_INT                            = 0x1404,
	CL_UNSIGNED_INT                   = 0x1405,
	CL_FLOAT                          = 0x1406,
	CL_2_BYTES                        = 0x1407,
	CL_3_BYTES                        = 0x1408,
	CL_4_BYTES                        = 0x1409,
	CL_DOUBLE                         = 0x140A,

	// DepthFunction:

	// CL_NEVER
	// CL_LESS
	// CL_EQUAL
	// CL_LEQUAL
	// CL_GREATER
	// CL_NOTEQUAL
	// CL_GEQUAL
	// CL_ALWAYS

	// DrawBufferMode:

	CL_NONE                           = 0,
	CL_FRONT_LEFT                     = 0x0400,
	CL_FRONT_RIGHT                    = 0x0401,
	CL_BACK_LEFT                      = 0x0402,
	CL_BACK_RIGHT                     = 0x0403,
	CL_FRONT                          = 0x0404,
	CL_BACK                           = 0x0405,
	CL_LEFT                           = 0x0406,
	CL_RIGHT                          = 0x0407,
	CL_FRONT_AND_BACK                 = 0x0408,
	CL_AUX0                           = 0x0409,
	CL_AUX1                           = 0x040A,
	CL_AUX2                           = 0x040B,
	CL_AUX3                           = 0x040C,

	// Enable:

	// CL_FOG
	// CL_LIGHTING
	// CL_TEXTURE_1D
	// CL_TEXTURE_2D
	// CL_LINE_STIPPLE
	// CL_POLYGON_STIPPLE
	// CL_CULL_FACE
	// CL_ALPHA_TEST
	// CL_BLEND
	// CL_INDEX_LOGIC_OP
	// CL_COLOR_LOGIC_OP
	// CL_DITHER
	// CL_STENCIL_TEST
	// CL_DEPTH_TEST
	// CL_CLIP_PLANE0
	// CL_CLIP_PLANE1
	// CL_CLIP_PLANE2
	// CL_CLIP_PLANE3
	// CL_CLIP_PLANE4
	// CL_CLIP_PLANE5
	// CL_LIGHT0
	// CL_LIGHT1
	// CL_LIGHT2
	// CL_LIGHT3
	// CL_LIGHT4
	// CL_LIGHT5
	// CL_LIGHT6
	// CL_LIGHT7
	// CL_TEXTURE_GEN_S
	// CL_TEXTURE_GEN_T
	// CL_TEXTURE_GEN_R
	// CL_TEXTURE_GEN_Q
	// CL_MAP1_VERTEX_3
	// CL_MAP1_VERTEX_4
	// CL_MAP1_COLOR_4
	// CL_MAP1_INDEX
	// CL_MAP1_NORMAL
	// CL_MAP1_TEXTURE_COORD_1
	// CL_MAP1_TEXTURE_COORD_2
	// CL_MAP1_TEXTURE_COORD_3
	// CL_MAP1_TEXTURE_COORD_4
	// CL_MAP2_VERTEX_3
	// CL_MAP2_VERTEX_4
	// CL_MAP2_COLOR_4
	// CL_MAP2_INDEX
	// CL_MAP2_NORMAL
	// CL_MAP2_TEXTURE_COORD_1
	// CL_MAP2_TEXTURE_COORD_2
	// CL_MAP2_TEXTURE_COORD_3
	// CL_MAP2_TEXTURE_COORD_4
	// CL_POINT_SMOOTH
	// CL_LINE_SMOOTH
	// CL_POLYGON_SMOOTH
	// CL_SCISSOR_TEST
	// CL_COLOR_MATERIAL
	// CL_NORMALIZE
	// CL_AUTO_NORMAL
	// CL_VERTEX_ARRAY
	// CL_NORMAL_ARRAY
	// CL_COLOR_ARRAY
	// CL_INDEX_ARRAY
	// CL_TEXTURE_COORD_ARRAY
	// CL_EDGE_FLAG_ARRAY
	// CL_POLYGON_OFFSET_POINT
	// CL_POLYGON_OFFSET_LINE
	// CL_POLYGON_OFFSET_FILL
	// CL_COLOR_TABLE
	// CL_POST_CONVOLUTION_COLOR_TABLE
	// CL_POST_COLOR_MATRIX_COLOR_TABLE
	// CL_CONVOLUTION_1D
	// CL_CONVOLUTION_2D
	// CL_SEPARABLE_2D
	// CL_HISTOGRAM
	// CL_MINMAX
	// CL_RESCALE_NORMAL
	// CL_TEXTURE_3D

	// ErrorCode:

	CL_NO_ERROR                     = 0,
	CL_INVALID_ENUM                 = 0x0500,
	CL_INVALID_VALUE                = 0x0501,
	CL_INVALID_OPERATION            = 0x0502,
	CL_STACK_OVERFLOW               = 0x0503,
	CL_STACK_UNDERFLOW              = 0x0504,
	CL_OUT_OF_MEMORY                = 0x0505,
	// CL_TABLE_TOO_LARGE

	// FeedBackMode:

	CL_2D                           = 0x0600,
	CL_3D                           = 0x0601,
	CL_3D_COLOR                     = 0x0602,
	CL_3D_COLOR_TEXTURE             = 0x0603,
	CL_4D_COLOR_TEXTURE             = 0x0604,

	// FeedBackToken:

	CL_PASS_THROUGH_TOKEN           = 0x0700,
	CL_POINT_TOKEN                  = 0x0701,
	CL_LINE_TOKEN                   = 0x0702,
	CL_POLYGON_TOKEN                = 0x0703,
	CL_BITMAP_TOKEN                 = 0x0704,
	CL_DRAW_PIXEL_TOKEN             = 0x0705,
	CL_COPY_PIXEL_TOKEN             = 0x0706,
	CL_LINE_RESET_TOKEN             = 0x0707,

	// FogMode:

	// CL_LINEAR
	CL_EXP                          = 0x0800,
	CL_EXP2                         = 0x0801,

	// FogParameter:

	// CL_FOG_COLOR
	// CL_FOG_DENSITY
	// CL_FOG_END
	// CL_FOG_INDEX
	// CL_FOG_MODE
	// CL_FOG_START

	// FrontFaceDirection:

	CL_CW                           = 0x0900,
	CL_CCW                          = 0x0901,

	// GetColorTableParameterPName:

	// CL_COLOR_TABLE_SCALE
	// CL_COLOR_TABLE_BIAS
	// CL_COLOR_TABLE_FORMAT
	// CL_COLOR_TABLE_WIDTH
	// CL_COLOR_TABLE_RED_SIZE
	// CL_COLOR_TABLE_GREEN_SIZE
	// CL_COLOR_TABLE_BLUE_SIZE
	// CL_COLOR_TABLE_ALPHA_SIZE
	// CL_COLOR_TABLE_LUMINANCE_SIZE
	// CL_COLOR_TABLE_INTENSITY_SIZE

	// GetConvolutionParameterPName:

	// CL_CONVOLUTION_BORDER_COLOR
	// CL_CONVOLUTION_BORDER_MODE
	// CL_CONVOLUTION_FILTER_SCALE
	// CL_CONVOLUTION_FILTER_BIAS
	// CL_CONVOLUTION_FORMAT
	// CL_CONVOLUTION_WIDTH
	// CL_CONVOLUTION_HEIGHT
	// CL_MAX_CONVOLUTION_WIDTH
	// CL_MAX_CONVOLUTION_HEIGHT

	// GetHistogramParameterPName:

	// CL_HISTOGRAM_WIDTH
	// CL_HISTOGRAM_FORMAT
	// CL_HISTOGRAM_RED_SIZE
	// CL_HISTOGRAM_GREEN_SIZE
	// CL_HISTOGRAM_BLUE_SIZE
	// CL_HISTOGRAM_ALPHA_SIZE
	// CL_HISTOGRAM_LUMINANCE_SIZE
	// CL_HISTOGRAM_SINK

	// GetMapTarget:

	CL_COEFF                        = 0x0A00,
	CL_ORDER                        = 0x0A01,
	CL_DOMAIN                       = 0x0A02,

	// GetMinmaxParameterPName:

	// CL_MINMAX_FORMAT
	// CL_MINMAX_SINK

	// GetPixelMap:

	// CL_PIXEL_MAP_I_TO_I
	// CL_PIXEL_MAP_S_TO_S
	// CL_PIXEL_MAP_I_TO_R
	// CL_PIXEL_MAP_I_TO_G
	// CL_PIXEL_MAP_I_TO_B
	// CL_PIXEL_MAP_I_TO_A
	// CL_PIXEL_MAP_R_TO_R
	// CL_PIXEL_MAP_G_TO_G
	// CL_PIXEL_MAP_B_TO_B
	// CL_PIXEL_MAP_A_TO_A

	// GetPointerTarget:

	// CL_VERTEX_ARRAY_POINTER
	// CL_NORMAL_ARRAY_POINTER
	// CL_COLOR_ARRAY_POINTER
	// CL_INDEX_ARRAY_POINTER
	// CL_TEXTURE_COORD_ARRAY_POINTER
	// CL_EDGE_FLAG_ARRAY_POINTER

	// GetTarget:

	CL_CURRENT_COLOR                = 0x0B00,
	CL_CURRENT_INDEX                = 0x0B01,
	CL_CURRENT_NORMAL               = 0x0B02,
	CL_CURRENT_TEXTURE_COORDS       = 0x0B03,
	CL_CURRENT_RASTER_COLOR         = 0x0B04,
	CL_CURRENT_RASTER_INDEX         = 0x0B05,
	CL_CURRENT_RASTER_TEXTURE_COORDS= 0x0B06,
	CL_CURRENT_RASTER_POSITION      = 0x0B07,
	CL_CURRENT_RASTER_POSITION_VALID= 0x0B08,
	CL_CURRENT_RASTER_DISTANCE      = 0x0B09,
	CL_POINT_SMOOTH                 = 0x0B10,
	CL_POINT_SIZE                   = 0x0B11,
	CL_POINT_SIZE_RANGE             = 0x0B12,
	CL_POINT_SIZE_GRANULARITY       = 0x0B13,
	CL_LINE_SMOOTH                  = 0x0B20,
	CL_LINE_WIDTH                   = 0x0B21,
	CL_LINE_WIDTH_RANGE             = 0x0B22,
	CL_LINE_WIDTH_GRANULARITY       = 0x0B23,
	CL_LINE_STIPPLE                 = 0x0B24,
	CL_LINE_STIPPLE_PATTERN         = 0x0B25,
	CL_LINE_STIPPLE_REPEAT          = 0x0B26,
	// CL_SMOOTH_POINT_SIZE_RANGE
	// CL_SMOOTH_POINT_SIZE_GRANULARITY
	// CL_SMOOTH_LINE_WIDTH_RANGE
	// CL_SMOOTH_LINE_WIDTH_GRANULARITY
	// CL_ALIASED_POINT_SIZE_RANGE
	// CL_ALIASED_LINE_WIDTH_RANGE
	CL_LIST_MODE                    = 0x0B30,
	CL_MAX_LIST_NESTING             = 0x0B31,
	CL_LIST_BASE                    = 0x0B32,
	CL_LIST_INDEX                   = 0x0B33,
	CL_POLYGON_MODE                 = 0x0B40,
	CL_POLYGON_SMOOTH               = 0x0B41,
	CL_POLYGON_STIPPLE              = 0x0B42,
	CL_EDGE_FLAG                    = 0x0B43,
	CL_CULL_FACE                    = 0x0B44,
	CL_CULL_FACE_MODE               = 0x0B45,
	CL_FRONT_FACE                   = 0x0B46,
	CL_LIGHTING                     = 0x0B50,
	CL_LIGHT_MODEL_LOCAL_VIEWER     = 0x0B51,
	CL_LIGHT_MODEL_TWO_SIDE         = 0x0B52,
	CL_LIGHT_MODEL_AMBIENT          = 0x0B53,
	CL_SHADE_MODEL                  = 0x0B54,
	CL_COLOR_MATERIAL_FACE          = 0x0B55,
	CL_COLOR_MATERIAL_PARAMETER     = 0x0B56,
	CL_COLOR_MATERIAL               = 0x0B57,
	CL_FOG                          = 0x0B60,
	CL_FOG_INDEX                    = 0x0B61,
	CL_FOG_DENSITY                  = 0x0B62,
	CL_FOG_START                    = 0x0B63,
	CL_FOG_END                      = 0x0B64,
	CL_FOG_MODE                     = 0x0B65,
	CL_FOG_COLOR                    = 0x0B66,
	CL_DEPTH_RANGE                  = 0x0B70,
	CL_DEPTH_TEST                   = 0x0B71,
	CL_DEPTH_WRITEMASK              = 0x0B72,
	CL_DEPTH_CLEAR_VALUE            = 0x0B73,
	CL_DEPTH_FUNC                   = 0x0B74,
	CL_ACCUM_CLEAR_VALUE            = 0x0B80,
	CL_STENCIL_TEST                 = 0x0B90,
	CL_STENCIL_CLEAR_VALUE          = 0x0B91,
	CL_STENCIL_FUNC                 = 0x0B92,
	CL_STENCIL_VALUE_MASK           = 0x0B93,
	CL_STENCIL_FAIL                 = 0x0B94,
	CL_STENCIL_PASS_DEPTH_FAIL      = 0x0B95,
	CL_STENCIL_PASS_DEPTH_PASS      = 0x0B96,
	CL_STENCIL_REF                  = 0x0B97,
	CL_STENCIL_WRITEMASK            = 0x0B98,
	CL_MATRIX_MODE                  = 0x0BA0,
	CL_NORMALIZE                    = 0x0BA1,
	CL_VIEWPORT                     = 0x0BA2,
	CL_MODELVIEW_STACK_DEPTH        = 0x0BA3,
	CL_PROJECTION_STACK_DEPTH       = 0x0BA4,
	CL_TEXTURE_STACK_DEPTH          = 0x0BA5,
	CL_MODELVIEW_MATRIX             = 0x0BA6,
	CL_PROJECTION_MATRIX            = 0x0BA7,
	CL_TEXTURE_MATRIX               = 0x0BA8,
	CL_ATTRIB_STACK_DEPTH           = 0x0BB0,
	CL_CLIENT_ATTRIB_STACK_DEPTH    = 0x0BB1,
	CL_ALPHA_TEST                   = 0x0BC0,
	CL_ALPHA_TEST_FUNC              = 0x0BC1,
	CL_ALPHA_TEST_REF               = 0x0BC2,
	CL_DITHER                       = 0x0BD0,
	CL_BLEND_DST                    = 0x0BE0,
	CL_BLEND_SRC                    = 0x0BE1,
	CL_BLEND                        = 0x0BE2,
	CL_LOGIC_OP_MODE                = 0x0BF0,
	CL_INDEX_LOGIC_OP               = 0x0BF1,
	CL_COLOR_LOGIC_OP               = 0x0BF2,
	CL_AUX_BUFFERS                  = 0x0C00,
	CL_DRAW_BUFFER                  = 0x0C01,
	CL_READ_BUFFER                  = 0x0C02,
	CL_SCISSOR_BOX                  = 0x0C10,
	CL_SCISSOR_TEST                 = 0x0C11,
	CL_INDEX_CLEAR_VALUE            = 0x0C20,
	CL_INDEX_WRITEMASK              = 0x0C21,
	CL_COLOR_CLEAR_VALUE            = 0x0C22,
	CL_COLOR_WRITEMASK              = 0x0C23,
	CL_INDEX_MODE                   = 0x0C30,
	CL_RGBA_MODE                    = 0x0C31,
	CL_DOUBLEBUFFER                 = 0x0C32,
	CL_STEREO                       = 0x0C33,
	CL_RENDER_MODE                  = 0x0C40,
	CL_PERSPECTIVE_CORRECTION_HINT  = 0x0C50,
	CL_POINT_SMOOTH_HINT            = 0x0C51,
	CL_LINE_SMOOTH_HINT             = 0x0C52,
	CL_POLYGON_SMOOTH_HINT          = 0x0C53,
	CL_FOG_HINT                     = 0x0C54,
	CL_TEXTURE_GEN_S                = 0x0C60,
	CL_TEXTURE_GEN_T                = 0x0C61,
	CL_TEXTURE_GEN_R                = 0x0C62,
	CL_TEXTURE_GEN_Q                = 0x0C63,
	CL_PIXEL_MAP_I_TO_I             = 0x0C70,
	CL_PIXEL_MAP_S_TO_S             = 0x0C71,
	CL_PIXEL_MAP_I_TO_R             = 0x0C72,
	CL_PIXEL_MAP_I_TO_G             = 0x0C73,
	CL_PIXEL_MAP_I_TO_B             = 0x0C74,
	CL_PIXEL_MAP_I_TO_A             = 0x0C75,
	CL_PIXEL_MAP_R_TO_R             = 0x0C76,
	CL_PIXEL_MAP_G_TO_G             = 0x0C77,
	CL_PIXEL_MAP_B_TO_B             = 0x0C78,
	CL_PIXEL_MAP_A_TO_A             = 0x0C79,
	CL_PIXEL_MAP_I_TO_I_SIZE        = 0x0CB0,
	CL_PIXEL_MAP_S_TO_S_SIZE        = 0x0CB1,
	CL_PIXEL_MAP_I_TO_R_SIZE        = 0x0CB2,
	CL_PIXEL_MAP_I_TO_G_SIZE        = 0x0CB3,
	CL_PIXEL_MAP_I_TO_B_SIZE        = 0x0CB4,
	CL_PIXEL_MAP_I_TO_A_SIZE        = 0x0CB5,
	CL_PIXEL_MAP_R_TO_R_SIZE        = 0x0CB6,
	CL_PIXEL_MAP_G_TO_G_SIZE        = 0x0CB7,
	CL_PIXEL_MAP_B_TO_B_SIZE        = 0x0CB8,
	CL_PIXEL_MAP_A_TO_A_SIZE        = 0x0CB9,
	CL_UNPACK_SWAP_BYTES            = 0x0CF0,
	CL_UNPACK_LSB_FIRST             = 0x0CF1,
	CL_UNPACK_ROW_LENGTH            = 0x0CF2,
	CL_UNPACK_SKIP_ROWS             = 0x0CF3,
	CL_UNPACK_SKIP_PIXELS           = 0x0CF4,
	CL_UNPACK_ALIGNMENT             = 0x0CF5,
	CL_PACK_SWAP_BYTES              = 0x0D00,
	CL_PACK_LSB_FIRST               = 0x0D01,
	CL_PACK_ROW_LENGTH              = 0x0D02,
	CL_PACK_SKIP_ROWS               = 0x0D03,
	CL_PACK_SKIP_PIXELS             = 0x0D04,
	CL_PACK_ALIGNMENT               = 0x0D05,
	CL_MAP_COLOR                    = 0x0D10,
	CL_MAP_STENCIL                  = 0x0D11,
	CL_INDEX_SHIFT                  = 0x0D12,
	CL_INDEX_OFFSET                 = 0x0D13,
	CL_RED_SCALE                    = 0x0D14,
	CL_RED_BIAS                     = 0x0D15,
	CL_ZOOM_X                       = 0x0D16,
	CL_ZOOM_Y                       = 0x0D17,
	CL_GREEN_SCALE                  = 0x0D18,
	CL_GREEN_BIAS                   = 0x0D19,
	CL_BLUE_SCALE                   = 0x0D1A,
	CL_BLUE_BIAS                    = 0x0D1B,
	CL_ALPHA_SCALE                  = 0x0D1C,
	CL_ALPHA_BIAS                   = 0x0D1D,
	CL_DEPTH_SCALE                  = 0x0D1E,
	CL_DEPTH_BIAS                   = 0x0D1F,
	CL_MAX_EVAL_ORDER               = 0x0D30,
	CL_MAX_LIGHTS                   = 0x0D31,
	CL_MAX_CLIP_PLANES              = 0x0D32,
	CL_MAX_TEXTURE_SIZE             = 0x0D33,
	CL_MAX_PIXEL_MAP_TABLE          = 0x0D34,
	CL_MAX_ATTRIB_STACK_DEPTH       = 0x0D35,
	CL_MAX_MODELVIEW_STACK_DEPTH    = 0x0D36,
	CL_MAX_NAME_STACK_DEPTH         = 0x0D37,
	CL_MAX_PROJECTION_STACK_DEPTH   = 0x0D38,
	CL_MAX_TEXTURE_STACK_DEPTH      = 0x0D39,
	CL_MAX_VIEWPORT_DIMS            = 0x0D3A,
	CL_MAX_CLIENT_ATTRIB_STACK_DEPTH= 0x0D3B,
	CL_SUBPIXEL_BITS                = 0x0D50,
	CL_INDEX_BITS                   = 0x0D51,
	CL_RED_BITS                     = 0x0D52,
	CL_GREEN_BITS                   = 0x0D53,
	CL_BLUE_BITS                    = 0x0D54,
	CL_ALPHA_BITS                   = 0x0D55,
	CL_DEPTH_BITS                   = 0x0D56,
	CL_STENCIL_BITS                 = 0x0D57,
	CL_ACCUM_RED_BITS               = 0x0D58,
	CL_ACCUM_GREEN_BITS             = 0x0D59,
	CL_ACCUM_BLUE_BITS              = 0x0D5A,
	CL_ACCUM_ALPHA_BITS             = 0x0D5B,
	CL_NAME_STACK_DEPTH             = 0x0D70,
	CL_AUTO_NORMAL                  = 0x0D80,
	CL_MAP1_COLOR_4                 = 0x0D90,
	CL_MAP1_INDEX                   = 0x0D91,
	CL_MAP1_NORMAL                  = 0x0D92,
	CL_MAP1_TEXTURE_COORD_1         = 0x0D93,
	CL_MAP1_TEXTURE_COORD_2         = 0x0D94,
	CL_MAP1_TEXTURE_COORD_3         = 0x0D95,
	CL_MAP1_TEXTURE_COORD_4         = 0x0D96,
	CL_MAP1_VERTEX_3                = 0x0D97,
	CL_MAP1_VERTEX_4                = 0x0D98,
	CL_MAP2_COLOR_4                 = 0x0DB0,
	CL_MAP2_INDEX                   = 0x0DB1,
	CL_MAP2_NORMAL                  = 0x0DB2,
	CL_MAP2_TEXTURE_COORD_1         = 0x0DB3,
	CL_MAP2_TEXTURE_COORD_2         = 0x0DB4,
	CL_MAP2_TEXTURE_COORD_3         = 0x0DB5,
	CL_MAP2_TEXTURE_COORD_4         = 0x0DB6,
	CL_MAP2_VERTEX_3                = 0x0DB7,
	CL_MAP2_VERTEX_4                = 0x0DB8,
	CL_MAP1_GRID_DOMAIN             = 0x0DD0,
	CL_MAP1_GRID_SEGMENTS           = 0x0DD1,
	CL_MAP2_GRID_DOMAIN             = 0x0DD2,
	CL_MAP2_GRID_SEGMENTS           = 0x0DD3,
	CL_TEXTURE_1D                   = 0x0DE0,
	CL_TEXTURE_2D                   = 0x0DE1,
	CL_FEEDBACK_BUFFER_POINTER      = 0x0DF0,
	CL_FEEDBACK_BUFFER_SIZE         = 0x0DF1,
	CL_FEEDBACK_BUFFER_TYPE         = 0x0DF2,
	CL_SELECTION_BUFFER_POINTER     = 0x0DF3,
	CL_SELECTION_BUFFER_SIZE        = 0x0DF4,
	// CL_TEXTURE_BINDING_1D
	// CL_TEXTURE_BINDING_2D
	// CL_TEXTURE_BINDING_3D
	// CL_VERTEX_ARRAY
	// CL_NORMAL_ARRAY
	// CL_COLOR_ARRAY
	// CL_INDEX_ARRAY
	// CL_TEXTURE_COORD_ARRAY
	// CL_EDGE_FLAG_ARRAY
	// CL_VERTEX_ARRAY_SIZE
	// CL_VERTEX_ARRAY_TYPE
	// CL_VERTEX_ARRAY_STRIDE
	// CL_NORMAL_ARRAY_TYPE
	// CL_NORMAL_ARRAY_STRIDE
	// CL_COLOR_ARRAY_SIZE
	// CL_COLOR_ARRAY_TYPE
	// CL_COLOR_ARRAY_STRIDE
	// CL_INDEX_ARRAY_TYPE
	// CL_INDEX_ARRAY_STRIDE
	// CL_TEXTURE_COORD_ARRAY_SIZE
	// CL_TEXTURE_COORD_ARRAY_TYPE
	// CL_TEXTURE_COORD_ARRAY_STRIDE
	// CL_EDGE_FLAG_ARRAY_STRIDE
	// CL_POLYGON_OFFSET_FACTOR
	// CL_POLYGON_OFFSET_UNITS
	// CL_COLOR_TABLE
	// CL_POST_CONVOLUTION_COLOR_TABLE
	// CL_POST_COLOR_MATRIX_COLOR_TABLE
	// CL_CONVOLUTION_1D
	// CL_CONVOLUTION_2D
	// CL_SEPARABLE_2D
	// CL_POST_CONVOLUTION_RED_SCALE
	// CL_POST_CONVOLUTION_GREEN_SCALE
	// CL_POST_CONVOLUTION_BLUE_SCALE
	// CL_POST_CONVOLUTION_ALPHA_SCALE
	// CL_POST_CONVOLUTION_RED_BIAS
	// CL_POST_CONVOLUTION_GREEN_BIAS
	// CL_POST_CONVOLUTION_BLUE_BIAS
	// CL_POST_CONVOLUTION_ALPHA_BIAS
	// CL_COLOR_MATRIX
	// CL_COLOR_MATRIX_STACK_DEPTH
	// CL_MAX_COLOR_MATRIX_STACK_DEPTH
	// CL_POST_COLOR_MATRIX_RED_SCALE
	// CL_POST_COLOR_MATRIX_GREEN_SCALE
	// CL_POST_COLOR_MATRIX_BLUE_SCALE
	// CL_POST_COLOR_MATRIX_ALPHA_SCALE
	// CL_POST_COLOR_MATRIX_RED_BIAS
	// CL_POST_COLOR_MATRIX_GREEN_BIAS
	// CL_POST_COLOR_MATRIX_BLUE_BIAS
	// CL_POST_COLOR_MATRIX_ALPHA_BIAS
	// CL_HISTOGRAM
	// CL_MINMAX
	// CL_MAX_ELEMENTS_VERTICES
	// CL_MAX_ELEMENTS_INDICES
	// CL_RESCALE_NORMAL
	// CL_LIGHT_MODEL_COLOR_CONTROL
	// CL_PACK_SKIP_IMAGES
	// CL_PACK_IMAGE_HEIGHT
	// CL_UNPACK_SKIP_IMAGES
	// CL_UNPACK_IMAGE_HEIGHT
	// CL_TEXTURE_3D
	// CL_MAX_3D_TEXTURE_SIZE
	// CL_BLEND_COLOR
	// CL_BLEND_EQUATION
	// CL_ACTIVE_TEXTURE
	// CL_CLIENT_ACTIVE_TEXTURE
	// CL_MAX_TEXTURE_UNITS

	// GetTextureParameter:

	// CL_TEXTURE_MAG_FILTER
	// CL_TEXTURE_MIN_FILTER
	// CL_TEXTURE_WRAP_S
	// CL_TEXTURE_WRAP_T
	CL_TEXTURE_WIDTH                = 0x1000,
	CL_TEXTURE_HEIGHT               = 0x1001,
	CL_TEXTURE_INTERNAL_FORMAT      = 0x1003,
	CL_TEXTURE_BORDER_COLOR         = 0x1004,
	CL_TEXTURE_BORDER               = 0x1005,
	// CL_TEXTURE_RED_SIZE
	// CL_TEXTURE_GREEN_SIZE
	// CL_TEXTURE_BLUE_SIZE
	// CL_TEXTURE_ALPHA_SIZE
	// CL_TEXTURE_LUMINANCE_SIZE
	// CL_TEXTURE_INTENSITY_SIZE
	// CL_TEXTURE_PRIORITY
	// CL_TEXTURE_RESIDENT
	// CL_TEXTURE_DEPTH
	// CL_TEXTURE_WRAP_R
	// CL_TEXTURE_MIN_LOD
	// CL_TEXTURE_MAX_LOD
	// CL_TEXTURE_BASE_LEVEL
	// CL_TEXTURE_MAX_LEVEL

	// HintMode:

	CL_DONT_CARE                    = 0x1100,
	CL_FASTEST                      = 0x1101,
	CL_NICEST                       = 0x1102,

	// HintTarget:

	// CL_PERSPECTIVE_CORRECTION_HINT
	// CL_POINT_SMOOTH_HINT
	// CL_LINE_SMOOTH_HINT
	// CL_POLYGON_SMOOTH_HINT
	// CL_FOG_HINT

	// HistogramTarget:

	// CL_HISTOGRAM
	// CL_PROXY_HISTOGRAM

	// IndexPointerType:

	// CL_SHORT
	// CL_INT
	// CL_FLOAT
	// CL_DOUBLE

	// LightModelColorControl:

	// CL_SINGLE_COLOR
	// CL_SEPARATE_SPECULAR_COLOR

	// LightModelParameter:

	// CL_LIGHT_MODEL_AMBIENT
	// CL_LIGHT_MODEL_LOCAL_VIEWER
	// CL_LIGHT_MODEL_TWO_SIDE
	// CL_LIGHT_MODEL_COLOR_CONTROL

	// LightName:

	CL_LIGHT0                       = 0x4000,
	CL_LIGHT1                       = 0x4001,
	CL_LIGHT2                       = 0x4002,
	CL_LIGHT3                       = 0x4003,
	CL_LIGHT4                       = 0x4004,
	CL_LIGHT5                       = 0x4005,
	CL_LIGHT6                       = 0x4006,
	CL_LIGHT7                       = 0x4007,

	// LightParameter:

	CL_AMBIENT                      = 0x1200,
	CL_DIFFUSE                      = 0x1201,
	CL_SPECULAR                     = 0x1202,
	CL_POSITION                     = 0x1203,
	CL_SPOT_DIRECTION               = 0x1204,
	CL_SPOT_EXPONENT                = 0x1205,
	CL_SPOT_CUTOFF                  = 0x1206,
	CL_CONSTANT_ATTENUATION         = 0x1207,
	CL_LINEAR_ATTENUATION           = 0x1208,
	CL_QUADRATIC_ATTENUATION        = 0x1209,

	// InterleavedArrays:

	// CL_V2F
	// CL_V3F
	// CL_C4UB_V2F
	// CL_C4UB_V3F
	// CL_C3F_V3F
	// CL_N3F_V3F
	// CL_C4F_N3F_V3F
	// CL_T2F_V3F
	// CL_T4F_V4F
	// CL_T2F_C4UB_V3F
	// CL_T2F_C3F_V3F
	// CL_T2F_N3F_V3F
	// CL_T2F_C4F_N3F_V3F
	// CL_T4F_C4F_N3F_V4F

	// ListMode:

	CL_COMPILE                      = 0x1300,
	CL_COMPILE_AND_EXECUTE          = 0x1301,

	// ListNameType:

	// CL_BYTE
	// CL_UNSIGNED_BYTE
	// CL_SHORT
	// CL_UNSIGNED_SHORT
	// CL_INT
	// CL_UNSIGNED_INT
	// CL_FLOAT
	// CL_2_BYTES
	// CL_3_BYTES
	// CL_4_BYTES

	// LogicOp:

	CL_CLEAR                        = 0x1500,
	CL_AND                          = 0x1501,
	CL_AND_REVERSE                  = 0x1502,
	CL_COPY                         = 0x1503,
	CL_AND_INVERTED                 = 0x1504,
	CL_NOOP                         = 0x1505,
	CL_XOR                          = 0x1506,
	CL_OR                           = 0x1507,
	CL_NOR                          = 0x1508,
	CL_EQUIV                        = 0x1509,
	CL_INVERT                       = 0x150A,
	CL_OR_REVERSE                   = 0x150B,
	CL_COPY_INVERTED                = 0x150C,
	CL_OR_INVERTED                  = 0x150D,
	CL_NAND                         = 0x150E,
	CL_SET                          = 0x150F,

	// MapTarget:

	// CL_MAP1_COLOR_4
	// CL_MAP1_INDEX
	// CL_MAP1_NORMAL
	// CL_MAP1_TEXTURE_COORD_1
	// CL_MAP1_TEXTURE_COORD_2
	// CL_MAP1_TEXTURE_COORD_3
	// CL_MAP1_TEXTURE_COORD_4
	// CL_MAP1_VERTEX_3
	// CL_MAP1_VERTEX_4
	// CL_MAP2_COLOR_4
	// CL_MAP2_INDEX
	// CL_MAP2_NORMAL
	// CL_MAP2_TEXTURE_COORD_1
	// CL_MAP2_TEXTURE_COORD_2
	// CL_MAP2_TEXTURE_COORD_3
	// CL_MAP2_TEXTURE_COORD_4
	// CL_MAP2_VERTEX_3
	// CL_MAP2_VERTEX_4

	// MaterialFace:

	// CL_FRONT
	// CL_BACK
	// CL_FRONT_AND_BACK

	// MaterialParameter:

	CL_EMISSION                     = 0x1600,
	CL_SHININESS                    = 0x1601,
	CL_AMBIENT_AND_DIFFUSE          = 0x1602,
	CL_COLOR_INDEXES                = 0x1603,
	// CL_AMBIENT
	// CL_DIFFUSE
	// CL_SPECULAR

	// MatrixMode:

	CL_MODELVIEW                    = 0x1700,
	CL_PROJECTION                   = 0x1701,
	CL_TEXTURE                      = 0x1702,

	// MeshMode1:

	// CL_POINT
	// CL_LINE

	// MeshMode2:

	// CL_POINT
	// CL_LINE
	// CL_FILL

	// MinmaxTarget:

	// CL_MINMAX

	// NormalPointerType:

	// CL_BYTE
	// CL_SHORT
	// CL_INT
	// CL_FLOAT
	// CL_DOUBLE

	// PixelCopyType:

	CL_COLOR                        = 0x1800,
	CL_DEPTH                        = 0x1801,
	CL_STENCIL                      = 0x1802,

	// PixelFormat:

	CL_COLOR_INDEX                  = 0x1900,
	CL_STENCIL_INDEX                = 0x1901,
	CL_DEPTH_COMPONENT              = 0x1902,
	CL_RED                          = 0x1903,
	CL_GREEN                        = 0x1904,
	CL_BLUE                         = 0x1905,
	CL_ALPHA                        = 0x1906,
	CL_RGB                          = 0x1907,
	CL_RGBA                         = 0x1908,
	CL_LUMINANCE                    = 0x1909,
	CL_LUMINANCE_ALPHA              = 0x190A,
	// CL_ABGR

	// PixelInternalFormat:

	// CL_ALPHA4
	// CL_ALPHA8
	// CL_ALPHA12
	// CL_ALPHA16
	// CL_LUMINANCE4
	// CL_LUMINANCE8
	// CL_LUMINANCE12
	// CL_LUMINANCE16
	// CL_LUMINANCE4_ALPHA4
	// CL_LUMINANCE6_ALPHA2
	// CL_LUMINANCE8_ALPHA8
	// CL_LUMINANCE12_ALPHA4
	// CL_LUMINANCE12_ALPHA12
	// CL_LUMINANCE16_ALPHA16
	// CL_INTENSITY
	// CL_INTENSITY4
	// CL_INTENSITY8
	// CL_INTENSITY12
	// CL_INTENSITY16
	// CL_R3_G3_B2
	// CL_RGB4
	// CL_RGB5
	// CL_RGB8
	// CL_RGB10
	// CL_RGB12
	// CL_RGB16
	// CL_RGBA2
	// CL_RGBA4
	// CL_RGB5_A1
	// CL_RGBA8
	// CL_RGB10_A2
	// CL_RGBA12
	// CL_RGBA16

	// PixelMap:

	// CL_PIXEL_MAP_I_TO_I
	// CL_PIXEL_MAP_S_TO_S
	// CL_PIXEL_MAP_I_TO_R
	// CL_PIXEL_MAP_I_TO_G
	// CL_PIXEL_MAP_I_TO_B
	// CL_PIXEL_MAP_I_TO_A
	// CL_PIXEL_MAP_R_TO_R
	// CL_PIXEL_MAP_G_TO_G
	// CL_PIXEL_MAP_B_TO_B
	// CL_PIXEL_MAP_A_TO_A

	// PixelStore:

	// CL_UNPACK_SWAP_BYTES
	// CL_UNPACK_LSB_FIRST
	// CL_UNPACK_ROW_LENGTH
	// CL_UNPACK_SKIP_ROWS
	// CL_UNPACK_SKIP_PIXELS
	// CL_UNPACK_ALIGNMENT
	// CL_PACK_SWAP_BYTES
	// CL_PACK_LSB_FIRST
	// CL_PACK_ROW_LENGTH
	// CL_PACK_SKIP_ROWS
	// CL_PACK_SKIP_PIXELS
	// CL_PACK_ALIGNMENT
	// CL_PACK_SKIP_IMAGES
	// CL_PACK_IMAGE_HEIGHT
	// CL_UNPACK_SKIP_IMAGES
	// CL_UNPACK_IMAGE_HEIGHT

	// PixelTransfer:

	// CL_MAP_COLOR
	// CL_MAP_STENCIL
	// CL_INDEX_SHIFT
	// CL_INDEX_OFFSET
	// CL_RED_SCALE
	// CL_RED_BIAS
	// CL_GREEN_SCALE
	// CL_GREEN_BIAS
	// CL_BLUE_SCALE
	// CL_BLUE_BIAS
	// CL_ALPHA_SCALE
	// CL_ALPHA_BIAS
	// CL_DEPTH_SCALE
	// CL_DEPTH_BIAS
	// CL_POST_CONVOLUTION_RED_SCALE
	// CL_POST_CONVOLUTION_GREEN_SCALE
	// CL_POST_CONVOLUTION_BLUE_SCALE
	// CL_POST_CONVOLUTION_ALPHA_SCALE
	// CL_POST_CONVOLUTION_RED_BIAS
	// CL_POST_CONVOLUTION_GREEN_BIAS
	// CL_POST_CONVOLUTION_BLUE_BIAS
	// CL_POST_CONVOLUTION_ALPHA_BIAS
	// CL_POST_COLOR_MATRIX_RED_SCALE
	// CL_POST_COLOR_MATRIX_GREEN_SCALE
	// CL_POST_COLOR_MATRIX_BLUE_SCALE
	// CL_POST_COLOR_MATRIX_ALPHA_SCALE
	// CL_POST_COLOR_MATRIX_RED_BIAS
	// CL_POST_COLOR_MATRIX_GREEN_BIAS
	// CL_POST_COLOR_MATRIX_BLUE_BIAS
	// CL_POST_COLOR_MATRIX_ALPHA_BIAS

	// PixelType:

	CL_BITMAP                       = 0x1A00,
	// CL_BYTE
	// CL_UNSIGNED_BYTE
	// CL_SHORT
	// CL_UNSIGNED_SHORT
	// CL_INT
	// CL_UNSIGNED_INT
	// CL_FLOAT
	// CL_BGR
	// CL_BGRA
	// CL_UNSIGNED_BYTE_3_3_2
	// CL_UNSIGNED_SHORT_4_4_4_4
	// CL_UNSIGNED_SHORT_5_5_5_1
	// CL_UNSIGNED_INT_8_8_8_8
	// CL_UNSIGNED_INT_10_10_10_2
	// CL_UNSIGNED_SHORT_5_6_5
	// CL_UNSIGNED_BYTE_2_3_3_REV
	// CL_UNSIGNED_SHORT_5_6_5_REV
	// CL_UNSIGNED_SHORT_4_4_4_4_REV
	// CL_UNSIGNED_SHORT_1_5_5_5_REV
	// CL_UNSIGNED_INT_8_8_8_8_REV
	// CL_UNSIGNED_INT_2_10_10_10_REV

	// PolygonMode:

	CL_POINT                        = 0x1B00,
	CL_LINE                         = 0x1B01,
	CL_FILL                         = 0x1B02,

	// ReadBufferMode:

	// CL_FRONT_LEFT
	// CL_FRONT_RIGHT
	// CL_BACK_LEFT
	// CL_BACK_RIGHT
	// CL_FRONT
	// CL_BACK
	// CL_LEFT
	// CL_RIGHT
	// CL_AUX0
	// CL_AUX1
	// CL_AUX2
	// CL_AUX3

	// RenderingMode:

	CL_RENDER                       = 0x1C00,
	CL_FEEDBACK                     = 0x1C01,
	CL_SELECT                       = 0x1C02,

	// SeparableTarget:

	// CL_SEPARABLE_2D

	// ShadingModel:

	CL_FLAT                         = 0x1D00,
	CL_SMOOTH                       = 0x1D01,

	// StencilFunction:

	// CL_NEVER
	// CL_LESS
	// CL_EQUAL
	// CL_LEQUAL
	// CL_GREATER
	// CL_NOTEQUAL
	// CL_GEQUAL
	// CL_ALWAYS

	// StencilOp:

	// CL_ZERO
	CL_KEEP                         = 0x1E00,
	CL_REPLACE                      = 0x1E01,
	CL_INCR                         = 0x1E02,
	CL_DECR                         = 0x1E03,
	// CL_INVERT

	// StringName:

	CL_VENDOR                       = 0x1F00,
	CL_RENDERER                     = 0x1F01,
	CL_VERSION                      = 0x1F02,
	CL_EXTENSIONS                   = 0x1F03,

	// TextureCoordName:

	CL_S                            = 0x2000,
	CL_T                            = 0x2001,
	CL_R                            = 0x2002,
	CL_Q                            = 0x2003,

	// TexCoordPointerType:

	// CL_SHORT
	// CL_INT
	// CL_FLOAT
	// CL_DOUBLE

	// TextureEnvMode:

	CL_MODULATE                     = 0x2100,
	CL_DECAL                        = 0x2101,
	// CL_BLEND
	// CL_REPLACE

	// TextureEnvParameter:

	CL_TEXTURE_ENV_MODE             = 0x2200,
	CL_TEXTURE_ENV_COLOR            = 0x2201,

	// TextureEnvTarget:

	CL_TEXTURE_ENV                  = 0x2300,

	// TextureGenMode:

	CL_EYE_LINEAR                   = 0x2400,
	CL_OBJECT_LINEAR                = 0x2401,
	CL_SPHERE_MAP                   = 0x2402,

	// TextureGenParameter:

	CL_TEXTURE_GEN_MODE             = 0x2500,
	CL_OBJECT_PLANE                 = 0x2501,
	CL_EYE_PLANE                    = 0x2502,

	// TextureMagFilter:

	CL_NEAREST                      = 0x2600,
	CL_LINEAR                       = 0x2601,

	// TextureMinFilter:

	// CL_NEAREST
	// CL_LINEAR
	CL_NEAREST_MIPMAP_NEAREST       = 0x2700,
	CL_LINEAR_MIPMAP_NEAREST        = 0x2701,
	CL_NEAREST_MIPMAP_LINEAR        = 0x2702,
	CL_LINEAR_MIPMAP_LINEAR         = 0x2703,

	// TextureParameterName:

	CL_TEXTURE_MAG_FILTER           = 0x2800,
	CL_TEXTURE_MIN_FILTER           = 0x2801,
	CL_TEXTURE_WRAP_S               = 0x2802,
	CL_TEXTURE_WRAP_T               = 0x2803,
	// CL_TEXTURE_BORDER_COLOR
	// CL_TEXTURE_PRIORITY
	// CL_TEXTURE_WRAP_R
	// CL_TEXTURE_MIN_LOD
	// CL_TEXTURE_MAX_LOD
	// CL_TEXTURE_BASE_LEVEL
	// CL_TEXTURE_MAX_LEVEL

	// TextureTarget:

	// CL_TEXTURE_1D
	// CL_TEXTURE_2D
	// CL_PROXY_TEXTURE_1D
	// CL_PROXY_TEXTURE_2D
	// CL_TEXTURE_3D
	// CL_PROXY_TEXTURE_3D

	// TextureUnit:

	// CL_TEXTURE0
	// CL_TEXTURE1
	// CL_TEXTURE2
	// CL_TEXTURE3
	// CL_TEXTURE4
	// CL_TEXTURE5
	// CL_TEXTURE6
	// CL_TEXTURE7
	// CL_TEXTURE8
	// CL_TEXTURE9
	// CL_TEXTURE10
	// CL_TEXTURE11
	// CL_TEXTURE12
	// CL_TEXTURE13
	// CL_TEXTURE14
	// CL_TEXTURE15
	// CL_TEXTURE16
	// CL_TEXTURE17
	// CL_TEXTURE18
	// CL_TEXTURE19
	// CL_TEXTURE20
	// CL_TEXTURE21
	// CL_TEXTURE22
	// CL_TEXTURE23
	// CL_TEXTURE24
	// CL_TEXTURE25
	// CL_TEXTURE26
	// CL_TEXTURE27
	// CL_TEXTURE28
	// CL_TEXTURE29
	// CL_TEXTURE30
	// CL_TEXTURE31

	// TextureWrapMode:

	CL_CLAMP                        = 0x2900,
	CL_REPEAT                       = 0x2901,
	// CL_CLAMP_TO_EDGE

	// VertexPointerType:

	// CL_SHORT
	// CL_INT
	// CL_FLOAT
	// CL_DOUBLE

	// ClientAttribMask:

	CL_CLIENT_PIXEL_STORE_BIT       = 0x00000001,
	CL_CLIENT_VERTEX_ARRAY_BIT      = 0x00000002,
	CL_CLIENT_ALL_ATTRIB_BITS       = 0xffffffff,

	// polygon_offset:

	CL_POLYGON_OFFSET_FACTOR        = 0x8038,
	CL_POLYGON_OFFSET_UNITS         = 0x2A00,
	CL_POLYGON_OFFSET_POINT         = 0x2A01,
	CL_POLYGON_OFFSET_LINE          = 0x2A02,
	CL_POLYGON_OFFSET_FILL          = 0x8037,

	// texture:

	CL_ALPHA4                       = 0x803B,
	CL_ALPHA8                       = 0x803C,
	CL_ALPHA12                      = 0x803D,
	CL_ALPHA16                      = 0x803E,
	CL_LUMINANCE4                   = 0x803F,
	CL_LUMINANCE8                   = 0x8040,
	CL_LUMINANCE12                  = 0x8041,
	CL_LUMINANCE16                  = 0x8042,
	CL_LUMINANCE4_ALPHA4            = 0x8043,
	CL_LUMINANCE6_ALPHA2            = 0x8044,
	CL_LUMINANCE8_ALPHA8            = 0x8045,
	CL_LUMINANCE12_ALPHA4           = 0x8046,
	CL_LUMINANCE12_ALPHA12          = 0x8047,
	CL_LUMINANCE16_ALPHA16          = 0x8048,
	CL_INTENSITY                    = 0x8049,
	CL_INTENSITY4                   = 0x804A,
	CL_INTENSITY8                   = 0x804B,
	CL_INTENSITY12                  = 0x804C,
	CL_INTENSITY16                  = 0x804D,
	CL_R3_G3_B2                     = 0x2A10,
	CL_RGB4                         = 0x804F,
	CL_RGB5                         = 0x8050,
	CL_RGB8                         = 0x8051,
	CL_RGB10                        = 0x8052,
	CL_RGB12                        = 0x8053,
	CL_RGB16                        = 0x8054,
	CL_RGBA2                        = 0x8055,
	CL_RGBA4                        = 0x8056,
	CL_RGB5_A1                      = 0x8057,
	CL_RGBA8                        = 0x8058,
	CL_RGB10_A2                     = 0x8059,
	CL_RGBA12                       = 0x805A,
	CL_RGBA16                       = 0x805B,
	CL_TEXTURE_RED_SIZE             = 0x805C,
	CL_TEXTURE_GREEN_SIZE           = 0x805D,
	CL_TEXTURE_BLUE_SIZE            = 0x805E,
	CL_TEXTURE_ALPHA_SIZE           = 0x805F,
	CL_TEXTURE_LUMINANCE_SIZE       = 0x8060,
	CL_TEXTURE_INTENSITY_SIZE       = 0x8061,
	CL_PROXY_TEXTURE_1D             = 0x8063,
	CL_PROXY_TEXTURE_2D             = 0x8064,

	// texture_object:

	CL_TEXTURE_PRIORITY             = 0x8066,
	CL_TEXTURE_RESIDENT             = 0x8067,
	CL_TEXTURE_BINDING_1D           = 0x8068,
	CL_TEXTURE_1D_BINDING           = 0x8068,
	CL_TEXTURE_BINDING_2D           = 0x8069,
	CL_TEXTURE_2D_BINDING           = 0x8069,
	CL_TEXTURE_BINDING_3D           = 0x806A,
	CL_TEXTURE_3D_BINDING           = 0x806A,

	// vertex_array:
	
	CL_VERTEX_ARRAY                 = 0x8074,
	CL_NORMAL_ARRAY                 = 0x8075,
	CL_COLOR_ARRAY                  = 0x8076,
	CL_INDEX_ARRAY                  = 0x8077,
	CL_TEXTURE_COORD_ARRAY          = 0x8078,
	CL_EDGE_FLAG_ARRAY              = 0x8079,
	CL_VERTEX_ARRAY_SIZE            = 0x807A,
	CL_VERTEX_ARRAY_TYPE            = 0x807B,
	CL_VERTEX_ARRAY_STRIDE          = 0x807C,
	CL_NORMAL_ARRAY_TYPE            = 0x807E,
	CL_NORMAL_ARRAY_STRIDE          = 0x807F,
	CL_COLOR_ARRAY_SIZE             = 0x8081,
	CL_COLOR_ARRAY_TYPE             = 0x8082,
	CL_COLOR_ARRAY_STRIDE           = 0x8083,
	CL_INDEX_ARRAY_TYPE             = 0x8085,
	CL_INDEX_ARRAY_STRIDE           = 0x8086,
	CL_TEXTURE_COORD_ARRAY_SIZE     = 0x8088,
	CL_TEXTURE_COORD_ARRAY_TYPE     = 0x8089,
	CL_TEXTURE_COORD_ARRAY_STRIDE   = 0x808A,
	CL_EDGE_FLAG_ARRAY_STRIDE       = 0x808C,
	CL_VERTEX_ARRAY_POINTER         = 0x808E,
	CL_NORMAL_ARRAY_POINTER         = 0x808F,
	CL_COLOR_ARRAY_POINTER          = 0x8090,
	CL_INDEX_ARRAY_POINTER          = 0x8091,
	CL_TEXTURE_COORD_ARRAY_POINTER  = 0x8092,
	CL_EDGE_FLAG_ARRAY_POINTER      = 0x8093,
	CL_V2F                          = 0x2A20,
	CL_V3F                          = 0x2A21,
	CL_C4UB_V2F                     = 0x2A22,
	CL_C4UB_V3F                     = 0x2A23,
	CL_C3F_V3F                      = 0x2A24,
	CL_N3F_V3F                      = 0x2A25,
	CL_C4F_N3F_V3F                  = 0x2A26,
	CL_T2F_V3F                      = 0x2A27,
	CL_T4F_V4F                      = 0x2A28,
	CL_T2F_C4UB_V3F                 = 0x2A29,
	CL_T2F_C3F_V3F                  = 0x2A2A,
	CL_T2F_N3F_V3F                  = 0x2A2B,
	CL_T2F_C4F_N3F_V3F              = 0x2A2C,
	CL_T4F_C4F_N3F_V4F              = 0x2A2D,

	// bgra

	CL_BGR                          = 0x80E0,
	CL_BGRA                         = 0x80E1,

	// blend_color

	CL_CONSTANT_COLOR               = 0x8001,
	CL_ONE_MINUS_CONSTANT_COLOR     = 0x8002,
	CL_CONSTANT_ALPHA               = 0x8003,
	CL_ONE_MINUS_CONSTANT_ALPHA     = 0x8004,
	CL_BLEND_COLOR                  = 0x8005,

	// blend_minmax

	CL_FUNC_ADD                     = 0x8006,
	CL_MIN                          = 0x8007,
	CL_MAX                          = 0x8008,
	CL_BLEND_EQUATION               = 0x8009,

	// blend_subtract

	CL_FUNC_SUBTRACT                = 0x800A,
	CL_FUNC_REVERSE_SUBTRACT        = 0x800B,

	// color_matrix

	CL_COLOR_MATRIX                 = 0x80B1,
	CL_COLOR_MATRIX_STACK_DEPTH     = 0x80B2,
	CL_MAX_COLOR_MATRIX_STACK_DEPTH = 0x80B3,
	CL_POST_COLOR_MATRIX_RED_SCALE  = 0x80B4,
	CL_POST_COLOR_MATRIX_GREEN_SCALE= 0x80B5,
	CL_POST_COLOR_MATRIX_BLUE_SCALE = 0x80B6,
	CL_POST_COLOR_MATRIX_ALPHA_SCALE= 0x80B7,
	CL_POST_COLOR_MATRIX_RED_BIAS   = 0x80B8,
	CL_POST_COLOR_MATRIX_GREEN_BIAS = 0x80B9,
	CL_POST_COLOR_MATRIX_BLUE_BIAS  = 0x80BA,
	CL_POST_COLOR_MATRIX_ALPHA_BIAS = 0x80BB,

	// color_table

	CL_COLOR_TABLE                  = 0x80D0,
	CL_POST_CONVOLUTION_COLOR_TABLE = 0x80D1,
	CL_POST_COLOR_MATRIX_COLOR_TABLE= 0x80D2,
	CL_PROXY_COLOR_TABLE            = 0x80D3,
	CL_PROXY_POST_CONVOLUTION_COLOR_TABLE = 0x80D4,
	CL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE = 0x80D5,
	CL_COLOR_TABLE_SCALE            = 0x80D6,
	CL_COLOR_TABLE_BIAS             = 0x80D7,
	CL_COLOR_TABLE_FORMAT           = 0x80D8,
	CL_COLOR_TABLE_WIDTH            = 0x80D9,
	CL_COLOR_TABLE_RED_SIZE         = 0x80DA,
	CL_COLOR_TABLE_GREEN_SIZE       = 0x80DB,
	CL_COLOR_TABLE_BLUE_SIZE        = 0x80DC,
	CL_COLOR_TABLE_ALPHA_SIZE       = 0x80DD,
	CL_COLOR_TABLE_LUMINANCE_SIZE   = 0x80DE,
	CL_COLOR_TABLE_INTENSITY_SIZE   = 0x80DF,

	// convolution

	CL_CONVOLUTION_1D               = 0x8010,
	CL_CONVOLUTION_2D               = 0x8011,
	CL_SEPARABLE_2D                 = 0x8012,
	CL_CONVOLUTION_BORDER_MODE      = 0x8013,
	CL_CONVOLUTION_FILTER_SCALE     = 0x8014,
	CL_CONVOLUTION_FILTER_BIAS      = 0x8015,
	CL_REDUCE                       = 0x8016,
	CL_CONVOLUTION_FORMAT           = 0x8017,
	CL_CONVOLUTION_WIDTH            = 0x8018,
	CL_CONVOLUTION_HEIGHT           = 0x8019,
	CL_MAX_CONVOLUTION_WIDTH        = 0x801A,
	CL_MAX_CONVOLUTION_HEIGHT       = 0x801B,
	CL_POST_CONVOLUTION_RED_SCALE   = 0x801C,
	CL_POST_CONVOLUTION_GREEN_SCALE = 0x801D,
	CL_POST_CONVOLUTION_BLUE_SCALE  = 0x801E,
	CL_POST_CONVOLUTION_ALPHA_SCALE = 0x801F,
	CL_POST_CONVOLUTION_RED_BIAS    = 0x8020,
	CL_POST_CONVOLUTION_GREEN_BIAS  = 0x8021,
	CL_POST_CONVOLUTION_BLUE_BIAS   = 0x8022,
	CL_POST_CONVOLUTION_ALPHA_BIAS  = 0x8023,
	CL_CONSTANT_BORDER              = 0x8151,
	CL_REPLICATE_BORDER             = 0x8153,
	CL_CONVOLUTION_BORDER_COLOR     = 0x8154,

	// draw_range_elements

	CL_MAX_ELEMENTS_VERTICES        = 0x80E8,
	CL_MAX_ELEMENTS_INDICES         = 0x80E9,

	// histogram

	CL_HISTOGRAM                    = 0x8024,
	CL_PROXY_HISTOGRAM              = 0x8025,
	CL_HISTOGRAM_WIDTH              = 0x8026,
	CL_HISTOGRAM_FORMAT             = 0x8027,
	CL_HISTOGRAM_RED_SIZE           = 0x8028,
	CL_HISTOGRAM_GREEN_SIZE         = 0x8029,
	CL_HISTOGRAM_BLUE_SIZE          = 0x802A,
	CL_HISTOGRAM_ALPHA_SIZE         = 0x802B,
	CL_HISTOGRAM_LUMINANCE_SIZE     = 0x802C,
	CL_HISTOGRAM_SINK               = 0x802D,
	CL_MINMAX                       = 0x802E,
	CL_MINMAX_FORMAT                = 0x802F,
	CL_MINMAX_SINK                  = 0x8030,
	CL_TABLE_TOO_LARGE              = 0x8031,

	// packed_pixels

	CL_UNSIGNED_BYTE_3_3_2          = 0x8032,
	CL_UNSIGNED_SHORT_4_4_4_4       = 0x8033,
	CL_UNSIGNED_SHORT_5_5_5_1       = 0x8034,
	CL_UNSIGNED_INT_8_8_8_8         = 0x8035,
	CL_UNSIGNED_INT_10_10_10_2      = 0x8036,
	CL_UNSIGNED_BYTE_2_3_3_REV      = 0x8362,
	CL_UNSIGNED_SHORT_5_6_5         = 0x8363,
	CL_UNSIGNED_SHORT_5_6_5_REV     = 0x8364,
	CL_UNSIGNED_SHORT_4_4_4_4_REV   = 0x8365,
	CL_UNSIGNED_SHORT_1_5_5_5_REV   = 0x8366,
	CL_UNSIGNED_INT_8_8_8_8_REV     = 0x8367,
	CL_UNSIGNED_INT_2_10_10_10_REV  = 0x8368,

	CL_MIRRORED_REPEAT              = 0x8370,

	// rescale_normal

	CL_RESCALE_NORMAL               = 0x803A,

	// separate_specular_color

	CL_LIGHT_MODEL_COLOR_CONTROL    = 0x81F8,
	CL_SINGLE_COLOR                 = 0x81F9,
	CL_SEPARATE_SPECULAR_COLOR      = 0x81FA,

	// texture3D

	CL_PACK_SKIP_IMAGES             = 0x806B,
	CL_PACK_IMAGE_HEIGHT            = 0x806C,
	CL_UNPACK_SKIP_IMAGES           = 0x806D,
	CL_UNPACK_IMAGE_HEIGHT          = 0x806E,
	CL_TEXTURE_3D                   = 0x806F,
	CL_PROXY_TEXTURE_3D             = 0x8070,
	CL_TEXTURE_DEPTH                = 0x8071,
	CL_TEXTURE_WRAP_R               = 0x8072,
	CL_MAX_3D_TEXTURE_SIZE          = 0x8073,

	// texture_edge_clamp

	CL_CLAMP_TO_BORDER              = 0x812D,
	CL_CLAMP_TO_EDGE                = 0x812F,

	// texture_lod

	CL_TEXTURE_MIN_LOD              = 0x813A,
	CL_TEXTURE_MAX_LOD              = 0x813B,
	CL_TEXTURE_BASE_LEVEL           = 0x813C,
	CL_TEXTURE_MAX_LEVEL            = 0x813D,

	// GetTarget1_2:

	CL_SMOOTH_POINT_SIZE_RANGE      = 0x0B12,
	CL_SMOOTH_POINT_SIZE_GRANULARITY= 0x0B13,
	CL_SMOOTH_LINE_WIDTH_RANGE      = 0x0B22,
	CL_SMOOTH_LINE_WIDTH_GRANULARITY= 0x0B23,
	CL_ALIASED_POINT_SIZE_RANGE     = 0x846D,
	CL_ALIASED_LINE_WIDTH_RANGE     = 0x846E,

	// multitexture:

	CL_TEXTURE0                 = 0x84C0,
	CL_TEXTURE1                 = 0x84C1,
	CL_TEXTURE2                 = 0x84C2,
	CL_TEXTURE3                 = 0x84C3,
	CL_TEXTURE4                 = 0x84C4,
	CL_TEXTURE5                 = 0x84C5,
	CL_TEXTURE6                 = 0x84C6,
	CL_TEXTURE7                 = 0x84C7,
	CL_TEXTURE8                 = 0x84C8,
	CL_TEXTURE9                 = 0x84C9,
	CL_TEXTURE10                = 0x84CA,
	CL_TEXTURE11                = 0x84CB,
	CL_TEXTURE12                = 0x84CC,
	CL_TEXTURE13                = 0x84CD,
	CL_TEXTURE14                = 0x84CE,
	CL_TEXTURE15                = 0x84CF,
	CL_TEXTURE16                = 0x84D0,
	CL_TEXTURE17                = 0x84D1,
	CL_TEXTURE18                = 0x84D2,
	CL_TEXTURE19                = 0x84D3,
	CL_TEXTURE20                = 0x84D4,
	CL_TEXTURE21                = 0x84D5,
	CL_TEXTURE22                = 0x84D6,
	CL_TEXTURE23                = 0x84D7,
	CL_TEXTURE24                = 0x84D8,
	CL_TEXTURE25                = 0x84D9,
	CL_TEXTURE26                = 0x84DA,
	CL_TEXTURE27                = 0x84DB,
	CL_TEXTURE28                = 0x84DC,
	CL_TEXTURE29                = 0x84DD,
	CL_TEXTURE30                = 0x84DE,
	CL_TEXTURE31                = 0x84DF,
	CL_ACTIVE_TEXTURE           = 0x84E0,
	CL_CLIENT_ACTIVE_TEXTURE    = 0x84E1,
	CL_MAX_TEXTURE_UNITS        = 0x84E2,

	// EXT_abgr:

	CL_ABGR                     = 0x8000,

	// EXT_blend_color:

	// CL_CONSTANT_COLOR           = 0x8001,
	// CL_ONE_MINUS_CONSTANT_COLOR = 0x8002,
	// CL_CONSTANT_ALPHA           = 0x8003,
	// CL_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
	// CL_BLEND_COLOR              = 0x8005,

	// EXT_blend_minmax:

	// CL_FUNC_ADD                 = 0x8006,
	// CL_MIN                      = 0x8007,
	// CL_MAX                      = 0x8008,
	// CL_BLEND_EQUATION           = 0x8009,

	// EXT_blend_subtract:

	// CL_FUNC_SUBTRACT            = 0x800A,
	// CL_FUNC_REVERSE_SUBTRACT    = 0x800B,

	// EXT_texture_env_combine:

	CL_COMBINE                  = 0x8570,
	CL_COMBINE_RGB              = 0x8571,
	CL_COMBINE_ALPHA            = 0x8572,
	CL_RGB_SCALE                = 0x8573,
	CL_ADD_SIGNED               = 0x8574,
	CL_INTERPOLATE              = 0x8575,
	CL_CONSTANT                 = 0x8576,
	CL_PRIMARY_COLOR            = 0x8577,
	CL_PREVIOUS                 = 0x8578,
	CL_SOURCE0_RGB              = 0x8580,
	CL_SOURCE1_RGB              = 0x8581,
	CL_SOURCE2_RGB              = 0x8582,
	CL_SOURCE0_ALPHA            = 0x8588,
	CL_SOURCE1_ALPHA            = 0x8589,
	CL_SOURCE2_ALPHA            = 0x858A,
	CL_OPERAND0_RGB             = 0x8590,
	CL_OPERAND1_RGB             = 0x8591,
	CL_OPERAND2_RGB             = 0x8592,
	CL_OPERAND0_ALPHA           = 0x8598,
	CL_OPERAND1_ALPHA           = 0x8599,
	CL_OPERAND2_ALPHA           = 0x859A,

	// OpenGL 1.5 token renames:
	CL_SRC0_RGB                 = CL_SOURCE0_RGB,
	CL_SRC1_RGB                 = CL_SOURCE1_RGB,
	CL_SRC2_RGB                 = CL_SOURCE2_RGB,
	CL_SRC0_ALPHA               = CL_SOURCE0_ALPHA,
	CL_SRC1_ALPHA               = CL_SOURCE1_ALPHA,
	CL_SRC2_ALPHA               = CL_SOURCE2_ALPHA,

	// ARB_texture_compression:

	CL_COMPRESSED_ALPHA           = 0x84e9,
	CL_COMPRESSED_LUMINANCE       = 0x84ea,
	CL_COMPRESSED_LUMINANCE_ALPHA = 0x84eb,
	CL_COMPRESSED_INTENSITY       = 0x84ec,
	CL_COMPRESSED_RGB             = 0x84ed,
	CL_COMPRESSED_RGBA            = 0x84ee,
	CL_TEXTURE_COMPRESSION_HINT   = 0x84ef,
	CL_TEXTURE_COMPRESSED_IMAGE_SIZE = 0x86a0,
	CL_TEXTURE_COMPRESSED         = 0x86a1,
	CL_NUM_COMPRESSED_TEXTURE_FORMATS = 0x86a2,
	CL_COMPRESSED_TEXTURE_FORMATS = 0x86a3,

	// ARB_texture_cube_map:

	CL_NORMAL_MAP                 = 0x8511,
	CL_REFLECTION_MAP             = 0x8512,
	CL_TEXTURE_CUBE_MAP           = 0x8513,
	CL_TEXTURE_BINDING_CUBE_MAP   = 0x8514,
	CL_TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515,
	CL_TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516,
	CL_TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517,
	CL_TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518,
	CL_TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519,
	CL_TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851a,
	CL_PROXY_TEXTURE_CUBE_MAP     = 0x851b,
	CL_MAX_CUBE_MAP_TEXTURE_SIZE  = 0x851c,

	// ARB_multisample:

	CL_MULTISAMPLE                = 0x809d,
	CL_SAMPLE_ALPHA_TO_COVERAGE   = 0x809e,
	CL_SAMPLE_ALPHA_TO_ONE        = 0x809f,
	CL_SAMPLE_COVERAGE            = 0x80a0,
	CL_SAMPLE_BUFFERS             = 0x80a8,
	CL_SAMPLES                    = 0x80a9,
	CL_SAMPLE_COVERAGE_VALUE      = 0x80aa,
	CL_SAMPLE_COVERAGE_INVERT     = 0x80ab,
	CL_MULTISAMPLE_BIT            = 0x20000000,

	// ARB_texture_env_combine:

	CL_SUBTRACT                   = 0x84e7,
	// CL_COMBINE                    = 0x8570,
	// CL_COMBINE_RGB                = 0x8571,
	// CL_COMBINE_ALPHA              = 0x8572,
	// CL_RGB_SCALE                  = 0x8573,
	// CL_ADD_SIGNED                 = 0x8574,
	// CL_INTERPOLATE                = 0x8575,
	// CL_CONSTANT                   = 0x8576,
	// CL_PRIMARY_COLOR              = 0x8577,
	// CL_PREVIOUS                   = 0x8578,
	// CL_SOURCE0_RGB                = 0x8580,
	// CL_SOURCE1_RGB                = 0x8581,
	// CL_SOURCE2_RGB                = 0x8582,
	// CL_SOURCE0_ALPHA              = 0x8588,
	// CL_SOURCE1_ALPHA              = 0x8589,
	// CL_SOURCE2_ALPHA              = 0x858a,
	// CL_OPERAND0_RGB               = 0x8590,
	// CL_OPERAND1_RGB               = 0x8591,
	// CL_OPERAND2_RGB               = 0x8592,
	// CL_OPERAND0_ALPHA             = 0x8598,
	// CL_OPERAND1_ALPHA             = 0x8599,
	// CL_OPERAND2_ALPHA             = 0x859a,

	// ARB_texture_env_dot3:

	CL_DOT3_RGB                   = 0x86ae,
	CL_DOT3_RGBA                  = 0x86af,

	// ARB_transpose_matrix:

	CL_TRANSPOSE_MODELVIEW_MATRIX  = 0x84e3,
	CL_TRANSPOSE_PROJECTION_MATRIX = 0x84e4,
	CL_TRANSPOSE_TEXTURE_MATRIX    = 0x84e5,
	CL_TRANSPOSE_COLOR_MATRIX      = 0x84e6,

	// SGIS_generate_mipmap:

	CL_GENERATE_MIPMAP             = 0x8191,
	CL_GENERATE_MIPMAP_HINT        = 0x8192,

	// ARB_depth_texture:

	CL_DEPTH_COMPONENT16           = 0x81a5,
	CL_DEPTH_COMPONENT24           = 0x81a6,
	CL_DEPTH_COMPONENT32           = 0x81a7,
	CL_TEXTURE_DEPTH_SIZE          = 0x884a,
	CL_DEPTH_TEXTURE_MODE          = 0x884b,

	// ARB_shadow:

	CL_TEXTURE_COMPARE_MODE        = 0x884c,
	CL_TEXTURE_COMPARE_FUNC        = 0x884d,
	CL_COMPARE_R_TO_TEXTURE        = 0x884e,

	// EXT_fog_coord:

	CL_FOG_COORDINATE_SOURCE       = 0x8450,
	CL_FOG_COORDINATE              = 0x8451,
	CL_FRAGMENT_DEPTH              = 0x8452,
	CL_CURRENT_FOG_COORDINATE      = 0x8453,
	CL_FOG_COORDINATE_ARRAY_TYPE   = 0x8454,
	CL_FOG_COORDINATE_ARRAY_STRIDE = 0x8455,
	CL_FOG_COORDINATE_ARRAY_POINTER = 0x8456,
	CL_FOG_COORDINATE_ARRAY        = 0x8457,
	
	// OpenGL 1.5 token renames:
	CL_FOG_COORD_SRC               = CL_FOG_COORDINATE_SOURCE,
	CL_FOG_COORD                   = CL_FOG_COORDINATE,
	CL_CURRENT_FOG_COORD           = CL_CURRENT_FOG_COORDINATE,
	CL_FOG_COORD_ARRAY_TYPE        = CL_FOG_COORDINATE_ARRAY_TYPE,
	CL_FOG_COORD_ARRAY_STRIDE      = CL_FOG_COORDINATE_ARRAY_STRIDE,
	CL_FOG_COORD_ARRAY_POINTER     = CL_FOG_COORDINATE_ARRAY_POINTER,
	CL_FOG_COORD_ARRAY             = CL_FOG_COORDINATE_ARRAY,
	
	// ARB_point_parameters:

	CL_POINT_SIZE_MIN              = 0x8126,
	CL_POINT_SIZE_MAX_ARB          = 0x8127,
	CL_POINT_FADE_THRESHOLD_SIZE   = 0x8128,
	CL_POINT_DISTANCE_ATTENUATION  = 0x8129,

	// EXT_secondary_color:

	CL_COLOR_SUM                   = 0x8458,
	CL_CURRENT_SECONDARY_COLOR     = 0x8459,
	CL_SECONDARY_COLOR_ARRAY_SIZE  = 0x845a,
	CL_SECONDARY_COLOR_ARRAY_TYPE  = 0x845b,
	CL_SECONDARY_COLOR_ARRAY_STRIDE = 0x845c,
	CL_SECONDARY_COLOR_ARRAY_POINTER = 0x845d,
	CL_SECONDARY_COLOR_ARRAY       = 0x845e,

	// EXT_blend_func_separate:

	CL_BLEND_DST_RGB               = 0x80c8,
	CL_BLEND_SRC_RGB               = 0x80c9,
	CL_BLEND_DST_ALPHA             = 0x80ca,
	CL_BLEND_SRC_ALPHA             = 0x80cb,

	// EXT_stencil_wrap:

	CL_INCR_WRAP                   = 0x8507,
	CL_DECR_WRAP                   = 0x8508,

	// EXT_texture_lod_bias:

	CL_MAX_TEXTURE_LOD_BIAS        = 0x84fd,
	CL_TEXTURE_FILTER_CONTROL      = 0x8500,
	CL_TEXTURE_LOD_BIAS            = 0x8501,

	// ARB_vertex_buffer_object:
	
	CL_ARRAY_BUFFER                        = 0x8892,
	CL_ELEMENT_ARRAY_BUFFER                = 0x8893,
	CL_ARRAY_BUFFER_BINDING                = 0x8894,
	CL_ELEMENT_ARRAY_BUFFER_BINDING        = 0x8895,
	CL_VERTEX_ARRAY_BUFFER_BINDING         = 0x8896,
	CL_NORMAL_ARRAY_BUFFER_BINDING         = 0x8897,
	CL_COLOR_ARRAY_BUFFER_BINDING          = 0x8898,
	CL_INDEX_ARRAY_BUFFER_BINDING          = 0x8899,
	CL_TEXTURE_COORD_ARRAY_BUFFER_BINDING  = 0x889A,
	CL_EDGE_FLAG_ARRAY_BUFFER_BINDING      = 0x889B,
	CL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING = 0x889C,
	CL_FOG_COORDINATE_ARRAY_BUFFER_BINDING = 0x889D,
	CL_FOG_COORD_ARRAY_BUFFER_BINDING      = CL_FOG_COORDINATE_ARRAY_BUFFER_BINDING,
	CL_WEIGHT_ARRAY_BUFFER_BINDING         = 0x889E,
	CL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING  = 0x889F,
	CL_STREAM_DRAW                         = 0x88E0,
	CL_STREAM_READ                         = 0x88E1,
	CL_STREAM_COPY                         = 0x88E2,
	CL_STATIC_DRAW                         = 0x88E4,
	CL_STATIC_READ                         = 0x88E5,
	CL_STATIC_COPY                         = 0x88E6,
	CL_DYNAMIC_DRAW                        = 0x88E8,
	CL_DYNAMIC_READ                        = 0x88E9,
	CL_DYNAMIC_COPY                        = 0x88EA,
	CL_READ_ONLY                           = 0x88B8,
	CL_WRITE_ONLY                          = 0x88B9,
	CL_READ_WRITE                          = 0x88BA,
	CL_BUFFER_SIZE                         = 0x8764,
	CL_BUFFER_USAGE                        = 0x8765,
	CL_BUFFER_ACCESS                       = 0x88BB,
	CL_BUFFER_MAPPED                       = 0x88BC,
	CL_BUFFER_MAP_POINTER                  = 0x88BD,

	// ARB_occlusion_query:

	CL_SAMPLES_PASSED                      = 0x8914,
	CL_QUERY_COUNTER_BITS                  = 0x8864,
	CL_CURRENT_QUERY                       = 0x8865,
	CL_QUERY_RESULT                        = 0x8866,
	CL_QUERY_RESULT_AVAILABLE              = 0x8867,

	// ARB_shader_objects:
	
	CL_PROGRAM_OBJECT                      = 0x8B40,
	CL_TYPE                                = 0x8B4E,
	CL_SUBTYPE                             = 0x8B4F,
	CL_DELETE_STATUS                       = 0x8B80,
	CL_COMPILE_STATUS                      = 0x8B81,
	CL_LINK_STATUS                         = 0x8B82,
	CL_VALIDATE_STATUS                     = 0x8B83,
	CL_INFO_LOG_LENGTH                     = 0x8B84,
	CL_ATTACHED_OBJECTS                    = 0x8B85,
	CL_ACTIVE_UNIFORMS                     = 0x8B86,
	CL_ACTIVE_UNIFORM_MAX_LENGTH           = 0x8B87,
	CL_SHADER_SOURCE_LENGTH                = 0x8B88,
	CL_OBJECT_TYPE                         = CL_TYPE,
	CL_OBJECT_SUBTYPE                      = CL_SUBTYPE,
	CL_OBJECT_DELETE_STATUS                = CL_DELETE_STATUS,
	CL_OBJECT_COMPILE_STATUS               = CL_COMPILE_STATUS,
	CL_OBJECT_LINK_STATUS                  = CL_LINK_STATUS,
	CL_OBJECT_VALIDATE_STATUS              = CL_VALIDATE_STATUS,
	CL_OBJECT_INFO_LOG_LENGTH              = CL_INFO_LOG_LENGTH,
	CL_OBJECT_ATTACHED_OBJECTS             = CL_ATTACHED_OBJECTS,
	CL_OBJECT_ACTIVE_UNIFORMS              = CL_ACTIVE_UNIFORMS,
	CL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH    = CL_ACTIVE_UNIFORM_MAX_LENGTH,
	CL_OBJECT_SHADER_SOURCE_LENGTH         = CL_SHADER_SOURCE_LENGTH,
	CL_SHADER_OBJECT                       = 0x8B48,
	// CL_FLOAT                               = 0x1406,
	// CL_FLOAT_VEC2                          = 0x8B50,
	// CL_FLOAT_VEC3                          = 0x8B51,
	// CL_FLOAT_VEC4                          = 0x8B52,
	// CL_INT                                 = 0x1404,
	CL_INT_VEC2                            = 0x8B53,
	CL_INT_VEC3                            = 0x8B54,
	CL_INT_VEC4                            = 0x8B55,
	CL_BOOL                                = 0x8B56,
	CL_BOOL_VEC2                           = 0x8B57,
	CL_BOOL_VEC3                           = 0x8B58,
	CL_BOOL_VEC4                           = 0x8B59,
	CL_FLOAT_MAT2                          = 0x8B5A,
	CL_FLOAT_MAT3                          = 0x8B5B,
	CL_FLOAT_MAT4                          = 0x8B5C,
	CL_SAMPLER_1D                          = 0x8B5D,
	CL_SAMPLER_2D                          = 0x8B5E,
	CL_SAMPLER_3D                          = 0x8B5F,
	CL_SAMPLER_CUBE                        = 0x8B60,
	CL_SAMPLER_1D_SHADOW                   = 0x8B61,
	CL_SAMPLER_2D_SHADOW                   = 0x8B62,
	CL_SAMPLER_2D_RECT                     = 0x8B63,
	CL_SAMPLER_2D_RECT_SHADOW              = 0x8B64,

	// ARB_vertex_shader:

	CL_VERTEX_SHADER                       = 0x8B31,
	CL_MAX_VERTEX_UNIFORM_COMPONENTS       = 0x8B4A,
	CL_MAX_VARYING_FLOATS                  = 0x8B4B,
	CL_MAX_VERTEX_ATTRIBS                  = 0x8869,
	CL_MAX_TEXTURE_IMAGE_UNITS             = 0x8872,
	CL_MAX_VERTEX_TEXTURE_IMAGE_UNITS      = 0x8B4C,
	CL_MAX_COMBINED_TEXTURE_IMAGE_UNITS    = 0x8B4D,
	CL_MAX_TEXTURE_COORDS                  = 0x8871,
	CL_VERTEX_PROGRAM_POINT_SIZE           = 0x8642,
	CL_VERTEX_PROGRAM_TWO_SIDE             = 0x8643,
	CL_ACTIVE_ATTRIBUTES                   = 0x8B89,
	CL_ACTIVE_ATTRIBUTE_MAX_LENGTH         = 0x8B8A,
	CL_OBJECT_ACTIVE_ATTRIBUTES            = CL_ACTIVE_ATTRIBUTES,
	CL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH  = CL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
	CL_VERTEX_ATTRIB_ARRAY_ENABLED         = 0x8622,
	CL_VERTEX_ATTRIB_ARRAY_SIZE            = 0x8623,
	CL_VERTEX_ATTRIB_ARRAY_STRIDE          = 0x8624,
	CL_VERTEX_ATTRIB_ARRAY_TYPE            = 0x8625,
	CL_VERTEX_ATTRIB_ARRAY_NORMALIZED      = 0x886A,
	CL_CURRENT_VERTEX_ATTRIB               = 0x8626,
	CL_VERTEX_ATTRIB_ARRAY_POINTER         = 0x8645,
	// CL_FLOAT                               = 0x1406,
	CL_FLOAT_VEC2                          = 0x8B50,
	CL_FLOAT_VEC3                          = 0x8B51,
	CL_FLOAT_VEC4                          = 0x8B52,
	// CL_FLOAT_MAT2                          = 0x8B5A,
	// CL_FLOAT_MAT3                          = 0x8B5B,
	// CL_FLOAT_MAT4                          = 0x8B5C,

	// ARB_fragment_shader:

	CL_FRAGMENT_SHADER                     = 0x8B30,
	CL_MAX_FRAGMENT_UNIFORM_COMPONENTS     = 0x8B49,
	// CL_MAX_TEXTURE_COORDS                  = 0x8871,
	// CL_MAX_TEXTURE_IMAGE_UNITS             = 0x8872,
	CL_FRAGMENT_SHADER_DERIVATIVE_HINT     = 0x8B8B,

	// ARB_draw_buffers:

	CL_MAX_DRAW_BUFFERS                    = 0x8824,
	CL_DRAW_BUFFER0                        = 0x8825,
	CL_DRAW_BUFFER1                        = 0x8826,
	CL_DRAW_BUFFER2                        = 0x8827,
	CL_DRAW_BUFFER3                        = 0x8828,
	CL_DRAW_BUFFER4                        = 0x8829,
	CL_DRAW_BUFFER5                        = 0x882A,
	CL_DRAW_BUFFER6                        = 0x882B,
	CL_DRAW_BUFFER7                        = 0x882C,
	CL_DRAW_BUFFER8                        = 0x882D,
	CL_DRAW_BUFFER9                        = 0x882E,
	CL_DRAW_BUFFER10                       = 0x882F,
	CL_DRAW_BUFFER11                       = 0x8830,
	CL_DRAW_BUFFER12                       = 0x8831,
	CL_DRAW_BUFFER13                       = 0x8832,
	CL_DRAW_BUFFER14                       = 0x8833,
	CL_DRAW_BUFFER15                       = 0x8834,

	// ARB_point_sprite:

	CL_POINT_SPRITE                        = 0x8861,
	CL_COORD_REPLACE                       = 0x8862,
	// CL_POINT_SPRITE_COORD_ORIGIN           = cant find this token value anywhere :/

	// ATI_separate_stencil:

	// CL_KEEP                                = 0x1E00,
	// CL_ZERO                                = 0x0000,
	// CL_REPLACE                             = 0x1E01,
	// CL_INCR                                = 0x1E02,
	// CL_DECR                                = 0x1E03,
	// CL_INVERT                              = 0x150A,
	// CL_NEVER                               = 0x0200,
	// CL_LESS                                = 0x0201,
	// CL_LEQUAL                              = 0x0203,
	// CL_GREATER                             = 0x0204,
	// CL_GEQUAL                              = 0x0206,
	// CL_EQUAL                               = 0x0202,
	// CL_NOTEQUAL                            = 0x0205,
	// CL_ALWAYS                              = 0x0207,
	// CL_FRONT                               = 0x0404,
	// CL_BACK                                = 0x0405,
	// CL_FRONT_AND_BACK                      = 0x0408,
	CL_STENCIL_BACK_FUNC                   = 0x8800,
	CL_STENCIL_BACK_FAIL                   = 0x8801,
	CL_STENCIL_BACK_PASS_DEPTH_FAIL        = 0x8802,
	CL_STENCIL_BACK_PASS_DEPTH_PASS        = 0x8803,
	
	// EXT_stencil_two_side additional states:

	CL_STENCIL_TEST_TWO_SIDE               = 0x8910,
	CL_ACTIVE_STENCIL_FACE                 = 0x8911
};

//! OpenGL 2.0 binds:
//- !group=GL/System!
//- !header=gl.h!
class CL_API_GL CL_GLFunctions
{
//! Typedefs:
public:
	typedef void (CL_GLFUNC *ptr_glAccum)(CLenum op, CLfloat value);
	typedef void (CL_GLFUNC *ptr_glAlphaFunc)(CLenum func, CLclampf ref);
	typedef CLboolean (CL_GLFUNC *ptr_glAreTexturesResident)(CLsizei n, const CLuint *textures, CLboolean *residences);
	typedef void (CL_GLFUNC *ptr_glArrayElement)(CLint i);
	typedef void (CL_GLFUNC *ptr_glBegin)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glBindTexture)(CLenum target, CLuint texture);
	typedef void (CL_GLFUNC *ptr_glBitmap)(CLsizei width, CLsizei height, CLfloat xorig, CLfloat yorig, CLfloat xmove, CLfloat ymove, const CLubyte *bitmap);
	typedef void (CL_GLFUNC *ptr_glBlendFunc)(CLenum sfactor, CLenum dfactor);
	typedef void (CL_GLFUNC *ptr_glCallList)(CLuint list);
	typedef void (CL_GLFUNC *ptr_glCallLists)(CLsizei n, CLenum type, const void *lists);
	typedef void (CL_GLFUNC *ptr_glClear)(CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glClearAccum)(CLfloat red, CLfloat green, CLfloat blue, CLfloat alpha);
	typedef void (CL_GLFUNC *ptr_glClearColor)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glClearDepth)(CLclampd depth);
	typedef void (CL_GLFUNC *ptr_glClearIndex)(CLfloat c);
	typedef void (CL_GLFUNC *ptr_glClearStencil)(CLint s);
	typedef void (CL_GLFUNC *ptr_glClipPlane)(CLenum plane, const CLdouble *equation);
	typedef void (CL_GLFUNC *ptr_glColor3b)(CLbyte red, CLbyte green, CLbyte blue);
	typedef void (CL_GLFUNC *ptr_glColor3bv)(const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glColor3d)(CLdouble red, CLdouble green, CLdouble blue);
	typedef void (CL_GLFUNC *ptr_glColor3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glColor3f)(CLfloat red, CLfloat green, CLfloat blue);
	typedef void (CL_GLFUNC *ptr_glColor3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glColor3i)(CLint red, CLint green, CLint blue);
	typedef void (CL_GLFUNC *ptr_glColor3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glColor3s)(CLshort red, CLshort green, CLshort blue);
	typedef void (CL_GLFUNC *ptr_glColor3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glColor3ub)(CLubyte red, CLubyte green, CLubyte blue);
	typedef void (CL_GLFUNC *ptr_glColor3ubv)(const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glColor3ui)(CLuint red, CLuint green, CLuint blue);
	typedef void (CL_GLFUNC *ptr_glColor3uiv)(const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glColor3us)(CLushort red, CLushort green, CLushort blue);
	typedef void (CL_GLFUNC *ptr_glColor3usv)(const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glColor4b)(CLbyte red, CLbyte green, CLbyte blue, CLbyte alpha);
	typedef void (CL_GLFUNC *ptr_glColor4bv)(const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glColor4d)(CLdouble red, CLdouble green, CLdouble blue, CLdouble alpha);
	typedef void (CL_GLFUNC *ptr_glColor4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glColor4f)(CLfloat red, CLfloat green, CLfloat blue, CLfloat alpha);
	typedef void (CL_GLFUNC *ptr_glColor4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glColor4i)(CLint red, CLint green, CLint blue, CLint alpha);
	typedef void (CL_GLFUNC *ptr_glColor4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glColor4s)(CLshort red, CLshort green, CLshort blue, CLshort alpha);
	typedef void (CL_GLFUNC *ptr_glColor4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glColor4ub)(CLubyte red, CLubyte green, CLubyte blue, CLubyte alpha);
	typedef void (CL_GLFUNC *ptr_glColor4ubv)(const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glColor4ui)(CLuint red, CLuint green, CLuint blue, CLuint alpha);
	typedef void (CL_GLFUNC *ptr_glColor4uiv)(const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glColor4us)(CLushort red, CLushort green, CLushort blue, CLushort alpha);
	typedef void (CL_GLFUNC *ptr_glColor4usv)(const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glColorMask)(CLboolean red, CLboolean green, CLboolean blue, CLboolean alpha);
	typedef void (CL_GLFUNC *ptr_glColorMaterial)(CLenum face, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glColorPointer)(CLint size, CLenum type, CLsizei stride, const void *pointer);
	typedef void (CL_GLFUNC *ptr_glCopyPixels)(CLint x, CLint y, CLsizei width, CLsizei height, CLenum type);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage1D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage2D)(CLenum target, CLint level, CLenum internalformat, CLint x, CLint y, CLsizei width, CLsizei height, CLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glCullFace)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glDeleteLists)(CLuint list, CLsizei range);
	typedef void (CL_GLFUNC *ptr_glDeleteTextures)(CLsizei n, const CLuint *textures);
	typedef void (CL_GLFUNC *ptr_glDepthFunc)(CLenum func);
	typedef void (CL_GLFUNC *ptr_glDepthMask)(CLboolean flag);
	typedef void (CL_GLFUNC *ptr_glDepthRange)(CLclampd zNear, CLclampd zFar);
	typedef void (CL_GLFUNC *ptr_glDisable)(CLenum cap);
	typedef void (CL_GLFUNC *ptr_glDisableClientState)(CLenum array);
	typedef void (CL_GLFUNC *ptr_glDrawArrays)(CLenum mode, CLint first, CLsizei count);
	typedef void (CL_GLFUNC *ptr_glDrawBuffer)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glDrawElements)(CLenum mode, CLsizei count, CLenum type, const void *indices);
	typedef void (CL_GLFUNC *ptr_glDrawPixels)(CLsizei width, CLsizei height, CLenum format, CLenum type, const void *pixels);
	typedef void (CL_GLFUNC *ptr_glEdgeFlag)(CLboolean flag);
	typedef void (CL_GLFUNC *ptr_glEdgeFlagPointer)(CLsizei stride, const void *pointer);
	typedef void (CL_GLFUNC *ptr_glEdgeFlagv)(const CLboolean *flag);
	typedef void (CL_GLFUNC *ptr_glEnable)(CLenum cap);
	typedef void (CL_GLFUNC *ptr_glEnableClientState)(CLenum array);
	typedef void (CL_GLFUNC *ptr_glEnd)();
	typedef void (CL_GLFUNC *ptr_glEndList)();
	typedef void (CL_GLFUNC *ptr_glEvalCoord1d)(CLdouble u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord1dv)(const CLdouble *u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord1f)(CLfloat u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord1fv)(const CLfloat *u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2d)(CLdouble u, CLdouble v);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2dv)(const CLdouble *u);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2f)(CLfloat u, CLfloat v);
	typedef void (CL_GLFUNC *ptr_glEvalCoord2fv)(const CLfloat *u);
	typedef void (CL_GLFUNC *ptr_glEvalMesh1)(CLenum mode, CLint i1, CLint i2);
	typedef void (CL_GLFUNC *ptr_glEvalMesh2)(CLenum mode, CLint i1, CLint i2, CLint j1, CLint j2);
	typedef void (CL_GLFUNC *ptr_glEvalPoint1)(CLint i);
	typedef void (CL_GLFUNC *ptr_glEvalPoint2)(CLint i, CLint j);
	typedef void (CL_GLFUNC *ptr_glFeedbackBuffer)(CLsizei size, CLenum type, CLfloat *buffer);
	typedef void (CL_GLFUNC *ptr_glFinish)();
	typedef void (CL_GLFUNC *ptr_glFlush)();
	typedef void (CL_GLFUNC *ptr_glFogf)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glFogfv)(CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glFogi)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glFogiv)(CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glFrontFace)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glFrustum)(CLdouble left, CLdouble right, CLdouble bottom, CLdouble top, CLdouble zNear, CLdouble zFar);
	typedef CLuint (CL_GLFUNC *ptr_glGenLists)(CLsizei range);
	typedef void (CL_GLFUNC *ptr_glGenTextures)(CLsizei n, CLuint *textures);
	typedef void (CL_GLFUNC *ptr_glGetBooleanv)(CLenum pname, CLboolean *params);
	typedef void (CL_GLFUNC *ptr_glGetClipPlane)(CLenum plane, CLdouble *equation);
	typedef void (CL_GLFUNC *ptr_glGetDoublev)(CLenum pname, CLdouble *params);
	typedef CLenum (CL_GLFUNC *ptr_glGetError)();
	typedef void (CL_GLFUNC *ptr_glGetFloatv)(CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetIntegerv)(CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetLightfv)(CLenum light, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetLightiv)(CLenum light, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetMapdv)(CLenum target, CLenum query, CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glGetMapfv)(CLenum target, CLenum query, CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glGetMapiv)(CLenum target, CLenum query, CLint *v);
	typedef void (CL_GLFUNC *ptr_glGetMaterialfv)(CLenum face, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetMaterialiv)(CLenum face, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetPixelMapfv)(CLenum map, CLfloat *values);
	typedef void (CL_GLFUNC *ptr_glGetPixelMapuiv)(CLenum map, CLuint *values);
	typedef void (CL_GLFUNC *ptr_glGetPixelMapusv)(CLenum map, CLushort *values);
	typedef void (CL_GLFUNC *ptr_glGetPointerv)(CLenum pname, void* *params);
	typedef void (CL_GLFUNC *ptr_glGetPolygonStipple)(CLubyte *mask);
	typedef const CLubyte *(CL_GLFUNC *ptr_glGetString)(CLenum name);
	typedef void (CL_GLFUNC *ptr_glGetTexEnvfv)(CLenum target, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexEnviv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexGendv)(CLenum coord, CLenum pname, CLdouble *params);
	typedef void (CL_GLFUNC *ptr_glGetTexGenfv)(CLenum coord, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexGeniv)(CLenum coord, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexImage)(CLenum target, CLint level, CLenum format, CLenum type, void *pixels);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameterfv)(CLenum target, CLint level, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameteriv)(CLenum target, CLint level, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterfv)(CLenum target, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameteriv)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glHint)(CLenum target, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glIndexMask)(CLuint mask);
	typedef void (CL_GLFUNC *ptr_glIndexPointer)(CLenum type, CLsizei stride, const void *pointer);
	typedef void (CL_GLFUNC *ptr_glIndexd)(CLdouble c);
	typedef void (CL_GLFUNC *ptr_glIndexdv)(const CLdouble *c);
	typedef void (CL_GLFUNC *ptr_glIndexf)(CLfloat c);
	typedef void (CL_GLFUNC *ptr_glIndexfv)(const CLfloat *c);
	typedef void (CL_GLFUNC *ptr_glIndexi)(CLint c);
	typedef void (CL_GLFUNC *ptr_glIndexiv)(const CLint *c);
	typedef void (CL_GLFUNC *ptr_glIndexs)(CLshort c);
	typedef void (CL_GLFUNC *ptr_glIndexsv)(const CLshort *c);
	typedef void (CL_GLFUNC *ptr_glIndexub)(CLubyte c);
	typedef void (CL_GLFUNC *ptr_glIndexubv)(const CLubyte *c);
	typedef void (CL_GLFUNC *ptr_glInitNames)();
	typedef void (CL_GLFUNC *ptr_glInterleavedArrays)(CLenum format, CLsizei stride, const void *pointer);
	typedef CLboolean (CL_GLFUNC *ptr_glIsEnabled)(CLenum cap);
	typedef CLboolean (CL_GLFUNC *ptr_glIsList)(CLuint list);
	typedef CLboolean (CL_GLFUNC *ptr_glIsTexture)(CLuint texture);
	typedef void (CL_GLFUNC *ptr_glLightModelf)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glLightModelfv)(CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glLightModeli)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glLightModeliv)(CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glLightf)(CLenum light, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glLightfv)(CLenum light, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glLighti)(CLenum light, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glLightiv)(CLenum light, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glLineStipple)(CLint factor, CLushort pattern);
	typedef void (CL_GLFUNC *ptr_glLineWidth)(CLfloat width);
	typedef void (CL_GLFUNC *ptr_glListBase)(CLuint base);
	typedef void (CL_GLFUNC *ptr_glLoadIdentity)();
	typedef void (CL_GLFUNC *ptr_glLoadMatrixd)(const CLdouble *m);
	typedef void (CL_GLFUNC *ptr_glLoadMatrixf)(const CLfloat *m);
	typedef void (CL_GLFUNC *ptr_glLoadName)(CLuint name);
	typedef void (CL_GLFUNC *ptr_glLogicOp)(CLenum opcode);
	typedef void (CL_GLFUNC *ptr_glMap1d)(CLenum target, CLdouble u1, CLdouble u2, CLint stride, CLint order, const CLdouble *points);
	typedef void (CL_GLFUNC *ptr_glMap1f)(CLenum target, CLfloat u1, CLfloat u2, CLint stride, CLint order, const CLfloat *points);
	typedef void (CL_GLFUNC *ptr_glMap2d)(CLenum target, CLdouble u1, CLdouble u2, CLint ustride, CLint uorder, CLdouble v1, CLdouble v2, CLint vstride, CLint vorder, const CLdouble *points);
	typedef void (CL_GLFUNC *ptr_glMap2f)(CLenum target, CLfloat u1, CLfloat u2, CLint ustride, CLint uorder, CLfloat v1, CLfloat v2, CLint vstride, CLint vorder, const CLfloat *points);
	typedef void (CL_GLFUNC *ptr_glMapGrid1d)(CLint un, CLdouble u1, CLdouble u2);
	typedef void (CL_GLFUNC *ptr_glMapGrid1f)(CLint un, CLfloat u1, CLfloat u2);
	typedef void (CL_GLFUNC *ptr_glMapGrid2d)(CLint un, CLdouble u1, CLdouble u2, CLint vn, CLdouble v1, CLdouble v2);
	typedef void (CL_GLFUNC *ptr_glMapGrid2f)(CLint un, CLfloat u1, CLfloat u2, CLint vn, CLfloat v1, CLfloat v2);
	typedef void (CL_GLFUNC *ptr_glMaterialf)(CLenum face, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glMaterialfv)(CLenum face, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glMateriali)(CLenum face, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glMaterialiv)(CLenum face, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glMatrixMode)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glMultMatrixd)(const CLdouble *m);
	typedef void (CL_GLFUNC *ptr_glMultMatrixf)(const CLfloat *m);
	typedef void (CL_GLFUNC *ptr_glNewList)(CLuint list, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glNormal3b)(CLbyte nx, CLbyte ny, CLbyte nz);
	typedef void (CL_GLFUNC *ptr_glNormal3bv)(const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glNormal3d)(CLdouble nx, CLdouble ny, CLdouble nz);
	typedef void (CL_GLFUNC *ptr_glNormal3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glNormal3f)(CLfloat nx, CLfloat ny, CLfloat nz);
	typedef void (CL_GLFUNC *ptr_glNormal3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glNormal3i)(CLint nx, CLint ny, CLint nz);
	typedef void (CL_GLFUNC *ptr_glNormal3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glNormal3s)(CLshort nx, CLshort ny, CLshort nz);
	typedef void (CL_GLFUNC *ptr_glNormal3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glNormalPointer)(CLenum type, CLsizei stride, const void *pointer);
	typedef void (CL_GLFUNC *ptr_glOrtho)(CLdouble left, CLdouble right, CLdouble bottom, CLdouble top, CLdouble zNear, CLdouble zFar);
	typedef void (CL_GLFUNC *ptr_glPassThrough)(CLfloat token);
	typedef void (CL_GLFUNC *ptr_glPixelMapfv)(CLenum map, CLint mapsize, const CLfloat *values);
	typedef void (CL_GLFUNC *ptr_glPixelMapuiv)(CLenum map, CLint mapsize, const CLuint *values);
	typedef void (CL_GLFUNC *ptr_glPixelMapusv)(CLenum map, CLint mapsize, const CLushort *values);
	typedef void (CL_GLFUNC *ptr_glPixelStoref)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glPixelStorei)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glPixelTransferf)(CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glPixelTransferi)(CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glPixelZoom)(CLfloat xfactor, CLfloat yfactor);
	typedef void (CL_GLFUNC *ptr_glPointSize)(CLfloat size);
	typedef void (CL_GLFUNC *ptr_glPolygonMode)(CLenum face, CLenum mode);
	typedef void (CL_GLFUNC *ptr_glPolygonOffset)(CLfloat factor, CLfloat units);
	typedef void (CL_GLFUNC *ptr_glPolygonStipple)(const CLubyte *mask);
	typedef void (CL_GLFUNC *ptr_glPopAttrib)();
	typedef void (CL_GLFUNC *ptr_glPopClientAttrib)();
	typedef void (CL_GLFUNC *ptr_glPopMatrix)();
	typedef void (CL_GLFUNC *ptr_glPopName)();
	typedef void (CL_GLFUNC *ptr_glPrioritizeTextures)(CLsizei n, const CLuint *textures, const CLclampf *priorities);
	typedef void (CL_GLFUNC *ptr_glPushAttrib)(CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glPushClientAttrib)(CLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glPushMatrix)();
	typedef void (CL_GLFUNC *ptr_glPushName)(CLuint name);
	typedef void (CL_GLFUNC *ptr_glRasterPos2d)(CLdouble x, CLdouble y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos2f)(CLfloat x, CLfloat y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos2i)(CLint x, CLint y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos2s)(CLshort x, CLshort y);
	typedef void (CL_GLFUNC *ptr_glRasterPos2sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3d)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3f)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3i)(CLint x, CLint y, CLint z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos3s)(CLshort x, CLshort y, CLshort z);
	typedef void (CL_GLFUNC *ptr_glRasterPos3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4d)(CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4f)(CLfloat x, CLfloat y, CLfloat z, CLfloat w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4i)(CLint x, CLint y, CLint z, CLint w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glRasterPos4s)(CLshort x, CLshort y, CLshort z, CLshort w);
	typedef void (CL_GLFUNC *ptr_glRasterPos4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glReadBuffer)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glReadPixels)(CLint x, CLint y, CLsizei width, CLsizei height, CLenum format, CLenum type, void *pixels);
	typedef void (CL_GLFUNC *ptr_glRectd)(CLdouble x1, CLdouble y1, CLdouble x2, CLdouble y2);
	typedef void (CL_GLFUNC *ptr_glRectdv)(const CLdouble *v1, const CLdouble *v2);
	typedef void (CL_GLFUNC *ptr_glRectf)(CLfloat x1, CLfloat y1, CLfloat x2, CLfloat y2);
	typedef void (CL_GLFUNC *ptr_glRectfv)(const CLfloat *v1, const CLfloat *v2);
	typedef void (CL_GLFUNC *ptr_glRecti)(CLint x1, CLint y1, CLint x2, CLint y2);
	typedef void (CL_GLFUNC *ptr_glRectiv)(const CLint *v1, const CLint *v2);
	typedef void (CL_GLFUNC *ptr_glRects)(CLshort x1, CLshort y1, CLshort x2, CLshort y2);
	typedef void (CL_GLFUNC *ptr_glRectsv)(const CLshort *v1, const CLshort *v2);
	typedef CLint (CL_GLFUNC *ptr_glRenderMode)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glRotated)(CLdouble angle, CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glRotatef)(CLfloat angle, CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glScaled)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glScalef)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glScissor)(CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glSelectBuffer)(CLsizei size, CLuint *buffer);
	typedef void (CL_GLFUNC *ptr_glShadeModel)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glStencilFunc)(CLenum func, CLint ref, CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilMask)(CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilOp)(CLenum fail, CLenum zfail, CLenum zpass);
	typedef void (CL_GLFUNC *ptr_glTexCoord1d)(CLdouble s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord1f)(CLfloat s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord1i)(CLint s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord1s)(CLshort s);
	typedef void (CL_GLFUNC *ptr_glTexCoord1sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2d)(CLdouble s, CLdouble t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2f)(CLfloat s, CLfloat t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2i)(CLint s, CLint t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord2s)(CLshort s, CLshort t);
	typedef void (CL_GLFUNC *ptr_glTexCoord2sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3d)(CLdouble s, CLdouble t, CLdouble r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3f)(CLfloat s, CLfloat t, CLfloat r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3i)(CLint s, CLint t, CLint r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord3s)(CLshort s, CLshort t, CLshort r);
	typedef void (CL_GLFUNC *ptr_glTexCoord3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4d)(CLdouble s, CLdouble t, CLdouble r, CLdouble q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4f)(CLfloat s, CLfloat t, CLfloat r, CLfloat q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4i)(CLint s, CLint t, CLint r, CLint q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glTexCoord4s)(CLshort s, CLshort t, CLshort r, CLshort q);
	typedef void (CL_GLFUNC *ptr_glTexCoord4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glTexCoordPointer)(CLint size, CLenum type, CLsizei stride, const void *pointer);
	typedef void (CL_GLFUNC *ptr_glTexEnvf)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexEnvfv)(CLenum target, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexEnvi)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glTexEnviv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexGend)(CLenum coord, CLenum pname, CLdouble param);
	typedef void (CL_GLFUNC *ptr_glTexGendv)(CLenum coord, CLenum pname, const CLdouble *params);
	typedef void (CL_GLFUNC *ptr_glTexGenf)(CLenum coord, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexGenfv)(CLenum coord, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexGeni)(CLenum coord, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glTexGeniv)(CLenum coord, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexImage1D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLint border, CLenum format, CLenum type, const void *pixels);
	typedef void (CL_GLFUNC *ptr_glTexImage2D)(CLenum target, CLint level, CLint internalformat, CLsizei width, CLsizei height, CLint border, CLenum format, CLenum type, const void *pixels);
	typedef void (CL_GLFUNC *ptr_glTexParameterf)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexParameterfv)(CLenum target, CLenum pname, const CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexParameteri)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glTexParameteriv)(CLenum target, CLenum pname, const CLint *params);
	typedef void (CL_GLFUNC *ptr_glTexSubImage1D)(CLenum target, CLint level, CLint xoffset, CLsizei width, CLenum format, CLenum type, const void *pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage2D)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLsizei width, CLsizei height, CLenum format, CLenum type, const void *pixels);
	typedef void (CL_GLFUNC *ptr_glTranslated)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glTranslatef)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glVertex2d)(CLdouble x, CLdouble y);
	typedef void (CL_GLFUNC *ptr_glVertex2dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertex2f)(CLfloat x, CLfloat y);
	typedef void (CL_GLFUNC *ptr_glVertex2fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertex2i)(CLint x, CLint y);
	typedef void (CL_GLFUNC *ptr_glVertex2iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertex2s)(CLshort x, CLshort y);
	typedef void (CL_GLFUNC *ptr_glVertex2sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertex3d)(CLdouble x, CLdouble y, CLdouble z);
	typedef void (CL_GLFUNC *ptr_glVertex3dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertex3f)(CLfloat x, CLfloat y, CLfloat z);
	typedef void (CL_GLFUNC *ptr_glVertex3fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertex3i)(CLint x, CLint y, CLint z);
	typedef void (CL_GLFUNC *ptr_glVertex3iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertex3s)(CLshort x, CLshort y, CLshort z);
	typedef void (CL_GLFUNC *ptr_glVertex3sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertex4d)(CLdouble x, CLdouble y, CLdouble z, CLdouble w);
	typedef void (CL_GLFUNC *ptr_glVertex4dv)(const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertex4f)(CLfloat x, CLfloat y, CLfloat z, CLfloat w);
	typedef void (CL_GLFUNC *ptr_glVertex4fv)(const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertex4i)(CLint x, CLint y, CLint z, CLint w);
	typedef void (CL_GLFUNC *ptr_glVertex4iv)(const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertex4s)(CLshort x, CLshort y, CLshort z, CLshort w);
	typedef void (CL_GLFUNC *ptr_glVertex4sv)(const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexPointer)(CLint size, CLenum type, CLsizei stride, const void *pointer);
	typedef void (CL_GLFUNC *ptr_glViewport)(CLint x, CLint y, CLsizei width, CLsizei height);

	typedef void (CL_GLFUNC *ptr_glDrawRangeElementsEXT)(CLenum mode, CLuint start, CLuint end, CLsizei count, CLenum type, const void *indices);
	typedef void (CL_GLFUNC *ptr_glTexImage3DEXT)(CLenum target, CLint level, CLenum internalformat, CLsizei width, CLsizei height, CLsizei depth, CLint border, CLenum format, CLenum type, const void* pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage3DEXT)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLsizei width, CLsizei height, CLsizei depth, CLenum format, CLenum type, const void *pixels);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage3DEXT)(CLenum target, CLint level, CLint xoffset, CLint yoffset, CLint zoffset, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glColorTableSGI)(CLenum target, CLenum internalformat, CLsizei width, CLenum format, CLenum type, const void* table);
	typedef void (CL_GLFUNC *ptr_glCopyColorTableSGI)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glColorTableParameterivSGI)(CLenum target, CLenum pname, const CLint* params);
	typedef void (CL_GLFUNC *ptr_glColorTableParameterfvSGI)(CLenum target, CLenum pname, const CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glGetColorTableSGI)(CLenum target, CLenum format, CLenum type, void* table);
	typedef void (CL_GLFUNC *ptr_glGetColorTableParameterivSGI)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetColorTableParameterfvSGI)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glColorSubTableEXT)(CLenum target, CLsizei start, CLsizei count, CLenum format, CLenum type, const void *data);
	typedef void (CL_GLFUNC *ptr_glCopyColorSubTableEXT)(CLenum target, CLsizei start, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glConvolutionFilter1DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLenum format, CLenum type, const void* image);
	typedef void (CL_GLFUNC *ptr_glConvolutionFilter2DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height, CLenum format, CLenum type, const void* image);
	typedef void (CL_GLFUNC *ptr_glCopyConvolutionFilter1DEXT)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width);
	typedef void (CL_GLFUNC *ptr_glCopyConvolutionFilter2DEXT)(CLenum target, CLenum internalformat, CLint x, CLint y, CLsizei width, CLsizei height);
	typedef void (CL_GLFUNC *ptr_glGetConvolutionFilterEXT)(CLenum target, CLenum format, CLenum type, void* image);
	typedef void (CL_GLFUNC *ptr_glSeparableFilter2DEXT)(CLenum target, CLenum internalformat, CLsizei width, CLsizei height, CLenum format, CLenum type, const void* row, const void* column);
	typedef void (CL_GLFUNC *ptr_glGetSeparableFilterEXT)(CLenum target, CLenum format, CLenum type, void* row, void* column, void* span);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameteriEXT)(CLenum target, CLenum pname, CLint param);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameterivEXT)(CLenum target, CLenum pname, const CLint* params);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameterfEXT)(CLenum target, CLenum pname, CLfloat param);
	typedef void (CL_GLFUNC *ptr_glConvolutionParameterfvEXT)(CLenum target, CLenum pname, const CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glGetConvolutionParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetConvolutionParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glHistogramEXT)(CLenum target, CLsizei width, CLenum internalformat, CLboolean sink);
	typedef void (CL_GLFUNC *ptr_glResetHistogramEXT)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetHistogramEXT)(CLenum target, CLboolean reset, CLenum format, CLenum type, void* values);
	typedef void (CL_GLFUNC *ptr_glGetHistogramParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetHistogramParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glMinmaxEXT)(CLenum target, CLenum internalformat, CLboolean sink);
	typedef void (CL_GLFUNC *ptr_glResetMinmaxEXT)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetMinmaxEXT)(CLenum target, CLboolean reset, CLenum format, CLenum type, void* values);
	typedef void (CL_GLFUNC *ptr_glGetMinmaxParameterivEXT)(CLenum target, CLenum pname, CLint* params);
	typedef void (CL_GLFUNC *ptr_glGetMinmaxParameterfvEXT)(CLenum target, CLenum pname, CLfloat* params);
	typedef void (CL_GLFUNC *ptr_glBlendColorEXT)(CLclampf red, CLclampf green, CLclampf blue, CLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glBlendEquationEXT)(CLenum mode);
	typedef void (CL_GLFUNC *ptr_glActiveTextureARB)(CLenum);
	typedef void (CL_GLFUNC *ptr_glClientActiveTextureARB)(CLenum);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1dARB)(CLenum, CLdouble);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1dvARB)(CLenum, CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1fARB)(CLenum, CLfloat);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1fvARB)(CLenum, CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1iARB)(CLenum, CLint);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1ivARB)(CLenum, CLint const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1sARB)(CLenum, CLshort);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord1svARB)(CLenum, CLshort const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2dARB)(CLenum, CLdouble, CLdouble);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2dvARB)(CLenum, CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2fARB)(CLenum, CLfloat, CLfloat);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2fvARB)(CLenum, CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2iARB)(CLenum, CLint, CLint);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2ivARB)(CLenum, CLint const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2sARB)(CLenum, CLshort, CLshort);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord2svARB)(CLenum, CLshort const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3dARB)(CLenum, CLdouble, CLdouble, CLdouble);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3dvARB)(CLenum, CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3fARB)(CLenum, CLfloat, CLfloat, CLfloat);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3fvARB)(CLenum, CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3iARB)(CLenum, CLint, CLint, CLint);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3ivARB)(CLenum, CLint const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3sARB)(CLenum, CLshort, CLshort, CLshort);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord3svARB)(CLenum, CLshort const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4dARB)(CLenum, CLdouble, CLdouble, CLdouble, CLdouble);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4dvARB)(CLenum, CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4fARB)(CLenum, CLfloat, CLfloat, CLfloat, CLfloat);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4fvARB)(CLenum, CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4iARB)(CLenum, CLint, CLint, CLint, CLint);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4ivARB)(CLenum, CLint const *);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4sARB)(CLenum, CLshort, CLshort, CLshort, CLshort);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoord4svARB)(CLenum, CLshort const *);

	typedef void (CL_GLFUNC *ptr_glCompressedTexImage1DARB)(CLenum, CLint, CLenum, CLsizei, CLint, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage2DARB)(CLenum, CLint, CLenum, CLsizei, CLsizei, CLint, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage3DARB)(CLenum, CLint, CLenum, CLsizei, CLsizei, CLsizei, CLint, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage1DARB)(CLenum, CLint, CLint, CLsizei, CLenum, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage2DARB)(CLenum, CLint, CLint, CLint, CLsizei, CLsizei, CLenum, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage3DARB)(CLenum, CLint, CLint, CLint, CLint, CLsizei, CLsizei, CLsizei, CLenum, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glGetCompressedTexImageARB)(CLenum, CLint, void *);
	typedef void (CL_GLFUNC *ptr_glSampleCoverageARB)(CLclampf, CLboolean);
	typedef void (CL_GLFUNC *ptr_glLoadTransposeMatrixdARB)(CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glLoadTransposeMatrixfARB)(CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glMultTransposeMatrixdARB)(CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glMultTransposeMatrixfARB)(CLfloat const *);

	typedef void (CL_GLFUNC *ptr_glFogCoorddEXT)(CLdouble);
	typedef void (CL_GLFUNC *ptr_glFogCoorddvEXT)(CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glFogCoordfEXT)(CLfloat);
	typedef void (CL_GLFUNC *ptr_glFogCoordfvEXT)(CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glFogCoordPointerEXT)(CLenum, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glMultiDrawArraysEXT)(CLenum, CLint *, CLsizei *, CLsizei);
	typedef void (CL_GLFUNC *ptr_glMultiDrawElementsEXT)(CLenum, CLsizei const *, CLenum, void const * *, CLsizei);
	typedef void (CL_GLFUNC *ptr_glPointParameterfARB)(CLenum, CLfloat);
	typedef void (CL_GLFUNC *ptr_glPointParameterfvARB)(CLenum, CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3bEXT)(CLbyte, CLbyte, CLbyte);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3bvEXT)(CLbyte const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3dEXT)(CLdouble, CLdouble, CLdouble);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3dvEXT)(CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3fEXT)(CLfloat, CLfloat, CLfloat);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3fvEXT)(CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3iEXT)(CLint, CLint, CLint);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3ivEXT)(CLint const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3sEXT)(CLshort, CLshort, CLshort);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3svEXT)(CLshort const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3ubEXT)(CLubyte, CLubyte, CLubyte);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3ubvEXT)(CLubyte const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3uiEXT)(CLuint, CLuint, CLuint);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3uivEXT)(CLuint const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3usEXT)(CLushort, CLushort, CLushort);
	typedef void (CL_GLFUNC *ptr_glSecondaryColor3usvEXT)(CLushort const *);
	typedef void (CL_GLFUNC *ptr_glSecondaryColorPointerEXT)(CLint, CLenum, CLsizei, void const *);
	typedef void (CL_GLFUNC *ptr_glBlendFuncSeparateEXT)(CLenum, CLenum, CLenum, CLenum);
	typedef void (CL_GLFUNC *ptr_glWindowPos2dARB)(CLdouble, CLdouble);
	typedef void (CL_GLFUNC *ptr_glWindowPos2dvARB)(CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glWindowPos2fARB)(CLfloat, CLfloat);
	typedef void (CL_GLFUNC *ptr_glWindowPos2fvARB)(CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glWindowPos2iARB)(CLint, CLint);
	typedef void (CL_GLFUNC *ptr_glWindowPos2ivARB)(CLint const *);
	typedef void (CL_GLFUNC *ptr_glWindowPos2sARB)(CLshort, CLshort);
	typedef void (CL_GLFUNC *ptr_glWindowPos2svARB)(CLshort const *);
	typedef void (CL_GLFUNC *ptr_glWindowPos3dARB)(CLdouble, CLdouble, CLdouble);
	typedef void (CL_GLFUNC *ptr_glWindowPos3dvARB)(CLdouble const *);
	typedef void (CL_GLFUNC *ptr_glWindowPos3fARB)(CLfloat, CLfloat, CLfloat);
	typedef void (CL_GLFUNC *ptr_glWindowPos3fvARB)(CLfloat const *);
	typedef void (CL_GLFUNC *ptr_glWindowPos3iARB)(CLint, CLint, CLint);
	typedef void (CL_GLFUNC *ptr_glWindowPos3ivARB)(CLint const *);
	typedef void (CL_GLFUNC *ptr_glWindowPos3sARB)(CLshort, CLshort, CLshort);
	typedef void (CL_GLFUNC *ptr_glWindowPos3svARB)(CLshort const *);

	typedef void (CL_GLFUNC *ptr_glBindBufferARB)(CLenum target, CLuint buffer);
	typedef void (CL_GLFUNC *ptr_glDeleteBuffersARB)(CLsizei n, const CLuint *buffers);
	typedef void (CL_GLFUNC *ptr_glGenBuffersARB)(CLsizei n, CLuint *buffers);
	typedef CLboolean (CL_GLFUNC *ptr_glIsBufferARB)(CLuint buffer);
	typedef void (CL_GLFUNC *ptr_glBufferDataARB)(CLenum target, CLsizeiptr size, const void *data, CLenum usage);
	typedef void (CL_GLFUNC *ptr_glBufferSubDataARB)(CLenum target, CLintptr offset, CLsizeiptr size, const void *data);
	typedef void (CL_GLFUNC *ptr_glGetBufferSubDataARB)(CLenum target, CLintptr offset, CLsizeiptr size, void *data);
	typedef void (CL_GLFUNC *ptr_glMapBufferARB)(CLenum target, CLenum access);
	typedef CLboolean (CL_GLFUNC *ptr_glUnmapBufferARB)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetBufferParameterivARB)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetBufferPointervARB)(CLenum target, CLenum pname, void **params);
	typedef void (CL_GLFUNC *ptr_glGenQueriesARB)(CLsizei n, CLuint *ids);
	typedef void (CL_GLFUNC *ptr_glDeleteQueriesARB)(CLsizei n, const CLuint *ids);
	typedef CLboolean (CL_GLFUNC *ptr_glIsQueryARB)(CLuint id);
	typedef void (CL_GLFUNC *ptr_glBeginQueryARB)(CLenum target, CLuint id);
	typedef void (CL_GLFUNC *ptr_glEndQueryARB)(CLenum target);
	typedef void (CL_GLFUNC *ptr_glGetQueryivARB)(CLenum target, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectivARB)(CLuint id, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectuivARB)(CLuint id, CLenum pname, CLuint *params);

	typedef void (CL_GLFUNC *ptr_glDeleteObjectARB)(CLuint obj);
	typedef CLuint (CL_GLFUNC *ptr_glGetHandleARB)(CLenum pname);
	typedef void (CL_GLFUNC *ptr_glDetachObjectARB)(CLuint containerObj, CLuint attachedObj);
	typedef CLuint (CL_GLFUNC *ptr_glCreateShaderObjectARB )(CLenum shaderType);
	typedef void (CL_GLFUNC *ptr_glShaderSourceARB)(CLuint shaderObj, CLsizei count, const CLchar* *string, const CLint *length);
	typedef void (CL_GLFUNC *ptr_glCompileShaderARB)(CLuint shaderObj);
	typedef CLuint (CL_GLFUNC *ptr_glCreateProgramObjectARB)();
	typedef void (CL_GLFUNC *ptr_glAttachObjectARB)(CLuint containerObj, CLuint attachedObj);
	typedef void (CL_GLFUNC *ptr_glLinkProgramARB)(CLuint programObj);
	typedef void (CL_GLFUNC *ptr_glUseProgramObjectARB)(CLuint programObj);
	typedef void (CL_GLFUNC *ptr_glValidateProgramARB)(CLuint programObj);
	typedef void (CL_GLFUNC *ptr_glUniform1fARB)(CLint location, CLfloat v0);
	typedef void (CL_GLFUNC *ptr_glUniform2fARB)(CLint location, CLfloat v0, CLfloat v1);
	typedef void (CL_GLFUNC *ptr_glUniform3fARB)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2);
	typedef void (CL_GLFUNC *ptr_glUniform4fARB)(CLint location, CLfloat v0, CLfloat v1, CLfloat v2, CLfloat v3);
	typedef void (CL_GLFUNC *ptr_glUniform1iARB)(CLint location, CLint v0);
	typedef void (CL_GLFUNC *ptr_glUniform2iARB)(CLint location, CLint v0, CLint v1);
	typedef void (CL_GLFUNC *ptr_glUniform3iARB)(CLint location, CLint v0, CLint v1, CLint v2);
	typedef void (CL_GLFUNC *ptr_glUniform4iARB)(CLint location, CLint v0, CLint v1, CLint v2, CLint v3);
	typedef void (CL_GLFUNC *ptr_glUniform1fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform2fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform3fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform4fvARB)(CLint location, CLsizei count, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform1ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform2ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform3ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform4ivARB)(CLint location, CLsizei count, const CLint *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4fvARB)(CLint location, CLsizei count, CLboolean transpose, const CLfloat *value);
	typedef void (CL_GLFUNC *ptr_glGetObjectParameterfvARB)(CLuint obj, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetObjectParameterivARB)(CLuint obj, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetInfoLogARB)(CLuint obj, CLsizei maxLength, CLsizei *length, CLchar *infoLog);
	typedef void (CL_GLFUNC *ptr_glGetAttachedObjectsARB)(CLuint containerObj, CLsizei maxCount, CLsizei *count, CLuint *obj);
	typedef CLint (CL_GLFUNC *ptr_glGetUniformLocationARB)(CLuint programObj, const CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformARB)(CLuint programObj, CLuint index, CLsizei maxLength, CLsizei *length, CLsizei *size, CLenum *type, CLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetUniformfvARB)(CLuint programObj, CLint location, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetUniformivARB)(CLuint programObj, CLint location, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetShaderSourceARB)(CLuint obj, CLsizei maxLength, CLsizei *length, CLchar *source);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1fARB)(CLuint index, CLfloat v0);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1sARB)(CLuint index, CLshort v0);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1dARB)(CLuint index, CLdouble v0);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2fARB)(CLuint index, CLfloat v0, CLfloat v1);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2sARB)(CLuint index, CLshort v0, CLshort v1);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2dARB)(CLuint index, CLdouble v0, CLdouble v1);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3fARB)(CLuint index, CLfloat v0, CLfloat v1, CLfloat v2);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3sARB)(CLuint index, CLshort v0, CLshort v1, CLshort v2);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3dARB)(CLuint index, CLdouble v0, CLdouble v1, CLdouble v2);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4fARB)(CLuint index, CLfloat v0, CLfloat v1, CLfloat v2, CLfloat v3);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4sARB)(CLuint index, CLshort v0, CLshort v1, CLshort v2, CLshort v3);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4dARB)(CLuint index, CLdouble v0, CLdouble v1, CLdouble v2, CLdouble v3);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4NubARB)(CLuint index, CLubyte x, CLubyte y, CLubyte z, CLubyte w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1fvARB)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1svARB)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1dvARB)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2fvARB)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2svARB)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2dvARB)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3fvARB)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3svARB)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3dvARB)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4fvARB)(CLuint index, const CLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4svARB)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4dvARB)(CLuint index, const CLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4ivARB)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4bvARB)(CLuint index, const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4ubvARB)(CLuint index, const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4usvARB)(CLuint index, const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4uivARB)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4NbvARB)(CLuint index, const CLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4NsvARB)(CLuint index, const CLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4NivARB)(CLuint index, const CLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4NubvARB)(CLuint index, const CLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4NusvARB)(CLuint index, const CLushort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4NuivARB)(CLuint index, const CLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribPointerARB)(CLuint index, CLint size, CLenum type, CLboolean normalized, CLsizei stride, const void *pointer);
	typedef void (CL_GLFUNC *ptr_glEnableVertexAttribArrayARB)(CLuint index);
	typedef void (CL_GLFUNC *ptr_glDisableVertexAttribArrayARB)(CLuint index);
	typedef void (CL_GLFUNC *ptr_glBindAttribLocationARB)(CLuint programObj, CLuint index, const char *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveAttribARB)(CLuint programObj, CLuint index, CLsizei maxLength, CLsizei *length, CLint *size, CLenum *type, char *name);
	typedef CLint (CL_GLFUNC *ptr_glGetAttribLocationARB)(CLuint programObj, const char *name);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribdvARB)(CLuint index, CLenum pname, CLdouble *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribfvARB)(CLuint index, CLenum pname, CLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribivARB)(CLuint index, CLenum pname, CLint *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribPointervARB)(CLuint index, CLenum pname, void **pointer);
	typedef void (CL_GLFUNC *ptr_glDrawBuffersARB)(CLsizei n, const CLenum *bufs);
	typedef void (CL_GLFUNC *ptr_glStencilFuncSeparateATI)(CLenum frontfunc, CLenum backfunc, CLint ref, CLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilOpSeparateATI)(CLenum face, CLenum sfail, CLenum dpfail, CLenum dppass);
	typedef void (CL_GLFUNC *ptr_glActiveStencilFaceEXT)(CLenum face);

//! Functions:
public:
	//: glAccum binding
	ptr_glAccum accum;

	//: glAplhaFunc binding
	ptr_glAlphaFunc alphaFunc;

	//: glAreTexturesResident binding
	ptr_glAreTexturesResident areTexturesResident;

	//: glArrayElement binding
	ptr_glArrayElement arrayElement;

	//: glBegin binding
	ptr_glBegin begin;

	//: glBindTexture binding
	ptr_glBindTexture bindTexture;

	//: glBitmap binding
	ptr_glBitmap bitmap;

	//: glBlendFunc binding
	ptr_glBlendFunc blendFunc;

	//: glCallList binding
	ptr_glCallList callList;

	//: glCallLists binding
	ptr_glCallLists callLists;

	//: glClear binding
	ptr_glClear clear;

	//: glClearAccum binding
	ptr_glClearAccum clearAccum;

	//: glClearColor binding
	ptr_glClearColor clearColor;

	//: glClearDepth binding
	ptr_glClearDepth clearDepth;

	//: glClearIndex binding
	ptr_glClearIndex clearIndex;

	//: glClearStencil binding
	ptr_glClearStencil clearStencil;

	//: glClipPlane binding
	ptr_glClipPlane clipPlane;

	//: glColor3b binding
	ptr_glColor3b color3b;

	//: glColor3bv binding
	ptr_glColor3bv color3bv;

	//: glColor3d binding
	ptr_glColor3d color3d;

	//: glColor3dv binding
	ptr_glColor3dv color3dv;

	//: glColor3f binding
	ptr_glColor3f color3f;

	//: glColor3fv binding
	ptr_glColor3fv color3fv;

	//: glColor3i binding
	ptr_glColor3i color3i;

	//: glColor3iv binding
	ptr_glColor3iv color3iv;

	//: glColor3s binding
	ptr_glColor3s color3s;

	//: glColor3sv binding
	ptr_glColor3sv color3sv;

	//: glColor3ub binding
	ptr_glColor3ub color3ub;

	//: glColor3ubv binding
	ptr_glColor3ubv color3ubv;

	//: glColor3ui binding
	ptr_glColor3ui color3ui;

	//: glColor3uiv binding
	ptr_glColor3uiv color3uiv;

	//: glColor3us binding
	ptr_glColor3us color3us;

	//: glColor3usv binding
	ptr_glColor3usv color3usv;

	//: glColor4b binding
	ptr_glColor4b color4b;

	//: glColor4bv binding
	ptr_glColor4bv color4bv;

	//: glColor4d binding
	ptr_glColor4d color4d;

	//: glColor4dv binding
	ptr_glColor4dv color4dv;

	//: glColor4f binding
	ptr_glColor4f color4f;

	//: glColor4fv binding
	ptr_glColor4fv color4fv;

	//: glColor4i binding
	ptr_glColor4i color4i;

	//: glColor4iv binding
	ptr_glColor4iv color4iv;

	//: glColor4s binding
	ptr_glColor4s color4s;

	//: glColor4sv binding
	ptr_glColor4sv color4sv;

	//: glColor4ub binding
	ptr_glColor4ub color4ub;

	//: glColor4ubv binding
	ptr_glColor4ubv color4ubv;

	//: glColor4ui binding
	ptr_glColor4ui color4ui;

	//: glColor4uiv binding
	ptr_glColor4uiv color4uiv;

	//: glColor4us binding
	ptr_glColor4us color4us;

	//: glColor4usv binding
	ptr_glColor4usv color4usv;

	//: glColorMask binding
	ptr_glColorMask colorMask;

	//: glColorMaterial binding
	ptr_glColorMaterial colorMaterial;

	//: glColorPointer binding
	ptr_glColorPointer colorPointer;

	//: glCopyPixels binding
	ptr_glCopyPixels copyPixels;

	//: glCopyTexImage1D binding
	ptr_glCopyTexImage1D copyTexImage1D;

	//: glCopyTexImage2D binding
	ptr_glCopyTexImage2D copyTexImage2D;

	//: glCopyTexSubImage1D binding
	ptr_glCopyTexSubImage1D copyTexSubImage1D;

	//: glCopyTexSubImage2D binding
	ptr_glCopyTexSubImage2D copyTexSubImage2D;

	//: glCullFace binding
	ptr_glCullFace cullFace;

	//: glDeleteLists binding
	ptr_glDeleteLists deleteLists;

	//: glDeleteTextures binding
	ptr_glDeleteTextures deleteTextures;

	//: glDepthFunc binding
	ptr_glDepthFunc depthFunc;

	//: glDepthMask binding
	ptr_glDepthMask depthMask;

	//: glDepthRange binding
	ptr_glDepthRange depthRange;

	//: glDisable binding
	ptr_glDisable disable;

	//: glDisableClientState binding
	ptr_glDisableClientState disableClientState;

	//: glDrawArrays binding
	ptr_glDrawArrays drawArrays;

	//: glDrawBuffer binding
	ptr_glDrawBuffer drawBuffer;

	//: glDrawElements binding
	ptr_glDrawElements drawElements;

	//: glDrawPixels binding
	ptr_glDrawPixels drawPixels;

	//: glEdgeFlag binding
	ptr_glEdgeFlag edgeFlag;

	//: glEdgeFlagPointer binding
	ptr_glEdgeFlagPointer edgeFlagPointer;

	//: glEdgeFlagv binding
	ptr_glEdgeFlagv edgeFlagv;

	//: glEnable binding
	ptr_glEnable enable;

	//: glEnableClientState binding
	ptr_glEnableClientState enableClientState;

	//: glEnd binding
	ptr_glEnd end;

	//: glEndList binding
	ptr_glEndList endList;

	//: glEvalCoord1d binding
	ptr_glEvalCoord1d evalCoord1d;

	//: glEvalCoord1dv binding
	ptr_glEvalCoord1dv evalCoord1dv;

	//: glEvalCoord1f binding
	ptr_glEvalCoord1f evalCoord1f;

	//: glEvalCoord1fv binding
	ptr_glEvalCoord1fv evalCoord1fv;

	//: glEvalCoord2d binding
	ptr_glEvalCoord2d evalCoord2d;

	//: glEvalCoord2dv binding
	ptr_glEvalCoord2dv evalCoord2dv;

	//: glEvalCoord2f binding
	ptr_glEvalCoord2f evalCoord2f;

	//: glEvalCoord2fv binding
	ptr_glEvalCoord2fv evalCoord2fv;

	//: glEvalMesh1 binding
	ptr_glEvalMesh1 evalMesh1;

	//: glEvalMesh2 binding
	ptr_glEvalMesh2 evalMesh2;

	//: glEvalPoint1 binding
	ptr_glEvalPoint1 evalPoint1;

	//: glEvalPoint2 binding
	ptr_glEvalPoint2 evalPoint2;

	//: glFeedbackBuffer binding
	ptr_glFeedbackBuffer feedbackBuffer;

	//: glFinish binding
	ptr_glFinish finish;

	//: glFlush binding
	ptr_glFlush flush;

	//: glFogf binding
	ptr_glFogf fogf;

	//: glFogfv binding
	ptr_glFogfv fogfv;

	//: glFogi binding
	ptr_glFogi fogi;

	//: glFogiv binding
	ptr_glFogiv fogiv;

	//: glFrontFace binding
	ptr_glFrontFace frontFace;

	//: glFrustum binding
	ptr_glFrustum frustum;

	//: glGenLists binding
	ptr_glGenLists genLists;

	//: glGenTextures binding
	ptr_glGenTextures genTextures;

	//: glGetBooleanv binding
	ptr_glGetBooleanv getBooleanv;

	//: glGetClipPlane binding
	ptr_glGetClipPlane getClipPlane;

	//: glGetDoublev binding
	ptr_glGetDoublev getDoublev;

	//: glGetError binding
	ptr_glGetError getError;

	//: glGetFloatv binding
	ptr_glGetFloatv getFloatv;

	//: glGetIntegerv binding
	ptr_glGetIntegerv getIntegerv;

	//: glGetLightfv binding
	ptr_glGetLightfv getLightfv;

	//: glGetLightiv binding
	ptr_glGetLightiv getLightiv;

	//: glGetMapdv binding
	ptr_glGetMapdv getMapdv;

	//: glGetMapfv binding
	ptr_glGetMapfv getMapfv;

	//: glGetMapiv binding
	ptr_glGetMapiv getMapiv;

	//: glGetMaterialfv binding
	ptr_glGetMaterialfv getMaterialfv;

	//: glGetMaterialiv binding
	ptr_glGetMaterialiv getMaterialiv;

	//: glGetPixelMapfv binding
	ptr_glGetPixelMapfv getPixelMapfv;

	//: glGetPixelMapuiv binding
	ptr_glGetPixelMapuiv getPixelMapuiv;

	//: glGetPixelMapusv binding
	ptr_glGetPixelMapusv getPixelMapusv;

	//: glGetPointerv binding
	ptr_glGetPointerv getPointerv;

	//: glGetPolygonStipple binding
	ptr_glGetPolygonStipple getPolygonStipple;

	//: glGetString binding
	ptr_glGetString getString;

	//: glGetTexEnvfv binding
	ptr_glGetTexEnvfv getTexEnvfv;

	//: glGetTexEnviv binding
	ptr_glGetTexEnviv getTexEnviv;

	//: glGetTexGendv binding
	ptr_glGetTexGendv getTexGendv;

	//: glGetTexGenfv binding
	ptr_glGetTexGenfv getTexGenfv;

	//: glGetTexGeniv binding
	ptr_glGetTexGeniv getTexGeniv;

	//: glGetTexImage binding
	ptr_glGetTexImage getTexImage;

	//: glGetTexLevelParameterfv binding
	ptr_glGetTexLevelParameterfv getTexLevelParameterfv;

	//: glGetTexLevelParameteriv binding
	ptr_glGetTexLevelParameteriv getTexLevelParameteriv;

	//: glGetTexParameterfv binding
	ptr_glGetTexParameterfv getTexParameterfv;

	//: glGetTexParameteriv binding
	ptr_glGetTexParameteriv getTexParameteriv;

	//: glHint binding
	ptr_glHint hint;

	//: glIndexMask binding
	ptr_glIndexMask indexMask;

	//: glIndexPointer binding
	ptr_glIndexPointer indexPointer;

	//: glIndexd binding
	ptr_glIndexd indexd;

	//: glIndexdv binding
	ptr_glIndexdv indexdv;

	//: glIndexf binding
	ptr_glIndexf indexf;

	//: glIndexfv binding
	ptr_glIndexfv indexfv;

	//: glIndexi binding
	ptr_glIndexi indexi;

	//: glIndexiv binding
	ptr_glIndexiv indexiv;

	//: glIndexs binding
	ptr_glIndexs indexs;

	//: glIndexsv binding
	ptr_glIndexsv indexsv;

	//: glIndexub binding
	ptr_glIndexub indexub;

	//: glIndexubv binding
	ptr_glIndexubv indexubv;

	//: glInitNames binding
	ptr_glInitNames initNames;

	//: glInterleavedArrays binding
	ptr_glInterleavedArrays interleavedArrays;

	//: glIsEnabled binding
	ptr_glIsEnabled isEnabled;

	//: glIsList binding
	ptr_glIsList isList;

	//: glIsTexture binding
	ptr_glIsTexture isTexture;

	//: glLightModelf binding
	ptr_glLightModelf lightModelf;

	//: glLightModelfv binding
	ptr_glLightModelfv lightModelfv;

	//: glLightModeli binding
	ptr_glLightModeli lightModeli;

	//: glLightModeliv binding
	ptr_glLightModeliv lightModeliv;

	//: glLightf binding
	ptr_glLightf lightf;

	//: glLightfv binding
	ptr_glLightfv lightfv;

	//: glLighti binding
	ptr_glLighti lighti;

	//: glLightiv binding
	ptr_glLightiv lightiv;

	//: glLineStipple binding
	ptr_glLineStipple lineStipple;

	//: glLineWidth binding
	ptr_glLineWidth lineWidth;

	//: glListBase binding
	ptr_glListBase listBase;

	//: glLoadIdentity binding
	ptr_glLoadIdentity loadIdentity;

	//: glLoadMatrixd binding
	ptr_glLoadMatrixd loadMatrixd;

	//: glLoadMatrixf binding
	ptr_glLoadMatrixf loadMatrixf;

	//: glLoadName binding
	ptr_glLoadName loadName;

	//: glLogicOp binding
	ptr_glLogicOp logicOp;

	//: glMap1d binding
	ptr_glMap1d map1d;

	//: glMap1f binding
	ptr_glMap1f map1f;

	//: glMap2d binding
	ptr_glMap2d map2d;

	//: glMap2f binding
	ptr_glMap2f map2f;

	//: glMapGrid1d binding
	ptr_glMapGrid1d mapGrid1d;

	//: glMapGrid1f binding
	ptr_glMapGrid1f mapGrid1f;

	//: glMapGrid2d binding
	ptr_glMapGrid2d mapGrid2d;

	//: glMapGrid2f binding
	ptr_glMapGrid2f mapGrid2f;

	//: glMaterialf binding
	ptr_glMaterialf materialf;

	//: glMaterialfv binding
	ptr_glMaterialfv materialfv;

	//: glMateriali binding
	ptr_glMateriali materiali;

	//: glMaterialiv binding
	ptr_glMaterialiv materialiv;

	//: glMatrixMode binding
	ptr_glMatrixMode matrixMode;

	//: glMultMatrixd binding
	ptr_glMultMatrixd multMatrixd;

	//: glMultMatrixf binding
	ptr_glMultMatrixf multMatrixf;

	//: glNewList binding
	ptr_glNewList newList;

	//: glNormal3b binding
	ptr_glNormal3b normal3b;

	//: glNormal3bv binding
	ptr_glNormal3bv normal3bv;

	//: glNormal3d binding
	ptr_glNormal3d normal3d;

	//: glNormal3dv binding
	ptr_glNormal3dv normal3dv;

	//: glNormal3f binding
	ptr_glNormal3f normal3f;

	//: glNormal3fv binding
	ptr_glNormal3fv normal3fv;

	//: glNormal3i binding
	ptr_glNormal3i normal3i;

	//: glNormal3iv binding
	ptr_glNormal3iv normal3iv;

	//: glNormal3s binding
	ptr_glNormal3s normal3s;

	//: glNormal3sv binding
	ptr_glNormal3sv normal3sv;

	//: glNormalPointer binding
	ptr_glNormalPointer normalPointer;

	//: glOrtho binding
	ptr_glOrtho ortho;

	//: glPassThrough binding
	ptr_glPassThrough passThrough;

	//: glPixelMapfv binding
	ptr_glPixelMapfv pixelMapfv;

	//: glPixelMapuiv binding
	ptr_glPixelMapuiv pixelMapuiv;

	//: glPixelMapusv binding
	ptr_glPixelMapusv pixelMapusv;

	//: glPixelStoref binding
	ptr_glPixelStoref pixelStoref;

	//: glPixelStorei binding
	ptr_glPixelStorei pixelStorei;

	//: glPixelTransferf binding
	ptr_glPixelTransferf pixelTransferf;

	//: glPixelTransferi binding
	ptr_glPixelTransferi pixelTransferi;

	//: glPixelZoom binding
	ptr_glPixelZoom pixelZoom;

	//: glPointSize binding
	ptr_glPointSize pointSize;

	//: glPolygonMode binding
	ptr_glPolygonMode polygonMode;

	//: glPolygonOffset binding
	ptr_glPolygonOffset polygonOffset;

	//: glPolygonStipple binding
	ptr_glPolygonStipple polygonStipple;

	//: glPopAttrib binding
	ptr_glPopAttrib popAttrib;

	//: glPopClientAttrib binding
	ptr_glPopClientAttrib popClientAttrib;

	//: glPopMatrix binding
	ptr_glPopMatrix popMatrix;

	//: glPopName binding
	ptr_glPopName popName;

	//: glPrioritizeTextures binding
	ptr_glPrioritizeTextures prioritizeTextures;

	//: glPushAttrib binding
	ptr_glPushAttrib pushAttrib;

	//: glPushClientAttrib binding
	ptr_glPushClientAttrib pushClientAttrib;

	//: glPushMatrix binding
	ptr_glPushMatrix pushMatrix;

	//: glPushName binding
	ptr_glPushName pushName;

	//: glRasterPos2d binding
	ptr_glRasterPos2d rasterPos2d;

	//: glRasterPos2dv binding
	ptr_glRasterPos2dv rasterPos2dv;

	//: glRasterPos2f binding
	ptr_glRasterPos2f rasterPos2f;

	//: glRasterPos2fv binding
	ptr_glRasterPos2fv rasterPos2fv;

	//: glRasterPos2i binding
	ptr_glRasterPos2i rasterPos2i;

	//: glRasterPos2iv binding
	ptr_glRasterPos2iv rasterPos2iv;

	//: glRasterPos2s binding
	ptr_glRasterPos2s rasterPos2s;

	//: glRasterPos2sv binding
	ptr_glRasterPos2sv rasterPos2sv;

	//: glRasterPos3d binding
	ptr_glRasterPos3d rasterPos3d;

	//: glRasterPos3dv binding
	ptr_glRasterPos3dv rasterPos3dv;

	//: glRasterPos3f binding
	ptr_glRasterPos3f rasterPos3f;

	//: glRasterPos3fv binding
	ptr_glRasterPos3fv rasterPos3fv;

	//: glRasterPos3i binding
	ptr_glRasterPos3i rasterPos3i;

	//: glRasterPos3iv binding
	ptr_glRasterPos3iv rasterPos3iv;

	//: glRasterPos3s binding
	ptr_glRasterPos3s rasterPos3s;

	//: glRasterPos3sv binding
	ptr_glRasterPos3sv rasterPos3sv;

	//: glRasterPos4d binding
	ptr_glRasterPos4d rasterPos4d;

	//: glRasterPos4dv binding
	ptr_glRasterPos4dv rasterPos4dv;

	//: glRasterPos4f binding
	ptr_glRasterPos4f rasterPos4f;

	//: glRasterPos4fv binding
	ptr_glRasterPos4fv rasterPos4fv;

	//: glRasterPos4i binding
	ptr_glRasterPos4i rasterPos4i;

	//: glRasterPos4iv binding
	ptr_glRasterPos4iv rasterPos4iv;

	//: glRasterPos4s binding
	ptr_glRasterPos4s rasterPos4s;

	//: glRasterPos4sv binding
	ptr_glRasterPos4sv rasterPos4sv;

	//: glReadBuffer binding
	ptr_glReadBuffer readBuffer;

	//: glReadPixels binding
	ptr_glReadPixels readPixels;

	//: glRectd binding
	ptr_glRectd rectd;

	//: glRectdv binding
	ptr_glRectdv rectdv;

	//: glRectf binding
	ptr_glRectf rectf;

	//: glRectfv binding
	ptr_glRectfv rectfv;

	//: glRecti binding
	ptr_glRecti recti;

	//: glRectiv binding
	ptr_glRectiv rectiv;

	//: glRects binding
	ptr_glRects rects;

	//: glRectsv binding
	ptr_glRectsv rectsv;

	//: glRenderMode binding
	ptr_glRenderMode renderMode;

	//: glRotated binding
	ptr_glRotated rotated;

	//: glRotatef binding
	ptr_glRotatef rotatef;

	//: glScaled binding
	ptr_glScaled scaled;

	//: glScalef binding
	ptr_glScalef scalef;

	//: glScissor binding
	ptr_glScissor scissor;

	//: glSelectBuffer binding
	ptr_glSelectBuffer selectBuffer;

	//: glShadeModel binding
	ptr_glShadeModel shadeModel;

	//: glStencilFunc binding
	ptr_glStencilFunc stencilFunc;

	//: glStencilMask binding
	ptr_glStencilMask stencilMask;

	//: glStencilOp binding
	ptr_glStencilOp stencilOp;

	//: glTexCoord1d binding
	ptr_glTexCoord1d texCoord1d;

	//: glTexCoord1dv binding
	ptr_glTexCoord1dv texCoord1dv;

	//: glTexCoord1f binding
	ptr_glTexCoord1f texCoord1f;

	//: glTexCoord1fv binding
	ptr_glTexCoord1fv texCoord1fv;

	//: glTexCoord1i binding
	ptr_glTexCoord1i texCoord1i;

	//: glTexCoord1iv binding
	ptr_glTexCoord1iv texCoord1iv;

	//: glTexCoord1s binding
	ptr_glTexCoord1s texCoord1s;

	//: glTexCoord1sv binding
	ptr_glTexCoord1sv texCoord1sv;

	//: glTexCoord2d binding
	ptr_glTexCoord2d texCoord2d;

	//: glTexCoord2dv binding
	ptr_glTexCoord2dv texCoord2dv;

	//: glTexCoord2f binding
	ptr_glTexCoord2f texCoord2f;

	//: glTexCoord2fv binding
	ptr_glTexCoord2fv texCoord2fv;

	//: glTexCoord2i binding
	ptr_glTexCoord2i texCoord2i;

	//: glTexCoord2iv binding
	ptr_glTexCoord2iv texCoord2iv;

	//: glTexCoord2s binding
	ptr_glTexCoord2s texCoord2s;

	//: glTexCoord2sv binding
	ptr_glTexCoord2sv texCoord2sv;

	//: glTexCoord3d binding
	ptr_glTexCoord3d texCoord3d;

	//: glTexCoord3dv binding
	ptr_glTexCoord3dv texCoord3dv;

	//: glTexCoord3f binding
	ptr_glTexCoord3f texCoord3f;

	//: glTexCoord3fv binding
	ptr_glTexCoord3fv texCoord3fv;

	//: glTexCoord3i binding
	ptr_glTexCoord3i texCoord3i;

	//: glTexCoord3iv binding
	ptr_glTexCoord3iv texCoord3iv;

	//: glTexCoord3s binding
	ptr_glTexCoord3s texCoord3s;

	//: glTexCoord3sv binding
	ptr_glTexCoord3sv texCoord3sv;

	//: glTexCoord4d binding
	ptr_glTexCoord4d texCoord4d;

	//: glTexCoord4dv binding
	ptr_glTexCoord4dv texCoord4dv;

	//: glTexCoord4f binding
	ptr_glTexCoord4f texCoord4f;

	//: glTexCoord4fv binding
	ptr_glTexCoord4fv texCoord4fv;

	//: glTexCoord4i binding
	ptr_glTexCoord4i texCoord4i;

	//: glTexCoord4iv binding
	ptr_glTexCoord4iv texCoord4iv;

	//: glTexCoord4s binding
	ptr_glTexCoord4s texCoord4s;

	//: glTexCoord4sv binding
	ptr_glTexCoord4sv texCoord4sv;

	//: glTexCoordPointer binding
	ptr_glTexCoordPointer texCoordPointer;

	//: glTexEnvf binding
	ptr_glTexEnvf texEnvf;

	//: glTexEnvfv binding
	ptr_glTexEnvfv texEnvfv;

	//: glTexEnvi binding
	ptr_glTexEnvi texEnvi;

	//: glTexEnviv binding
	ptr_glTexEnviv texEnviv;

	//: glTexGend binding
	ptr_glTexGend texGend;

	//: glTexGendv binding
	ptr_glTexGendv texGendv;

	//: glTexGenf binding
	ptr_glTexGenf texGenf;

	//: glTexGenfv binding
	ptr_glTexGenfv texGenfv;

	//: glTexGeni binding
	ptr_glTexGeni texGeni;

	//: glTexGeniv binding
	ptr_glTexGeniv texGeniv;

	//: glTexImage1D binding
	ptr_glTexImage1D texImage1D;

	//: glTexImage2D binding
	ptr_glTexImage2D texImage2D;

	//: glTexParameterf binding
	ptr_glTexParameterf texParameterf;

	//: glTexParameterfv binding
	ptr_glTexParameterfv texParameterfv;

	//: glTexParameteri binding
	ptr_glTexParameteri texParameteri;

	//: glTexParameteriv binding
	ptr_glTexParameteriv texParameteriv;

	//: glTexSubImage1D binding
	ptr_glTexSubImage1D texSubImage1D;

	//: glTexSubImage2D binding
	ptr_glTexSubImage2D texSubImage2D;

	//: glTranslated binding
	ptr_glTranslated translated;

	//: glTranslatef binding
	ptr_glTranslatef translatef;

	//: glVertex2d binding
	ptr_glVertex2d vertex2d;

	//: glVertex2dv binding
	ptr_glVertex2dv vertex2dv;

	//: glVertex2f binding
	ptr_glVertex2f vertex2f;

	//: glVertex2fv binding
	ptr_glVertex2fv vertex2fv;

	//: glVertex2i binding
	ptr_glVertex2i vertex2i;

	//: glVertex2iv binding
	ptr_glVertex2iv vertex2iv;

	//: glVertex2s binding
	ptr_glVertex2s vertex2s;

	//: glVertex2sv binding
	ptr_glVertex2sv vertex2sv;

	//: glVertex3d binding
	ptr_glVertex3d vertex3d;

	//: glVertex3dv binding
	ptr_glVertex3dv vertex3dv;

	//: glVertex3f binding
	ptr_glVertex3f vertex3f;

	//: glVertex3fv binding
	ptr_glVertex3fv vertex3fv;

	//: glVertex3i binding
	ptr_glVertex3i vertex3i;

	//: glVertex3iv binding
	ptr_glVertex3iv vertex3iv;

	//: glVertex3s binding
	ptr_glVertex3s vertex3s;

	//: glVertex3sv binding
	ptr_glVertex3sv vertex3sv;

	//: glVertex4d binding
	ptr_glVertex4d vertex4d;

	//: glVertex4dv binding
	ptr_glVertex4dv vertex4dv;

	//: glVertex4f binding
	ptr_glVertex4f vertex4f;

	//: glVertex4fv binding
	ptr_glVertex4fv vertex4fv;

	//: glVertex4i binding
	ptr_glVertex4i vertex4i;

	//: glVertex4iv binding
	ptr_glVertex4iv vertex4iv;

	//: glVertex4s binding
	ptr_glVertex4s vertex4s;

	//: glVertex4sv binding
	ptr_glVertex4sv vertex4sv;

	//: glVertexPointer binding
	ptr_glVertexPointer vertexPointer;

	//: glViewport binding
	ptr_glViewport viewport;

	//: glDrawRangeElements binding (OpenGL 1.2, EXT_draw_range_elements)
	ptr_glDrawRangeElementsEXT drawRangeElements;

	//: glTexImage3D binding (OpenGL 1.2, EXT_texture3D)
	ptr_glTexImage3DEXT texImage3D;

	//: glTexSubImage3D binding (OpenGL 1.2, EXT_texture3D)
	ptr_glTexSubImage3DEXT texSubImage3D;

	//: glCopyTexSubImage3D binding (OpenGL 1.2, EXT_texture3D)
	ptr_glCopyTexSubImage3DEXT copyTexSubImage3D;

	//: glColorTable (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glColorTableSGI colorTable;

	//: glCopyColorTable (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glCopyColorTableSGI copyColorTable;

	//: glColorTableParameteriv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glColorTableParameterivSGI colorTableParameteriv;

	//: glColorTableParameterfv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glColorTableParameterfvSGI colorTableParameterfv;

	//: glGetColorTable (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glGetColorTableSGI getColorTable;

	//: glGetColorTableParameteriv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glGetColorTableParameterivSGI getColorTableParameteriv;

	//: glGetColorTableParameterfv (OpenGL 1.2 Imaging subset, EXT_color_table, SGI_color_table)
	ptr_glGetColorTableParameterfvSGI getColorTableParameterfv;

	//: glColorSubTable (OpenGL 1.2 Imaging subset, EXT_color_subtable)
	ptr_glColorSubTableEXT colorSubTable;

	//: glCopyColorSubTable (OpenGL 1.2 Imaging subset, EXT_color_subtable)
	ptr_glCopyColorSubTableEXT copyColorSubTable;

	//: glConvolutionFilter1D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionFilter1DEXT convolutionFilter1D;

	//: glConvolutionFilter2D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionFilter2DEXT convolutionFilter2D;

	//: glCopyConvolutionFilter1D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glCopyConvolutionFilter1DEXT copyConvolutionFilter1D;

	//: glCopyConvolutionFilter2D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glCopyConvolutionFilter2DEXT copyConvolutionFilter2D;

	//: glGetConvolutionFilter (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetConvolutionFilterEXT getConvolutionFilter;

	//: glSeparableFilter2D (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glSeparableFilter2DEXT separableFilter2D;

	//: glGetSeparableFilter (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetSeparableFilterEXT getSeparableFilter;

	//: glConvolutionParameteri (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameteriEXT convolutionParameteri;

	//: glConvolutionParameterv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameterivEXT convolutionParameteriv;

	//: glConvolutionParameterf (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameterfEXT convolutionParameterf;

	//: glConvolutionParameterfv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glConvolutionParameterfvEXT convolutionParameterfv;

	//: glGetConvolutionParameteriv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetConvolutionParameterivEXT getConvolutionParameteriv;

	//: glGetConvolutionParameterfv (OpenGL 1.2 Imaging subset, EXT_convolution)
	ptr_glGetConvolutionParameterfvEXT getConvolutionParameterfv;

	//: glHistogram (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glHistogramEXT histogram;

	//: glResetHistogram (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glResetHistogramEXT resetHistogram;

	//: glGetHistogram (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetHistogramEXT getHistogram;

	//: glHistogramParameteriv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetHistogramParameterivEXT getHistogramParameteriv;

	//: glHistogramParameterfv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetHistogramParameterfvEXT getHistogramParameterfv;

	//: glMinmax (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glMinmaxEXT minmax;

	//: glResetMinmax (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glResetMinmaxEXT resetMinmax;

	//: glGetMinmax (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetMinmaxEXT getMinmax;

	//: glGetMinmaxParameteriv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetMinmaxParameterivEXT getMinmaxParameteriv;

	//: glGetMinmaxParameterfv (OpenGL 1.2 Imaging subset, EXT_histogram)
	ptr_glGetMinmaxParameterfvEXT getMinmaxParameterfv;

	//: glBlendColor (OpenGL 1.2 Imaging subset, OpenGL 1.4, EXT_blend_color)
	ptr_glBlendColorEXT blendColor;

	//: glBlendEquation (OpenGL 1.2 Imaging subset, EXT_blend_minmax)
	ptr_glBlendEquationEXT blendEquation;

	//: glActiveTexture (OpenGL 1.2.1, ARB_multitexture)
	ptr_glActiveTextureARB activeTexture;

	//: glClientActiveTexture (OpenGL 1.2.1, ARB_multitexture)
	ptr_glClientActiveTextureARB clientActiveTexture;

	//: glMultiTexCoord1d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1dARB multiTexCoord1d;

	//: glMultiTexCoord1dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1dvARB multiTexCoord1dv;

	//: glMultiTexCoord1f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1fARB multiTexCoord1f;

	//: glMultiTexCoord1fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1fvARB multiTexCoord1fv;

	//: glMultiTexCoord1i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1iARB multiTexCoord1i;

	//: glMultiTexCoord1iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1ivARB multiTexCoord1iv;

	//: glMultiTexCoord1s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1sARB multiTexCoord1s;

	//: glMultiTexCoord1sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord1svARB multiTexCoord1sv;

	//: glMultiTexCoord2d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2dARB multiTexCoord2d;

	//: glMultiTexCoord2dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2dvARB multiTexCoord2dv;

	//: glMultiTexCoord2f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2fARB multiTexCoord2f;

	//: glMultiTexCoord2fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2fvARB multiTexCoord2fv;

	//: glMultiTexCoord2i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2iARB multiTexCoord2i;

	//: glMultiTexCoord2iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2ivARB multiTexCoord2iv;

	//: glMultiTexCoord2s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2sARB multiTexCoord2s;

	//: glMultiTexCoord2sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord2svARB multiTexCoord2sv;

	//: glMultiTexCoord3d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3dARB multiTexCoord3d;

	//: glMultiTexCoord3dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3dvARB multiTexCoord3dv;

	//: glMultiTexCoord3f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3fARB multiTexCoord3f;

	//: glMultiTexCoord3fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3fvARB multiTexCoord3fv;

	//: glMultiTexCoord3i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3iARB multiTexCoord3i;

	//: glMultiTexCoord3iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3ivARB multiTexCoord3iv;

	//: glMultiTexCoord3s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3sARB multiTexCoord3s;

	//: glMultiTexCoord3sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord3svARB multiTexCoord3sv;

	//: glMultiTexCoord4d (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4dARB multiTexCoord4d;

	//: glMultiTexCoord4dv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4dvARB multiTexCoord4dv;

	//: glMultiTexCoord4f (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4fARB multiTexCoord4f;

	//: glMultiTexCoord4fv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4fvARB multiTexCoord4fv;

	//: glMultiTexCoord4i (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4iARB multiTexCoord4i;

	//: glMultiTexCoord4iv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4ivARB multiTexCoord4iv;

	//: glMultiTexCoord4s (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4sARB multiTexCoord4s;

	//: glMultiTexCoord4sv (OpenGL 1.2.1, ARB_multitexture)
	ptr_glMultiTexCoord4svARB multiTexCoord4sv;

	//: glCompressedTexImage1D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexImage1DARB compressedTexImage1D;

	//: glCompressedTexImage2D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexImage2DARB compressedTexImage2D;

	//: glCompressedTexImage3D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexImage3DARB compressedTexImage3D;

	//: glCompressedTexSubImage1D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexSubImage1DARB compressedTexSubImage1D;

	//: glCompressedTexSubImage2D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexSubImage2DARB compressedTexSubImage2D;

	//: glCompressedTexSubImage3D (OpenGL 1.3, ARB_texture_compression)
	ptr_glCompressedTexSubImage3DARB compressedTexSubImage3D;

	//: glGetCompressedTexImage (OpenGL 1.3, ARB_texture_compression)
	ptr_glGetCompressedTexImageARB getCompressedTexImage;

	//: glSampleCoverage (OpenGL 1.3, ARB_multisample)
	ptr_glSampleCoverageARB sampleCoverage;

	//: glLoadTransposeMatrixd (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glLoadTransposeMatrixdARB glLoadTransposeMatrixd;

	//: glLoadTransposeMatrixf (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glLoadTransposeMatrixfARB glLoadTransposeMatrixf;

	//: glMultTransposeMatrixd (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glMultTransposeMatrixdARB glMultTransposeMatrixd;

	//: glMultTransposeMatrixf (OpenGL 1.3, ARB_transpose_matrix)
	ptr_glMultTransposeMatrixfARB glMultTransposeMatrixf;

	//: glFogCoordd (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoorddEXT fogCoordd;

	//: glFogCoorddv (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoorddvEXT fogCoorddv;

	//: glFogCoordf (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoordfEXT fogCoordf;

	//: glFogCoordfv (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoordfvEXT fogCoordfv;

	//: glFogCoordPointer (OpenGL 1.4, EXT_fog_coord)
	ptr_glFogCoordPointerEXT fogCoordPointer;

	//: glMultiDrawArrays (OpenGL 1.4, EXT_multi_draw_arrays)
	ptr_glMultiDrawArraysEXT multiDrawArrays;

	//: glMultiDrawElements (OpenGL 1.4, EXT_multi_draw_arrays)
	ptr_glMultiDrawElementsEXT multiDrawElementsEXT;

	//: glPointParameterf (OpenGL 1.4, ARB_point_parameters)
	ptr_glPointParameterfARB pointParameterf;

	//: glPointParameterfv (OpenGL 1.4, ARB_point_parameters)
	ptr_glPointParameterfvARB pointParameterfv;

	//: glSecondaryColor3b (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3bEXT secondaryColor3b;

	//: glSecondaryColor3bv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3bvEXT secondaryColor3bv;

	//: glSecondaryColor3d (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3dEXT secondaryColor3d;

	//: glSecondaryColor3dv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3dvEXT secondaryColor3dv;

	//: glSecondaryColor3f (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3fEXT secondaryColor3f;

	//: glSecondaryColor3fv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3fvEXT secondaryColor3fv;

	//: glSecondaryColor3i (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3iEXT secondaryColor3i;

	//: glSecondaryColor3iv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3ivEXT secondaryColor3iv;

	//: glSecondaryColor3s (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3sEXT secondaryColor3s;

	//: glSecondaryColor3sv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3svEXT secondaryColor3sv;

	//: glSecondaryColor3ub (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3ubEXT secondaryColor3ub;

	//: glSecondaryColor3ubv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3ubvEXT secondaryColor3ubv;

	//: glSecondaryColor3ui (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3uiEXT secondaryColor3ui;

	//: glSecondaryColor3uiv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3uivEXT secondaryColor3uiv;

	//: glSecondaryColor3us (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3usEXT secondaryColor3us;

	//: glSecondaryColor3usv (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColor3usvEXT secondaryColor3usv;

	//: glSecondaryColorPointer (OpenGL 1.4, EXT_secondary_color)
	ptr_glSecondaryColorPointerEXT secondaryColorPointer;

	//: glBlendFuncSeparate (OpenGL 1.4, EXT_blend_func_separate)
	ptr_glBlendFuncSeparateEXT blendFuncSeparate;

	//: glWindowPos2d (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2dARB windowPos2d;

	//: glWindowPos2dv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2dvARB windowPos2dv;

	//: glWindowPos2f (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2fARB windowPos2f;

	//: glWindowPos2fv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2fvARB windowPos2fv;

	//: glWindowPos2i (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2iARB windowPos2i;

	//: glWindowPos2iv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2ivARB windowPos2iv;

	//: glWindowPos2s (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2sARB windowPos2s;

	//: glWindowPos2sv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos2svARB windowPos2sv;

	//: glWindowPos3d (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3dARB windowPos3d;

	//: glWindowPos3dv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3dvARB windowPos3dv;

	//: glWindowPos3f (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3fARB windowPos3f;

	//: glWindowPos3fv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3fvARB windowPos3fv;

	//: glWindowPos3i (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3iARB windowPos3i;

	//: glWindowPos3iv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3ivARB windowPos3iv;

	//: glWindowPos3s (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3sARB windowPos3s;

	//: glWindowPos3sv (OpenGL 1.4, ARB_window_pos)
	ptr_glWindowPos3svARB windowPos3sv;

	//: glBindBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glBindBufferARB bindBuffer;

	//: glDeleteBuffers (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glDeleteBuffersARB deleteBuffers;

	//: glGenBuffers (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGenBuffersARB genBuffers;

	//: glIsBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glIsBufferARB isBuffer;

	//: glBufferData (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glBufferDataARB bufferData;

	//: glBufferData (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glBufferSubDataARB bufferSubData;

	//: glBufferSubData (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGetBufferSubDataARB getBufferSubData;

	//: glMapBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glMapBufferARB mapBuffer;

	//: glUnmapBuffer (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glUnmapBufferARB unmapBuffer;
	
	//: glGetBufferParameteriv (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGetBufferParameterivARB getBufferParameteriv;
	
	//: glGetBufferPointerv (OpenGL 1.5, ARB_vertex_buffer_object)
	ptr_glGetBufferPointervARB getBufferPointerv;
	
	//: glGenQueries (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGenQueriesARB genQueries;

	//: glDeleteQueries (OpenGL 1.5, ARB_occlusion_query)
	ptr_glDeleteQueriesARB deleteQueries;

	//: glIsQuery (OpenGL 1.5, ARB_occlusion_query)
	ptr_glIsQueryARB isQuery;

	//: glBeginQuery (OpenGL 1.5, ARB_occlusion_query)
	ptr_glBeginQueryARB beginQuery;

	//: glEndQuery (OpenGL 1.5, ARB_occlusion_query)
	ptr_glEndQueryARB endQuery;

	//: glGetQueryiv (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGetQueryivARB getQueryiv;

	//: glGetQueryObjectiv (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGetQueryObjectivARB getQueryObjectiv;

	//: glGenQueryObjectuiv (OpenGL 1.5, ARB_occlusion_query)
	ptr_glGetQueryObjectuivARB getQueryObjectuiv;

	//: glDeleteShader (OpenGL 2.0, ARB_shader_objects)
	ptr_glDeleteObjectARB deleteShader;

	//: glDeleteProgram (OpenGL 2.0, ARB_shader_objects)
	ptr_glDeleteObjectARB deleteProgram;

	//: glGetHandle (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetHandleARB getHandle;

	//: glDetachObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glDetachObjectARB detachShader;

	//: glCreateShader (OpenGL 2.0, ARB_shader_objects)
	ptr_glCreateShaderObjectARB createShader;

	//: glShaderSource (OpenGL 2.0, ARB_shader_objects)
	ptr_glShaderSourceARB shaderSource;

	//: glCompileShader (OpenGL 2.0, ARB_shader_objects)
	ptr_glCompileShaderARB compileShader;

	//: glCreateProgramObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glCreateProgramObjectARB createProgram;

	//: glAttachObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glAttachObjectARB attachShader;

	//: glLinkProgram (OpenGL 2.0, ARB_shader_objects)
	ptr_glLinkProgramARB linkProgram;

	//: glUseProgramObject (OpenGL 2.0, ARB_shader_objects)
	ptr_glUseProgramObjectARB useProgram;

	//: glValidateProgram (OpenGL 2.0, ARB_shader_objects)
	ptr_glValidateProgramARB validateProgram;

	//: glUniform1f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1fARB uniform1f;

	//: glUniform2f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2fARB uniform2f;

	//: glUniform3f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3fARB uniform3f;

	//: glUniform4f (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4fARB uniform4f;

	//: glUniform1i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1iARB uniform1i;

	//: glUniform2i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2iARB uniform2i;

	//: glUniform3i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3iARB uniform3i;

	//: glUniform4i (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4iARB uniform4i;

	//: glUniform1fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1fvARB uniform1fv;

	//: glUniform2fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2fvARB uniform2fv;

	//: glUniform3fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3fvARB uniform3fv;

	//: glUniform4fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4fvARB uniform4fv;

	//: glUniform1iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform1ivARB uniform1iv;

	//: glUniform2iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform2ivARB uniform2iv;

	//: glUniform3iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform3ivARB uniform3iv;

	//: glUniform4iv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniform4ivARB uniform4iv;

	//: glUniformMatrix2fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniformMatrix2fvARB uniformMatrix2fv;

	//: glUniformMatrix3fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniformMatrix3fvARB uniformMatrix3fv;

	//: glUniformMatrix4fv (OpenGL 2.0, ARB_shader_objects)
	ptr_glUniformMatrix4fvARB uniformMatrix4fv;

	//: glGetProgramfv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterfvARB getProgramfv;

	//: glGetProgramiv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterivARB getProgramiv;

	//: glGetShaderfv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterfvARB getShaderfv;

	//: glGetShaderiv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetObjectParameterivARB getShaderiv;

	//: glGetShaderInfoLog (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetInfoLogARB getShaderInfoLog;

	//: glGetProgramInfoLog (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetInfoLogARB getProgramInfoLog;

	//: glGetAttachedObjects (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetAttachedObjectsARB getAttachedShaders;

	//: glGetUniformLocation (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetUniformLocationARB getUniformLocation;

	//: glGetActiveUniform (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetActiveUniformARB getActiveUniform;

	//: glGetUniformfv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetUniformfvARB getUniformfv;

	//: glGetUniformiv (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetUniformivARB getUniformiv;

	//: glGetShaderSource (OpenGL 2.0, ARB_shader_objects)
	ptr_glGetShaderSourceARB getShaderSource;

	//: glVertexAttrib1f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1fARB vertexAttrib1f;

	//: glVertexAttrib1s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1sARB vertexAttrib1s;

	//: glVertexAttrib1d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1dARB vertexAttrib1d;

	//: glVertexAttrib2f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2fARB vertexAttrib2f;

	//: glVertexAttrib2s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2sARB vertexAttrib2s;

	//: glVertexAttrib2d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2dARB vertexAttrib2d;

	//: glVertexAttrib3f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3fARB vertexAttrib3f;

	//: glVertexAttrib3s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3sARB vertexAttrib3s;

	//: glVertexAttrib3d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3dARB vertexAttrib3d;

	//: glVertexAttrib4f (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4fARB vertexAttrib4f;

	//: glVertexAttrib4s (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4sARB vertexAttrib4s;

	//: glVertexAttrib4d (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4dARB vertexAttrib4d;

	//: glVertexAttrib4Nub (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NubARB vertexAttrib4Nub;

	//: glVertexAttrib1fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1fvARB vertexAttrib1fv;

	//: glVertexAttrib1sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1svARB vertexAttrib1sv;

	//: glVertexAttrib1dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib1dvARB vertexAttrib1dv;

	//: glVertexAttrib2fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2fvARB vertexAttrib2fv;

	//: glVertexAttrib2sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2svARB vertexAttrib2sv;

	//: glVertexAttrib2dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib2dvARB vertexAttrib2dv;

	//: glVertexAttrib3fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3fvARB vertexAttrib3fv;

	//: glVertexAttrib3sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3svARB vertexAttrib3sv;

	//: glVertexAttrib3dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib3dvARB vertexAttrib3dv;

	//: glVertexAttrib4fv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4fvARB vertexAttrib4fv;

	//: glVertexAttrib4sv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4svARB vertexAttrib4sv;

	//: glVertexAttrib4dv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4dvARB vertexAttrib4dv;

	//: glVertexAttrib4iv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4ivARB vertexAttrib4iv;

	//: glVertexAttrib4bv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4bvARB vertexAttrib4bv;

	//: glVertexAttrib4ubv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4ubvARB vertexAttrib4ubv;

	//: glVertexAttrib4usv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4usvARB vertexAttrib4usv;

	//: glVertexAttrib4uiv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4uivARB vertexAttrib4uiv;

	//: glVertexAttrib4Nbv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NbvARB vertexAttrib4Nbv;

	//: glVertexAttrib4Nsv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NsvARB vertexAttrib4Nsv;

	//: glVertexAttrib4Niv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NivARB vertexAttrib4Niv;

	//: glVertexAttrib4Nubv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NubvARB vertexAttrib4Nubv;

	//: glVertexAttrib4Nusv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NusvARB vertexAttrib4Nusv;

	//: glVertexAttrib4Nuiv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttrib4NuivARB vertexAttrib4Nuiv;

	//: glVertexAttribPointer (OpenGL 2.0, ARB_vertex_shader)
	ptr_glVertexAttribPointerARB vertexAttribPointer;

	//: glEnableVertexAttribArray (OpenGL 2.0, ARB_vertex_shader)
	ptr_glEnableVertexAttribArrayARB enableVertexAttribArray;

	//: glDisableVertexAttribArray (OpenGL 2.0, ARB_vertex_shader)
	ptr_glDisableVertexAttribArrayARB disableVertexAttribArray;

	//: glBindAttribLocation (OpenGL 2.0, ARB_vertex_shader)
	ptr_glBindAttribLocationARB bindAttribLocation;

	//: glGetActiveAttrib (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetActiveAttribARB getActiveAttrib;

	//: glGetAttribLocation (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetAttribLocationARB getAttribLocation;

	//: glGetVertexAttribdv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribdvARB getVertexAttribdv;

	//: glGetVertexAttribfv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribfvARB getVertexAttribfv;

	//: glGetVertexAttribiv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribivARB getVertexAttribiv;

	//: glGetVertexAttribPointerv (OpenGL 2.0, ARB_vertex_shader)
	ptr_glGetVertexAttribPointervARB getVertexAttribPointerv;

	//: glDrawBuffers (OpenGL 2.0, ARB_draw_buffers)
	ptr_glDrawBuffersARB drawBuffers;

	//: glStencilFuncSeparate (OpenGL 2.0, ATI_separate_stencil)
	ptr_glStencilFuncSeparateATI stencilFuncSeparate;

	//: glStencilOpSeparate (OpenGL 2.0, ATI_separate_stencil)
	ptr_glStencilOpSeparateATI stencilOpSeparate;

	//: glActiveStencilFace (EXT_stencil_two_side)
	ptr_glActiveStencilFaceEXT activeStencilFaceEXT;
};

#define clAccum CL_OpenGL::functions->accum
#define clAlphaFunc CL_OpenGL::functions->alphaFunc
#define clAreTexturesResident CL_OpenGL::functions->areTexturesResident
#define clArrayElement CL_OpenGL::functions->arrayElement
#define clBegin CL_OpenGL::functions->begin
#define clBindTexture CL_OpenGL::functions->bindTexture
#define clBitmap CL_OpenGL::functions->bitmap
#define clBlendFunc CL_OpenGL::functions->blendFunc
#define clCallList CL_OpenGL::functions->callList
#define clCallLists CL_OpenGL::functions->callLists
#define clClear CL_OpenGL::functions->clear
#define clClearAccum CL_OpenGL::functions->clearAccum
#define clClearColor CL_OpenGL::functions->clearColor
#define clClearDepth CL_OpenGL::functions->clearDepth
#define clClearIndex CL_OpenGL::functions->clearIndex
#define clClearStencil CL_OpenGL::functions->clearStencil
#define clClipPlane CL_OpenGL::functions->clipPlane
#define clColor3b CL_OpenGL::functions->color3b
#define clColor3bv CL_OpenGL::functions->color3bv
#define clColor3d CL_OpenGL::functions->color3d
#define clColor3dv CL_OpenGL::functions->color3dv
#define clColor3f CL_OpenGL::functions->color3f
#define clColor3fv CL_OpenGL::functions->color3fv
#define clColor3i CL_OpenGL::functions->color3i
#define clColor3iv CL_OpenGL::functions->color3iv
#define clColor3s CL_OpenGL::functions->color3s
#define clColor3sv CL_OpenGL::functions->color3sv
#define clColor3ub CL_OpenGL::functions->color3ub
#define clColor3ubv CL_OpenGL::functions->color3ubv
#define clColor3ui CL_OpenGL::functions->color3ui
#define clColor3uiv CL_OpenGL::functions->color3uiv
#define clColor3us CL_OpenGL::functions->color3us
#define clColor3usv CL_OpenGL::functions->color3usv
#define clColor4b CL_OpenGL::functions->color4b
#define clColor4bv CL_OpenGL::functions->color4bv
#define clColor4d CL_OpenGL::functions->color4d
#define clColor4dv CL_OpenGL::functions->color4dv
#define clColor4f CL_OpenGL::functions->color4f
#define clColor4fv CL_OpenGL::functions->color4fv
#define clColor4i CL_OpenGL::functions->color4i
#define clColor4iv CL_OpenGL::functions->color4iv
#define clColor4s CL_OpenGL::functions->color4s
#define clColor4sv CL_OpenGL::functions->color4sv
#define clColor4ub CL_OpenGL::functions->color4ub
#define clColor4ubv CL_OpenGL::functions->color4ubv
#define clColor4ui CL_OpenGL::functions->color4ui
#define clColor4uiv CL_OpenGL::functions->color4uiv
#define clColor4us CL_OpenGL::functions->color4us
#define clColor4usv CL_OpenGL::functions->color4usv
#define clColorMask CL_OpenGL::functions->colorMask
#define clColorMaterial CL_OpenGL::functions->colorMaterial
#define clColorPointer CL_OpenGL::functions->colorPointer
#define clCopyPixels CL_OpenGL::functions->copyPixels
#define clCopyTexImage1D CL_OpenGL::functions->copyTexImage1D
#define clCopyTexImage2D CL_OpenGL::functions->copyTexImage2D
#define clCopyTexSubImage1D CL_OpenGL::functions->copyTexSubImage1D
#define clCopyTexSubImage2D CL_OpenGL::functions->copyTexSubImage2D
#define clCopyTexSubImage3D CL_OpenGL::functions->copyTexSubImage3D
#define clCullFace CL_OpenGL::functions->cullFace
#define clDeleteLists CL_OpenGL::functions->deleteLists
#define clDeleteTextures CL_OpenGL::functions->deleteTextures
#define clDepthFunc CL_OpenGL::functions->depthFunc
#define clDepthMask CL_OpenGL::functions->depthMask
#define clDepthRange CL_OpenGL::functions->depthRange
#define clDisable CL_OpenGL::functions->disable
#define clDisableClientState CL_OpenGL::functions->disableClientState
#define clDrawArrays CL_OpenGL::functions->drawArrays
#define clDrawBuffer CL_OpenGL::functions->drawBuffer
#define clDrawElements CL_OpenGL::functions->drawElements
#define clDrawPixels CL_OpenGL::functions->drawPixels
#define clEdgeFlag CL_OpenGL::functions->edgeFlag
#define clEdgeFlagPointer CL_OpenGL::functions->edgeFlagPointer
#define clEdgeFlagv CL_OpenGL::functions->edgeFlagv
#define clEnable CL_OpenGL::functions->enable
#define clEnableClientState CL_OpenGL::functions->enableClientState
#define clEnd CL_OpenGL::functions->end
#define clEndList CL_OpenGL::functions->endList
#define clEvalCoord1d CL_OpenGL::functions->evalCoord1d
#define clEvalCoord1dv CL_OpenGL::functions->evalCoord1dv
#define clEvalCoord1f CL_OpenGL::functions->evalCoord1f
#define clEvalCoord1fv CL_OpenGL::functions->evalCoord1fv
#define clEvalCoord2d CL_OpenGL::functions->evalCoord2d
#define clEvalCoord2dv CL_OpenGL::functions->evalCoord2dv
#define clEvalCoord2f CL_OpenGL::functions->evalCoord2f
#define clEvalCoord2fv CL_OpenGL::functions->evalCoord2fv
#define clEvalMesh1 CL_OpenGL::functions->evalMesh1
#define clEvalMesh2 CL_OpenGL::functions->evalMesh2
#define clEvalPoint1 CL_OpenGL::functions->evalPoint1
#define clEvalPoint2 CL_OpenGL::functions->evalPoint2
#define clFeedbackBuffer CL_OpenGL::functions->feedbackBuffer
#define clFinish CL_OpenGL::functions->finish
#define clFlush CL_OpenGL::functions->flush
#define clFogf CL_OpenGL::functions->fogf
#define clFogfv CL_OpenGL::functions->fogfv
#define clFogi CL_OpenGL::functions->fogi
#define clFogiv CL_OpenGL::functions->fogiv
#define clFrontFace CL_OpenGL::functions->frontFace
#define clFrustum CL_OpenGL::functions->frustum
#define clGenLists CL_OpenGL::functions->genLists
#define clGenTextures CL_OpenGL::functions->genTextures
#define clGetBooleanv CL_OpenGL::functions->getBooleanv
#define clGetClipPlane CL_OpenGL::functions->getClipPlane
#define clGetDoublev CL_OpenGL::functions->getDoublev
#define clGetError CL_OpenGL::functions->getError
#define clGetFloatv CL_OpenGL::functions->getFloatv
#define clGetIntegerv CL_OpenGL::functions->getIntegerv
#define clGetLightfv CL_OpenGL::functions->getLightfv
#define clGetLightiv CL_OpenGL::functions->getLightiv
#define clGetMapdv CL_OpenGL::functions->getMapdv
#define clGetMapfv CL_OpenGL::functions->getMapfv
#define clGetMapiv CL_OpenGL::functions->getMapiv
#define clGetMaterialfv CL_OpenGL::functions->getMaterialfv
#define clGetMaterialiv CL_OpenGL::functions->getMaterialiv
#define clGetPixelMapfv CL_OpenGL::functions->getPixelMapfv
#define clGetPixelMapuiv CL_OpenGL::functions->getPixelMapuiv
#define clGetPixelMapusv CL_OpenGL::functions->getPixelMapusv
#define clGetPointerv CL_OpenGL::functions->getPointerv
#define clGetPolygonStipple CL_OpenGL::functions->getPolygonStipple
#define clGetString CL_OpenGL::functions->getString
#define clGetTexEnvfv CL_OpenGL::functions->getTexEnvfv
#define clGetTexEnviv CL_OpenGL::functions->getTexEnviv
#define clGetTexGendv CL_OpenGL::functions->getTexGendv
#define clGetTexGenfv CL_OpenGL::functions->getTexGenfv
#define clGetTexGeniv CL_OpenGL::functions->getTexGeniv
#define clGetTexImage CL_OpenGL::functions->getTexImage
#define clGetTexLevelParameterfv CL_OpenGL::functions->getTexLevelParameterfv
#define clGetTexLevelParameteriv CL_OpenGL::functions->getTexLevelParameteriv
#define clGetTexParameterfv CL_OpenGL::functions->getTexParameterfv
#define clGetTexParameteriv CL_OpenGL::functions->getTexParameteriv
#define clHint CL_OpenGL::functions->hint
#define clIndexMask CL_OpenGL::functions->indexMask
#define clIndexPointer CL_OpenGL::functions->indexPointer
#define clIndexd CL_OpenGL::functions->indexd
#define clIndexdv CL_OpenGL::functions->indexdv
#define clIndexf CL_OpenGL::functions->indexf
#define clIndexfv CL_OpenGL::functions->indexfv
#define clIndexi CL_OpenGL::functions->indexi
#define clIndexiv CL_OpenGL::functions->indexiv
#define clIndexs CL_OpenGL::functions->indexs
#define clIndexsv CL_OpenGL::functions->indexsv
#define clIndexub CL_OpenGL::functions->indexub
#define clIndexubv CL_OpenGL::functions->indexubv
#define clInitNames CL_OpenGL::functions->initNames
#define clInterleavedArrays CL_OpenGL::functions->interleavedArrays
#define clIsEnabled CL_OpenGL::functions->isEnabled
#define clIsList CL_OpenGL::functions->isList
#define clIsTexture CL_OpenGL::functions->isTexture
#define clLightModelf CL_OpenGL::functions->lightModelf
#define clLightModelfv CL_OpenGL::functions->lightModelfv
#define clLightModeli CL_OpenGL::functions->lightModeli
#define clLightModeliv CL_OpenGL::functions->lightModeliv
#define clLightf CL_OpenGL::functions->lightf
#define clLightfv CL_OpenGL::functions->lightfv
#define clLighti CL_OpenGL::functions->lighti
#define clLightiv CL_OpenGL::functions->lightiv
#define clLineStipple CL_OpenGL::functions->lineStipple
#define clLineWidth CL_OpenGL::functions->lineWidth
#define clListBase CL_OpenGL::functions->listBase
#define clLoadIdentity CL_OpenGL::functions->loadIdentity
#define clLoadMatrixd CL_OpenGL::functions->loadMatrixd
#define clLoadMatrixf CL_OpenGL::functions->loadMatrixf
#define clLoadName CL_OpenGL::functions->loadName
#define clLogicOp CL_OpenGL::functions->logicOp
#define clMap1d CL_OpenGL::functions->map1d
#define clMap1f CL_OpenGL::functions->map1f
#define clMap2d CL_OpenGL::functions->map2d
#define clMap2f CL_OpenGL::functions->map2f
#define clMapGrid1d CL_OpenGL::functions->mapGrid1d
#define clMapGrid1f CL_OpenGL::functions->mapGrid1f
#define clMapGrid2d CL_OpenGL::functions->mapGrid2d
#define clMapGrid2f CL_OpenGL::functions->mapGrid2f
#define clMaterialf CL_OpenGL::functions->materialf
#define clMaterialfv CL_OpenGL::functions->materialfv
#define clMateriali CL_OpenGL::functions->materiali
#define clMaterialiv CL_OpenGL::functions->materialiv
#define clMatrixMode CL_OpenGL::functions->matrixMode
#define clMultMatrixd CL_OpenGL::functions->multMatrixd
#define clMultMatrixf CL_OpenGL::functions->multMatrixf
#define clNewList CL_OpenGL::functions->newList
#define clNormal3b CL_OpenGL::functions->normal3b
#define clNormal3bv CL_OpenGL::functions->normal3bv
#define clNormal3d CL_OpenGL::functions->normal3d
#define clNormal3dv CL_OpenGL::functions->normal3dv
#define clNormal3f CL_OpenGL::functions->normal3f
#define clNormal3fv CL_OpenGL::functions->normal3fv
#define clNormal3i CL_OpenGL::functions->normal3i
#define clNormal3iv CL_OpenGL::functions->normal3iv
#define clNormal3s CL_OpenGL::functions->normal3s
#define clNormal3sv CL_OpenGL::functions->normal3sv
#define clNormalPointer CL_OpenGL::functions->normalPointer
#define clOrtho CL_OpenGL::functions->ortho
#define clPassThrough CL_OpenGL::functions->passThrough
#define clPixelMapfv CL_OpenGL::functions->pixelMapfv
#define clPixelMapuiv CL_OpenGL::functions->pixelMapuiv
#define clPixelMapusv CL_OpenGL::functions->pixelMapusv
#define clPixelStoref CL_OpenGL::functions->pixelStoref
#define clPixelStorei CL_OpenGL::functions->pixelStorei
#define clPixelTransferf CL_OpenGL::functions->pixelTransferf
#define clPixelTransferi CL_OpenGL::functions->pixelTransferi
#define clPixelZoom CL_OpenGL::functions->pixelZoom
#define clPointSize CL_OpenGL::functions->pointSize
#define clPolygonMode CL_OpenGL::functions->polygonMode
#define clPolygonOffset CL_OpenGL::functions->polygonOffset
#define clPolygonStipple CL_OpenGL::functions->polygonStipple
#define clPopAttrib CL_OpenGL::functions->popAttrib
#define clPopClientAttrib CL_OpenGL::functions->popClientAttrib
#define clPopMatrix CL_OpenGL::functions->popMatrix
#define clPopName CL_OpenGL::functions->popName
#define clPrioritizeTextures CL_OpenGL::functions->prioritizeTextures
#define clPushAttrib CL_OpenGL::functions->pushAttrib
#define clPushClientAttrib CL_OpenGL::functions->pushClientAttrib
#define clPushMatrix CL_OpenGL::functions->pushMatrix
#define clPushName CL_OpenGL::functions->pushName
#define clRasterPos2d CL_OpenGL::functions->rasterPos2d
#define clRasterPos2dv CL_OpenGL::functions->rasterPos2dv
#define clRasterPos2f CL_OpenGL::functions->rasterPos2f
#define clRasterPos2fv CL_OpenGL::functions->rasterPos2fv
#define clRasterPos2i CL_OpenGL::functions->rasterPos2i
#define clRasterPos2iv CL_OpenGL::functions->rasterPos2iv
#define clRasterPos2s CL_OpenGL::functions->rasterPos2s
#define clRasterPos2sv CL_OpenGL::functions->rasterPos2sv
#define clRasterPos3d CL_OpenGL::functions->rasterPos3d
#define clRasterPos3dv CL_OpenGL::functions->rasterPos3dv
#define clRasterPos3f CL_OpenGL::functions->rasterPos3f
#define clRasterPos3fv CL_OpenGL::functions->rasterPos3fv
#define clRasterPos3i CL_OpenGL::functions->rasterPos3i
#define clRasterPos3iv CL_OpenGL::functions->rasterPos3iv
#define clRasterPos3s CL_OpenGL::functions->rasterPos3s
#define clRasterPos3sv CL_OpenGL::functions->rasterPos3sv
#define clRasterPos4d CL_OpenGL::functions->rasterPos4d
#define clRasterPos4dv CL_OpenGL::functions->rasterPos4dv
#define clRasterPos4f CL_OpenGL::functions->rasterPos4f
#define clRasterPos4fv CL_OpenGL::functions->rasterPos4fv
#define clRasterPos4i CL_OpenGL::functions->rasterPos4i
#define clRasterPos4iv CL_OpenGL::functions->rasterPos4iv
#define clRasterPos4s CL_OpenGL::functions->rasterPos4s
#define clRasterPos4sv CL_OpenGL::functions->rasterPos4sv
#define clReadBuffer CL_OpenGL::functions->readBuffer
#define clReadPixels CL_OpenGL::functions->readPixels
#define clRectd CL_OpenGL::functions->rectd
#define clRectdv CL_OpenGL::functions->rectdv
#define clRectf CL_OpenGL::functions->rectf
#define clRectfv CL_OpenGL::functions->rectfv
#define clRecti CL_OpenGL::functions->recti
#define clRectiv CL_OpenGL::functions->rectiv
#define clRects CL_OpenGL::functions->rects
#define clRectsv CL_OpenGL::functions->rectsv
#define clRenderMode CL_OpenGL::functions->renderMode
#define clRotated CL_OpenGL::functions->rotated
#define clRotatef CL_OpenGL::functions->rotatef
#define clScaled CL_OpenGL::functions->scaled
#define clScalef CL_OpenGL::functions->scalef
#define clScissor CL_OpenGL::functions->scissor
#define clSelectBuffer CL_OpenGL::functions->selectBuffer
#define clShadeModel CL_OpenGL::functions->shadeModel
#define clStencilFunc CL_OpenGL::functions->stencilFunc
#define clStencilMask CL_OpenGL::functions->stencilMask
#define clStencilOp CL_OpenGL::functions->stencilOp
#define clTexCoord1d CL_OpenGL::functions->texCoord1d
#define clTexCoord1dv CL_OpenGL::functions->texCoord1dv
#define clTexCoord1f CL_OpenGL::functions->texCoord1f
#define clTexCoord1fv CL_OpenGL::functions->texCoord1fv
#define clTexCoord1i CL_OpenGL::functions->texCoord1i
#define clTexCoord1iv CL_OpenGL::functions->texCoord1iv
#define clTexCoord1s CL_OpenGL::functions->texCoord1s
#define clTexCoord1sv CL_OpenGL::functions->texCoord1sv
#define clTexCoord2d CL_OpenGL::functions->texCoord2d
#define clTexCoord2dv CL_OpenGL::functions->texCoord2dv
#define clTexCoord2f CL_OpenGL::functions->texCoord2f
#define clTexCoord2fv CL_OpenGL::functions->texCoord2fv
#define clTexCoord2i CL_OpenGL::functions->texCoord2i
#define clTexCoord2iv CL_OpenGL::functions->texCoord2iv
#define clTexCoord2s CL_OpenGL::functions->texCoord2s
#define clTexCoord2sv CL_OpenGL::functions->texCoord2sv
#define clTexCoord3d CL_OpenGL::functions->texCoord3d
#define clTexCoord3dv CL_OpenGL::functions->texCoord3dv
#define clTexCoord3f CL_OpenGL::functions->texCoord3f
#define clTexCoord3fv CL_OpenGL::functions->texCoord3fv
#define clTexCoord3i CL_OpenGL::functions->texCoord3i
#define clTexCoord3iv CL_OpenGL::functions->texCoord3iv
#define clTexCoord3s CL_OpenGL::functions->texCoord3s
#define clTexCoord3sv CL_OpenGL::functions->texCoord3sv
#define clTexCoord4d CL_OpenGL::functions->texCoord4d
#define clTexCoord4dv CL_OpenGL::functions->texCoord4dv
#define clTexCoord4f CL_OpenGL::functions->texCoord4f
#define clTexCoord4fv CL_OpenGL::functions->texCoord4fv
#define clTexCoord4i CL_OpenGL::functions->texCoord4i
#define clTexCoord4iv CL_OpenGL::functions->texCoord4iv
#define clTexCoord4s CL_OpenGL::functions->texCoord4s
#define clTexCoord4sv CL_OpenGL::functions->texCoord4sv
#define clTexCoordPointer CL_OpenGL::functions->texCoordPointer
#define clTexEnvf CL_OpenGL::functions->texEnvf
#define clTexEnvfv CL_OpenGL::functions->texEnvfv
#define clTexEnvi CL_OpenGL::functions->texEnvi
#define clTexEnviv CL_OpenGL::functions->texEnviv
#define clTexGend CL_OpenGL::functions->texGend
#define clTexGendv CL_OpenGL::functions->texGendv
#define clTexGenf CL_OpenGL::functions->texGenf
#define clTexGenfv CL_OpenGL::functions->texGenfv
#define clTexGeni CL_OpenGL::functions->texGeni
#define clTexGeniv CL_OpenGL::functions->texGeniv
#define clTexImage1D CL_OpenGL::functions->texImage1D
#define clTexImage2D CL_OpenGL::functions->texImage2D
#define clTexParameterf CL_OpenGL::functions->texParameterf
#define clTexParameterfv CL_OpenGL::functions->texParameterfv
#define clTexParameteri CL_OpenGL::functions->texParameteri
#define clTexParameteriv CL_OpenGL::functions->texParameteriv
#define clTexSubImage1D CL_OpenGL::functions->texSubImage1D
#define clTexSubImage2D CL_OpenGL::functions->texSubImage2D
#define clTexSubImage3D CL_OpenGL::functions->texSubImage3D
#define clTranslated CL_OpenGL::functions->translated
#define clTranslatef CL_OpenGL::functions->translatef
#define clVertex2d CL_OpenGL::functions->vertex2d
#define clVertex2dv CL_OpenGL::functions->vertex2dv
#define clVertex2f CL_OpenGL::functions->vertex2f
#define clVertex2fv CL_OpenGL::functions->vertex2fv
#define clVertex2i CL_OpenGL::functions->vertex2i
#define clVertex2iv CL_OpenGL::functions->vertex2iv
#define clVertex2s CL_OpenGL::functions->vertex2s
#define clVertex2sv CL_OpenGL::functions->vertex2sv
#define clVertex3d CL_OpenGL::functions->vertex3d
#define clVertex3dv CL_OpenGL::functions->vertex3dv
#define clVertex3f CL_OpenGL::functions->vertex3f
#define clVertex3fv CL_OpenGL::functions->vertex3fv
#define clVertex3i CL_OpenGL::functions->vertex3i
#define clVertex3iv CL_OpenGL::functions->vertex3iv
#define clVertex3s CL_OpenGL::functions->vertex3s
#define clVertex3sv CL_OpenGL::functions->vertex3sv
#define clVertex4d CL_OpenGL::functions->vertex4d
#define clVertex4dv CL_OpenGL::functions->vertex4dv
#define clVertex4f CL_OpenGL::functions->vertex4f
#define clVertex4fv CL_OpenGL::functions->vertex4fv
#define clVertex4i CL_OpenGL::functions->vertex4i
#define clVertex4iv CL_OpenGL::functions->vertex4iv
#define clVertex4s CL_OpenGL::functions->vertex4s
#define clVertex4sv CL_OpenGL::functions->vertex4sv
#define clVertexPointer CL_OpenGL::functions->vertexPointer
#define clViewport CL_OpenGL::functions->viewport
#define clDrawRangeElements CL_OpenGL::functions->drawRangeElements
#define clTexImage3D CL_OpenGL::functions->texImage3D
#define clColorTable CL_OpenGL::functions->colorTable
#define clCopyColorTable CL_OpenGL::functions->copyColorTable
#define clColorTableParameteriv CL_OpenGL::functions->colorTableParameteriv
#define clColorTableParameterfv CL_OpenGL::functions->colorTableParameterfv
#define clGetColorTable CL_OpenGL::functions->getColorTable
#define clGetColorTableParameteriv CL_OpenGL::functions->getColorTableParameteriv
#define clGetColorTableParameterfv CL_OpenGL::functions->getColorTableParameterfv
#define clColorSubTable CL_OpenGL::functions->colorSubTable
#define clCopyColorSubTable CL_OpenGL::functions->copyColorSubTable
#define clConvolutionFilter1D CL_OpenGL::functions->convolutionFilter1D
#define clConvolutionFilter2D CL_OpenGL::functions->convolutionFilter2D
#define clCopyConvolutionFilter1D CL_OpenGL::functions->copyConvolutionFilter1D
#define clCopyConvolutionFilter2D CL_OpenGL::functions->copyConvolutionFilter2D
#define clGetConvolutionFilter CL_OpenGL::functions->getConvolutionFilter
#define clSeparableFilter2D CL_OpenGL::functions->separableFilter2D
#define clGetSeparableFilter CL_OpenGL::functions->getSeparableFilter
#define clConvolutionParameteri CL_OpenGL::functions->convolutionParameteri
#define clConvolutionParameteriv CL_OpenGL::functions->convolutionParameteriv
#define clConvolutionParameterf CL_OpenGL::functions->convolutionParameterf
#define clConvolutionParameterfv CL_OpenGL::functions->convolutionParameterfv
#define clGetConvolutionParameteriv CL_OpenGL::functions->getConvolutionParameteriv
#define clGetConvolutionParameterfv CL_OpenGL::functions->getConvolutionParameterfv
#define clHistogram CL_OpenGL::functions->histogram
#define clResetHistogram CL_OpenGL::functions->resetHistogram
#define clGetHistogram CL_OpenGL::functions->getHistogram
#define clGetHistogramParameteriv CL_OpenGL::functions->getHistogramParameteriv
#define clGetHistogramParameterfv CL_OpenGL::functions->getHistogramParameterfv
#define clMinmax CL_OpenGL::functions->minmax
#define clResetMinmax CL_OpenGL::functions->resetMinmax
#define clGetMinmax CL_OpenGL::functions->getMinmax
#define clGetMinmaxParameteriv CL_OpenGL::functions->getMinmaxParameteriv
#define clGetMinmaxParameterfv CL_OpenGL::functions->getMinmaxParameterfv
#define clBlendColor CL_OpenGL::functions->blendColor
#define clBlendEquation CL_OpenGL::functions->blendEquation
#define clActiveTexture CL_OpenGL::functions->activeTexture
#define clClientActiveTexture CL_OpenGL::functions->clientActiveTexture
#define clMultiTexCoord1d CL_OpenGL::functions->multiTexCoord1d
#define clMultiTexCoord1dv CL_OpenGL::functions->multiTexCoord1dv
#define clMultiTexCoord1f CL_OpenGL::functions->multiTexCoord1f
#define clMultiTexCoord1fv CL_OpenGL::functions->multiTexCoord1fv
#define clMultiTexCoord1i CL_OpenGL::functions->multiTexCoord1i
#define clMultiTexCoord1iv CL_OpenGL::functions->multiTexCoord1iv
#define clMultiTexCoord1s CL_OpenGL::functions->multiTexCoord1s
#define clMultiTexCoord1sv CL_OpenGL::functions->multiTexCoord1sv
#define clMultiTexCoord2d CL_OpenGL::functions->multiTexCoord2d
#define clMultiTexCoord2dv CL_OpenGL::functions->multiTexCoord2dv
#define clMultiTexCoord2f CL_OpenGL::functions->multiTexCoord2f
#define clMultiTexCoord2fv CL_OpenGL::functions->multiTexCoord2fv
#define clMultiTexCoord2i CL_OpenGL::functions->multiTexCoord2i
#define clMultiTexCoord2iv CL_OpenGL::functions->multiTexCoord2iv
#define clMultiTexCoord2s CL_OpenGL::functions->multiTexCoord2s
#define clMultiTexCoord2sv CL_OpenGL::functions->multiTexCoord2sv
#define clMultiTexCoord3d CL_OpenGL::functions->multiTexCoord3d
#define clMultiTexCoord3dv CL_OpenGL::functions->multiTexCoord3dv
#define clMultiTexCoord3f CL_OpenGL::functions->multiTexCoord3f
#define clMultiTexCoord3fv CL_OpenGL::functions->multiTexCoord3fv
#define clMultiTexCoord3i CL_OpenGL::functions->multiTexCoord3i
#define clMultiTexCoord3iv CL_OpenGL::functions->multiTexCoord3iv
#define clMultiTexCoord3s CL_OpenGL::functions->multiTexCoord3s
#define clMultiTexCoord3sv CL_OpenGL::functions->multiTexCoord3sv
#define clMultiTexCoord4d CL_OpenGL::functions->multiTexCoord4d
#define clMultiTexCoord4dv CL_OpenGL::functions->multiTexCoord4dv
#define clMultiTexCoord4f CL_OpenGL::functions->multiTexCoord4f
#define clMultiTexCoord4fv CL_OpenGL::functions->multiTexCoord4fv
#define clMultiTexCoord4i CL_OpenGL::functions->multiTexCoord4i
#define clMultiTexCoord4iv CL_OpenGL::functions->multiTexCoord4iv
#define clMultiTexCoord4s CL_OpenGL::functions->multiTexCoord4s
#define clMultiTexCoord4sv CL_OpenGL::functions->multiTexCoord4sv
#define clCompressedTexImage1D CL_OpenGL::functions->compressedTexImage1D
#define clCompressedTexImage2D CL_OpenGL::functions->compressedTexImage2D
#define clCompressedTexImage3D CL_OpenGL::functions->compressedTexImage3D
#define clCompressedTexSubImage1D CL_OpenGL::functions->compressedTexSubImage1D
#define clCompressedTexSubImage2D CL_OpenGL::functions->compressedTexSubImage2D
#define clCompressedTexSubImage3D CL_OpenGL::functions->compressedTexSubImage3D
#define clGetCompressedTexImage CL_OpenGL::functions->getCompressedTexImage
#define clSampleCoverage CL_OpenGL::functions->sampleCoverage
#define clLoadTransposeMatrixd CL_OpenGL::functions->glLoadTransposeMatrixd
#define clLoadTransposeMatrixf CL_OpenGL::functions->glLoadTransposeMatrixf
#define clMultTransposeMatrixd CL_OpenGL::functions->glMultTransposeMatrixd
#define clMultTransposeMatrixf CL_OpenGL::functions->glMultTransposeMatrixf
#define clFogCoordd CL_OpenGL::functions->fogCoordd
#define clFogCoorddv CL_OpenGL::functions->fogCoorddv
#define clFogCoordf CL_OpenGL::functions->fogCoordf
#define clFogCoordfv CL_OpenGL::functions->fogCoordfv
#define clFogCoordPointer CL_OpenGL::functions->fogCoordPointer
#define clMultiDrawArrays CL_OpenGL::functions->multiDrawArrays
#define clMultiDrawElements CL_OpenGL::functions->multiDrawElementsEXT
#define clPointParameterf CL_OpenGL::functions->pointParameterf
#define clPointParameterfv CL_OpenGL::functions->pointParameterfv
#define clSecondaryColor3b CL_OpenGL::functions->secondaryColor3b
#define clSecondaryColor3bv CL_OpenGL::functions->secondaryColor3bv
#define clSecondaryColor3d CL_OpenGL::functions->secondaryColor3d
#define clSecondaryColor3dv CL_OpenGL::functions->secondaryColor3dv
#define clSecondaryColor3f CL_OpenGL::functions->secondaryColor3f
#define clSecondaryColor3fv CL_OpenGL::functions->secondaryColor3fv
#define clSecondaryColor3i CL_OpenGL::functions->secondaryColor3i
#define clSecondaryColor3iv CL_OpenGL::functions->secondaryColor3iv
#define clSecondaryColor3s CL_OpenGL::functions->secondaryColor3s
#define clSecondaryColor3sv CL_OpenGL::functions->secondaryColor3sv
#define clSecondaryColor3ub CL_OpenGL::functions->secondaryColor3ub
#define clSecondaryColor3ubv CL_OpenGL::functions->secondaryColor3ubv
#define clSecondaryColor3ui CL_OpenGL::functions->secondaryColor3ui
#define clSecondaryColor3uiv CL_OpenGL::functions->secondaryColor3uiv
#define clSecondaryColor3us CL_OpenGL::functions->secondaryColor3us
#define clSecondaryColor3usv CL_OpenGL::functions->secondaryColor3usv
#define clSecondaryColorPointer CL_OpenGL::functions->secondaryColorPointer
#define clBlendFuncSeparate CL_OpenGL::functions->blendFuncSeparate
#define clWindowPos2d CL_OpenGL::functions->windowPos2d
#define clWindowPos2dv CL_OpenGL::functions->windowPos2dv
#define clWindowPos2f CL_OpenGL::functions->windowPos2f
#define clWindowPos2fv CL_OpenGL::functions->windowPos2fv
#define clWindowPos2i CL_OpenGL::functions->windowPos2i
#define clWindowPos2iv CL_OpenGL::functions->windowPos2iv
#define clWindowPos2s CL_OpenGL::functions->windowPos2s
#define clWindowPos2sv CL_OpenGL::functions->windowPos2sv
#define clWindowPos3d CL_OpenGL::functions->windowPos3d
#define clWindowPos3dv CL_OpenGL::functions->windowPos3dv
#define clWindowPos3f CL_OpenGL::functions->windowPos3f
#define clWindowPos3fv CL_OpenGL::functions->windowPos3fv
#define clWindowPos3i CL_OpenGL::functions->windowPos3i
#define clWindowPos3iv CL_OpenGL::functions->windowPos3iv
#define clWindowPos3s CL_OpenGL::functions->windowPos3s
#define clWindowPos3sv CL_OpenGL::functions->windowPos3sv
#define clBindBuffer CL_OpenGL::functions->bindBuffer
#define clDeleteBuffers CL_OpenGL::functions->deleteBuffers
#define clGenBuffers CL_OpenGL::functions->genBuffers
#define clIsBuffer CL_OpenGL::functions->isBuffer
#define clBufferData CL_OpenGL::functions->bufferData
#define clBufferSubData CL_OpenGL::functions->bufferSubData
#define clGetBufferSubData CL_OpenGL::functions->getBufferSubData
#define clMapBuffer CL_OpenGL::functions->mapBuffer
#define clUnmapBuffer CL_OpenGL::functions->unmapBuffer
#define clGetBufferParameteriv CL_OpenGL::functions->getBufferParameteriv
#define clGetBufferPointerv CL_OpenGL::functions->getBufferPointerv
#define clGenQueries CL_OpenGL::functions->genQueries
#define clDeleteQueries CL_OpenGL::functions->deleteQueries
#define clIsQuery CL_OpenGL::functions->isQuery
#define clBeginQuery CL_OpenGL::functions->beginQuery
#define clEndQuery CL_OpenGL::functions->endQuery
#define clGetQueryiv CL_OpenGL::functions->getQueryiv
#define clGetQueryObjectiv CL_OpenGL::functions->getQueryObjectiv
#define clGenQueryObjectuiv CL_OpenGL::functions->getQueryObjectuiv
#define clDeleteShader CL_OpenGL::functions->deleteShader
#define clDeleteProgram CL_OpenGL::functions->deleteProgram
#define clGetHandle CL_OpenGL::functions->getHandle
#define clDetachShader CL_OpenGL::functions->detachShader
#define clCreateShader CL_OpenGL::functions->createShader
#define clShaderSource CL_OpenGL::functions->shaderSource
#define clCompileShader CL_OpenGL::functions->compileShader
#define clCreateProgram CL_OpenGL::functions->createProgram
#define clAttachShader CL_OpenGL::functions->attachShader
#define clLinkProgram CL_OpenGL::functions->linkProgram
#define clUseProgram CL_OpenGL::functions->useProgram
#define clValidateProgram CL_OpenGL::functions->validateProgram
#define clUniform1f CL_OpenGL::functions->uniform1f
#define clUniform2f CL_OpenGL::functions->uniform2f
#define clUniform3f CL_OpenGL::functions->uniform3f
#define clUniform4f CL_OpenGL::functions->uniform4f
#define clUniform1i CL_OpenGL::functions->uniform1i
#define clUniform2i CL_OpenGL::functions->uniform2i
#define clUniform3i CL_OpenGL::functions->uniform3i
#define clUniform4i CL_OpenGL::functions->uniform4i
#define clUniform1fv CL_OpenGL::functions->uniform1fv
#define clUniform2fv CL_OpenGL::functions->uniform2fv
#define clUniform3fv CL_OpenGL::functions->uniform3fv
#define clUniform4fv CL_OpenGL::functions->uniform4fv
#define clUniform1iv CL_OpenGL::functions->uniform1iv
#define clUniform2iv CL_OpenGL::functions->uniform2iv
#define clUniform3iv CL_OpenGL::functions->uniform3iv
#define clUniform4iv CL_OpenGL::functions->uniform4iv
#define clUniformMatrix2fv CL_OpenGL::functions->uniformMatrix2fv
#define clUniformMatrix3fv CL_OpenGL::functions->uniformMatrix3fv
#define clUniformMatrix4fv CL_OpenGL::functions->uniformMatrix4fv
#define clGetProgramfv CL_OpenGL::functions->getProgramfv
#define clGetProgramiv CL_OpenGL::functions->getProgramiv
#define clGetShaderfv CL_OpenGL::functions->getShaderfv
#define clGetShaderiv CL_OpenGL::functions->getShaderiv
#define clGetShaderInfoLog CL_OpenGL::functions->getShaderInfoLog
#define clGetProgramInfoLog CL_OpenGL::functions->getProgramInfoLog
#define clGetAttachedShaders CL_OpenGL::functions->getAttachedShaders
#define clGetUniformLocation CL_OpenGL::functions->getUniformLocation
#define clGetActiveUniform CL_OpenGL::functions->getActiveUniform
#define clGetUniformfv CL_OpenGL::functions->getUniformfv
#define clGetUniformiv CL_OpenGL::functions->getUniformiv
#define clGetShaderSource CL_OpenGL::functions->getShaderSource
#define clVertexAttrib1f CL_OpenGL::functions->vertexAttrib1f
#define clVertexAttrib1s CL_OpenGL::functions->vertexAttrib1s
#define clVertexAttrib1d CL_OpenGL::functions->vertexAttrib1d
#define clVertexAttrib2f CL_OpenGL::functions->vertexAttrib2f
#define clVertexAttrib2s CL_OpenGL::functions->vertexAttrib2s
#define clVertexAttrib2d CL_OpenGL::functions->vertexAttrib2d
#define clVertexAttrib3f CL_OpenGL::functions->vertexAttrib3f
#define clVertexAttrib3s CL_OpenGL::functions->vertexAttrib3s
#define clVertexAttrib3d CL_OpenGL::functions->vertexAttrib3d
#define clVertexAttrib4f CL_OpenGL::functions->vertexAttrib4f
#define clVertexAttrib4s CL_OpenGL::functions->vertexAttrib4s
#define clVertexAttrib4d CL_OpenGL::functions->vertexAttrib4d
#define clVertexAttrib4Nub CL_OpenGL::functions->vertexAttrib4Nub
#define clVertexAttrib1fv CL_OpenGL::functions->vertexAttrib1fv
#define clVertexAttrib1sv CL_OpenGL::functions->vertexAttrib1sv
#define clVertexAttrib1dv CL_OpenGL::functions->vertexAttrib1dv
#define clVertexAttrib2fv CL_OpenGL::functions->vertexAttrib2fv
#define clVertexAttrib2sv CL_OpenGL::functions->vertexAttrib2sv
#define clVertexAttrib2dv CL_OpenGL::functions->vertexAttrib2dv
#define clVertexAttrib3fv CL_OpenGL::functions->vertexAttrib3fv
#define clVertexAttrib3sv CL_OpenGL::functions->vertexAttrib3sv
#define clVertexAttrib3dv CL_OpenGL::functions->vertexAttrib3dv
#define clVertexAttrib4fv CL_OpenGL::functions->vertexAttrib4fv
#define clVertexAttrib4sv CL_OpenGL::functions->vertexAttrib4sv
#define clVertexAttrib4dv CL_OpenGL::functions->vertexAttrib4dv
#define clVertexAttrib4iv CL_OpenGL::functions->vertexAttrib4iv
#define clVertexAttrib4bv CL_OpenGL::functions->vertexAttrib4bv
#define clVertexAttrib4ubv CL_OpenGL::functions->vertexAttrib4ubv
#define clVertexAttrib4usv CL_OpenGL::functions->vertexAttrib4usv
#define clVertexAttrib4uiv CL_OpenGL::functions->vertexAttrib4uiv
#define clVertexAttrib4Nbv CL_OpenGL::functions->vertexAttrib4Nbv
#define clVertexAttrib4Nsv CL_OpenGL::functions->vertexAttrib4Nsv
#define clVertexAttrib4Niv CL_OpenGL::functions->vertexAttrib4Niv
#define clVertexAttrib4Nubv CL_OpenGL::functions->vertexAttrib4Nubv
#define clVertexAttrib4Nusv CL_OpenGL::functions->vertexAttrib4Nusv
#define clVertexAttrib4Nuiv CL_OpenGL::functions->vertexAttrib4Nuiv
#define clVertexAttribPointer CL_OpenGL::functions->vertexAttribPointer
#define clEnableVertexAttribArray CL_OpenGL::functions->enableVertexAttribArray
#define clDisableVertexAttribArray CL_OpenGL::functions->disableVertexAttribArray
#define clBindAttribLocation CL_OpenGL::functions->bindAttribLocation
#define clGetActiveAttrib CL_OpenGL::functions->getActiveAttrib
#define clGetAttribLocation CL_OpenGL::functions->getAttribLocation
#define clGetVertexAttribdv CL_OpenGL::functions->getVertexAttribdv
#define clGetVertexAttribfv CL_OpenGL::functions->getVertexAttribfv
#define clGetVertexAttribiv CL_OpenGL::functions->getVertexAttribiv
#define clGetVertexAttribPointerv CL_OpenGL::functions->getVertexAttribPointerv
#define clDrawBuffers CL_OpenGL::functions->drawBuffers
#define clStencilFuncSeparate CL_OpenGL::functions->stencilFuncSeparate
#define clStencilOpSeparate CL_OpenGL::functions->stencilOpSeparate

#endif
