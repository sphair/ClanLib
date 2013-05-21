AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libclan30Core.la

libclan30Core_la_SOURCES = \
Text/console_logger.cpp \
Text/utf8_reader.cpp \
Text/string_format.cpp \
Text/file_logger.cpp \
Text/logger.cpp \
Text/console.cpp \
Text/string_help.cpp \
Resources/resource_manager.cpp \
Resources/resource.cpp \
ErrorReporting/crash_reporter.cpp \
ErrorReporting/detect_hang.cpp \
ErrorReporting/exception_dialog.cpp \
Crypto/random.cpp \
Crypto/aes128_decrypt.cpp \
Crypto/x509_impl.cpp \
Crypto/aes192_encrypt.cpp \
Crypto/aes192_encrypt_impl.cpp \
Crypto/asn1.cpp \
Crypto/sha512_256.cpp \
Crypto/tls_client_impl.cpp \
Crypto/sha.cpp \
Crypto/sha1_impl.cpp \
Crypto/aes256_encrypt_impl.cpp \
Crypto/md5_impl.cpp \
Crypto/secret.cpp \
Crypto/sha256.cpp \
Crypto/rsa.cpp \
Crypto/aes128_encrypt.cpp \
Crypto/sha512_224.cpp \
Crypto/sha1.cpp \
Crypto/sha224.cpp \
Crypto/aes128_decrypt_impl.cpp \
Crypto/sha512_impl.cpp \
Crypto/aes256_encrypt.cpp \
Crypto/aes256_decrypt_impl.cpp \
Crypto/md5.cpp \
Crypto/hash_functions.cpp \
Crypto/rsa_impl.cpp \
Crypto/aes_impl.cpp \
Crypto/aes192_decrypt.cpp \
Crypto/aes192_decrypt_impl.cpp \
Crypto/sha384.cpp \
Crypto/secret_impl.cpp \
Crypto/tls_client.cpp \
Crypto/aes128_encrypt_impl.cpp \
Crypto/random_impl.cpp \
Crypto/sha512.cpp \
Crypto/sha256_impl.cpp \
Crypto/aes256_decrypt.cpp \
Crypto/x509.cpp \
precomp.cpp \
IOData/iodevice_provider_memory.cpp \
IOData/iodevice_memory.cpp \
IOData/file_system_provider_zip.cpp \
IOData/pipe_listen_impl.cpp \
IOData/pipe_listen.cpp \
IOData/file_help.cpp \
IOData/html_url.cpp \
IOData/directory_listing.cpp \
IOData/file_system.cpp \
IOData/directory.cpp \
IOData/file_system_provider_file.cpp \
IOData/directory_listing_entry.cpp \
IOData/iodevice_provider_file.cpp \
IOData/endianess.cpp \
IOData/directory_scanner.cpp \
IOData/pipe_connection.cpp \
IOData/iodevice.cpp \
IOData/file.cpp \
IOData/iodevice_provider_pipe_connection.cpp \
IOData/path_help.cpp \
XML/dom_processing_instruction.cpp \
XML/dom_exception.cpp \
XML/xpath_exception.cpp \
XML/dom_document_fragment.cpp \
XML/xml_writer.cpp \
XML/dom_element.cpp \
XML/xml_tokenizer.cpp \
XML/dom_cdata_section.cpp \
XML/dom_notation.cpp \
XML/dom_node_list.cpp \
XML/xpath_evaluator.cpp \
XML/dom_attr.cpp \
XML/dom_entity_reference.cpp \
XML/xpath_evaluator_impl.cpp \
XML/dom_node.cpp \
XML/dom_document_type.cpp \
XML/xpath_object.cpp \
XML/dom_document.cpp \
XML/dom_character_data.cpp \
XML/dom_implementation.cpp \
XML/dom_entity.cpp \
XML/dom_text.cpp \
XML/dom_document_generic.cpp \
XML/dom_comment.cpp \
XML/dom_named_node_map.cpp \
Zip/zip_iodevice_fileentry.cpp \
Zip/zip_file_header.cpp \
Zip/zip_64_end_of_central_directory_locator.cpp \
Zip/zip_file_entry.cpp \
Zip/zip_writer.cpp \
Zip/zip_end_of_central_directory_record.cpp \
Zip/zip_local_file_header.cpp \
Zip/zip_64_end_of_central_directory_record.cpp \
Zip/zip_digital_signature.cpp \
Zip/zlib_compression.cpp \
Zip/zip_reader.cpp \
Zip/zip_local_file_descriptor.cpp \
Zip/zip_archive.cpp \
Math/mat3.cpp \
Math/intersection_test.cpp \
Math/line.cpp \
Math/rect_packer_impl.cpp \
Math/angle.cpp \
Math/triangle_math.cpp \
Math/big_int_impl.cpp \
Math/base64_decoder.cpp \
Math/bezier_curve_impl.cpp \
Math/mat2.cpp \
Math/line_ray.cpp \
Math/ear_clip_triangulator_impl.cpp \
Math/ear_clip_triangulator.cpp \
Math/origin.cpp \
Math/rect.cpp \
Math/vec4.cpp \
Math/quad.cpp \
Math/outline_triangulator_generic.cpp \
Math/bezier_curve.cpp \
Math/delauney_triangulator.cpp \
Math/half_float.cpp \
Math/ear_clip_result.cpp \
Math/base64_encoder.cpp \
Math/delauney_triangulator_generic.cpp \
Math/big_int.cpp \
Math/frustum_planes.cpp \
Math/line_segment.cpp \
Math/line_math.cpp \
Math/quaternion.cpp \
Math/pointset_math.cpp \
Math/mat4.cpp \
Math/vec2.cpp \
Math/outline_triangulator.cpp \
Math/rect_packer.cpp \
Math/vec3.cpp \
core_global.cpp \
core_iostream.cpp \
System/runnable.cpp \
System/setup_core.cpp \
System/service_impl.cpp \
System/event_impl.cpp \
System/thread.cpp \
System/game_time.cpp \
System/mutex.cpp \
System/keep_alive.cpp \
System/timer.cpp \
System/console_window_generic.cpp \
System/exception.cpp \
System/databuffer.cpp \
System/system.cpp \
System/block_allocator.cpp \
System/registry_key.cpp \
System/console_window.cpp \
System/disposable_object.cpp \
System/event.cpp \
System/thread_local_storage.cpp \
System/detect_cpu_ext.cpp \
System/service.cpp \
System/thread_local_storage_impl.cpp \
System/datetime.cpp

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

libclan30Core_la_LIBADD = @LIBPTHREAD@

libclan30Core_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanCore)

libclan30Core_la_CXXFLAGS=$(clanCore_CXXFLAGS) $(extra_CFLAGS_clanCore)

# EOF #
