AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libclan30Core.la

libclan30Core_la_SOURCES = \
core_global.cpp \
Math/triangle_math.cpp \
Math/outline_triangulator_generic.cpp \
Math/rect.cpp \
Math/vec1.cpp \
Math/half_float.cpp \
Math/mat3.cpp \
Math/ear_clip_result.cpp \
Math/line_math.cpp \
Math/bezier_curve.cpp \
Math/line_ray.cpp \
Math/line_segment.cpp \
Math/rect_packer_impl.cpp \
Math/ear_clip_triangulator_impl.cpp \
Math/vec2.cpp \
Math/delauney_triangulator_generic.cpp \
Math/base64_decoder.cpp \
Math/mat4.cpp \
Math/quaternion.cpp \
Math/rect_packer.cpp \
Math/line.cpp \
Math/origin.cpp \
Math/mat2.cpp \
Math/vec4.cpp \
Math/bezier_curve_impl.cpp \
Math/delauney_triangulator.cpp \
Math/base64_encoder.cpp \
Math/outline_triangulator.cpp \
Math/ear_clip_triangulator.cpp \
Math/vec3.cpp \
Math/angle.cpp \
Math/quad.cpp \
Math/pointset_math.cpp \
XML/dom_entity.cpp \
XML/dom_attr.cpp \
XML/dom_cdata_section.cpp \
XML/xml_tokenizer.cpp \
XML/dom_node.cpp \
XML/dom_document_fragment.cpp \
XML/xpath_evaluator_impl.cpp \
XML/dom_document.cpp \
XML/xpath_exception.cpp \
XML/dom_entity_reference.cpp \
XML/dom_processing_instruction.cpp \
XML/xpath_evaluator.cpp \
XML/xpath_object.cpp \
XML/dom_character_data.cpp \
XML/dom_node_list.cpp \
XML/dom_document_generic.cpp \
XML/dom_exception.cpp \
XML/dom_text.cpp \
XML/dom_comment.cpp \
XML/dom_named_node_map.cpp \
XML/dom_document_type.cpp \
XML/dom_implementation.cpp \
XML/dom_element.cpp \
XML/dom_notation.cpp \
XML/xml_writer.cpp \
core_iostream.cpp \
Crypto/sha1_impl.cpp \
Crypto/md5.cpp \
Crypto/aes256_encrypt.cpp \
Crypto/sha512_224.cpp \
Crypto/sha224.cpp \
Crypto/aes128_encrypt_impl.cpp \
Crypto/sha.cpp \
Crypto/sha512_impl.cpp \
Crypto/aes_impl.cpp \
Crypto/random.cpp \
Crypto/big_int.cpp \
Crypto/aes256_decrypt_impl.cpp \
Crypto/aes192_encrypt.cpp \
Crypto/aes256_decrypt.cpp \
Crypto/aes192_decrypt.cpp \
Crypto/aes128_decrypt.cpp \
Crypto/aes128_encrypt.cpp \
Crypto/sha384.cpp \
Crypto/aes128_decrypt_impl.cpp \
Crypto/secret.cpp \
Crypto/secret_impl.cpp \
Crypto/aes192_decrypt_impl.cpp \
Crypto/sha512.cpp \
Crypto/sha256.cpp \
Crypto/hash_functions.cpp \
Crypto/sha1.cpp \
Crypto/random_impl.cpp \
Crypto/sha512_256.cpp \
Crypto/rsa.cpp \
Crypto/md5_impl.cpp \
Crypto/rsa_impl.cpp \
Crypto/aes256_encrypt_impl.cpp \
Crypto/sha256_impl.cpp \
Crypto/aes192_encrypt_impl.cpp \
precomp.cpp \
Resources/resource.cpp \
Resources/resource_manager.cpp \
Resources/resource_data_session.cpp \
System/system.cpp \
System/disposable_object.cpp \
System/exception.cpp \
System/runnable.cpp \
System/console_window_generic.cpp \
System/event.cpp \
System/command_line_generic.cpp \
System/thread.cpp \
System/keep_alive.cpp \
System/detect_cpu_ext.cpp \
System/databuffer.cpp \
System/console_window.cpp \
System/service.cpp \
System/timer.cpp \
System/setup_core.cpp \
System/thread_local_storage.cpp \
System/block_allocator.cpp \
System/service_impl.cpp \
System/datetime.cpp \
System/mutex.cpp \
System/event_impl.cpp \
System/thread_local_storage_impl.cpp \
System/command_line.cpp \
Text/utf8_reader.cpp \
Text/string_format.cpp \
Text/logger.cpp \
Text/string_help.cpp \
Text/file_logger.cpp \
Text/console.cpp \
Text/console_logger.cpp \
IOData/iodevice_provider_pipe_connection.cpp \
IOData/file_help.cpp \
IOData/pipe_listen_impl.cpp \
IOData/path_help.cpp \
IOData/iodevice.cpp \
IOData/virtual_file_source_zip.cpp \
IOData/pipe_listen.cpp \
IOData/security_descriptor.cpp \
IOData/virtual_directory.cpp \
IOData/directory.cpp \
IOData/virtual_directory_listing_entry.cpp \
IOData/iodevice_provider_memory.cpp \
IOData/virtual_file_system.cpp \
IOData/file.cpp \
IOData/html_url.cpp \
IOData/virtual_file_source_file.cpp \
IOData/iodevice_memory.cpp \
IOData/endianess.cpp \
IOData/directory_scanner.cpp \
IOData/security_identifier.cpp \
IOData/access_control_list.cpp \
IOData/iodevice_provider_file.cpp \
IOData/pipe_connection.cpp \
IOData/virtual_directory_listing.cpp \
Zip/zip_file_header.cpp \
Zip/zip_64_end_of_central_directory_locator.cpp \
Zip/zip_archive.cpp \
Zip/zip_end_of_central_directory_record.cpp \
Zip/zip_file_entry.cpp \
Zip/zip_writer.cpp \
Zip/zlib_compression.cpp \
Zip/zip_64_end_of_central_directory_record.cpp \
Zip/zip_local_file_header.cpp \
Zip/zip_local_file_descriptor.cpp \
Zip/zip_iodevice_fileentry.cpp \
Zip/zip_reader.cpp \
Zip/zip_digital_signature.cpp

if WIN32
libclan30Core_la_SOURCES += \
System/Win32/service_win32.cpp \
System/Win32/init_win32.cpp \
System/Win32/event_provider_win32.cpp \
System/Win32/thread_win32.cpp \
IOData/Win32/directory_scanner_win32.cpp

else
libclan30Core_la_SOURCES += \
IOData/Unix/directory_scanner_unix.cpp \
System/Unix/init_linux.cpp \
System/Unix/service_unix.cpp \
System/Unix/event_provider_socketpair.cpp \
System/Unix/thread_unix.cpp

endif

# FIXME: Hardcoding these here might not be a good idea
libclan30Core_la_LIBADD = @LIBPTHREAD@

libclan30Core_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanCore)

libclan30Core_la_CXXFLAGS=$(clanCore_CXXFLAGS) $(extra_CFLAGS_clanCore)

# EOF #
