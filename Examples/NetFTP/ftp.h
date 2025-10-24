#ifndef _FTPD_H
#define _FTPD_H

#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <string>
#include <vector>

class FTPD
{
public:

  FTPD(CL_Socket&);
  ~FTPD();

  void read_sock();
  void write_sock();
  void err_sock() {;}

  void process_cmd(const char*);

  std::string prep_msg(const char *) const;
  void get_arg(const char *);

  //Commands
  void user(const char *buf);
  void pass(const char *buf);
  void type(const char *buf);
  void mode(char *buf);
  void port(const char *buf);
  void rest(const char *buf);
  void pasv();
  void list();
  void retr(const char *buf);

  void conn();
  void error();
  void scan_dir();
private:
	CL_Socket *sock;
  CL_Socket *raw;
  CL_Socket *data;
  char ftp_mode;
  char ftp_type;

  std::string dir;
  std::vector<std::string> dir_list;
  bool dir_list_ok;

  unsigned int target_ip;
  unsigned int target_port;
};

#endif
