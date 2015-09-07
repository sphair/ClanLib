/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "Core/precomp.h"
#include "tls_client_impl.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Crypto/aes128_encrypt.h"
#include "API/Core/Crypto/aes128_decrypt.h"
#include "API/Core/Crypto/aes256_encrypt.h"
#include "API/Core/Crypto/aes256_decrypt.h"
#include "API/Core/IOData/file.h"
#include <ctime>
#include <algorithm>
#include "x509.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{
	TLSClient_Impl::TLSClient_Impl() :
		recv_in_data_read_pos(0), recv_out_data_read_pos(0), send_in_data_read_pos(0), send_out_data_read_pos(0), handshake_in_read_pos(0),
		conversation_state(cl_tls_state_send_client_hello), security_parameters(), protocol(), is_protocol_chosen()
	{
		// Set TLS 3.1
		protocol.major = 3;
		protocol.minor = 1;
		is_protocol_chosen = false;

		create_security_parameters_client_random();
	}

	TLSClient_Impl::~TLSClient_Impl()
	{
	}

	const void *TLSClient_Impl::get_decrypted_data() const
	{
		return recv_out_data.get_data() + recv_out_data_read_pos;
	}

	int TLSClient_Impl::get_decrypted_data_available() const
	{
		return recv_out_data.get_size() - recv_out_data_read_pos;
	}

	const void *TLSClient_Impl::get_encrypted_data() const
	{
		return send_out_data.get_data() + send_out_data_read_pos;
	}

	int TLSClient_Impl::get_encrypted_data_available() const
	{
		return send_out_data.get_size() - send_out_data_read_pos;
	}

	int TLSClient_Impl::encrypt(const void *data, int size)
	{
		if (size == 0)
			return 0;

		int insert_pos = send_in_data.get_size();
		int buffer_space_available = desired_buffer_size - insert_pos;
		int bytes_consumed = clan::min(size, buffer_space_available);

		send_in_data.set_size(insert_pos + bytes_consumed);
		memcpy(send_in_data.get_data() + insert_pos, data, bytes_consumed);

		progress_conversation();

		return bytes_consumed;
	}

	int TLSClient_Impl::decrypt(const void *data, int size)
	{
		if (size == 0)
			return 0;

		int insert_pos = recv_in_data.get_size();
		int buffer_space_available = desired_buffer_size - insert_pos;
		int bytes_consumed = clan::min(size, buffer_space_available);

		recv_in_data.set_size(insert_pos + bytes_consumed);
		memcpy(recv_in_data.get_data() + insert_pos, data, bytes_consumed);

		progress_conversation();

		return bytes_consumed;
	}

	void TLSClient_Impl::decrypted_data_consumed(int size)
	{
		if (size == 0)
			return;
		if (recv_out_data_read_pos + size > recv_out_data.get_size())
			throw Exception("TLSClient::decrypted_data_consumed misuse");

		recv_out_data_read_pos += size;
		if (recv_out_data_read_pos > desired_buffer_size / 2)
		{
			int available = recv_out_data.get_size() - recv_out_data_read_pos;
			memmove(recv_out_data.get_data(), recv_out_data.get_data() + recv_out_data_read_pos, available);
			recv_out_data.set_size(available);
			recv_out_data_read_pos = 0;
		}

		progress_conversation();
	}

	void TLSClient_Impl::encrypted_data_consumed(int size)
	{
		if (size == 0)
			return;
		if (send_out_data_read_pos + size > send_out_data.get_size())
			throw Exception("TLSClient::encrypted_data_consumed misuse");

		send_out_data_read_pos += size;
		if (send_out_data_read_pos > desired_buffer_size / 2)
		{
			int available = send_out_data.get_size() - send_out_data_read_pos;
			memmove(send_out_data.get_data(), send_out_data.get_data() + send_out_data_read_pos, available);
			send_out_data.set_size(available);
			send_out_data_read_pos = 0;
		}

		progress_conversation();
	}

	void TLSClient_Impl::progress_conversation()
	{
		try
		{
			bool should_continue;
			do
			{
				should_continue = false;
				switch (conversation_state)
				{
				case cl_tls_state_send_client_hello:
					should_continue = send_client_hello();
					break;
				case cl_tls_state_receive_server_hello:
					break;
				case cl_tls_state_receive_certificate:
					break;
				//FIXME: Implement "7.4.3. Server key exchange message"
				case cl_tls_state_receive_server_hello_done:
					break;
				// FIXME: Should be send a "client certificate message" ?
				case cl_tls_state_send_client_key_exchange:
					should_continue = send_client_key_exchange();
					break;
				case cl_tls_state_send_change_cipher_spec:
					should_continue = send_change_cipher_spec();
					break;
				case cl_tls_state_send_finished:
					should_continue = send_finished();
					break;
				case cl_tls_state_receive_change_cipher_spec:
					break;
				case cl_tls_state_receive_finished:
					break;
				case cl_tls_state_connected:
					should_continue = send_application_data();
					break;

				case cl_tls_state_error:
					return; // TBD: Should we rather throw an exception when the conversation is in error state?

				default:
					throw Exception("Unknown TLSClient conversation state");
				}

				if (receive_record())
					should_continue = true;

			} while (should_continue);
		}
		catch (...)
		{
			conversation_state = cl_tls_state_error;
			throw;
		}
	}

	bool TLSClient_Impl::send_application_data()
	{
		if (send_in_data.get_size() == send_in_data_read_pos || !can_send_record())
			return false;

		const char *data = send_in_data.get_data() + send_in_data_read_pos;
		int size = send_in_data.get_size() - send_in_data_read_pos;

		unsigned int max_record_length_gcc_fix = max_record_length;
		unsigned int data_in_record = clan::min((unsigned int)size, max_record_length_gcc_fix);

		int offset = 0;
		int offset_tls_record = offset;					offset += sizeof(TLS_Record);
		int offset_tls_appdata = offset;				offset += data_in_record;

		Secret message(offset);
		unsigned char *message_ptr = message.get_data();

		set_tls_record(message_ptr + offset_tls_record, cl_tls_content_application_data, offset - offset_tls_record);

		memcpy(message_ptr + offset_tls_appdata, data, data_in_record);

		send_record(message_ptr, offset);


		send_in_data_read_pos += data_in_record;
		if (send_in_data_read_pos > desired_buffer_size / 2 || send_in_data_read_pos == 0)
		{
			int available = send_in_data.get_size() - send_in_data_read_pos;
			memmove(send_in_data.get_data(), send_in_data.get_data() + send_in_data_read_pos, available);
			send_in_data.set_size(available);
			send_in_data_read_pos = 0;
		}

		return true;
	}

	bool TLSClient_Impl::receive_record()
	{
		// Do not read more records if our application data output buffer is full
		if (recv_out_data.get_size() - recv_out_data_read_pos >= desired_buffer_size)
			return false;

		int data_available = recv_in_data.get_size() - recv_in_data_read_pos;
		if (data_available < sizeof(TLS_Record))
			return false;

		TLS_Record record;
		memcpy(&record, recv_in_data.get_data() + recv_in_data_read_pos, sizeof(TLS_Record));

		int record_length;
		record_length = record.length[0] << 8 | record.length[1];
		if (record_length > max_record_length)
			throw Exception("Maximum record length exceeded when receieving");
		if (record_length == 0)	// The TLS Record Layer receives uninterpreted data from higher layers in non-empty blocks of arbitrary size.
			throw Exception("Received an empty block");

		if (sizeof(TLS_Record) + record_length > data_available)
			return false;

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

		record_data_buffer.set_size(record_length);
		memcpy(record_data_buffer.get_data(), recv_in_data.get_data() + recv_in_data_read_pos + sizeof(TLS_Record), record_length);

		recv_in_data_read_pos += sizeof(TLS_Record) + record_length;
		if (recv_in_data_read_pos > desired_buffer_size / 2)
		{
			int available = recv_in_data.get_size() - recv_in_data_read_pos;
			memmove(recv_in_data.get_data(), recv_in_data.get_data() + recv_in_data_read_pos, available);
			recv_in_data.set_size(available);
			recv_in_data_read_pos = 0;
		}

		DataBuffer plaintext;
		if (security_parameters.is_receive_encrypted)
			plaintext = decrypt_record(record, record_data_buffer);
		else
			plaintext = record_data_buffer;

		security_parameters.read_sequence_number++;
		if (security_parameters.read_sequence_number == 0)
			throw Exception("Sequence number wraparound");

		switch (record.type)
		{
		case cl_tls_content_change_cipher_spec:
			change_cipher_spec_data(plaintext);
			break;

		case cl_tls_content_alert:
			alert_data(plaintext);
			break;

		case cl_tls_content_handshake:
			handshake_data(plaintext);
			break;

		case cl_tls_content_application_data:
			application_data(plaintext);
			break;

		default:
			// In order to allow extension of the TLS protocol additional record types can be supported by the record protocol. 
			// If a TLS implementation receives a record type it does not understand, it should just ignore it.
			break;
		}

		return true;
	}

	void TLSClient_Impl::change_cipher_spec_data(DataBuffer record_plaintext)
	{
		if (conversation_state != cl_tls_state_receive_change_cipher_spec)
			throw Exception("Unexpected TLS change cipher record received");

		if (record_plaintext.get_size() != 1)
			throw Exception("Invalid TLS content change cipher spec size");

		security_parameters.read_sequence_number = 0;

		uint8_t value = record_plaintext.get_data<uint8_t>()[0];
		if (value != 1)
			throw Exception("TLS server change cipher spec did not send 1");

		security_parameters.is_receive_encrypted = true;

		conversation_state = cl_tls_state_receive_finished;
	}

	void TLSClient_Impl::alert_data(DataBuffer record_plaintext)
	{
		if (record_plaintext.get_size() != 2) // To do: theoretically this is not safe - it could be split into two 1 byte records.
			throw Exception("Invalid TLS content alert message");

		uint8_t *alert_data = record_plaintext.get_data<uint8_t>();

		if (alert_data[0] == cl_tls_warning)
			return;

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

	void TLSClient_Impl::handshake_data(DataBuffer record_plaintext)
	{
		// Copy handshake data into input buffer for easier processing:
		// "RFC 2246 (5.2.1) multiple client messages of the same ContentType may be coalesced into a single TLSPlaintext record"
		int pos = handshake_in_data.get_size();
		handshake_in_data.set_size(pos + record_plaintext.get_size());
		memcpy(handshake_in_data.get_data() + pos, record_plaintext.get_data(), record_plaintext.get_size());

		// Check if we have received enough data to peek at the handshake header:
		int available = handshake_in_data.get_size() - handshake_in_read_pos;
		if (available < sizeof(TLS_Handshake))
			return;

		// Check if we have received enough data to read the entire handshake message:
		TLS_Handshake &handshake = *reinterpret_cast<TLS_Handshake*>(handshake_in_data.get_data() + handshake_in_read_pos);
		int length = handshake.length[0] << 16 | handshake.length[1] << 8 | handshake.length[2];
		if (sizeof(TLS_Handshake) + length > available)
			return;

		const char *data = handshake_in_data.get_data() + handshake_in_read_pos + sizeof(TLS_Handshake);

		// We got a full message.

		// All handshake messages except handshake_finished needs to be included in the handshake hash calculation:
		if (handshake.msg_type != cl_tls_handshake_finished)
		{
			hash_handshake(&handshake, length + sizeof(TLS_Handshake));
		}

		// Dispatch message for further parsing:
		switch (handshake.msg_type)
		{
		case cl_tls_handshake_hello_request:
			handshake_hello_request_received(data, length);
			break;
		case cl_tls_handshake_client_hello:
			handshake_client_hello_received(data, length);
			break;
		case cl_tls_handshake_server_hello:
			handshake_server_hello_received(data, length);
			break;
		case cl_tls_handshake_certificate:
			handshake_certificate_received(data, length);
			break;
		case cl_tls_handshake_server_key_exchange:
			handshake_server_key_exchange_received(data, length);
			break;
		case cl_tls_handshake_certificate_request:
			handshake_certificate_request_received(data, length);
			break;
		case cl_tls_handshake_server_hello_done:
			handshake_server_hello_done_received(data, length);
			break;
		case cl_tls_handshake_certificate_verify:
			handshake_certificate_verify_received(data, length);
			break;
		case cl_tls_handshake_client_key_exchange:
			handshake_client_key_exchange_received(data, length);
			break;
		case cl_tls_handshake_finished:
			handshake_finished_received(data, length);
			break;
		default:
			throw Exception("Unknown handshake type");
		}

		// Remove processed handshake message from the input buffer:
		handshake_in_read_pos += sizeof(TLS_Handshake) + length;
		if (handshake_in_read_pos >= desired_buffer_size / 2)
		{
			available = handshake_in_data.get_size() - handshake_in_read_pos;
			memmove(handshake_in_data.get_data(), handshake_in_data.get_data() + handshake_in_read_pos, available);
			handshake_in_data.set_size(available);
		}
	}

	void TLSClient_Impl::application_data(DataBuffer record_plaintext)
	{
		if (conversation_state != cl_tls_state_connected)
			throw Exception("Unexpected application data record received");

		int pos = recv_out_data.get_size();
		recv_out_data.set_size(pos + record_plaintext.get_size());
		memcpy(recv_out_data.get_data() + pos, record_plaintext.get_data(), record_plaintext.get_size());
	}

	void TLSClient_Impl::handshake_hello_request_received(const void *data, int size)
	{
		// RFC 2246:
		// "Hello request is a simple notification that the client should
		// begin the negotiation process anew by sending a client hello
		// message when convenient. This message will be ignored by the
		// client if the client is currently negotiating a session. This
		// message may be ignored by the client if it does not wish to
		// renegotiate a session, or the client may, if it wishes, respond
		// with a no_renegotiation alert."

		// Since we allowed to do nothing, we do nothing.
	}

	void TLSClient_Impl::handshake_client_hello_received(const void *data, int size)
	{
		throw Exception("Unexpected client hello handshake message received");
	}

	void TLSClient_Impl::handshake_server_hello_received(const void *data, int size)
	{
		if (conversation_state != cl_tls_state_receive_server_hello)
			throw Exception("TLS Expected server hello");

		// The server responds with a ServerHello message, containing the chosen protocol version, 
		// a random number, CipherSuite and compression method from the choices offered by the client. 
		// To confirm or allow resumed handshakes the server may send a session ID. 
		// The chosen protocol version should be the highest that both the client and server support. 
		// For example, if the client supports TLS1.1 and the server supports TLS1.2, TLS1.1 should be 
		// selected; SSL 3.0 should not be selected

		TLS_ProtocolVersion server_version;

		copy_data(&server_version, sizeof(server_version), data, size);

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
		copy_data(security_parameters.server_random.get_data(), security_parameters.server_random.get_size(), data, size);

		// "This structure is generated by the server and must be different from (and independent of) ClientHello.random"
		if (!memcmp(security_parameters.server_random.get_data(), security_parameters.client_random.get_data(), security_parameters.server_random.get_size()))
			throw Exception("TLS Client and Server random numbers must not match");

		uint8_t session_id_length;
		copy_data(&session_id_length, 1, data, size);
		Secret session_id(session_id_length);
		copy_data(session_id.get_data(), session_id_length, data, size);

		int8_t buffer[3];
		copy_data(buffer, 3, data, size);

		select_cipher_suite(buffer[0], buffer[1]);
		select_compression_method(buffer[2]);

		conversation_state = cl_tls_state_receive_certificate;
	}

	void TLSClient_Impl::handshake_certificate_received(const void *data, int size)
	{
		if (conversation_state != cl_tls_state_receive_certificate)
			throw Exception("TLS Expected certificate");

		uint8_t buffer[3];
		copy_data(buffer, 3, data, size);

		unsigned int certificate_list_size = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
		if ( (size < certificate_list_size) || (certificate_list_size == 0) )
			throw Exception("Invalid certification size");

		while(certificate_list_size > 0)
		{
			if (certificate_list_size < 3)
				throw Exception("Invalid record length");
			copy_data(buffer, 3, data, size);
			certificate_list_size -= 3;

			unsigned int certificate_size = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
			if ( (certificate_list_size < certificate_size) || (certificate_size == 0) )
				throw Exception("Invalid certification size");

			std::vector<unsigned char> cert_buffer;
			cert_buffer.resize(certificate_size);
			copy_data(&cert_buffer[0], certificate_size, data, size);

			inspect_certificate(cert_buffer);

			certificate_list_size -= certificate_size;
		}

		conversation_state = cl_tls_state_receive_server_hello_done;
	}

	void TLSClient_Impl::handshake_server_key_exchange_received(const void *data, int size)
	{
		throw Exception("Unexpected server key exchange handshake message received");
	}

	void TLSClient_Impl::handshake_certificate_request_received(const void *data, int size)
	{
		throw Exception("Unexpected certificate request handshake message received");
	}

	void TLSClient_Impl::handshake_server_hello_done_received(const void *data, int size)
	{
		if (conversation_state != cl_tls_state_receive_server_hello_done)
			throw Exception("TLS Expected server hello done");

		set_server_public_key();

		conversation_state = cl_tls_state_send_client_key_exchange;
	}

	void TLSClient_Impl::handshake_certificate_verify_received(const void *data, int size)
	{
		throw Exception("Unexpected certificate verify handshake message received");
	}

	void TLSClient_Impl::handshake_client_key_exchange_received(const void *data, int size)
	{
		throw Exception("Unexpected client key exchange handshake message received");
	}

	void TLSClient_Impl::handshake_finished_received(const void *data, int size)
	{
		if (conversation_state != cl_tls_state_receive_finished)
			throw Exception("TLS Expected server finished");

		const int verify_data_size = 12;
		Secret server_verify_data(verify_data_size);
		copy_data(server_verify_data.get_data(), verify_data_size, data, size);

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

		conversation_state = cl_tls_state_connected;
	}

	bool TLSClient_Impl::can_send_record() const
	{
		return send_out_data.get_size() < desired_buffer_size;
	}

	void TLSClient_Impl::send_record(void *data_ptr, unsigned int data_size)
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
			Secret mac = calculate_mac(data_ptr, data_size, nullptr, 0, security_parameters.write_sequence_number, security_parameters.client_write_mac_secret);	// MAC includes the header and sequence number
			DataBuffer encrypted = encrypt_data(input_ptr , input_size, mac.get_data(), mac.get_size());

			// Update the length
			int new_length = encrypted.get_size();
			record_ptr->length[0] = new_length >> 8;
			record_ptr->length[1] = new_length;

			int pos = send_out_data.get_size();
			send_out_data.set_size(pos + sizeof(TLS_Record) + new_length);
			memcpy(send_out_data.get_data() + pos, data_ptr, sizeof(TLS_Record));
			memcpy(send_out_data.get_data() + pos + sizeof(TLS_Record), encrypted.get_data(), new_length);
		}
		else
		{
			int pos = send_out_data.get_size();
			send_out_data.set_size(pos + data_size);
			memcpy(send_out_data.get_data() + pos, data_ptr, data_size);
		}

		security_parameters.write_sequence_number++;
		if (security_parameters.write_sequence_number == 0)
			throw Exception("Sequence number wraparound");
	}

	void TLSClient_Impl::reset()
	{
		security_parameters.reset();

		client_handshake_md5_hash.reset();
		client_handshake_sha1_hash.reset();
		server_handshake_md5_hash.reset();
		server_handshake_sha1_hash.reset();
	}

	void TLSClient_Impl::copy_data(void *out_data, int size, const void *&data, int &data_left)
	{
		if (size > data_left)
			throw Exception("Invalid handshake message");
		memcpy(out_data, data, size);

		data = static_cast<const char*>(data) + size;
		data_left -= size;
	}

	void TLSClient_Impl::set_tls_record(unsigned char *dest_ptr, TLS_ContentType content_type, unsigned int length)
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

	void TLSClient_Impl::set_tls_handshake(unsigned char *dest_ptr, TLS_HandshakeType handshake_type, unsigned int length)
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

	void TLSClient_Impl::set_tls_protocol_version(unsigned char *dest_ptr)
	{
		TLS_ProtocolVersion *tls_protocol_version = (TLS_ProtocolVersion *) (dest_ptr);
		tls_protocol_version->major = protocol.major;
		tls_protocol_version->minor = protocol.minor;
	}

	void TLSClient_Impl::create_security_parameters_client_random()
	{
		unsigned char *key_ptr = security_parameters.client_random.get_data();
		m_Random.get_random_bytes(key_ptr + 4, 28);
		time_t current_time = time(nullptr);
		key_ptr[0] = current_time >> 24;
		key_ptr[1] = current_time >> 16;
		key_ptr[2] = current_time >> 8;
		key_ptr[3] = current_time;

	}

	void TLSClient_Impl::set_tls_random(unsigned char *dest_ptr, Secret &time_and_random_struct) const
	{
		memcpy(dest_ptr, time_and_random_struct.get_data(), time_and_random_struct.get_size());
	}

	int TLSClient_Impl::get_session_id_length() const
	{
		// Enough to write single NUL
		return 1;
	}

	void TLSClient_Impl::set_session_id(unsigned char *dest_ptr) const
	{
		// Session id's are not supported
		*dest_ptr = 0;
	}

	int TLSClient_Impl::get_compression_methods_length() const
	{
		// CompressionMethod compression_methods<1..2^8-1>;
		return 1+1;	// Only support cl_tls_compression_null
	}

	void TLSClient_Impl::set_compression_methods(unsigned char *dest_ptr) const
	{
		*(dest_ptr++) = 1;
		*(dest_ptr) = cl_tls_compression_null;
	}

	void TLSClient_Impl::select_compression_method(uint8_t value)
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


	int TLSClient_Impl::get_cipher_suites_length() const
	{
		// CipherSuite cipher_suites<2..2^16-1>;
		return 2 + (4*2);	// We support 4 cipher suites, each id contains 2 bytes
	}

	void TLSClient_Impl::set_cipher_suites(unsigned char *dest_ptr) const
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

	void TLSClient_Impl::select_cipher_suite(uint8_t value1, uint8_t value2)
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

	bool TLSClient_Impl::send_client_hello()
	{
		if (!can_send_record())
			return false;

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

		conversation_state = cl_tls_state_receive_server_hello;
		return true;
	}

	void TLSClient_Impl::inspect_certificate(std::vector<unsigned char> &cert)
	{
		X509 x509(&cert[0], cert.size());
		certificate_chain.push_back(x509);
	}

	bool TLSClient_Impl::send_client_key_exchange()
	{
		if (!can_send_record())
			return false;

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

		conversation_state = cl_tls_state_send_change_cipher_spec;
		return true;
	}

	void TLSClient_Impl::PRF(void *output_ptr, unsigned int output_size, const Secret &secret, const char *label_ptr, const Secret &seed_part1, const Secret &seed_part2)
	{
		const uint8_t *secret_part1 = secret.get_data();
		int secret_length = secret.get_size();
		int split_length = secret_length / 2;

		const uint8_t *secret_part2 = secret_part1 + split_length;

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

	void TLSClient_Impl::set_server_public_key()
	{
		if (certificate_chain.empty())
			throw Exception("No certificates received");

		X509 x509 = certificate_chain[0];
		x509.get_rsa_public_key(server_public_exponent, server_public_modulus);
	}

	bool TLSClient_Impl::send_change_cipher_spec()
	{
		if (!can_send_record())
			return false;

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

		conversation_state = cl_tls_state_send_finished;
		return true;
	}

	bool TLSClient_Impl::send_finished()
	{
		if (!can_send_record())
			return false;

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

		conversation_state = cl_tls_state_receive_change_cipher_spec;
		return true;
	}

	DataBuffer TLSClient_Impl::encrypt_data(const void *data_ptr, unsigned int data_size, const void *mac_ptr, unsigned int mac_size)
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

	Secret TLSClient_Impl::calculate_mac(const void *data_ptr, unsigned int data_size, const void *data2_ptr, unsigned int data2_size, uint64_t sequence_number, const Secret &mac_secret)
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

	void TLSClient_Impl::hash_handshake(const void *data_ptr, unsigned int data_size)
	{
		client_handshake_md5_hash.add(data_ptr, data_size);
		client_handshake_sha1_hash.add(data_ptr, data_size);
		server_handshake_md5_hash.add(data_ptr, data_size);
		server_handshake_sha1_hash.add(data_ptr, data_size);
	}

	DataBuffer TLSClient_Impl::decrypt_data(const void *data_ptr, unsigned int data_size)
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

	DataBuffer TLSClient_Impl::decrypt_record(TLS_Record &record, const DataBuffer &record_data)
	{
		DataBuffer decrypted = decrypt_data(record_data.get_data(), record_data.get_size());

		unsigned char *decrypted_data = (unsigned char *) decrypted.get_data();

		int decoded_size = decrypted.get_size() - security_parameters.hash_size;
		if (decoded_size < 0)
			throw Exception("Invalid decoded_size");

		// Update the length
		record.length[0] = decoded_size >> 8;
		record.length[1] = decoded_size;

		Secret mac = calculate_mac(&record, sizeof(record), decrypted_data , decoded_size, security_parameters.read_sequence_number, security_parameters.server_write_mac_secret);	// MAC includes the header and sequence number

		if (memcmp(mac.get_data(), decrypted_data + decoded_size, mac.get_size()))
			throw Exception("HMAC failed");

		decrypted.set_size(decoded_size);
		return decrypted;
	}
}
