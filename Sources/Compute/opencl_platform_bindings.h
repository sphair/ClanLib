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
*/

#pragma once

#include "CL/cl.h"
#include "CL/cl_gl.h"
#ifdef WIN32
#include "CL/cl_d3d11.h"
#endif

namespace clan
{

class OpenCLPlatformBindings
{
public:
	void bind(cl_platform_id platform);

	template<typename FuncType>
	FuncType get_function(int min_major_version, int min_minor_version, const char *name);

	template<typename FuncType>
	FuncType get_extension_function(const char *name);

	typedef cl_int (CL_API_CALL FuncGetDeviceIDs)(
		cl_platform_id   /* platform */,
		cl_device_type   /* device_type */, 
		cl_uint          /* num_entries */, 
		cl_device_id *   /* devices */, 
		cl_uint *        /* num_devices */);

	typedef cl_int (CL_API_CALL FuncGetDeviceInfo)(
		cl_device_id    /* device */,
		cl_device_info  /* param_name */, 
		size_t          /* param_value_size */, 
		void *          /* param_value */,
		size_t *        /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncCreateSubDevices)(
		cl_device_id                         /* in_device */,
		const cl_device_partition_property * /* properties */,
		cl_uint                              /* num_devices */,
		cl_device_id *                       /* out_devices */,
		cl_uint *                            /* num_devices_ret */);

	typedef cl_int (CL_API_CALL FuncRetainDevice)(cl_device_id /* device */);
	typedef cl_int (CL_API_CALL FuncReleaseDevice)(cl_device_id /* device */);
    
	typedef cl_context (CL_API_CALL FuncCreateContext)(
		const cl_context_properties * /* properties */,
		cl_uint                 /* num_devices */,
		const cl_device_id *    /* devices */,
		void (CL_CALLBACK * /* pfn_notify */)(const char *, const void *, size_t, void *),
		void *                  /* user_data */,
		cl_int *                /* errcode_ret */);

	typedef cl_context (CL_API_CALL FuncCreateContextFromType)(
		const cl_context_properties * /* properties */,
		cl_device_type          /* device_type */,
		void (CL_CALLBACK *     /* pfn_notify*/ )(const char *, const void *, size_t, void *),
		void *                  /* user_data */,
		cl_int *                /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncRetainContext)(cl_context /* context */);
	typedef cl_int (CL_API_CALL FuncReleaseContext)(cl_context /* context */);

	typedef cl_int (CL_API_CALL FuncGetContextInfo)(
		cl_context         /* context */, 
		cl_context_info    /* param_name */, 
		size_t             /* param_value_size */, 
		void *             /* param_value */, 
		size_t *           /* param_value_size_ret */);

	typedef cl_command_queue (CL_API_CALL FuncCreateCommandQueue)(
		cl_context                     /* context */, 
		cl_device_id                   /* device */, 
		cl_command_queue_properties    /* properties */,
		cl_int *                       /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncRetainCommandQueue)(cl_command_queue /* command_queue */);
	typedef cl_int (CL_API_CALL FuncReleaseCommandQueue)(cl_command_queue /* command_queue */);

	typedef cl_int (CL_API_CALL FuncGetCommandQueueInfo)(
		cl_command_queue      /* command_queue */,
		cl_command_queue_info /* param_name */,
		size_t                /* param_value_size */,
		void *                /* param_value */,
		size_t *              /* param_value_size_ret */);

	typedef cl_mem (CL_API_CALL FuncCreateBuffer)(
		cl_context   /* context */,
		cl_mem_flags /* flags */,
		size_t       /* size */,
		void *       /* host_ptr */,
		cl_int *     /* errcode_ret */);

	typedef cl_mem (CL_API_CALL FuncCreateSubBuffer)(
		cl_mem                   /* buffer */,
		cl_mem_flags             /* flags */,
		buffer_create_type    /* buffer_create_type */,
		const void *             /* buffer_create_info */,
		cl_int *                 /* errcode_ret */);

	typedef cl_mem (CL_API_CALL FuncCreateImage)(
		cl_context              /* context */,
		cl_mem_flags            /* flags */,
		const cl_image_format * /* image_format */,
		const cl_image_desc *   /* image_desc */, 
		void *                  /* host_ptr */,
		cl_int *                /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncRetainMemObject)(cl_mem /* memobj */);
	typedef cl_int (CL_API_CALL FuncReleaseMemObject)(cl_mem /* memobj */);

	typedef cl_int (CL_API_CALL FuncGetSupportedImageFormats)(
		cl_context           /* context */,
		cl_mem_flags         /* flags */,
		cl_mem_object_type   /* image_type */,
		cl_uint              /* num_entries */,
		cl_image_format *    /* image_formats */,
		cl_uint *            /* num_image_formats */);

	typedef cl_int (CL_API_CALL FuncGetMemObjectInfo)(
		cl_mem           /* memobj */,
		cl_mem_info      /* param_name */, 
		size_t           /* param_value_size */,
		void *           /* param_value */,
		size_t *         /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncGetImageInfo)(
		cl_mem           /* image */,
		cl_image_info    /* param_name */, 
		size_t           /* param_value_size */,
		void *           /* param_value */,
		size_t *         /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncSetMemObjectDestructorCallback)(
		cl_mem /* memobj */, 
		void (CL_CALLBACK * /*pfn_notify*/)( cl_mem /* memobj */, void* /*user_data*/), 
		void * /*user_data */ );

	typedef cl_sampler (CL_API_CALL FuncCreateSampler)(
		cl_context          /* context */,
		cl_bool             /* normalized_coords */, 
		cl_addressing_mode  /* addressing_mode */, 
		cl_filter_mode      /* filter_mode */,
		cl_int *            /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncRetainSampler)(cl_sampler /* sampler */);
	typedef cl_int (CL_API_CALL FuncReleaseSampler)(cl_sampler /* sampler */);

	typedef cl_int (CL_API_CALL FuncGetSamplerInfo)(
		cl_sampler         /* sampler */,
		cl_sampler_info    /* param_name */,
		size_t             /* param_value_size */,
		void *             /* param_value */,
		size_t *           /* param_value_size_ret */);

	typedef cl_program (CL_API_CALL FuncCreateProgramWithSource)(
		cl_context        /* context */,
		cl_uint           /* count */,
		const char **     /* strings */,
		const size_t *    /* lengths */,
		cl_int *          /* errcode_ret */);

	typedef cl_program (CL_API_CALL FuncCreateProgramWithBinary)(
		cl_context                     /* context */,
		cl_uint                        /* num_devices */,
		const cl_device_id *           /* device_list */,
		const size_t *                 /* lengths */,
		const unsigned char **         /* binaries */,
		cl_int *                       /* binary_status */,
		cl_int *                       /* errcode_ret */);

	typedef cl_program (CL_API_CALL FuncCreateProgramWithBuiltInKernels)(
		cl_context            /* context */,
		cl_uint               /* num_devices */,
		const cl_device_id *  /* device_list */,
		const char *          /* kernel_names */,
		cl_int *              /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncRetainProgram)(cl_program /* program */);
	typedef cl_int (CL_API_CALL FuncReleaseProgram)(cl_program /* program */);

	typedef cl_int (CL_API_CALL FuncBuildProgram)(
		cl_program           /* program */,
		cl_uint              /* num_devices */,
		const cl_device_id * /* device_list */,
		const char *         /* options */, 
		void (CL_CALLBACK *  /* pfn_notify */)(cl_program /* program */, void * /* user_data */),
		void *               /* user_data */);

	typedef cl_int (CL_API_CALL FuncCompileProgram)(
		cl_program           /* program */,
		cl_uint              /* num_devices */,
		const cl_device_id * /* device_list */,
		const char *         /* options */, 
		cl_uint              /* num_input_headers */,
		const cl_program *   /* input_headers */,
		const char **        /* header_include_names */,
		void (CL_CALLBACK *  /* pfn_notify */)(cl_program /* program */, void * /* user_data */),
		void *               /* user_data */);

	typedef cl_program (CL_API_CALL FuncLinkProgram)(
		cl_context           /* context */,
		cl_uint              /* num_devices */,
		const cl_device_id * /* device_list */,
		const char *         /* options */, 
		cl_uint              /* num_input_programs */,
		const cl_program *   /* input_programs */,
		void (CL_CALLBACK *  /* pfn_notify */)(cl_program /* program */, void * /* user_data */),
		void *               /* user_data */,
		cl_int *             /* errcode_ret */ );

	typedef cl_int (CL_API_CALL FuncUnloadPlatformCompiler)(cl_platform_id /* platform */);

	typedef cl_int (CL_API_CALL FuncGetProgramInfo)(
		cl_program         /* program */,
		cl_program_info    /* param_name */,
		size_t             /* param_value_size */,
		void *             /* param_value */,
		size_t *           /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncGetProgramBuildInfo)(
		cl_program            /* program */,
		cl_device_id          /* device */,
		cl_program_build_info /* param_name */,
		size_t                /* param_value_size */,
		void *                /* param_value */,
		size_t *              /* param_value_size_ret */);

	typedef cl_kernel (CL_API_CALL FuncCreateKernel)(
		cl_program      /* program */,
		const char *    /* kernel_name */,
		cl_int *        /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncCreateKernelsInProgram)(
		cl_program     /* program */,
		cl_uint        /* num_kernels */,
		cl_kernel *    /* kernels */,
		cl_uint *      /* num_kernels_ret */);

	typedef cl_int (CL_API_CALL FuncRetainKernel)(cl_kernel /* kernel */);
	typedef cl_int (CL_API_CALL FuncReleaseKernel)(cl_kernel /* kernel */);

	typedef cl_int (CL_API_CALL FuncSetKernelArg)(
		cl_kernel    /* kernel */,
		cl_uint      /* arg_index */,
		size_t       /* arg_size */,
		const void * /* arg_value */);

	typedef cl_int (CL_API_CALL FuncGetKernelInfo)(
		cl_kernel       /* kernel */,
		cl_kernel_info  /* param_name */,
		size_t          /* param_value_size */,
		void *          /* param_value */,
		size_t *        /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncGetKernelArgInfo)(
		cl_kernel       /* kernel */,
		cl_uint         /* arg_indx */,
		cl_kernel_arg_info  /* param_name */,
		size_t          /* param_value_size */,
		void *          /* param_value */,
		size_t *        /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncGetKernelWorkGroupInfo)(
		cl_kernel                  /* kernel */,
		cl_device_id               /* device */,
		cl_kernel_work_group_info  /* param_name */,
		size_t                     /* param_value_size */,
		void *                     /* param_value */,
		size_t *                   /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncWaitForEvents)(
		cl_uint             /* num_events */,
		const cl_event *    /* event_list */);

	typedef cl_int (CL_API_CALL FuncGetEventInfo)(
		cl_event         /* event */,
		cl_event_info    /* param_name */,
		size_t           /* param_value_size */,
		void *           /* param_value */,
		size_t *         /* param_value_size_ret */);

	typedef cl_event (CL_API_CALL FuncCreateUserEvent)(
		cl_context    /* context */,
		cl_int *      /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncRetainEvent)(cl_event /* event */);
	typedef cl_int (CL_API_CALL FuncReleaseEvent)(cl_event /* event */);

	typedef cl_int (CL_API_CALL FuncSetUserEventStatus)(
		cl_event   /* event */,
		cl_int     /* execution_status */);

	typedef cl_int (CL_API_CALL FuncSetEventCallback)(
		cl_event    /* event */,
		cl_int      /* command_exec_callback_type */,
		void (CL_CALLBACK * /* pfn_notify */)(cl_event, cl_int, void *),
		void *      /* user_data */);

	typedef cl_int (CL_API_CALL FuncGetEventProfilingInfo)(
		cl_event            /* event */,
		cl_profiling_info   /* param_name */,
		size_t              /* param_value_size */,
		void *              /* param_value */,
		size_t *            /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncFlush)(cl_command_queue /* command_queue */);
	typedef cl_int (CL_API_CALL FuncFinish)(cl_command_queue /* command_queue */);

	typedef cl_int (CL_API_CALL FuncEnqueueReadBuffer)(
		cl_command_queue    /* command_queue */,
		cl_mem              /* buffer */,
		cl_bool             /* blocking_read */,
		size_t              /* offset */,
		size_t              /* size */, 
		void *              /* ptr */,
		cl_uint             /* num_events_in_wait_list */,
		const cl_event *    /* event_wait_list */,
		cl_event *          /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueReadBufferRect)(
		cl_command_queue    /* command_queue */,
		cl_mem              /* buffer */,
		cl_bool             /* blocking_read */,
		const size_t *      /* buffer_offset */,
		const size_t *      /* host_offset */, 
		const size_t *      /* region */,
		size_t              /* buffer_row_pitch */,
		size_t              /* buffer_slice_pitch */,
		size_t              /* host_row_pitch */,
		size_t              /* host_slice_pitch */,                        
		void *              /* ptr */,
		cl_uint             /* num_events_in_wait_list */,
		const cl_event *    /* event_wait_list */,
		cl_event *          /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueWriteBuffer)(
		cl_command_queue   /* command_queue */, 
		cl_mem             /* buffer */, 
		cl_bool            /* blocking_write */, 
		size_t             /* offset */, 
		size_t             /* size */, 
		const void *       /* ptr */, 
		cl_uint            /* num_events_in_wait_list */, 
		const cl_event *   /* event_wait_list */, 
		cl_event *         /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueWriteBufferRect)(
		cl_command_queue    /* command_queue */,
		cl_mem              /* buffer */,
		cl_bool             /* blocking_write */,
		const size_t *      /* buffer_offset */,
		const size_t *      /* host_offset */, 
		const size_t *      /* region */,
		size_t              /* buffer_row_pitch */,
		size_t              /* buffer_slice_pitch */,
		size_t              /* host_row_pitch */,
		size_t              /* host_slice_pitch */,                        
		const void *        /* ptr */,
		cl_uint             /* num_events_in_wait_list */,
		const cl_event *    /* event_wait_list */,
		cl_event *          /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueFillBuffer)(
		cl_command_queue   /* command_queue */,
		cl_mem             /* buffer */, 
		const void *       /* pattern */, 
		size_t             /* pattern_size */, 
		size_t             /* offset */, 
		size_t             /* size */, 
		cl_uint            /* num_events_in_wait_list */, 
		const cl_event *   /* event_wait_list */, 
		cl_event *         /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueCopyBuffer)(
		cl_command_queue    /* command_queue */, 
		cl_mem              /* src_buffer */,
		cl_mem              /* dst_buffer */, 
		size_t              /* src_offset */,
		size_t              /* dst_offset */,
		size_t              /* size */, 
		cl_uint             /* num_events_in_wait_list */,
		const cl_event *    /* event_wait_list */,
		cl_event *          /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueCopyBufferRect)(
		cl_command_queue    /* command_queue */, 
		cl_mem              /* src_buffer */,
		cl_mem              /* dst_buffer */, 
		const size_t *      /* src_origin */,
		const size_t *      /* dst_origin */,
		const size_t *      /* region */, 
		size_t              /* src_row_pitch */,
		size_t              /* src_slice_pitch */,
		size_t              /* dst_row_pitch */,
		size_t              /* dst_slice_pitch */,
		cl_uint             /* num_events_in_wait_list */,
		const cl_event *    /* event_wait_list */,
		cl_event *          /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueReadImage)(
		cl_command_queue     /* command_queue */,
		cl_mem               /* image */,
		cl_bool              /* blocking_read */, 
		const size_t *       /* origin[3] */,
		const size_t *       /* region[3] */,
		size_t               /* row_pitch */,
		size_t               /* slice_pitch */, 
		void *               /* ptr */,
		cl_uint              /* num_events_in_wait_list */,
		const cl_event *     /* event_wait_list */,
		cl_event *           /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueWriteImage)(
		cl_command_queue    /* command_queue */,
		cl_mem              /* image */,
		cl_bool             /* blocking_write */, 
		const size_t *      /* origin[3] */,
		const size_t *      /* region[3] */,
		size_t              /* input_row_pitch */,
		size_t              /* input_slice_pitch */, 
		const void *        /* ptr */,
		cl_uint             /* num_events_in_wait_list */,
		const cl_event *    /* event_wait_list */,
		cl_event *          /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueFillImage)(
		cl_command_queue   /* command_queue */,
		cl_mem             /* image */, 
		const void *       /* fill_color */, 
		const size_t *     /* origin[3] */, 
		const size_t *     /* region[3] */, 
		cl_uint            /* num_events_in_wait_list */, 
		const cl_event *   /* event_wait_list */, 
		cl_event *         /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueCopyImage)(
		cl_command_queue     /* command_queue */,
		cl_mem               /* src_image */,
		cl_mem               /* dst_image */, 
		const size_t *       /* src_origin[3] */,
		const size_t *       /* dst_origin[3] */,
		const size_t *       /* region[3] */, 
		cl_uint              /* num_events_in_wait_list */,
		const cl_event *     /* event_wait_list */,
		cl_event *           /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueCopyImageToBuffer)(
		cl_command_queue /* command_queue */,
		cl_mem           /* src_image */,
		cl_mem           /* dst_buffer */, 
		const size_t *   /* src_origin[3] */,
		const size_t *   /* region[3] */, 
		size_t           /* dst_offset */,
		cl_uint          /* num_events_in_wait_list */,
		const cl_event * /* event_wait_list */,
		cl_event *       /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueCopyBufferToImage)(
		cl_command_queue /* command_queue */,
		cl_mem           /* src_buffer */,
		cl_mem           /* dst_image */, 
		size_t           /* src_offset */,
		const size_t *   /* dst_origin[3] */,
		const size_t *   /* region[3] */, 
		cl_uint          /* num_events_in_wait_list */,
		const cl_event * /* event_wait_list */,
		cl_event *       /* event */);

	typedef void * (CL_API_CALL FuncEnqueueMapBuffer)(
		cl_command_queue /* command_queue */,
		cl_mem           /* buffer */,
		cl_bool          /* blocking_map */, 
		cl_map_flags     /* map_flags */,
		size_t           /* offset */,
		size_t           /* size */,
		cl_uint          /* num_events_in_wait_list */,
		const cl_event * /* event_wait_list */,
		cl_event *       /* event */,
		cl_int *         /* errcode_ret */);

	typedef void * (CL_API_CALL FuncEnqueueMapImage)(
		cl_command_queue  /* command_queue */,
		cl_mem            /* image */, 
		cl_bool           /* blocking_map */, 
		cl_map_flags      /* map_flags */, 
		const size_t *    /* origin[3] */,
		const size_t *    /* region[3] */,
		size_t *          /* image_row_pitch */,
		size_t *          /* image_slice_pitch */,
		cl_uint           /* num_events_in_wait_list */,
		const cl_event *  /* event_wait_list */,
		cl_event *        /* event */,
		cl_int *          /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncEnqueueUnmapMemObject)(
		cl_command_queue /* command_queue */,
		cl_mem           /* memobj */,
		void *           /* mapped_ptr */,
		cl_uint          /* num_events_in_wait_list */,
		const cl_event *  /* event_wait_list */,
		cl_event *        /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueMigrateMemObjects)(
		cl_command_queue       /* command_queue */,
		cl_uint                /* num_mem_objects */,
		const cl_mem *         /* mem_objects */,
		cl_mem_migration_flags /* flags */,
		cl_uint                /* num_events_in_wait_list */,
		const cl_event *       /* event_wait_list */,
		cl_event *             /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueNDRangeKernel)(
		cl_command_queue /* command_queue */,
		cl_kernel        /* kernel */,
		cl_uint          /* work_dim */,
		const size_t *   /* global_work_offset */,
		const size_t *   /* global_work_size */,
		const size_t *   /* local_work_size */,
		cl_uint          /* num_events_in_wait_list */,
		const cl_event * /* event_wait_list */,
		cl_event *       /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueTask)(
		cl_command_queue  /* command_queue */,
		cl_kernel         /* kernel */,
		cl_uint           /* num_events_in_wait_list */,
		const cl_event *  /* event_wait_list */,
		cl_event *        /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueNativeKernel)(
		cl_command_queue  /* command_queue */,
		void (CL_CALLBACK * /*user_func*/)(void *), 
		void *            /* args */,
		size_t            /* cb_args */, 
		cl_uint           /* num_mem_objects */,
		const cl_mem *    /* mem_list */,
		const void **     /* args_mem_loc */,
		cl_uint           /* num_events_in_wait_list */,
		const cl_event *  /* event_wait_list */,
		cl_event *        /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueMarkerWithWaitList)(
		cl_command_queue /* command_queue */,
		cl_uint           /* num_events_in_wait_list */,
		const cl_event *  /* event_wait_list */,
		cl_event *        /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueBarrierWithWaitList)(
		cl_command_queue /* command_queue */,
		cl_uint           /* num_events_in_wait_list */,
		const cl_event *  /* event_wait_list */,
		cl_event *        /* event */);

	typedef cl_int (CL_API_CALL FuncSetPrintfCallback)(
		cl_context          /* context */,
		void (CL_CALLBACK * /* pfn_notify */)(cl_context /* program */, cl_uint /*printf_data_len */, char * /* printf_data_ptr */, void * /* user_data */),
		void *              /* user_data */);

	typedef void * (CL_API_CALL FuncGetExtensionFunctionAddress)(
		const char *   /* func_name */);

	typedef void * (CL_API_CALL FuncGetExtensionFunctionAddressForPlatform)(
		cl_platform_id /* platform */,
		const char *   /* func_name */);

	typedef cl_mem (CL_API_CALL FuncCreateFromGLBuffer)(
		cl_context     /* context */,
		cl_mem_flags   /* flags */,
		cl_GLuint      /* bufobj */,
		int *          /* errcode_ret */);

	typedef cl_mem (CL_API_CALL FuncCreateFromGLTexture)(
		cl_context      /* context */,
		cl_mem_flags    /* flags */,
		cl_GLenum       /* target */,
		cl_GLint        /* miplevel */,
		cl_GLuint       /* texture */,
		cl_int *        /* errcode_ret */);

	typedef cl_mem (CL_API_CALL FuncCreateFromGLRenderbuffer)(
		cl_context   /* context */,
		cl_mem_flags /* flags */,
		cl_GLuint    /* renderbuffer */,
		cl_int *     /* errcode_ret */);

	typedef cl_int (CL_API_CALL FuncGetGLObjectInfo)(
		cl_mem                /* memobj */,
		cl_gl_object_type *   /* gl_object_type */,
		cl_GLuint *           /* gl_object_name */);

	typedef cl_int (CL_API_CALL FuncGetGLTextureInfo)(
		cl_mem               /* memobj */,
		cl_gl_texture_info   /* param_name */,
		size_t               /* param_value_size */,
		void *               /* param_value */,
		size_t *             /* param_value_size_ret */);

	typedef cl_int (CL_API_CALL FuncEnqueueAcquireGLObjects)(
		cl_command_queue      /* command_queue */,
		cl_uint               /* num_objects */,
		const cl_mem *        /* mem_objects */,
		cl_uint               /* num_events_in_wait_list */,
		const cl_event *      /* event_wait_list */,
		cl_event *            /* event */);

	typedef cl_int (CL_API_CALL FuncEnqueueReleaseGLObjects)(
		cl_command_queue      /* command_queue */,
		cl_uint               /* num_objects */,
		const cl_mem *        /* mem_objects */,
		cl_uint               /* num_events_in_wait_list */,
		const cl_event *      /* event_wait_list */,
		cl_event *            /* event */);

    typedef cl_mem (CL_API_CALL FuncCreateFromGLTexture2D)(
		cl_context      /* context */,
		cl_mem_flags    /* flags */,
		cl_GLenum       /* target */,
		cl_GLint        /* miplevel */,
		cl_GLuint       /* texture */,
		cl_int *        /* errcode_ret */);
    
    typedef cl_mem (CL_API_CALL FuncCreateFromGLTexture3D)(
		cl_context      /* context */,
		cl_mem_flags    /* flags */,
		cl_GLenum       /* target */,
		cl_GLint        /* miplevel */,
		cl_GLuint       /* texture */,
		cl_int *        /* errcode_ret */);

	cl_platform_id platform_id;
	std::string profile;
	std::string version;
	std::string name;
	std::string vendor;
	std::string extensions;
	int major_version, minor_version;

	std::string get_info_string(cl_platform_info param_name) const;

	FuncGetDeviceIDs *GetDeviceIDs;
	FuncGetDeviceInfo *GetDeviceInfo;
	FuncCreateSubDevices *CreateSubDevices;
	FuncRetainDevice *RetainDevice;
	FuncReleaseDevice *ReleaseDevice;
	FuncCreateContext *CreateContext;
	FuncCreateContextFromType *CreateContextFromType;
	FuncRetainContext *RetainContext;
	FuncReleaseContext *ReleaseContext;
	FuncGetContextInfo *GetContextInfo;
	FuncCreateCommandQueue *CreateCommandQueue;
	FuncRetainCommandQueue *RetainCommandQueue;
	FuncReleaseCommandQueue *ReleaseCommandQueue;
	FuncGetCommandQueueInfo *GetCommandQueueInfo;
	FuncCreateBuffer *CreateBuffer;
	FuncCreateSubBuffer *CreateSubBuffer;
	FuncCreateImage *CreateImage;
	FuncRetainMemObject *RetainMemObject;
	FuncReleaseMemObject *ReleaseMemObject;
	FuncGetSupportedImageFormats *GetSupportedImageFormats;
	FuncGetMemObjectInfo *GetMemObjectInfo;
	FuncGetImageInfo *GetImageInfo;
	FuncSetMemObjectDestructorCallback *SetMemObjectDestructorCallback;
	FuncCreateSampler *CreateSampler;
	FuncRetainSampler *RetainSampler;
	FuncReleaseSampler *ReleaseSampler;
	FuncGetSamplerInfo *GetSamplerInfo;
	FuncCreateProgramWithSource *CreateProgramWithSource;
	FuncCreateProgramWithBinary *CreateProgramWithBinary;
	FuncCreateProgramWithBuiltInKernels *CreateProgramWithBuiltInKernels;
	FuncRetainProgram *RetainProgram;
	FuncReleaseProgram *ReleaseProgram;
	FuncBuildProgram *BuildProgram;
	FuncCompileProgram *CompileProgram;
	FuncLinkProgram *LinkProgram;
	FuncUnloadPlatformCompiler *UnloadPlatformCompiler;
	FuncGetProgramInfo *GetProgramInfo;
	FuncGetProgramBuildInfo *GetProgramBuildInfo;
	FuncCreateKernel *CreateKernel;
	FuncCreateKernelsInProgram *CreateKernelsInProgram;
	FuncRetainKernel *RetainKernel;
	FuncReleaseKernel *ReleaseKernel;
	FuncSetKernelArg *SetKernelArg;
	FuncGetKernelInfo *GetKernelInfo;
	FuncGetKernelArgInfo *GetKernelArgInfo;
	FuncGetKernelWorkGroupInfo *GetKernelWorkGroupInfo;
	FuncWaitForEvents *WaitForEvents;
	FuncGetEventInfo *GetEventInfo;
	FuncCreateUserEvent *CreateUserEvent;
	FuncRetainEvent *RetainEvent;
	FuncReleaseEvent *ReleaseEvent;
	FuncSetUserEventStatus *SetUserEventStatus;
	FuncSetEventCallback *SetEventCallback;
	FuncGetEventProfilingInfo *GetEventProfilingInfo;
	FuncFlush *Flush;
	FuncFinish *Finish;
	FuncEnqueueReadBuffer *EnqueueReadBuffer;
	FuncEnqueueReadBufferRect *EnqueueReadBufferRect;
	FuncEnqueueWriteBuffer *EnqueueWriteBuffer;
	FuncEnqueueWriteBufferRect *EnqueueWriteBufferRect;
	FuncEnqueueFillBuffer *EnqueueFillBuffer;
	FuncEnqueueCopyBuffer *EnqueueCopyBuffer;
	FuncEnqueueCopyBufferRect *EnqueueCopyBufferRect;
	FuncEnqueueReadImage *EnqueueReadImage;
	FuncEnqueueWriteImage *EnqueueWriteImage;
	FuncEnqueueFillImage *EnqueueFillImage;
	FuncEnqueueCopyImage *EnqueueCopyImage;
	FuncEnqueueCopyImageToBuffer *EnqueueCopyImageToBuffer;
	FuncEnqueueCopyBufferToImage *EnqueueCopyBufferToImage;
	FuncEnqueueMapBuffer *EnqueueMapBuffer;
	FuncEnqueueMapImage *EnqueueMapImage;
	FuncEnqueueUnmapMemObject *EnqueueUnmapMemObject;
	FuncEnqueueMigrateMemObjects *EnqueueMigrateMemObjects;
	FuncEnqueueNDRangeKernel *EnqueueNDRangeKernel;
	FuncEnqueueTask *EnqueueTask;
	FuncEnqueueNativeKernel *EnqueueNativeKernel;
	FuncEnqueueMarkerWithWaitList *EnqueueMarkerWithWaitList;
	FuncEnqueueBarrierWithWaitList *EnqueueBarrierWithWaitList;
	FuncSetPrintfCallback *SetPrintfCallback;
	FuncGetExtensionFunctionAddress *GetExtensionFunctionAddress; // OpenCL 1.1
	FuncGetExtensionFunctionAddressForPlatform *GetExtensionFunctionAddressForPlatform; // OpenCL 1.2

	FuncCreateFromGLBuffer *CreateFromGLBuffer;
	FuncCreateFromGLTexture *CreateFromGLTexture;
	FuncCreateFromGLRenderbuffer *CreateFromGLRenderbuffer;
	FuncGetGLObjectInfo *GetGLObjectInfo;
	FuncGetGLTextureInfo *GetGLTextureInfo;
	FuncEnqueueAcquireGLObjects *EnqueueAcquireGLObjects;
	FuncEnqueueReleaseGLObjects *EnqueueReleaseGLObjects;
	FuncCreateFromGLTexture2D *CreateFromGLTexture2D;
	FuncCreateFromGLTexture3D *CreateFromGLTexture3D;

#ifdef WIN32
	clGetDeviceIDsFromD3D11KHR_fn GetDeviceIDsFromD3D11KHR;
	clCreateFromD3D11BufferKHR_fn CreateFromD3D11BufferKHR;
	clCreateFromD3D11Texture2DKHR_fn CreateFromD3D11Texture2DKHR;
	clCreateFromD3D11Texture3DKHR_fn CreateFromD3D11Texture3DKHR;
	clEnqueueAcquireD3D11ObjectsKHR_fn EnqueueAcquireD3D11ObjectsKHR;
	clEnqueueReleaseD3D11ObjectsKHR_fn EnqueueReleaseD3D11ObjectsKHR;
#endif
};

}
