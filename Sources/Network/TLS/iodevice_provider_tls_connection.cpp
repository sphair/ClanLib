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

#include "Network/precomp.h"
#include "iodevice_provider_tls_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Crypto/aes128_encrypt.h"
#include "API/Core/Crypto/aes128_decrypt.h"
#include "API/Core/Crypto/aes256_encrypt.h"
#include "API/Core/Crypto/aes256_decrypt.h"
#include "API/Core/IOData/file.h"
#include <ctime>
#include "x509.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Construction:

IODeviceProvider_TLSConnection::IODeviceProvider_TLSConnection()
{
}
	
IODeviceProvider_TLSConnection::~IODeviceProvider_TLSConnection()
{
	disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Attributes:

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Operations:

void IODeviceProvider_TLSConnection::connect(TCPConnection &device)
{
	disconnect();


	// Set TLS 3.1
	protocol.major = 3;
	protocol.minor = 1;
	is_protocol_chosen = false;
	receive_record_unprocessed_plaintext.reserve(2 << 14);	// Reserve 16K (max block size)
	receive_record_unprocessed_plaintext.clear();
	receive_record_unprocessed_plaintext_type = cl_tls_content_alert;
	receive_record_unprocessed_plaintext_offset = 0;

	create_security_parameters_client_random();

	connected_device = device;

	send_client_hello();
	receive_server_hello();
	receive_certificate();	// Required for AES ciphers
	//FIXME: Implement "7.4.3. Server key exchange message"

	set_server_public_key();

	receive_server_hello_done();
	//FIXME: Should be send a "client certificate message" ?
	send_client_key_exchange();

	send_change_cipher_spec();
	send_finished();

	receive_change_cipher_spec();
	receive_finished();

}

void IODeviceProvider_TLSConnection::disconnect()
{
	connected_device = TCPConnection();
	reset();
}

int IODeviceProvider_TLSConnection::send(const void *data, int len, bool send_all)
{
	int offset = 0;
	int offset_tls_record = offset;					offset += sizeof(TLS_Record);
	int offset_tls_appdata = offset;				offset += len;

	Secret message(offset);
	unsigned char *message_ptr = message.get_data();

	set_tls_record(message_ptr + offset_tls_record, cl_tls_content_application_data, offset - offset_tls_record);

	memcpy(message_ptr + offset_tls_appdata, data, len);

	send_record(message_ptr, offset);
	return len;
}

int IODeviceProvider_TLSConnection::receive(void *data, int len, bool receive_all)
{
	unsigned int record_length;
	int content_type;
	unsigned char *dest_ptr = (unsigned char *) data;

	receive_record_type(record_length, content_type, cl_tls_content_application_data);
	len = receive_plaintext(dest_ptr, len, false);
	return len;
}

int IODeviceProvider_TLSConnection::peek(void *data, int len)
{
	throw Exception("Not implemented");
}

IODeviceProvider *IODeviceProvider_TLSConnection::duplicate()
{
	throw Exception("IODeviceProvider_TLSConnection::duplicate() - duplicate not supported for TLS connections.");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TLSConnection Implementation:

void IODeviceProvider_TLSConnection::reset()
{
	security_parameters.reset();

	client_handshake_md5_hash.reset();
	client_handshake_sha1_hash.reset();
	server_handshake_md5_hash.reset();
	server_handshake_sha1_hash.reset();
}

void IODeviceProvider_TLSConnection::set_tls_record(unsigned char *dest_ptr, TLS_ContentType content_type, unsigned int length)
{
	length -= sizeof(TLS_Record);

	TLS_Record *tls_record = (TLS_Record *) (dest_ptr);
	tls_record->version = protocol;
	tls_record->type = content_type;

	if (length > max_record_length)
		throw Exception("TLS Record exceeded maximum number of bytes");

	tls_record->length[0] = length >> 8;
	tls_record->length[1] = length;
}

void IODeviceProvider_TLSConnection::set_tls_handshake(unsigned char *dest_ptr, TLS_HandshakeType handshake_type, unsigned int length)
{
	length -= sizeof(TLS_Handshake);

	TLS_Handshake *tls_handshake = (TLS_Handshake *) (dest_ptr);
	tls_handshake->msg_type = handshake_type;

	if (length > max_handshake_length)
		throw Exception("TLS handshake exceeded maximum number of bytes");

	tls_handshake->length[0] = length >> 24;
	tls_handshake->length[1] = length >> 8;
	tls_handshake->length[2] = length;
}

void IODeviceProvider_TLSConnection::set_tls_protocol_version(unsigned char *dest_ptr)
{
	TLS_ProtocolVersion *tls_protocol_version = (TLS_ProtocolVersion *) (dest_ptr);
	tls_protocol_version->major = protocol.major;
	tls_protocol_version->minor = protocol.minor;
}

void IODeviceProvider_TLSConnection::create_security_parameters_client_random()
{
	unsigned char *key_ptr = security_parameters.client_random.get_data();
	m_Random.get_random_bytes(key_ptr + 4, 28);
	time_t current_time = time(NULL);
	key_ptr[0] = current_time >> 24;
	key_ptr[1] = current_time >> 16;
	key_ptr[2] = current_time >> 8;
	key_ptr[3] = current_time;

}

void IODeviceProvider_TLSConnection::set_tls_random(unsigned char *dest_ptr, Secret &time_and_random_struct) const
{
	memcpy(dest_ptr, time_and_random_struct.get_data(), time_and_random_struct.get_size());
}

int IODeviceProvider_TLSConnection::get_session_id_length() const
{
	// Enough to write single NUL
	return 1;
}

void IODeviceProvider_TLSConnection::set_session_id(unsigned char *dest_ptr) const
{
	// Session id's are not supported
	*dest_ptr = 0;
}

int IODeviceProvider_TLSConnection::get_compression_methods_length() const
{
	// CompressionMethod compression_methods<1..2^8-1>;
	return 1+1;	// Only support cl_tls_compression_null
}

void IODeviceProvider_TLSConnection::set_compression_methods(unsigned char *dest_ptr) const
{
	*(dest_ptr++) = 1;
	*(dest_ptr) = cl_tls_compression_null;
}

void IODeviceProvider_TLSConnection::select_compression_method(ubyte8 value)
{
	switch (value)
	{
		case cl_tls_compression_null:
		{
			security_parameters.compression_algorithm = cl_tls_compression_null;
			break;
		}
		default:
			throw Exception("TLS unsupported compression method");
	}
}


int IODeviceProvider_TLSConnection::get_cipher_suites_length() const
{
	// CipherSuite cipher_suites<2..2^16-1>;
	return 2 + (4*2);	// We support 4 cipher suites, each id contains 2 bytes
}

void IODeviceProvider_TLSConnection::set_cipher_suites(unsigned char *dest_ptr) const
{
	const int num_ciphers = 4;	// If changing, you MUST change get_cipher_suites_length
	int length = num_ciphers * 2;
	*(dest_ptr++) = length >> 8;
	*(dest_ptr++) = length;

	// Strongest first ... maybe that should be controlled by the user, strong and fast first
	*(dest_ptr++) = 0x00;	*(dest_ptr++) = 0x3D;	// TLS_RSA_WITH_AES_256_CBC_SHA256
	*(dest_ptr++) = 0x00;	*(dest_ptr++) = 0x3C;	// TLS_RSA_WITH_AES_128_CBC_SHA256
	*(dest_ptr++) = 0x00;	*(dest_ptr++) = 0x35;	// TLS_RSA_WITH_AES_256_CBC_SHA
	*(dest_ptr++) = 0x00;	*(dest_ptr++) = 0x2F;	// TLS_RSA_WITH_AES_128_CBC_SHA
}

void IODeviceProvider_TLSConnection::select_cipher_suite(ubyte8 value1, ubyte8 value2)
{
	if (value1 == 0)
	{
		switch (value2)
		{
			case 0x3D:	// TLS_RSA_WITH_AES_256_CBC_SHA256
			{
				security_parameters.mac_algorithm = cl_tls_mac_algorithm_sha256;
				security_parameters.bulk_cipher_algorithm = cl_tls_cipher_algorithm_aes256;
				security_parameters.hash_size = SHA256::hash_size;
				security_parameters.iv_size = AES256_Encrypt::iv_size;
				security_parameters.key_material_length = AES256_Encrypt::key_size;
				break;
			}
			case 0x3C:	// TLS_RSA_WITH_AES_128_CBC_SHA256
			{
				security_parameters.mac_algorithm = cl_tls_mac_algorithm_sha256;
				security_parameters.bulk_cipher_algorithm = cl_tls_cipher_algorithm_aes128;
				security_parameters.hash_size = SHA256::hash_size;
				security_parameters.iv_size = AES128_Encrypt::iv_size;
				security_parameters.key_material_length = AES128_Encrypt::key_size;
				break;
			}
			case 0x35:	// TLS_RSA_WITH_AES_256_CBC_SHA
			{
				security_parameters.mac_algorithm = cl_tls_mac_algorithm_sha;
				security_parameters.bulk_cipher_algorithm = cl_tls_cipher_algorithm_aes256;
				security_parameters.hash_size = SHA1::hash_size;
				security_parameters.iv_size = AES256_Encrypt::iv_size;
				security_parameters.key_material_length = AES256_Encrypt::key_size;
				break;
			}
			case 0x2F:	// TLS_RSA_WITH_AES_128_CBC_SHA
			{
				security_parameters.mac_algorithm = cl_tls_mac_algorithm_sha;
				security_parameters.bulk_cipher_algorithm = cl_tls_cipher_algorithm_aes128;
				security_parameters.hash_size = SHA1::hash_size;
				security_parameters.iv_size = AES128_Encrypt::iv_size;
				security_parameters.key_material_length = AES128_Encrypt::key_size;
				break;
			}

			default:
				throw Exception("TLS unsupported cipher suite");
		}
	}
	else
	{
		throw Exception("TLS unsupported cipher suite");
	}
}

void IODeviceProvider_TLSConnection::send_client_hello()
{
	int offset = 0;
	int offset_tls_record = offset;					offset += sizeof(TLS_Record);
	int offset_tls_handshake = offset;				offset += sizeof(TLS_Handshake);
	int offset_tls_protocol_version = offset;		offset += sizeof(TLS_ProtocolVersion);
	int offset_tls_random = offset;					offset += sizeof(TLS_Random);
	int offset_tls_session_id = offset;				offset += get_session_id_length();
	int offset_tls_cipher_suites = offset;			offset += get_cipher_suites_length();
	int offset_tls_compression_methods = offset;	offset += get_compression_methods_length();

	Secret message(offset);	// keep data secure
	unsigned char *message_ptr = message.get_data();

	set_tls_record(message_ptr + offset_tls_record, cl_tls_content_handshake, offset - offset_tls_record);
	set_tls_handshake(message_ptr + offset_tls_handshake, cl_tls_handshake_client_hello, offset - offset_tls_handshake);
	set_tls_protocol_version(message_ptr + offset_tls_protocol_version);
	set_tls_random(message_ptr + offset_tls_random, security_parameters.client_random);
	set_session_id(message_ptr + offset_tls_session_id);
	set_cipher_suites(message_ptr + offset_tls_cipher_suites);
	set_compression_methods(message_ptr + offset_tls_compression_methods);

	hash_handshake( message_ptr + offset_tls_handshake, offset - offset_tls_handshake);

	send_record(message_ptr, offset);
}

unsigned int IODeviceProvider_TLSConnection::receive_plaintext(void *destination_ptr, unsigned int size, bool receive_all)
{
	unsigned char *dest_ptr = (unsigned char *) destination_ptr;
	while(size > 0)
	{
		// Check for unprocessed cached plaintext
		if (receive_record_unprocessed_plaintext.size())
		{
			int size_to_copy = receive_record_unprocessed_plaintext.size() - receive_record_unprocessed_plaintext_offset;
			if (size_to_copy > size)
				size_to_copy = size;

			memcpy(dest_ptr, &receive_record_unprocessed_plaintext[receive_record_unprocessed_plaintext_offset], size_to_copy);
			receive_record_unprocessed_plaintext_offset += size_to_copy;
			if (receive_record_unprocessed_plaintext_offset >= receive_record_unprocessed_plaintext.size())
			{
				receive_record_unprocessed_plaintext.clear();
				receive_record_unprocessed_plaintext_offset = 0;
			}
			dest_ptr += size_to_copy;
			size -= size_to_copy;
		}

		// Need to read more data from the socket (RFC 2246 (6.2.1) "a single message may be fragmented across several records").
		if(size > 0)
		{
			if (!receive_all)
			{
				break;
			}

			unsigned int record_length;	// Indirectly this will be saved into receive_record_unprocessed_plaintext.size()
			int content_type;
			int last_receive_record_unprocessed_plaintext_type = receive_record_unprocessed_plaintext_type;
			receive_record(record_length, content_type);

			if (content_type != last_receive_record_unprocessed_plaintext_type)
				throw Exception("Unexpected TLS content type, mixed content in fragment");
		}
	}
	return dest_ptr - ((unsigned char *) destination_ptr);

}

void IODeviceProvider_TLSConnection::receive_record(unsigned int &out_record_length, int &out_content_type)
{
	// We have a cached unprocessed plaintext
	// "RFC 2246 (5.2.1) multiple client messages of the same ContentType may be coalesced into a single TLSPlaintext record"
	if (receive_record_unprocessed_plaintext.size())
	{
		// Create a fake record
		out_content_type = receive_record_unprocessed_plaintext_type;
		out_record_length = receive_record_unprocessed_plaintext.size() - receive_record_unprocessed_plaintext_offset;
		return;
	}

	TLS_Record record;
	int offset = 0;
	int size_left = sizeof(TLS_Record);
	do
	{
		bool wakeup_reason = connected_device.get_read_event().wait(connection_timeout_value);
		if (!wakeup_reason)
			throw Exception("Time out receiving TLS record");

		int size = connected_device.receive(((unsigned char *) &record) + offset, size_left, false);
		if (!size)
			throw Exception("Server disconnected the connection");

		size_left -=size;
		offset += size;
	} while(size_left > 0);

	int record_length;
	record_length = record.length[0] << 8 | record.length[1];
	if (record_length > max_record_length)
		throw Exception("Maximum record length exceeded when receieving");
	if (record_length == 0)	// The TLS Record Layer receives uninterpreted data from higher layers in non-empty blocks of arbitrary size.
		throw Exception("Received an empty block");

	if (is_protocol_chosen)
	{
		if ((record.version.major != protocol.major) || (record.version.minor != protocol.minor))
		{
			throw Exception("Protocol changed after initial exchange");
		}
	}
	else
	{
		// We set the protocol version in ServerHello
	}

	receive_record_unprocessed_plaintext.resize(record_length);
	receive_record_unprocessed_plaintext_type = record.type;
	receive_record_unprocessed_plaintext_offset = 0;
	offset = 0;
	do
	{
		bool wakeup_reason = connected_device.get_read_event().wait(connection_timeout_value);
		if (!wakeup_reason)
			throw Exception("Time out receiving plaintext");

		int receive_size = connected_device.receive(&receive_record_unprocessed_plaintext[offset], record_length, false);
		if (!receive_size)
			throw Exception("Server disconnected the connection");
		record_length -= receive_size;
		offset += receive_size;
	} while(record_length > 0);

	if (security_parameters.is_receive_encrypted)
		decrypt_record(record, receive_record_unprocessed_plaintext);

	out_record_length = receive_record_unprocessed_plaintext.size();
	out_content_type = record.type;

	security_parameters.read_sequence_number++;
	if (security_parameters.read_sequence_number == 0)
		throw Exception("Sequence number wraparound");

}

void IODeviceProvider_TLSConnection::receive_record_type(unsigned int &out_record_length, int &out_content_type, TLS_ContentType expected_type)
{
	// In order to allow extension of the TLS protocol additional record types can be supported by the record protocol. 
	// If a TLS implementation receives a record type it does not understand, it should just ignore it.

	while(true)
	{
		receive_record(out_record_length, out_content_type);
		if (out_content_type == cl_tls_content_alert)
		{
			process_alert(out_record_length);
		}
		else if (out_content_type == expected_type)
		{
			break;
		}

		// Ignore record and try again
		receive_record_unprocessed_plaintext.clear();
	}

}

int IODeviceProvider_TLSConnection::process_alert(int last_record_length)
{
	const int alert_data_size = 2;
	ubyte8 alert_data[alert_data_size];
	receive_plaintext(alert_data, alert_data_size);

	if (alert_data[0] == cl_tls_warning)
		return alert_data[1];

	const char *desc = "Unknown";

	switch (alert_data[1])
	{
		case cl_tls_close_notify:
			desc = "close_notify";
			break;

		case cl_tls_unexpected_message:
			desc = "unexpected_message";
			break;

		case cl_tls_bad_record_mac:
			desc = "bad_record_mac";
			break;

		case cl_tls_decryption_failed:
			desc = "decryption_failed";
			break;

		case cl_tls_record_overflow:
			desc = "record_overflow";
			break;

		case cl_tls_decompression_failure:
			desc = "decompression_failure";
			break;

		case cl_tls_handshake_failure:
			desc = "handshake_failure";
			break;

		case cl_tls_bad_certificate:
			desc = "bad_certificate";
			break;

		case cl_tls_unsupported_certificate:
			desc = "unsupported_certificate";
			break;

		case cl_tls_certificate_revoked:
			desc = "certificate_revoked";
			break;

		case cl_tls_certificate_expired:
			desc = "certificate_expired";
			break;

		case cl_tls_certificate_unknown:
			desc = "certificate_unknown";
			break;

		case cl_tls_illegal_parameter:
			desc = "illegal_parameter";
			break;

		case cl_tls_unknown_ca:
			desc = "unknown_ca";
			break;

		case cl_tls_access_denied:
			desc = "access_denied";
			break;

		case cl_tls_decode_error:
			desc = "decode_error";
			break;

		case cl_tls_decrypt_error:
			desc = "decrypt_error";
			break;

		case cl_tls_export_restriction:
			desc = "export_restriction";
			break;

		case cl_tls_protocol_version:
			desc = "protocol_version";
			break;

		case cl_tls_insufficient_security:
			desc = "insufficient_security";
			break;

		case cl_tls_internal_error:
			desc = "internal_error";
			break;

		case cl_tls_user_canceled:
			desc = "user_canceled";
			break;

		case cl_tls_no_renegotiation:
			desc = "no_renegotiation";
			break;
	}

	std::string string(string_format("TLS Alert %1", desc));
	throw Exception(string);
}

void IODeviceProvider_TLSConnection::receive_handshake(unsigned int &out_handshake_length, int &out_handshake_type, bool include_in_hash_handshake)
{
	TLS_Handshake handshake;

	unsigned int record_length;
	int content_type;

	receive_record_type(record_length, content_type, cl_tls_content_handshake);

	if (include_in_hash_handshake)
	{
		if (!receive_record_unprocessed_plaintext_offset)	// Don't hash handshake if already processed on fragmented items
		{
			hash_handshake( &receive_record_unprocessed_plaintext[0], receive_record_unprocessed_plaintext.size() );
		}
	}

	receive_plaintext(&handshake, sizeof(handshake));

	out_handshake_length = handshake.length[0] << 16 | handshake.length[1] << 8 | handshake.length[2];
	out_handshake_type = handshake.msg_type;
}

void IODeviceProvider_TLSConnection::receive_server_hello()
{
	// The server responds with a ServerHello message, containing the chosen protocol version, 
	// a random number, CipherSuite and compression method from the choices offered by the client. 
	// To confirm or allow resumed handshakes the server may send a session ID. 
	// The chosen protocol version should be the highest that both the client and server support. 
	// For example, if the client supports TLS1.1 and the server supports TLS1.2, TLS1.1 should be 
	// selected; SSL 3.0 should not be selected

	unsigned int handshake_length;
	int out_handshake_type;
	receive_handshake(handshake_length, out_handshake_type, true);

	if (out_handshake_type != cl_tls_handshake_server_hello)
		throw Exception("TLS Expected server hello");

	TLS_ProtocolVersion server_version;

	receive_plaintext(&server_version, sizeof(server_version) );

	bool invalid_protocol = false;

	// Check server version is not higher than this version
	if (server_version.major > protocol.major)
		invalid_protocol = true;

	if (server_version.major == protocol.major)
		if (server_version.minor > protocol.minor)
			invalid_protocol = true;

	// Check minimum is 3.1
	if (server_version.major < 3)
		invalid_protocol = true;

	if (server_version.major == 3)
		if (server_version.minor < 1)
			invalid_protocol = true;

	if (invalid_protocol)
		throw Exception("Server Protocol not supported");

	// Use the server protocol (will be lower that "protocol" but higher than 3.1)
	protocol.major = server_version.major;
	protocol.minor = server_version.minor;

	is_protocol_chosen = true;

	// Get TLS_Random
	receive_plaintext(security_parameters.server_random.get_data(), security_parameters.server_random.get_size() );

	// "This structure is generated by the server and must be different from (and independent of) ClientHello.random"
	if (!memcmp(security_parameters.server_random.get_data(), security_parameters.client_random.get_data(), security_parameters.server_random.get_size()))
		throw Exception("TLS Client and Server random numbers must not match");

	ubyte8 session_id_length;
	receive_plaintext(&session_id_length, 1);
	Secret session_id(session_id_length);
	receive_plaintext(session_id.get_data(), session_id_length);

	byte8 buffer[3];
	receive_plaintext(buffer, 3);

	select_cipher_suite(buffer[0], buffer[1]);
	select_compression_method(buffer[2]);

}

void IODeviceProvider_TLSConnection::receive_certificate()
{
	unsigned int handshake_length;
	int out_handshake_type;
	receive_handshake(handshake_length, out_handshake_type, true);

	if (out_handshake_type != cl_tls_handshake_certificate)
		throw Exception("TLS Expected certificate");

	if (handshake_length < 3)
		throw Exception("Invalid record length");
	ubyte8 buffer[3];
	receive_plaintext(buffer, 3);
	handshake_length -= 3;

	unsigned int certificate_list_size = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
	if ( (handshake_length < certificate_list_size) || (certificate_list_size == 0) )
		throw Exception("Invalid certification size");

	while(certificate_list_size > 0)
	{
		if (certificate_list_size < 3)
			throw Exception("Invalid record length");
		receive_plaintext(buffer, 3);
		certificate_list_size -= 3;

		unsigned int certificate_size = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
		if ( (certificate_list_size < certificate_size) || (certificate_size == 0) )
			throw Exception("Invalid certification size");

		std::vector<unsigned char> cert_buffer;
		cert_buffer.resize(certificate_size);
		receive_plaintext(&cert_buffer[0], certificate_size);

		inspect_certificate(cert_buffer);

		certificate_list_size -= certificate_size;

	}
}

void IODeviceProvider_TLSConnection::inspect_certificate(std::vector<unsigned char> &cert)
{
	X509 x509(&cert[0], cert.size());
	certificate_chain.push_back(x509);
}

void IODeviceProvider_TLSConnection::receive_server_hello_done()
{
	unsigned int handshake_length;
	int out_handshake_type;
	receive_handshake(handshake_length, out_handshake_type, true);

	if (out_handshake_type != cl_tls_handshake_server_hello_done)
		throw Exception("TLS Expected server hello done");
}

void IODeviceProvider_TLSConnection::send_client_key_exchange()
{
	// If RSA is being used for key agreement and authentication, the
	// client generates a 48-byte premaster secret, encrypts it using
	// the public key from the server's certificate or the temporary RSA
	// key provided in a server key exchange message, and sends the
	// result in an encrypted premaster secret message. This structure
	// is a variant of the client key exchange message, not a message in
	// itself.
	Secret pre_master_secret(48);
	unsigned char *pms_ptr = pre_master_secret.get_data();
	m_Random.get_random_bytes(pms_ptr + 2, 46);
	pms_ptr[0] = protocol.major;	// Version number
	pms_ptr[1] = protocol.minor;

	DataBuffer wrapped_pre_master_secret = RSA::encrypt(2, m_Random, server_public_exponent,  server_public_modulus, pre_master_secret);

	PRF(security_parameters.master_secret.get_data(), security_parameters.master_secret.get_size(), pre_master_secret, "master secret", security_parameters.client_random, security_parameters.server_random);

	Secret key_block( 2 * (security_parameters.hash_size + security_parameters.key_material_length + security_parameters.iv_size ) );
	PRF(key_block.get_data(), key_block.get_size(), security_parameters.master_secret, "key expansion", security_parameters.server_random, security_parameters.client_random);

	security_parameters.client_write_mac_secret = Secret(security_parameters.hash_size);
	security_parameters.server_write_mac_secret = Secret(security_parameters.hash_size);
	security_parameters.client_write_key = Secret(security_parameters.key_material_length);
	security_parameters.server_write_key = Secret(security_parameters.key_material_length);
	security_parameters.client_write_iv = Secret(security_parameters.iv_size);
	security_parameters.server_write_iv = Secret(security_parameters.iv_size);

	unsigned char *key_block_ptr = key_block.get_data();

	memcpy(security_parameters.client_write_mac_secret.get_data(), key_block_ptr, security_parameters.client_write_mac_secret.get_size());
	key_block_ptr+=security_parameters.client_write_mac_secret.get_size();

	memcpy(security_parameters.server_write_mac_secret.get_data(), key_block_ptr, security_parameters.server_write_mac_secret.get_size());
	key_block_ptr+=security_parameters.server_write_mac_secret.get_size();

	memcpy(security_parameters.client_write_key.get_data(), key_block_ptr, security_parameters.client_write_key.get_size());
	key_block_ptr+=security_parameters.client_write_key.get_size();

	memcpy(security_parameters.server_write_key.get_data(), key_block_ptr, security_parameters.server_write_key.get_size());
	key_block_ptr+=security_parameters.server_write_key.get_size();

	memcpy(security_parameters.client_write_iv.get_data(), key_block_ptr, security_parameters.client_write_iv.get_size());
	key_block_ptr+=security_parameters.client_write_iv.get_size();

	memcpy(security_parameters.server_write_iv.get_data(), key_block_ptr, security_parameters.server_write_iv.get_size());
	key_block_ptr+=security_parameters.server_write_iv.get_size();

	const int wrapped_pre_master_secret_length = wrapped_pre_master_secret.get_size();

	int offset = 0;
	int offset_tls_record = offset;					offset += sizeof(TLS_Record);
	int offset_tls_handshake = offset;				offset += sizeof(TLS_Handshake);
	int offset_tls_encrypted_pre_master_secret_length = offset;	offset+= 2;
	int offset_tls_encrypted_pre_master_secret = offset;	offset+= wrapped_pre_master_secret_length;

	Secret message(offset);	// keep data secure
	unsigned char *message_ptr = message.get_data();
	set_tls_record(message_ptr + offset_tls_record, cl_tls_content_handshake, offset - offset_tls_record);
	set_tls_handshake(message_ptr + offset_tls_handshake, cl_tls_handshake_client_key_exchange, offset - offset_tls_handshake);

	memcpy(message_ptr + offset_tls_encrypted_pre_master_secret, wrapped_pre_master_secret.get_data(), wrapped_pre_master_secret_length);
	message_ptr[offset_tls_encrypted_pre_master_secret_length] = wrapped_pre_master_secret_length >> 8;
	message_ptr[offset_tls_encrypted_pre_master_secret_length+1] = wrapped_pre_master_secret_length;

	hash_handshake( message_ptr + offset_tls_handshake, offset - offset_tls_handshake);

	send_record(message_ptr, offset);
}

void IODeviceProvider_TLSConnection::testit()
{

}

void IODeviceProvider_TLSConnection::PRF(void *output_ptr, unsigned int output_size, const Secret &secret, const char *label_ptr, const Secret &seed_part1, const Secret &seed_part2)
{
	const ubyte8 *secret_part1 = secret.get_data();
	int secret_length = secret.get_size();
	int split_length = secret_length / 2;

	const ubyte8 *secret_part2 = secret_part1 + split_length;

	if (secret_length & 1)	// Is an odd number
	{
		split_length+=1;	// ceil( double(secret_length) / 2.0f )
		secret_part2-=1;	// Sharing a byte
	}

	int label_length = strlen(label_ptr);

	Secret md5_output_a(MD5::hash_size);
	Secret md5_output_b(MD5::hash_size);
	Secret sha1_output_a(SHA1::hash_size);
	Secret sha1_output_b(SHA1::hash_size);

	MD5 md5;
	md5.set_hmac(secret_part1, split_length);
	md5.add(label_ptr, label_length);
	md5.add(seed_part1.get_data(), seed_part1.get_size());
	md5.add(seed_part2.get_data(), seed_part2.get_size());
	md5.calculate();
	md5.get_hash(md5_output_a.get_data());

	SHA1 sha1;
	
	sha1.set_hmac(secret_part2, split_length);
	sha1.add(label_ptr, label_length);
	sha1.add(seed_part1.get_data(), seed_part1.get_size());
	sha1.add(seed_part2.get_data(), seed_part2.get_size());
	sha1.calculate();
	sha1.get_hash(sha1_output_a.get_data());

	unsigned char *out_ptr = ( unsigned char *) output_ptr;
	unsigned char *md5_ptr = md5_output_b.get_data();
	unsigned char *sha1_ptr = sha1_output_b.get_data();
	int md5_position = MD5::hash_size;
	int sha1_position = SHA1::hash_size;
	for (int cnt=0; cnt<output_size; cnt++)
	{
		if (md5_position == MD5::hash_size)
		{
			md5.set_hmac(secret_part1, split_length);
			md5.add(md5_output_a.get_data(), md5_output_a.get_size());
			md5.add(label_ptr, label_length);
			md5.add(seed_part1.get_data(), seed_part1.get_size());
			md5.add(seed_part2.get_data(), seed_part2.get_size());
			md5.calculate();
			md5.get_hash(md5_ptr);

			md5.set_hmac(secret_part1, split_length);
			md5.add(md5_output_a.get_data(), md5_output_a.get_size());
			md5.calculate();
			md5.get_hash(md5_output_a.get_data());

			md5_position = 0;
		}

		if (sha1_position == SHA1::hash_size)
		{
			sha1.set_hmac(secret_part2, split_length);
			sha1.add(sha1_output_a.get_data(), sha1_output_a.get_size());
			sha1.add(label_ptr, label_length);
			sha1.add(seed_part1.get_data(), seed_part1.get_size());
			sha1.add(seed_part2.get_data(), seed_part2.get_size());
			sha1.calculate();
			sha1.get_hash(sha1_ptr);

			sha1.set_hmac(secret_part2, split_length);
			sha1.add(sha1_output_a.get_data(), sha1_output_a.get_size());
			sha1.calculate();
			sha1.get_hash(sha1_output_a.get_data());

			sha1_position = 0;
		}

		*(out_ptr++) = md5_ptr[md5_position] ^ sha1_ptr[sha1_position];
		md5_position++;
		sha1_position++;
	}

}

void IODeviceProvider_TLSConnection::set_server_public_key()
{
	if (certificate_chain.empty())
		throw Exception("No certificates received");

	X509 x509 = certificate_chain[0];
	x509.get_rsa_public_key(server_public_exponent, server_public_modulus);
}

void IODeviceProvider_TLSConnection::send_change_cipher_spec()
{
	int offset = 0;
	int offset_tls_record = offset;					offset += sizeof(TLS_Record);
	int offset_tls_change_cipher_spec = offset;		offset += 1;
	
	Secret message(offset);	// keep data secure
	unsigned char *message_ptr = message.get_data();

	set_tls_record(message_ptr + offset_tls_record, cl_tls_content_change_cipher_spec, offset - offset_tls_record);
	message_ptr[offset_tls_change_cipher_spec] = 1;
	send_record(message_ptr, offset);

	security_parameters.is_send_encrypted = true;
	security_parameters.write_sequence_number = 0;

	testit();


}

void IODeviceProvider_TLSConnection::send_finished()
{
	const int verify_data_size = 12;
	int offset = 0;
	int offset_tls_record = offset;					offset += sizeof(TLS_Record);
	int offset_tls_handshake = offset;				offset += sizeof(TLS_Handshake);
	int offset_tls_finished = offset;				offset += verify_data_size;

	Secret message(offset);	// keep data secure
	unsigned char *message_ptr = message.get_data();

	set_tls_record(message_ptr + offset_tls_record, cl_tls_content_handshake, offset - offset_tls_record);
	set_tls_handshake(message_ptr + offset_tls_handshake, cl_tls_handshake_finished, offset - offset_tls_handshake);

	Secret md5_handshake_messages(MD5::hash_size);
	Secret sha1_handshake_messages(SHA1::hash_size);

	client_handshake_md5_hash.calculate();
	client_handshake_sha1_hash.calculate();

	client_handshake_md5_hash.get_hash(md5_handshake_messages.get_data());
	client_handshake_sha1_hash.get_hash(sha1_handshake_messages.get_data());

	PRF(message_ptr + offset_tls_finished, verify_data_size, security_parameters.master_secret, "client finished", md5_handshake_messages, sha1_handshake_messages);

	hash_handshake( message_ptr + offset_tls_handshake, offset - offset_tls_handshake);
	send_record(message_ptr, offset);
}

void IODeviceProvider_TLSConnection::send_record(void *data_ptr, unsigned int data_size)
{
	TLS_Record *record_ptr = (TLS_Record *) data_ptr;

	int record_length;
	record_length = record_ptr->length[0] << 8 | record_ptr->length[1];
	if (record_length > max_record_length)
		throw Exception("Maximum record length exceeded when sending");
	if (record_length == 0)
		throw Exception("Trying to send an empty block");

	if (record_length + sizeof(TLS_Record) != data_size)
		throw Exception("Record length mismatch");

	if (security_parameters.is_send_encrypted)
	{
		// "the encryption and MAC functions convert TLSCompressed.fragment structures to and from block TLSCiphertext.fragment structures."
		const unsigned char *input_ptr = (const unsigned char *) data_ptr + sizeof(TLS_Record);
		unsigned int input_size = data_size - sizeof(TLS_Record);
		Secret mac = calculate_mac(data_ptr, data_size, NULL, 0, security_parameters.write_sequence_number, security_parameters.client_write_mac_secret);	// MAC includes the header and sequence number
		DataBuffer encrypted = encrypt_data(input_ptr , input_size, mac.get_data(), mac.get_size());

		// Update the length
		int new_length = encrypted.get_size();
		record_ptr->length[0] = new_length >> 8;
		record_ptr->length[1] = new_length;

		connected_device.write(data_ptr, sizeof(TLS_Record));
		connected_device.write(encrypted.get_data(), new_length);
	}
	else
	{
		connected_device.write(data_ptr, data_size);
	}

	security_parameters.write_sequence_number++;
	if (security_parameters.write_sequence_number == 0)
		throw Exception("Sequence number wraparound");


}

DataBuffer IODeviceProvider_TLSConnection::encrypt_data(const void *data_ptr, unsigned int data_size, const void *mac_ptr, unsigned int mac_size)
{
	int additional_unpadded_blocks;
	m_Random.get_random_bool() ? additional_unpadded_blocks = 1 : additional_unpadded_blocks = 0;

	DataBuffer buffer;
	if (security_parameters.bulk_cipher_algorithm == cl_tls_cipher_algorithm_aes128)
	{
		AES128_Encrypt encrypt;
		encrypt.set_padding(true, false, additional_unpadded_blocks);
		encrypt.set_iv(security_parameters.client_write_iv.get_data());
		encrypt.set_key(security_parameters.client_write_key.get_data());
		encrypt.add(data_ptr, data_size);
		encrypt.add(mac_ptr, mac_size);
		encrypt.calculate();
		buffer = encrypt.get_data();
	}
	else if (security_parameters.bulk_cipher_algorithm == cl_tls_cipher_algorithm_aes256)
	{
		AES256_Encrypt encrypt;
		encrypt.set_padding(true, false, additional_unpadded_blocks);
		encrypt.set_iv(security_parameters.client_write_iv.get_data());
		encrypt.set_key(security_parameters.client_write_key.get_data());
		encrypt.add(data_ptr, data_size);
		encrypt.add(mac_ptr, mac_size);
		encrypt.calculate();
		buffer = encrypt.get_data();
	}
	else
	{
		throw Exception("Unsupported cipher");
	}
	memcpy(security_parameters.client_write_iv.get_data(), buffer.get_data() + buffer.get_size() - security_parameters.client_write_iv.get_size(), security_parameters.client_write_iv.get_size());
	return buffer;

}

Secret IODeviceProvider_TLSConnection::calculate_mac(const void *data_ptr, unsigned int data_size, const void *data2_ptr, unsigned int data2_size, ubyte64 sequence_number, const Secret &mac_secret)
{
	unsigned char sequence_number_buffer[8];

	sequence_number_buffer[0] = sequence_number >> 56; 
	sequence_number_buffer[1] = sequence_number >> 48; 
	sequence_number_buffer[2] = sequence_number >> 40; 
	sequence_number_buffer[3] = sequence_number >> 32; 
	sequence_number_buffer[4] = sequence_number >> 24; 
	sequence_number_buffer[5] = sequence_number >> 16; 
	sequence_number_buffer[6] = sequence_number >> 8; 
	sequence_number_buffer[7] = sequence_number; 

	if (security_parameters.mac_algorithm == cl_tls_mac_algorithm_sha)
	{
		Secret hash(20);
		SHA1 sha1;
		sha1.set_hmac(mac_secret.get_data(), mac_secret.get_size());
		sha1.add(sequence_number_buffer, 8);
		if (data_ptr)
			sha1.add(data_ptr, data_size);
		if (data2_ptr)
			sha1.add(data2_ptr, data2_size);
		sha1.calculate();
		sha1.get_hash(hash.get_data());
		return hash;
	}
	else if (security_parameters.mac_algorithm == cl_tls_mac_algorithm_sha256)
	{
		Secret hash(32);
		SHA256 sha256;
		sha256.set_hmac(mac_secret.get_data(), mac_secret.get_size());
		sha256.add(sequence_number_buffer, 8);
		if (data_ptr)
			sha256.add(data_ptr, data_size);
		if (data2_ptr)
			sha256.add(data2_ptr, data2_size);
		sha256.calculate();
		sha256.get_hash(hash.get_data());
		return hash;
	}
	else
	{
		throw Exception("Unsupported MAC");
	}
}

void IODeviceProvider_TLSConnection::hash_handshake(const void *data_ptr, unsigned int data_size)
{
	client_handshake_md5_hash.add(data_ptr, data_size);
	client_handshake_sha1_hash.add(data_ptr, data_size);
	server_handshake_md5_hash.add(data_ptr, data_size);
	server_handshake_sha1_hash.add(data_ptr, data_size);
}

void IODeviceProvider_TLSConnection::receive_change_cipher_spec()
{
	unsigned int record_length;
	int content_type;

	receive_record_type(record_length, content_type, cl_tls_content_change_cipher_spec);

	security_parameters.read_sequence_number = 0;

	ubyte8 value;
	receive_plaintext(&value, 1);
	if (value != 1)
		throw Exception("TLS server change cipher spec did not send 1");

	security_parameters.is_receive_encrypted = true;

}

void IODeviceProvider_TLSConnection::receive_finished()
{
	unsigned int handshake_length;
	int out_handshake_type;
	receive_handshake(handshake_length, out_handshake_type, false);

	if (out_handshake_type != cl_tls_handshake_finished)
		throw Exception("TLS Expected server finished");

	const int verify_data_size = 12;
	Secret server_verify_data(verify_data_size);
	receive_plaintext(server_verify_data.get_data(), verify_data_size);

	Secret client_verify_data(verify_data_size);

	Secret md5_handshake_messages(16);
	Secret sha1_handshake_messages(20);

	server_handshake_md5_hash.calculate();
	server_handshake_sha1_hash.calculate();

	server_handshake_md5_hash.get_hash(md5_handshake_messages.get_data());
	server_handshake_sha1_hash.get_hash(sha1_handshake_messages.get_data());

	PRF(client_verify_data.get_data(), verify_data_size, security_parameters.master_secret, "server finished", md5_handshake_messages, sha1_handshake_messages);

	if (memcmp(client_verify_data.get_data(), server_verify_data.get_data(), verify_data_size))
		throw Exception("TLS server finished verify data failed");

}

DataBuffer IODeviceProvider_TLSConnection::decrypt_data(const void *data_ptr, unsigned int data_size)
{
	DataBuffer buffer;
	if (security_parameters.bulk_cipher_algorithm == cl_tls_cipher_algorithm_aes128)
	{
		AES128_Decrypt decrypt;
		decrypt.set_padding(true, false);
		decrypt.set_iv(security_parameters.server_write_iv.get_data());
		decrypt.set_key(security_parameters.server_write_key.get_data());
		decrypt.add(data_ptr, data_size);
		decrypt.calculate();
		buffer = decrypt.get_data();
	}
	else if (security_parameters.bulk_cipher_algorithm == cl_tls_cipher_algorithm_aes256)
	{
		AES256_Decrypt decrypt;
		decrypt.set_padding(true, false);
		decrypt.set_iv(security_parameters.server_write_iv.get_data());
		decrypt.set_key(security_parameters.server_write_key.get_data());
		decrypt.add(data_ptr, data_size);
		decrypt.calculate();
		buffer = decrypt.get_data();
	}
	else
	{
		throw Exception("Unsupported cipher");
	}
	const unsigned char *last_block = (const unsigned char *) data_ptr;
	last_block += data_size - security_parameters.server_write_iv.get_size();
	memcpy(security_parameters.server_write_iv.get_data(), last_block, security_parameters.server_write_iv.get_size());
	return buffer;

}

void IODeviceProvider_TLSConnection::decrypt_record(TLS_Record &record, std::vector<unsigned char> &plaintext)
{
	DataBuffer decrypted = decrypt_data(&plaintext[0] , plaintext.size());

	unsigned char *decrypted_data = (unsigned char *) decrypted.get_data();

	int decoded_size = decrypted.get_size() - security_parameters.hash_size;

	// Update the length
	record.length[0] = decoded_size >> 8;
	record.length[1] = decoded_size;

	Secret mac = calculate_mac(&record, sizeof(record), decrypted_data , decoded_size, security_parameters.read_sequence_number, security_parameters.server_write_mac_secret);	// MAC includes the header and sequence number

	if (memcmp(mac.get_data(), decrypted_data + decoded_size, mac.get_size()))
		throw Exception("HMAC failed");

	// Copy the data
	plaintext.resize(decoded_size);
	memcpy(&plaintext[0], decrypted_data, decoded_size);

}

}
