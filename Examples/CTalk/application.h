
#ifndef file_application
#define file_application

class Application : public CL_ClanApplication
{
//! Construction:
public:
	Application();

	~Application();

//! Attributes:
public:
	static Application *instance;

//! Operations:
public:

//! Implementation:
private:
	int main(int argc, char **argv);
};

#endif
