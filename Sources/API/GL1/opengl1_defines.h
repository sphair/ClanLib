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
*/

/// \addtogroup clanGL1_Display clanGL1 Display
/// \{

#pragma once
#include "api_gl1.h"

#ifndef CL_DISPLAYDEFINES_DEFINES
#define CL_DISPLAYDEFINES_DEFINES

#include <cstddef>

#ifndef __APPLE__
// Win32 and linux
typedef unsigned int CLenum;
typedef unsigned char CLboolean;
typedef unsigned int CLbitfield;
typedef signed char CLbyte;
typedef short CLshort;
typedef int CLint;
typedef int CLsizei;
typedef unsigned char CLubyte;
typedef unsigned short CLushort;
typedef unsigned int CLuint;
typedef float CLfloat;
typedef float CLclampf;
typedef double CLdouble;
typedef double CLclampd;
typedef void CLvoid;
typedef char CLchar;
typedef ptrdiff_t CLintptr;
typedef ptrdiff_t CLsizeiptr;

#else
typedef unsigned long CLenum;
typedef unsigned char CLboolean;
typedef unsigned long CLbitfield;
typedef signed char CLbyte;
typedef short CLshort;
typedef long CLint;
typedef long CLsizei;
typedef unsigned char CLubyte;
typedef unsigned short CLushort;
typedef unsigned long CLuint;
typedef float CLfloat;
typedef float CLclampf;
typedef double CLdouble;
typedef double CLclampd;
typedef void CLvoid;
typedef char CLchar;
typedef ptrdiff_t CLintptr;
typedef ptrdiff_t CLsizeiptr;
#endif

/// \brief Display Defines
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_DisplayDefines
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

	CL_LOGIC_OP                       = 0x0BF1,

	// BlendingFactorDest:

	CL_ZERO                           = 0,
	CL_ONE                            = 1,
	CL_SRC_COLOR                      = 0x0300,
	CL_ONE_MINUS_SRC_COLOR            = 0x0301,
	CL_SRC_ALPHA                      = 0x0302,
	CL_ONE_MINUS_SRC_ALPHA            = 0x0303,
	CL_DST_ALPHA                      = 0x0304,
	CL_ONE_MINUS_DST_ALPHA            = 0x0305,

	// BlendingFactorSrc:

	CL_DST_COLOR                      = 0x0306,
	CL_ONE_MINUS_DST_COLOR            = 0x0307,
	CL_SRC_ALPHA_SATURATE             = 0x0308,

	// Boolean:

	CL_TRUE                           = 1,
	CL_FALSE                          = 0,

	// ClipPlaneName:

	CL_CLIP_PLANE0                    = 0x3000,
	CL_CLIP_PLANE1                    = 0x3001,
	CL_CLIP_PLANE2                    = 0x3002,
	CL_CLIP_PLANE3                    = 0x3003,
	CL_CLIP_PLANE4                    = 0x3004,
	CL_CLIP_PLANE5                    = 0x3005,

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

	// FrontFaceDirection:

	CL_CW                           = 0x0900,
	CL_CCW                          = 0x0901,

	// GetMapTarget:

	CL_COEFF                        = 0x0A00,
	CL_ORDER                        = 0x0A01,
	CL_DOMAIN                       = 0x0A02,

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

	// GetTextureParameter:

	CL_TEXTURE_WIDTH                = 0x1000,
	CL_TEXTURE_HEIGHT               = 0x1001,
	CL_TEXTURE_INTERNAL_FORMAT      = 0x1003,
	CL_TEXTURE_BORDER_COLOR         = 0x1004,
	CL_TEXTURE_BORDER               = 0x1005,

	// HintMode:

	CL_DONT_CARE                    = 0x1100,
	CL_FASTEST                      = 0x1101,
	CL_NICEST                       = 0x1102,

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

	// ListMode:

	CL_COMPILE                      = 0x1300,
	CL_COMPILE_AND_EXECUTE          = 0x1301,

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

	// MaterialParameter:

	CL_EMISSION                     = 0x1600,
	CL_SHININESS                    = 0x1601,
	CL_AMBIENT_AND_DIFFUSE          = 0x1602,
	CL_COLOR_INDEXES                = 0x1603,

	// MatrixMode:

	CL_MODELVIEW                    = 0x1700,
	CL_PROJECTION                   = 0x1701,
	CL_TEXTURE                      = 0x1702,

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

	// PixelType:

	CL_BITMAP                       = 0x1A00,
	
	// PolygonMode:

	CL_POINT                        = 0x1B00,
	CL_LINE                         = 0x1B01,
	CL_FILL                         = 0x1B02,

	// RenderingMode:

	CL_RENDER                       = 0x1C00,
	CL_FEEDBACK                     = 0x1C01,
	CL_SELECT                       = 0x1C02,

	// ShadingModel:

	CL_FLAT                         = 0x1D00,
	CL_SMOOTH                       = 0x1D01,

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

	CL_NEAREST_MIPMAP_NEAREST       = 0x2700,
	CL_LINEAR_MIPMAP_NEAREST        = 0x2701,
	CL_NEAREST_MIPMAP_LINEAR        = 0x2702,
	CL_LINEAR_MIPMAP_LINEAR         = 0x2703,

	// TextureParameterName:

	CL_TEXTURE_MAG_FILTER           = 0x2800,
	CL_TEXTURE_MIN_FILTER           = 0x2801,
	CL_TEXTURE_WRAP_S               = 0x2802,
	CL_TEXTURE_WRAP_T               = 0x2803,

	// TextureWrapMode:

	CL_CLAMP                        = 0x2900,
	CL_REPEAT                       = 0x2901,

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
	CL_POINT_SIZE_MAX              = 0x8127,
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
	CL_CURRENT_PROGRAM                     = 0x8B8D,
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
	CL_FLOAT_VEC2                          = 0x8B50,
	CL_FLOAT_VEC3                          = 0x8B51,
	CL_FLOAT_VEC4                          = 0x8B52,

	// ARB_fragment_shader:

	CL_FRAGMENT_SHADER                     = 0x8B30,
	CL_MAX_FRAGMENT_UNIFORM_COMPONENTS     = 0x8B49,
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
	CL_POINT_SPRITE_COORD_ORIGIN           = 0x8CA0,
	CL_LOWER_LEFT                          = 0x8CA1,
	CL_UPPER_LEFT                          = 0x8CA2,

	// ATI_separate_stencil:

	CL_STENCIL_BACK_FUNC                   = 0x8800,
	CL_STENCIL_BACK_FAIL                   = 0x8801,
	CL_STENCIL_BACK_PASS_DEPTH_FAIL        = 0x8802,
	CL_STENCIL_BACK_PASS_DEPTH_PASS        = 0x8803,

	// EXT_stencil_two_side additional states:

	CL_STENCIL_TEST_TWO_SIDE               = 0x8910,
	CL_ACTIVE_STENCIL_FACE                 = 0x8911,

	// EXT_framebuffer_object:

	CL_FRAMEBUFFER                         = 0x8D40,
	CL_RENDERBUFFER                        = 0x8D41,
	CL_STENCIL_INDEX1                      = 0x8D46,
	CL_STENCIL_INDEX4                      = 0x8D47,
	CL_STENCIL_INDEX8                      = 0x8D48,
	CL_STENCIL_INDEX16                     = 0x8D49,
	CL_RENDERBUFFER_WIDTH                  = 0x8D42,
	CL_RENDERBUFFER_HEIGHT                 = 0x8D43,
	CL_RENDERBUFFER_INTERNAL_FORMAT        = 0x8D44,
	CL_RENDERBUFFER_RED_SIZE               = 0x8D50,
	CL_RENDERBUFFER_GREEN_SIZE             = 0x8D51,
	CL_RENDERBUFFER_BLUE_SIZE              = 0x8D52,
	CL_RENDERBUFFER_ALPHA_SIZE             = 0x8D53,
	CL_RENDERBUFFER_DEPTH_SIZE             = 0x8D54,
	CL_RENDERBUFFER_STENCIL_SIZE           = 0x8D55,
	CL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE              = 0x8CD0,
	CL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME              = 0x8CD1,
	CL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL            = 0x8CD2,
	CL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE    = 0x8CD3,
	CL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET       = 0x8CD4,
	CL_COLOR_ATTACHMENT0                   = 0x8CE0,
	CL_COLOR_ATTACHMENT1                   = 0x8CE1,
	CL_COLOR_ATTACHMENT2                   = 0x8CE2,
	CL_COLOR_ATTACHMENT3                   = 0x8CE3,
	CL_COLOR_ATTACHMENT4                   = 0x8CE4,
	CL_COLOR_ATTACHMENT5                   = 0x8CE5,
	CL_COLOR_ATTACHMENT6                   = 0x8CE6,
	CL_COLOR_ATTACHMENT7                   = 0x8CE7,
	CL_COLOR_ATTACHMENT8                   = 0x8CE8,
	CL_COLOR_ATTACHMENT9                   = 0x8CE9,
	CL_COLOR_ATTACHMENT10                  = 0x8CEA,
	CL_COLOR_ATTACHMENT11                  = 0x8CEB,
	CL_COLOR_ATTACHMENT12                  = 0x8CEC,
	CL_COLOR_ATTACHMENT13                  = 0x8CED,
	CL_COLOR_ATTACHMENT14                  = 0x8CEE,
	CL_COLOR_ATTACHMENT15                  = 0x8CEF,
	CL_DEPTH_ATTACHMENT                    = 0x8D00,
	CL_STENCIL_ATTACHMENT                  = 0x8D20,
	CL_FRAMEBUFFER_COMPLETE                            = 0x8CD5,
	CL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT               = 0x8CD6,
	CL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT       = 0x8CD7,
	CL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS               = 0x8CD9,
	CL_FRAMEBUFFER_INCOMPLETE_FORMATS                  = 0x8CDA,
	CL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER              = 0x8CDB,
	CL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER              = 0x8CDC,
	CL_FRAMEBUFFER_UNSUPPORTED                         = 0x8CDD,
	CL_FRAMEBUFFER_BINDING                 = 0x8CA6,
	CL_RENDERBUFFER_BINDING                = 0x8CA7,
	CL_MAX_COLOR_ATTACHMENTS               = 0x8CDF,
	CL_MAX_RENDERBUFFER_SIZE               = 0x84E8,
	CL_INVALID_FRAMEBUFFER_OPERATION       = 0x0506,

	// EXT_texture_compression_s3tc:

	CL_COMPRESSED_RGB_S3TC_DXT1_EXT        = 0x83F0,
	CL_COMPRESSED_RGBA_S3TC_DXT1_EXT       = 0x83F1,
	CL_COMPRESSED_RGBA_S3TC_DXT3_EXT       = 0x83F2,
	CL_COMPRESSED_RGBA_S3TC_DXT5_EXT       = 0x83F3,

	// EXT_framebuffer_blit:

	CL_READ_FRAMEBUFFER                    = 0x8CA8,
	CL_DRAW_FRAMEBUFFER                    = 0x8CA9,
	CL_DRAW_FRAMEBUFFER_BINDING            = 0x8CA6, // alias CL_FRAMEBUFFER_BINDING
	CL_READ_FRAMEBUFFER_BINDING            = 0x8CAA
};

#endif

/// \}
