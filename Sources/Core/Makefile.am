AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libclan42Core.la

libclan42Core_la_SOURCES = \
System/block_allocator.cpp \
System/service_impl.cpp \
System/exception.cpp \
System/system.cpp \
System/databuffer.cpp \
System/work_queue.cpp \
System/game_time.cpp \
System/thread_local_storage.cpp \
System/registry_key.cpp \
System/console_window.cpp \
System/console_window_generic.cpp \
System/setup_core.cpp \
System/datetime.cpp \
System/detect_cpu_ext.cpp \
System/disposable_object.cpp \
System/service.cpp \
System/thread_local_storage_impl.cpp \
Zip/zip_iodevice_fileentry.cpp \
Zip/zip_64_end_of_central_directory_locator.cpp \
Zip/zip_file_header.cpp \
Zip/zip_end_of_central_directory_record.cpp \
Zip/zip_writer.cpp \
Zip/zip_digital_signature.cpp \
Zip/zip_file_entry.cpp \
Zip/zip_64_end_of_central_directory_record.cpp \
Zip/zip_local_file_header.cpp \
Zip/zlib_compression.cpp \
Zip/zip_reader.cpp \
Zip/zip_local_file_descriptor.cpp \
Zip/zip_archive.cpp \
core_iostream.cpp \
Math/base64_decoder.cpp \
Math/rect_packer.cpp \
Math/line.cpp \
Math/bezier_curve_impl.cpp \
Math/outline_triangulator_generic.cpp \
Math/vec3.cpp \
Math/rect_packer_impl.cpp \
Math/line_ray.cpp \
Math/ear_clip_triangulator_impl.cpp \
Math/frustum_planes.cpp \
Math/base64_encoder.cpp \
Math/pointset_math.cpp \
Math/delauney_triangulator.cpp \
Math/ear_clip_triangulator.cpp \
Math/mat2.cpp \
Math/line_segment.cpp \
Math/ear_clip_result.cpp \
Math/delauney_triangulator_generic.cpp \
Math/bezier_curve.cpp \
Math/vec4.cpp \
Math/easing.cpp \
Math/origin.cpp \
Math/vec2.cpp \
Math/half_float.cpp \
Math/quad.cpp \
Math/mat4.cpp \
Math/line_math.cpp \
Math/angle.cpp \
Math/rect.cpp \
Math/outline_triangulator.cpp \
Math/quaternion.cpp \
Math/intersection_test.cpp \
Math/big_int_impl.cpp \
Math/mat3.cpp \
Math/big_int.cpp \
Math/triangle_math.cpp \
System/tls_instance.cpp \
ErrorReporting/crash_reporter.cpp \
ErrorReporting/exception_dialog.cpp \
JSON/json_value.cpp \
Text/string_format.cpp \
Text/file_logger.cpp \
Text/utf8_reader.cpp \
Text/console.cpp \
Text/string_help.cpp \
Text/logger.cpp \
Text/console_logger.cpp \
precomp.cpp \
IOData/file_help.cpp \
IOData/memory_device.cpp \
IOData/directory_listing_entry.cpp \
IOData/html_url.cpp \
IOData/iodevice.cpp \
IOData/file.cpp \
IOData/directory_listing.cpp \
IOData/file_system_provider_zip.cpp \
IOData/path_help.cpp \
IOData/endianess.cpp \
IOData/file_system_provider_file.cpp \
IOData/iodevice_provider_memory.cpp \
IOData/directory.cpp \
IOData/directory_scanner.cpp \
IOData/iodevice_provider_file.cpp \
IOData/file_system.cpp \
Resources/file_resource_manager.cpp \
Resources/resource_manager.cpp \
Resources/file_resource_document.cpp \
Crypto/sha384.cpp \
Crypto/md5.cpp \
Crypto/aes192_decrypt.cpp \
Crypto/secret.cpp \
Crypto/aes256_encrypt_impl.cpp \
Crypto/sha512_256.cpp \
Crypto/sha1.cpp \
Crypto/aes128_decrypt_impl.cpp \
Crypto/aes256_encrypt.cpp \
Crypto/random_impl.cpp \
Crypto/sha256_impl.cpp \
Crypto/aes256_decrypt_impl.cpp \
Crypto/aes_impl.cpp \
Crypto/md5_impl.cpp \
Crypto/sha512_224.cpp \
Crypto/hash_functions.cpp \
Crypto/sha.cpp \
Crypto/random.cpp \
Crypto/aes256_decrypt.cpp \
Crypto/sha512_impl.cpp \
Crypto/tls_client_impl.cpp \
Crypto/sha512.cpp \
Crypto/x509.cpp \
Crypto/x509_impl.cpp \
Crypto/aes128_decrypt.cpp \
Crypto/aes192_decrypt_impl.cpp \
Crypto/tls_client.cpp \
Crypto/sha1_impl.cpp \
Crypto/secret_impl.cpp \
Crypto/rsa_impl.cpp \
Crypto/asn1.cpp \
Crypto/sha224.cpp \
Crypto/aes192_encrypt_impl.cpp \
Crypto/rsa.cpp \
Crypto/aes192_encrypt.cpp \
Crypto/aes128_encrypt_impl.cpp \
Crypto/sha256.cpp \
Crypto/aes128_encrypt.cpp

if WIN32
libclan42Core_la_SOURCES += \
System/Win32/system_win32.cpp \
System/Win32/service_win32.cpp \
IOData/Win32/directory_scanner_win32.cpp

else
libclan42Core_la_SOURCES += \
System/Unix/system_unix.cpp \
System/Unix/service_unix.cpp \
IOData/Unix/directory_scanner_unix.cpp

endif

libclan42Core_la_LIBADD = @LIBPTHREAD@

libclan42Core_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanCore)

libclan42Core_la_CXXFLAGS=$(clanCore_CXXFLAGS) $(extra_CFLAGS_clanCore)

# EOF #
