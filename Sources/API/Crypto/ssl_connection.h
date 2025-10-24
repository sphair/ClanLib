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

#pragma once

/// \addtogroup clanCrypto_System clanCrypto System
/// \{

#include <prio.h>
#include <certt.h>
#include <keyt.h>
#include <ssl.h>

class CL_TCPConnection;
class CL_PK11PasswordHandler;

/// \brief SSL Connection class
///
/// \xmlonly !group=Crypto/System! !header=crypto.h! \endxmlonly
class CL_SSLConnection
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a SSLConnection
	///
	/// \param connection = TCPConnection
	/// \param handler = PK11 Password Handler
	CL_SSLConnection(CL_TCPConnection *connection, CL_PK11PasswordHandler *handler);

	~CL_SSLConnection();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Create prfd
	///
	/// \return PRFile Desc
	PRFileDesc *create_prfd();

	PRFileDesc *ssl_fd;

	CL_TCPConnection *connection;

	CL_PK11PasswordHandler *password_handler;

	/// \brief Auth certificate
	///
	/// \param arg = void
	/// \param ssl_fd = PRFile Desc
	/// \param check_sig = PRBool
	/// \param is_server = PRBool
	///
	/// \return SECStatus
	static SECStatus auth_certificate(void *arg, PRFileDesc *ssl_fd, PRBool check_sig, PRBool is_server);

	/// \brief Bad certificate
	///
	/// \param arg = void
	/// \param ssl_fd = PRFile Desc
	///
	/// \return SECStatus
	static SECStatus bad_certificate(void *arg, PRFileDesc *ssl_fd);

	static SECStatus get_client_auth_data(
		void *arg,
		PRFileDesc *fd,
		CERTDistNames *ca_names,
		CERTCertificate **ret_cert,
		SECKEYPrivateKey **ret_key);

	/// \brief Handshake completed
	///
	/// \param ssl_fd = PRFile Desc
	/// \param arg = void
	static void handshake_completed(PRFileDesc *ssl_fd, void *arg);

	/// \brief Prfd dtor
	///
	/// \param fd = PRFile Desc
	static void prfd_dtor(PRFileDesc *fd);

	/// \brief Prfd close
	///
	/// \param fd = PRFile Desc
	///
	/// \return PRStatus
	static PRStatus prfd_close(PRFileDesc *fd);

	/// \brief Prfd read
	///
	/// \param fd = PRFile Desc
	/// \param buf = void
	/// \param amount = PRInt32
	///
	/// \return PRInt32
	static PRInt32 prfd_read(PRFileDesc *fd, void *buf, PRInt32 amount);

	/// \brief Prfd write
	///
	/// \param fd = PRFile Desc
	/// \param buf = void
	/// \param amount = PRInt32
	///
	/// \return PRInt32
	static PRInt32 prfd_write(PRFileDesc *fd, const void *buf, PRInt32 amount);

	/// \brief Prfd available
	///
	/// \param fd = PRFile Desc
	///
	/// \return PRInt32
	static PRInt32 prfd_available(PRFileDesc *fd);

	/// \brief Prfd available64
	///
	/// \param fd = PRFile Desc
	///
	/// \return PRInt64
	static PRInt64 prfd_available64(PRFileDesc *fd);

	/// \brief Prfd fsync
	///
	/// \param fd = PRFile Desc
	///
	/// \return PRStatus
	static PRStatus prfd_fsync(PRFileDesc *fd);

	/// \brief Prfd seek
	///
	/// \param fd = PRFile Desc
	/// \param offset = PRInt32
	/// \param whence = PRSeek Whence
	///
	/// \return PRInt32
	static PRInt32 prfd_seek(PRFileDesc *fd, PRInt32 offset, PRSeekWhence whence);

	/// \brief Prfd seek64
	///
	/// \param fd = PRFile Desc
	/// \param offset = PRInt64
	/// \param whence = PRSeek Whence
	///
	/// \return PRInt64
	static PRInt64 prfd_seek64(PRFileDesc *fd, PRInt64 offset, PRSeekWhence whence);

	/// \brief Prfd file info
	///
	/// \param fd = PRFile Desc
	/// \param info = PRFile Info
	///
	/// \return PRStatus
	static PRStatus prfd_file_info(PRFileDesc *fd, PRFileInfo *info);

	/// \brief Prfd file info64
	///
	/// \param fd = PRFile Desc
	/// \param info64 = PRFile Info64
	///
	/// \return PRStatus
	static PRStatus prfd_file_info64(PRFileDesc *fd, PRFileInfo64 *info64);

	/// \brief Prfd writev
	///
	/// \param fd = PRFile Desc
	/// \param iov = PRIOVec
	/// \param size = PRInt32
	/// \param timeout = PRInterval Time
	///
	/// \return PRInt32
	static PRInt32 prfd_writev(PRFileDesc *fd, const PRIOVec *iov, PRInt32 size, PRIntervalTime timeout);

	/// \brief Prfd connect
	///
	/// \param fd = PRFile Desc
	/// \param addr = PRNet Addr
	/// \param timeout = PRInterval Time
	///
	/// \return PRStatus
	static PRStatus prfd_connect(PRFileDesc *fd, const PRNetAddr *addr, PRIntervalTime timeout);

	/// \brief Prfd accept
	///
	/// \param fd = PRFile Desc
	/// \param addr = PRNet Addr
	/// \param timeout = PRInterval Time
	///
	/// \return PRFile Desc
	static PRFileDesc *prfd_accept(PRFileDesc *fd, PRNetAddr *addr, PRIntervalTime timeout);

	/// \brief Prfd bind
	///
	/// \param fd = PRFile Desc
	/// \param addr = PRNet Addr
	///
	/// \return PRStatus
	static PRStatus prfd_bind(PRFileDesc *fd, const PRNetAddr *addr);

	/// \brief Prfd listen
	///
	/// \param fd = PRFile Desc
	/// \param backlog = PRIntn
	///
	/// \return PRStatus
	static PRStatus prfd_listen(PRFileDesc *fd, PRIntn backlog);

	/// \brief Prfd shutdown
	///
	/// \param fd = PRFile Desc
	/// \param PRIntn = PRShutdown How
	///
	/// \return PRStatus
	static PRStatus prfd_shutdown(PRFileDesc *fd, /*PRShutdownHow*/ PRIntn how);

	static PRInt32 prfd_recv(
		PRFileDesc *fd,
		void *buf,
		PRInt32 amount,
		PRIntn flags,
		PRIntervalTime timeout);

	static PRInt32 prfd_send(
		PRFileDesc *fd,
		const void *buf,
		PRInt32 amount,
		PRIntn flags,
		PRIntervalTime timeout);

	static PRInt32 prfd_recvfrom(
		PRFileDesc *fd,
		void *buf,
		PRInt32 amount,
		PRIntn flags,
		PRNetAddr *addr,
		PRIntervalTime timeout);

	static PRInt32 prfd_sendto(
		PRFileDesc *fd,
		const void *buf,
		PRInt32 amount,
		PRIntn flags,
		const PRNetAddr *addr,
		PRIntervalTime timeout);

	/// \brief Prfd poll
	///
	/// \param fd = PRFile Desc
	/// \param in_flags = PRInt16
	/// \param out_flags = PRInt16
	///
	/// \return PRInt16
	static PRInt16 prfd_poll(PRFileDesc *fd, PRInt16 in_flags, PRInt16 *out_flags);

	static PRInt32 prfd_acceptread(
		PRFileDesc *listen_sock,
		PRFileDesc **accepted_sock,
		PRNetAddr **peer_addr,
		void *buf,
		PRInt32 amount,
		PRIntervalTime timeout);

	static PRInt32 prfd_transmitfile(
		PRFileDesc *network_socket,
		PRFileDesc *source_file,
		const void *headers,
		PRInt32 hlen,
		PRTransmitFileFlags flags,
		PRIntervalTime timeout);

	/// \brief Prfd getsockname
	///
	/// \param fd = PRFile Desc
	/// \param addr = PRNet Addr
	///
	/// \return PRStatus
	static PRStatus prfd_getsockname(PRFileDesc *fd, PRNetAddr *addr);

	/// \brief Prfd getpeername
	///
	/// \param fd = PRFile Desc
	/// \param addr = PRNet Addr
	///
	/// \return PRStatus
	static PRStatus prfd_getpeername(PRFileDesc *fd, PRNetAddr *addr);

	/// \brief Prfd getsockopt
	///
	/// \param fd = PRFile Desc
	/// \param data = PRSocket Option Data
	///
	/// \return PRStatus
	static PRStatus prfd_getsockopt(PRFileDesc *fd, PRSocketOptionData *data);

	/// \brief Prfd setsockopt
	///
	/// \param fd = PRFile Desc
	/// \param data = PRSocket Option Data
	///
	/// \return PRStatus
	static PRStatus prfd_setsockopt(PRFileDesc *fd, const PRSocketOptionData *data);

	static PRInt32 prfd_sendfile(
		PRFileDesc *network_socket,
		PRSendFileData *send_data,
		PRTransmitFileFlags flags,
		PRIntervalTime timeout);

	/// \brief Prfd connectcontinue
	///
	/// \param fd = PRFile Desc
	/// \param out_flags = PRInt16
	///
	/// \return PRStatus
	static PRStatus prfd_connectcontinue(PRFileDesc *fd, PRInt16 out_flags);

	/// \brief Prfd reserved
	///
	/// \param fd = PRFile Desc
	///
	/// \return PRIntn
	static PRIntn prfd_reserved(PRFileDesc *fd);

	static PRIOMethods methods;
/// \}
};

/// \}
