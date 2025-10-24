AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libclanCore.la

libclanCore_la_SOURCES = \
core_iostream.cpp \
IOData/Generic/datafile_inputprovider.cpp \
IOData/Generic/directory.cpp \
IOData/Generic/directory_scanner.cpp \
IOData/Generic/endianess.cpp \
IOData/Generic/inputsource.cpp \
IOData/Generic/inputsource_file.cpp \
IOData/Generic/inputsource_memory_generic.cpp \
IOData/Generic/inputsource_provider_file.cpp \
IOData/Generic/inputsource_zipped.cpp \
IOData/Generic/outputsource.cpp \
IOData/Generic/outputsource_file.cpp \
IOData/Generic/outputsource_memory_generic.cpp \
IOData/Generic/outputsource_zipped.cpp \
IOData/inputsource_memory.cpp \
IOData/inputsource_zip_fileentry.cpp \
IOData/outputsource_memory.cpp \
IOData/zip_64_end_of_central_directory_locator.cpp \
IOData/zip_64_end_of_central_directory_record.cpp \
IOData/zip_archive.cpp \
IOData/zip_digital_signature.cpp \
IOData/zip_end_of_central_directory_record.cpp \
IOData/zip_file_entry.cpp \
IOData/zip_file_header.cpp \
IOData/zip_local_file_descriptor.cpp \
IOData/zip_local_file_header.cpp \
Math/matrix4x4.cpp \
Math/cl_vector.cpp \
Math/line_math.cpp \
Math/triangle_math.cpp \
Math/pointset_math.cpp \
Math/origin.cpp \
Math/rect.cpp \
Math/quad.cpp \
Math/vector2.cpp \
Math/number_pool.cpp \
Math/delauney_triangulator.cpp \
Math/delauney_triangulator_generic.cpp \
Math/outline_triangulator.cpp \
Math/outline_triangulator_generic.cpp \
Resources/resource.cpp \
Resources/resource_generic.cpp \
Resources/resource_manager.cpp \
Resources/resource_manager_generic.cpp \
Resources/resourcedata.cpp \
Resources/resourcetype_boolean.cpp \
Resources/resourcetype_integer.cpp \
Resources/resourcetype_float.cpp \
Resources/resourcetype_raw.cpp \
Resources/resourcetype_string.cpp \
System/Generic/cl_assert.cpp \
System/Generic/clanstring.cpp \
System/Generic/command_line.cpp \
System/Generic/command_line_generic.cpp \
System/Generic/console_window.cpp \
System/Generic/console_window_generic.cpp \
System/Generic/detect_3dnow.cpp \
System/Generic/detect_mmx.cpp \
System/Generic/error.cpp \
System/Generic/event_listener.cpp \
System/Generic/event_listener_generic.cpp \
System/Generic/event_trigger.cpp \
System/Generic/event_trigger_generic.cpp \
System/Generic/keep_alive_generic.cpp \
System/Generic/setupcore.cpp \
System/Generic/timer_generic.cpp \
System/Generic/log.cpp \
System/Generic/call_stack.cpp \
System/Generic/crash_reporter.cpp \
System/Generic/clipboard.cpp \
XML/dom_attr.cpp \
XML/dom_cdata_section.cpp \
XML/dom_character_data.cpp \
XML/dom_comment.cpp \
XML/dom_document.cpp \
XML/dom_document_fragment.cpp \
XML/dom_document_generic.cpp \
XML/dom_document_type.cpp \
XML/dom_element.cpp \
XML/dom_entity.cpp \
XML/dom_entity_reference.cpp \
XML/dom_exception.cpp \
XML/dom_implementation.cpp \
XML/dom_named_node_map.cpp \
XML/dom_node.cpp \
XML/dom_node_list.cpp \
XML/dom_notation.cpp \
XML/dom_processing_instruction.cpp \
XML/dom_text.cpp \
XML/xml_tokenizer.cpp \
XML/xml_token_load.cpp \
XML/xml_token_save.cpp \
XML/xml_writer.cpp \
precomp.cpp

if WIN32
libclanCore_la_SOURCES += \
System/Win32/event_listener_win32.cpp \
System/Win32/event_listener_win32.h \
System/Win32/event_trigger_win32.cpp \
System/Win32/event_trigger_win32.h \
System/Win32/init_win32.cpp \
System/Win32/init_win32.h \
System/Win32/mutex_win32.cpp \
System/Win32/mutex_win32.h \
System/Win32/thread_win32.cpp \
System/Win32/thread_win32.h \
IOData/Win32/directory_scanner_win32.cpp \
IOData/Win32/directory_scanner_win32.h \
System/Win32/crash_reporter_win32.cpp
else
libclanCore_la_SOURCES += \
System/Unix/event_listener_unix.cpp \
System/Unix/event_trigger_unix.cpp \
System/Unix/init_linux.cpp \
System/Unix/mutex_pthread.cpp \
System/Unix/thread_pthread.cpp \
System/Unix/cl_library.cpp \
System/Unix/crash_reporter_unix.cpp \
IOData/Unix/directory_scanner_unix.cpp
endif

# FIXME: Hardcoding these here might not be a good idea
libclanCore_la_LIBADD = @LIBPTHREAD@

libclanCore_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
