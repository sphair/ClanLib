#include <ClanLib/core.h>
#include <ClanLib/application.h>

#include <iostream>

using namespace std;

void t() {
	cerr << "Current time: " << CL_System::get_time() << "\n";
}

void w() {
	cerr << "Sleeping for 100ms\n";
	CL_System::keep_alive(100);
}

void s() {
	cerr << "Suspending\n";
	CL_System::suspend_time();
}

void r() {
	cerr << "Resuming\n";
	CL_System::resume_time();
}

struct App : public CL_ClanApplication {
	int main(int, char**) {
		CL_SetupCore::init();
		
		while (CL_System::get_time() % 100 != 0)
			CL_System::sleep(1);
		
		t();
		w();
		t();
		s();
		t();
		w();
		t();
		r();
		t();
		w();
		t();
		s();
		t();
		w();
		t();
		r();
		t();
		w();
		t();
		
		CL_SetupCore::deinit();
		return 0;
	}
} app;
