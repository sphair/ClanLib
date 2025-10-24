#include "ftp.h"
#include <vector>
#include <string>

#ifdef _MSC_VER
#include <io.h>
#include <direct.h>
#define S_ISDIR(a) ((a & _S_IFDIR) == _S_IFDIR)
#else
#include <dirent.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "main.h"

const int max_command = 14;
const int three_cmd_max = 2;
const char *commands[max_command] = {"USER","PASS","TYPE","PORT","MODE","STRU","RETR","STOR","NOOP","QUIT","REST","PASV","LIST","NLST"};
const char *month[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
const char *three_cmd[three_cmd_max] = {"PWD","CWD"};

std::string CL_itoa(int num, char *ptr, int base)
{
	char buf[20];
	memset(buf,0,20);
	sprintf(buf, "%d", num);
	return std::string(buf);
}

void CL_ToUpper(char* buf)
{
	while(*buf != '\r')
	{
		toupper(*buf);
		buf++;
	}
}

std::string FTPD::prep_msg(const char *buf) const
{
	char a = 0x0a;
	char d = 0x0d;
	char *str = (char*)buf;

	int i=0;
	while(*str != '\0')
	{
		i++;
		str++;
	}
	str = new char[i+2];
	memset(str,0,i+3);
	memcpy(str,buf,i);
	str[i] = d;
	str[i+1] = a;

	std::string rtn(str);
	delete[] str;
	return rtn;
}

void FTPD::get_arg(const char *buf)
{
	std::vector<std::string> list;
	int i = 0;
	std::string back;
	while(*buf != 0x0a)
	{
		while(1)
		{
			if(*buf == 0x0a) return;
			if(*buf == ',') break;
			if(*buf == ' ') break;

			back += *buf;
			buf++;
		}
		list.push_back(back);
		back="";
		buf++;
	}
}

FTPD::FTPD(CL_Socket &socket)
{
	sock = new CL_Socket(socket);
	sock->sig_read_triggered().connect(this,&FTPD::read_sock);
	sock->sig_write_triggered().connect(this,&FTPD::write_sock);
	sock->sig_exception_triggered().connect(this,&FTPD::err_sock);
	raw = NULL;
	data = NULL;
	dir = ".";
}

FTPD::~FTPD()
{
 	delete sock;
 	delete raw;
	delete data;
}

void FTPD::write_sock()
{
	char buf[128];
	memset(buf,0,128);
	sock->recv(buf,127);
	std::cout << "Command Received on write: " << buf << std::endl;
	process_cmd(buf);

	sock->sig_read_triggered().connect(this,&FTPD::read_sock);
	sock->sig_write_triggered().connect(this,&FTPD::write_sock);
	sock->sig_exception_triggered().connect(this,&FTPD::err_sock);
}

void FTPD::read_sock()
{
	char buf[128];
	memset((void*)&buf,0,127);
	sock->recv(buf,127);
	sock->sig_read_triggered().connect(this,&FTPD::read_sock);
	sock->sig_write_triggered().connect(this,&FTPD::write_sock);
	sock->sig_exception_triggered().connect(this,&FTPD::err_sock);

	std::cout << "Command Received on read: " << buf << std::endl;
	process_cmd(buf);
}

void FTPD::process_cmd(const char* buf)
{
	int i;

	CL_ToUpper((char*)buf);
	
	for(i=0;i<max_command;i++)
	{
		if(strncmp(buf,commands[i],4) == 0)
		{
			std::cout << "Oh yeah!  We found a match of " << commands[i] << std::endl;
			switch(i)
			{
				case(0):
					//USER
					user(buf);
					break;
				case(1):
					//PASS
					pass(buf);
					break;
				case(2):
					//TYPE
					type(buf+4);
					break;
				case(3):
					//PORT
					port(buf+5);
					break;
				case(4):
					//MODE
					mode((char*)buf+4);
					break;
				case(5):
					//STRU
					break;
				case(6):
					//RETR
					retr(buf);
					break;
				case(7):
					//STOR
					break;
				case(8):
					//NO0P
					break;
				case(9):
					//app.pop_server(this);
					sock->send(prep_msg("221 Bye"));
					return;
				case(10):
					//REST
					rest(buf);
					break;
				case(11):
					//PASV
					pasv();
					break;
				case(12):
					//List
					sock->send(prep_msg("150 Opening ASCII Socket"));
					std::cout << "Sending List" << std::endl;
					list();
					sock->send(prep_msg("226 Transfer Complete"));
					std::cout << "Sending done" << std::endl;
					break;
				default:
					std::cout << "Unimplmented Command" << std::endl;
					std::cout << buf << std::endl;
			}
			return;
		}
	}
	
	for(i=0;i<three_cmd_max;i++)
	{
		if(strncmp(buf,three_cmd[i],3)==0)
		{
			switch(i)
			{
				case(0):
					//PWD
					sock->send(prep_msg("257 \"/Pub\""));
					break;
				case(1):
				    //CWD
					sock->send(prep_msg("200 Ok!"));
					break;
			}
			return;
		}
	}

	std::cout << "Unknown command: " << (char*)buf << std::endl;
	std::string bar("502 ");
	// bar += buf;
	bar += " not implemented";
	std::cout << bar << std::endl;
	sock->send(prep_msg(bar.c_str()));
}

//Commands!

void FTPD::user(const char *buf)
{
	sock->send(prep_msg("331 Send Pass"));
}

void FTPD::pass(const char *buf)
{
	sock->send(prep_msg("230 welcome aboard"));
}

void FTPD::type(const char *buf)
{ /*
		if(*buf != ' ')
  	prep_msg("501 Error");
    std::cout << "First Char isn't a space" << std::endl;
    return;
		buf++;
		if(*(buf+1) != ' ')
  	prep_msg("501 Error");
    std::cout << "Last Char isn't a space" << std::endl;
    return;
		if(*buf == 'A' || *buf == 'I')
  	{
    ftp_type = *buf;
    prep_msg("200 Go for new type");
    }
		else
		{
  	prep_msg("504 Unused command");
  	return;
		}      */
				std::cout << "FIXME: Type unused" << std::endl;
				sock->send(prep_msg("200 Type set to I."));
}

void FTPD::mode(char *buf)
{
	if(!(*buf == ' '))
		return; //Error
	if(!(*(buf+2) == ' '))
		return; //Error
	buf++;
	if((*buf) == 'S' || *buf == 'B' || *buf == 'C')
	{
		ftp_mode = *(buf+1);
		sock->send(prep_msg("200 Mode Change ok!"));
	}
	else
		return; //Error
}

void FTPD::port(const char *buf)
{
	std::string str(buf);
	std::string delimit(",");
	std::vector<std::string> vec;

	vec = CL_String::tokenize(buf, delimit);

	for(std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
		std::cout << *it << std::endl;

	std::string ip_addr;
	std::string port;

	for(int i = 0; i < 4; i++)
	{
		ip_addr += vec[i];
		if(i != 3)
			ip_addr += ".";
	}

	int high = CL_String::to_int(vec[4]);
	int low = CL_String::to_int(vec[5]);

	int num_port = (high << 8) | low;

	port = CL_String::from_int(num_port);

	if(data != NULL)
		delete data;

	data = new CL_Socket(CL_Socket::tcp);

	CL_IPAddress ip(ip_addr, port);

	data->connect(ip);

	std::cout  << "Connected" << std::endl;

	sock->send(prep_msg("200 PORT OK"));
}

void FTPD::rest(const char *buf)
{
	get_arg(buf);
	sock->send(prep_msg("502"));
}

void FTPD::pasv()
{
	if(data != NULL)
		delete data;

	CL_IPAddress ip("13878");
	data = new CL_Socket(CL_Socket::tcp);
	data->bind(ip);
	data->sig_read_triggered().connect(this,&FTPD::conn);
	data->sig_write_triggered().connect(this,&FTPD::conn);
	data->sig_exception_triggered().connect(this,&FTPD::conn);
	data->listen(4);
	sock->send(prep_msg("227 (24,77,3,254,54,54)"));
}

void FTPD::conn()
{
	raw = new CL_Socket(data->accept());
	raw->sig_read_triggered().connect(this,&FTPD::error);
	raw->sig_write_triggered().connect(this,&FTPD::error);
	raw->sig_exception_triggered().connect(this,&FTPD::error);
}

void FTPD::error()
{
	raw->send(prep_msg("usr"));
	raw->send(prep_msg("226"));
}

void FTPD::list()
{
	sock->send(prep_msg("150"));
	scan_dir();
	for(std::vector<std::string>::iterator it= dir_list.begin(); it != dir_list.end();it++)
	{
		data->send(*it);
		std::cout << *it << std::endl;
	}

	std::cout << "Done sending" << std::endl;

	sock->send(prep_msg("226"));

	std::cout << "Done sending" << std::endl;

	delete data;
}

void FTPD::retr(const char *data)
{
	char *buf = (char*)data;
	buf += 5;
	if( raw == NULL)
		conn();
	else
	{
		delete raw;
		conn();
	}

	char *foo = buf;
	while( *foo != '\n')
	{
		if(*foo == '\r')
			*foo = '\0';
		if(*foo == '\n')
		{
			*foo = '\0';
			break;
		}
		foo++;
	}

	sock->send(prep_msg("150 Opening Bianry socket"));
	struct stat statbuf;
	stat(buf,&statbuf);
	FILE *bar = fopen(buf,"r");
	unsigned char *file = new unsigned char[statbuf.st_size];
	if( statbuf.st_size != 0)
	{
		fread(file,1,statbuf.st_size,bar);
	}
	if( S_ISDIR(statbuf.st_mode) )
	{
		sock->send(prep_msg("553 Bad client!"));
	}
	else
		raw->send(file,statbuf.st_size);

	sock->send(prep_msg("226 Transfer Complete"));
	delete[] file;
	delete raw;
	raw = NULL;
}

void FTPD::scan_dir()
{
#if _MSC_VER
	struct _finddata_t targets;
	long open=_findfirst( dir.c_str(), &targets );
#else
	DIR *open = opendir(dir.c_str());
	dirent *targets = readdir(open);
#endif

	std::string *push;
	std::vector<std::string> file_list;
	struct stat statbuf;
	std::string attr("");
	std::string *final;
	struct tm *time;
	char *foo = new char[100];
	memset(foo,0,100);
	bool proceed=true;

#ifdef _MSC_VER
	proceed=(open!=-1);
#else
	proceed=(targets != NULL);
#endif

	while(proceed)
	{
		final = new std::string("-rw-r--r-- 1 ");
#ifdef _MSC_VER
		push = new std::string(targets.name);
#else
		push = new std::string(targets->d_name);
#endif
		std::cout << push->c_str() << std::endl;
		//std::string *attr = new std::string("-rw-r--r--    1 root     other");
		stat(push->c_str(),&statbuf);

		*final += std::string(CL_itoa(statbuf.st_uid,foo,10));
		*final += " ";
		*final += CL_itoa(statbuf.st_gid,foo,10);
								//*final += attr;
		*final += " ";
		*final += CL_itoa(statbuf.st_size,foo,10);
								//*final += attr;
		*final += " ";
		time = gmtime((const time_t*)&statbuf.st_mtime);
		*final += std::string(month[time->tm_mon]);
		*final += " ";
		*final += CL_itoa(time->tm_mday,foo,10);
		*final += " ";
		*final += CL_itoa(((time->tm_year)+1900),foo,10);
		*final += "  ";
		*final += *push;

		dir_list.push_back(prep_msg(final->c_str()));

#ifdef _MSC_VER
		int result=_findnext(open,&targets);
		proceed=(result!=-1);
#else
		targets = readdir(open);
		proceed=(targets != NULL);
#endif
	}

#ifdef _MSC_VER
	_findclose(open);
#else
	closedir(open);
#endif
}
