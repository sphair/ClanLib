AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libclan24Core.la

libclan24Core_la_SOURCES = \
core_global.cpp \
core_iostream.cpp \
CSS/css_document.cpp \
CSS/css_document_impl.cpp \
CSS/css_property.cpp \
CSS/css_selector.cpp \
CSS/css_tokenizer.cpp \
CSS/css_tokenizer_impl.cpp \
IOData/access_control_list.cpp \
IOData/directory.cpp \
IOData/directory_scanner.cpp \
IOData/endianess.cpp \
IOData/file.cpp \
IOData/html_url.cpp \
IOData/file_help.cpp \
IOData/iodevice.cpp \
IOData/iodevice_memory.cpp \
IOData/iodevice_provider_file.cpp \
IOData/iodevice_provider_memory.cpp \
IOData/iodevice_provider_pipe_connection.cpp \
IOData/path_help.cpp \
IOData/pipe_connection.cpp \
IOData/pipe_listen.cpp \
IOData/pipe_listen_impl.cpp \
IOData/security_descriptor.cpp \
IOData/security_identifier.cpp \
IOData/virtual_directory.cpp \
IOData/virtual_directory_listing.cpp \
IOData/virtual_directory_listing_entry.cpp \
IOData/virtual_file_source_file.cpp \
IOData/virtual_file_source_zip.cpp \
IOData/virtual_file_system.cpp \
Zip/zip_64_end_of_central_directory_locator.cpp \
Zip/zip_64_end_of_central_directory_record.cpp \
Zip/zip_archive.cpp \
Zip/zip_digital_signature.cpp \
Zip/zip_end_of_central_directory_record.cpp \
Zip/zip_file_entry.cpp \
Zip/zip_file_header.cpp \
Zip/zip_iodevice_fileentry.cpp \
Zip/zip_local_file_descriptor.cpp \
Zip/zip_local_file_header.cpp \
Zip/zip_writer.cpp \
Zip/zip_reader.cpp \
Math/base64_encoder.cpp \
Math/base64_decoder.cpp \
Math/bezier_curve.cpp \
Math/bezier_curve_impl.cpp \
Math/quaternion.cpp \
Math/vec2.cpp \
Math/vec3.cpp \
Math/vec4.cpp \
Math/delauney_triangulator.cpp \
Math/delauney_triangulator_generic.cpp \
Math/ear_clip_result.cpp \
Math/ear_clip_triangulator.cpp \
Math/ear_clip_triangulator_impl.cpp \
Math/line_math.cpp \
Math/mat2.cpp \
Math/mat3.cpp \
Math/mat4.cpp \
Math/origin.cpp \
Math/outline_triangulator.cpp \
Math/outline_triangulator_generic.cpp \
Math/pointset_math.cpp \
Math/quad.cpp \
Math/rect.cpp \
Math/rect_packer.cpp \
Math/rect_packer_impl.cpp \
Math/rect_packer_impl.h \
Crypto/secret.cpp \
Crypto/secret_impl.cpp \
Crypto/sha1.cpp \
Crypto/sha224.cpp \
Crypto/sha256.cpp \
Crypto/sha384.cpp \
Crypto/sha512.cpp \
Crypto/sha512_224.cpp \
Crypto/sha512_256.cpp \
Crypto/aes128_encrypt.cpp \
Crypto/aes128_encrypt_impl.cpp \
Crypto/aes128_decrypt.cpp \
Crypto/aes128_decrypt_impl.cpp \
Crypto/aes192_encrypt.cpp \
Crypto/aes192_encrypt_impl.cpp \
Crypto/aes192_decrypt.cpp \
Crypto/aes192_decrypt_impl.cpp \
Crypto/aes256_encrypt.cpp \
Crypto/aes256_encrypt_impl.cpp \
Crypto/aes256_decrypt.cpp \
Crypto/aes256_decrypt_impl.cpp \
Crypto/aes_impl.cpp \
Math/line.cpp \
Crypto/hash_functions.cpp \
Math/line_ray.cpp \
Math/line_segment.cpp \
Crypto/random.cpp \
Crypto/random_impl.cpp \
Crypto/sha1_impl.cpp \
Crypto/sha.cpp \
Crypto/md5_impl.cpp \
Crypto/md5.cpp \
Crypto/sha256_impl.cpp \
Crypto/sha512_impl.cpp \
Crypto/big_int.cpp \
Crypto/rsa.cpp \
Crypto/rsa_impl.cpp \
Math/triangle_math.cpp \
Math/angle.cpp \
precomp.cpp \
Resources/resource.cpp \
Resources/resource_data_session.cpp \
Resources/resource_manager.cpp \
System/block_allocator.cpp \
System/command_line.cpp \
System/command_line_generic.cpp \
System/console_window.cpp \
System/console_window_generic.cpp \
System/databuffer.cpp \
System/datetime.cpp \
System/detect_cpu_ext.cpp \
System/event.cpp \
System/event_impl.cpp \
System/exception.cpp \
System/mutex.cpp \
System/keep_alive.cpp \
System/runnable.cpp \
System/service.cpp \
System/service_impl.cpp \
System/setup_core.cpp \
System/system.cpp \
System/thread.cpp \
System/timer.cpp \
System/thread_local_storage.cpp \
System/thread_local_storage_impl.cpp \
System/disposable_object.cpp \
Text/console.cpp \
Text/console_logger.cpp \
Text/file_logger.cpp \
Text/logger.cpp \
Text/string_format.cpp \
Text/string_help.cpp \
Text/utf8_reader.cpp \
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
XML/xml_writer.cpp \
XML/xpath_evaluator.cpp \
XML/xpath_exception.cpp \
XML/xpath_evaluator_impl.cpp \
XML/xpath_object.cpp

if WIN32
libclan24Core_la_SOURCES += \
IOData/Win32/directory_scanner_win32.cpp \
System/Win32/event_provider_win32.cpp \
System/Win32/init_win32.cpp \
System/Win32/service_win32.cpp \
System/Win32/thread_win32.cpp
else
libclan24Core_la_SOURCES += \
IOData/Unix/directory_scanner_unix.cpp \
System/Unix/event_provider_socketpair.cpp \
System/Unix/init_linux.cpp \
System/Unix/service_unix.cpp \
System/Unix/thread_unix.cpp
endif

# FIXME: Hardcoding these here might not be a good idea
libclan24Core_la_LIBADD = @LIBPTHREAD@

libclan24Core_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanCore)

libclan24Core_la_CXXFLAGS=$(clanCore_CXXFLAGS) $(extra_CFLAGS_clanCore)

# EOF #
