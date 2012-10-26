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
**    Mark Page
*/

#pragma once


#include "API/Core/System/event.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/Crypto/secret.h"
#include "API/Core/Crypto/random.h"
#include "API/Core/Crypto/rsa.h"
#include "API/Core/Crypto/hash_functions.h"
#include "x509.h"

namespace clan
{

class Event;

enum TLS_ConnectionEnd
{
	cl_tls_connection_server,
	cl_tls_connection_client
};

enum TLS_BulkCipherAlgorithm
{
	cl_tls_cipher_algorithm_null,
	cl_tls_cipher_algorithm_rc4,
	cl_tls_cipher_algorithm_rc2,
	cl_tls_cipher_algorithm_des,
	cl_tls_cipher_algorithm_3des,
	cl_tls_cipher_algorithm_des40,
	cl_tls_cipher_algorithm_aes128,
	cl_tls_cipher_algorithm_aes256
};

enum TLS_CipherType
{
	cl_tls_cipher_type_stream,
	cl_tls_cipher_type_block
};

enum TLS_MACAlgorithm
{
	cl_tls_mac_algorithm_null,
	cl_tls_mac_algorithm_md5,
	cl_tls_mac_algorithm_sha,
	cl_tls_mac_algorithm_sha256
};

enum TLS_CompressionMethod
{
	cl_tls_compression_null = 0
};

enum TLS_HandshakeType {
	cl_tls_handshake_hello_request = 0,
	cl_tls_handshake_client_hello = 1, 
	cl_tls_handshake_server_hello = 2,
	cl_tls_handshake_certificate = 11, 
	cl_tls_handshake_server_key_exchange = 12,
	cl_tls_handshake_certificate_request = 13,
	cl_tls_handshake_server_hello_done = 14,
	cl_tls_handshake_certificate_verify = 15,
	cl_tls_handshake_client_key_exchange = 16,
	cl_tls_handshake_finished = 20
};

enum TLS_ContentType{
    cl_tls_content_change_cipher_spec = 20,
	cl_tls_content_alert = 21,
	cl_tls_content_handshake = 22,
    cl_tls_content_application_data = 23
};

enum TLS_AlertLevel
{
	cl_tls_warning = 1,
	cl_tls_fatal = 2
};

enum TLS_AlertDescription
{
	cl_tls_close_notify = 0,
	cl_tls_unexpected_message = 10,
	cl_tls_bad_record_mac = 20,
	cl_tls_decryption_failed = 21,
	cl_tls_record_overflow = 22,
	cl_tls_decompression_failure = 30,
	cl_tls_handshake_failure = 40,
	cl_tls_bad_certificate = 42,
	cl_tls_unsupported_certificate = 43,
	cl_tls_certificate_revoked = 44,
	cl_tls_certificate_expired = 45,
	cl_tls_certificate_unknown = 46,
	cl_tls_illegal_parameter = 47,
	cl_tls_unknown_ca = 48,
	cl_tls_access_denied = 49,
	cl_tls_decode_error = 50,
	cl_tls_decrypt_error = 51,
	cl_tls_export_restriction = 60,
	cl_tls_protocol_version = 70,
	cl_tls_insufficient_security = 71,
	cl_tls_internal_error = 80,
	cl_tls_user_canceled = 90,
	cl_tls_no_renegotiation = 100
};


typedef struct	// As stated in TLS specification
{
	ubyte8 major, minor;
} TLS_ProtocolVersion;

typedef struct {	// As stated in TLS specification
	ubyte8 type;		// TLS_ContentType
	TLS_ProtocolVersion version;
	ubyte8 length[2];
} TLS_Record;

typedef struct	// As stated in TLS specification
{
	ubyte8 msg_type;		// TLS_HandshakeType
	ubyte8 length[3];			// bytes in message
} TLS_Handshake;

typedef struct	// As stated in TLS specification
{
	ubyte8 gmt_unix_time[4];
	ubyte8 random_bytes[28];		// opaque
} TLS_Random;

class TLS_SecurityParameters
{
public:
	TLS_SecurityParameters()
	{
		reset();
	}
	void reset()
	{
		entity = cl_tls_connection_client;
		bulk_cipher_algorithm = cl_tls_cipher_algorithm_null;
		cipher_type = cl_tls_cipher_type_block;
		key_size = 0;
		key_material_length = 0;
		iv_size = 0;
		is_exportable = false;
		mac_algorithm = cl_tls_mac_algorithm_null;
		hash_size = 0;
		compression_algorithm = cl_tls_compression_null;
		master_secret = Secret(48);
		client_random = Secret(32);
		server_random = Secret(32);
		is_send_encrypted = false;
		is_receive_encrypted = false;

		client_write_mac_secret = Secret();
		server_write_mac_secret = Secret();
		client_write_key = Secret();
		server_write_key = Secret();
		client_write_iv = Secret();
		server_write_iv = Secret();
		read_sequence_number = 0;
		write_sequence_number = 0;
	}

	TLS_ConnectionEnd entity;
	TLS_BulkCipherAlgorithm bulk_cipher_algorithm;
	TLS_CipherType cipher_type;
	ubyte8 key_size;
	ubyte8 key_material_length;
	ubyte8 iv_size;
	bool is_exportable;
	TLS_MACAlgorithm mac_algorithm;
	ubyte8 hash_size;
	TLS_CompressionMethod compression_algorithm;
	Secret master_secret;
	Secret client_random;
	Secret server_random;

	Secret client_write_mac_secret;
	Secret server_write_mac_secret;
	Secret client_write_key;
	Secret server_write_key;
	Secret client_write_iv;
	Secret server_write_iv;
	ubyte64 read_sequence_number;
	ubyte64 write_sequence_number;

	bool is_send_encrypted;
	bool is_receive_encrypted;

};

class IODeviceProvider_TLSConnection : public IODeviceProvider
{
/// \name Construction
/// \{

public:
	IODeviceProvider_TLSConnection();
	~IODeviceProvider_TLSConnection();


/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	void connect(TCPConnection &device);
	void disconnect();
	int send(const void *data, int len, bool send_all);
	int receive(void *data, int len, bool receive_all);
	int peek(void *data, int len);
	IODeviceProvider *duplicate();

	void testit();

/// \}
/// \name Implementation
/// \{

private:
	static const unsigned int max_record_length = 2<<14;	// RFC 2246 (6.2.1)
	static const unsigned int max_handshake_length = 2<<24;	// RFC 2246 (implied by length in7.4)
	static const int connection_timeout_value = 5000;

	void send_client_hello();
	void reset();
	void set_tls_record(unsigned char *dest_ptr, TLS_ContentType content_type, unsigned int length);
	void set_tls_handshake(unsigned char *dest_ptr, TLS_HandshakeType handshake_type, unsigned int length);
	void set_tls_protocol_version(unsigned char *dest_ptr);
	void create_security_parameters_client_random();
	void set_tls_random(unsigned char *dest_ptr, Secret &time_and_random_struct) const;
	int get_session_id_length() const;
	void set_session_id(unsigned char *dest_ptr) const;
	int get_compression_methods_length() const;
	void set_compression_methods(unsigned char *dest_ptr) const;
	int get_cipher_suites_length() const;
	void set_cipher_suites(unsigned char *dest_ptr) const;
	void receive_server_hello();
	void receive_record(unsigned int &out_record_length, int &out_content_type);
	void receive_handshake(unsigned int &out_handshake_length, int &out_handshake_type, bool include_in_hash_handshake);
	unsigned int receive_plaintext(void *destination_ptr, unsigned int size, bool receive_all = true);
	int process_alert(int last_record_length);
	void select_cipher_suite(ubyte8 value1, ubyte8 value2);
	void select_compression_method(ubyte8 value);
	void receive_certificate();
	void inspect_certificate(std::vector<unsigned char> &cert);
	void receive_server_hello_done();
	void send_client_key_exchange();
	void set_server_public_key();
	void PRF(void *output_ptr, unsigned int output_size, const Secret &secret, const char *label_ptr, const Secret &seed_part1, const Secret &seed_part2);
	void send_change_cipher_spec();
	void send_finished();
	void hash_handshake(const void *data_ptr, unsigned int data_size);
	void send_record(void *data_ptr, unsigned int data_size);	// !< Note "data_ptr" may be written to
	void receive_change_cipher_spec();
	void receive_finished();
	void receive_record_type(unsigned int &out_record_length, int &out_content_type, TLS_ContentType expected_type);
	void decrypt_record(TLS_Record &record, std::vector<unsigned char> &plaintext);
	DataBuffer decrypt_data(const void *data_ptr, unsigned int data_size);


	Secret calculate_mac(const void *data_ptr, unsigned int data_size, const void *data2_ptr, unsigned int data2_size, ubyte64 sequence_number, const Secret &mac_secret);
	DataBuffer encrypt_data(const void *data_ptr, unsigned int data_size, const void *mac_ptr, unsigned int mac_size);

	TCPConnection connected_device;
	TLS_SecurityParameters security_parameters;
	TLS_ProtocolVersion protocol;

	DataBuffer server_public_exponent;
	DataBuffer server_public_modulus;

	bool is_protocol_chosen;	// Set by the server hello response

	// These 2 variables are only to be used by receive_plaintext() and receive_record()
	std::vector<unsigned char> receive_record_unprocessed_plaintext;
	int receive_record_unprocessed_plaintext_offset;
	int receive_record_unprocessed_plaintext_type;	// TLS_ContentType

	Random m_Random;

	MD5 client_handshake_md5_hash;
	SHA1 client_handshake_sha1_hash;
	MD5 server_handshake_md5_hash;
	SHA1 server_handshake_sha1_hash;

	std::vector<X509> certificate_chain;

/// \}
};

}
