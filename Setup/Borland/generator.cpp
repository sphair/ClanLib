/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include <fstream.h>
#include <dir.h>
#include "../Generic/project.h"
#include "../Generic/workspace.h"
#include <string>
#include "generator.h"
#include <strstreambuf>

void create_release(Workspace &work) {
	mkdir("Borland");
  mkdir("Debug");
  mkdir("Release");
  mkdir("Debug/Intermediate");
  mkdir("Release/Intermediate");

  std::list<Project>::iterator it;
	bool debug=true;
  for(it=work.projects.begin();it!=work.projects.end();it++)   {
		bpr_create_file(*it,debug);
    debug = debug ^ true;
		bpr_create_file(*it,debug);
  }
}




void create_makefile(Workspace &work) {

  std::list<Project>::iterator it;

  mkdir("Borland");
  mkdir("Debug");
  mkdir("Release");
  mkdir("Debug/Intermediate");
  mkdir("Release/Intermediate");

  bool debug=true;

  for(it=work.projects.begin();it!=work.projects.end();it++)   {
  	mak_create_file(*it,debug);
    debug ^= true;
    mak_create_file(*it,debug);

  	cout<<it->name<<endl;
	}
}

void bpr_create_file(Project &proj, bool debug)
{
  	cout<<proj.name<<endl;
    ofstream bpr;
    //ifstream head,body,tail;

    ifstream head("Setup/Borland/template_head.txt");
    if(!head)
    	cout << "Error" << endl; //Shit
    ifstream body("Setup/Borland/template_body.txt");
			if(!head)
    		cout << "Error" << endl; //Shit
    ifstream tail("Setup/Borland/template_tail.txt");
			if(!head)
    		cout << "Error" << endl; //Shit




    if(debug)
    {
			bpr.open(("Borland/"+(proj.libname)+"d.bpr").c_str(), ios::trunc);
    }
    else
    {
			bpr.open(("Borland/"+(proj.libname)+".bpr").c_str(), ios::trunc);
    }

    ofstream  project(("Borland/"+(proj.libname)+".bpf").c_str());

		for(std::list<string>::iterator str=proj.file_paths.begin();str!=proj.file_paths.end();str++)
			project << "USEUNIT(\"..\\" << str->c_str() << ".cpp\");"<<endl;
    project << "//---------------------------------------------------------------------------"<<endl;
		project << "#define Library" <<endl;

    project.close();

    bpr << head.rdbuf();
    bpr.flush();

    //bpr << head.;

    if(debug)
    	bpr <<"\t\t<PROJECT value=\"..\\Debug\\"<<proj.libname.c_str()<<"d.lib"<<"\"/>"<<endl;
    else
    	bpr <<"\t\t<PROJECT value=\"..\\Release\\"<<proj.libname.c_str()<<".lib"<<"\"/>"<<endl;

    bpr<< "<OBJFILES value=\"";

    if(debug) {
			for(std::list<string>::iterator str=proj.files.begin();str!=proj.files.end();str++) {
    		bpr<<"..\\Debug\\Intermediate\\" << str->c_str();
      	bpr<<endl;
    	}
    }
    else {
    	for(std::list<string>::iterator str=proj.files.begin();str!=proj.files.end();str++) {
    		bpr<<"..\\Release\\\Intermediate\\" << str->c_str();
      	bpr<<endl;
    	}
    }
    
    bpr <<"\"/>"<<endl;

    bpr << "<USERDEFINES value=\"";
    for(std::list<string>::iterator str=proj.defines_list.begin();str!=proj.defines_list.end();str++) {
    	bpr<<str->c_str() <<";";
    }

    if(debug)
    	bpr << "_DEBUG;DEBUG\"/>" <<endl;
    else
    	bpr << "NDEBUG;\"/>" <<endl;

    bpr << "<MAINSOURCE value=\""<< (proj.libname)+".bpf" << "\"/>" <<endl;



    bpr << body.rdbuf();
    bpr.flush();

    if(debug) {
			bpr << "<CFLAG1 value=\"-Od -H=$(BCB)\\lib\\CL_COREd.csm -Hc -w -xp -VF -X- -r- -a8 -5 -b -k -y -v -vi- -q -I. -c -tWM\"/> "<<endl;
    	bpr << "<PFLAGS value=\"=-N2..\\Debug\\Intermediate -N0..\\Debug\\Intermediate -$Y+ -$W -$O- -tWM\"/> "<<endl;
    	bpr << "<RFLAGS value=\"/l 0x409 /d &quot;_DEBUG&quot; /i$(BCB)\\include; -tWM\"/>  "<<endl;
    	bpr << "<AFLAGS value=\"/mx /mv35 /w1 /zn -tWM\"/> "<<endl;
    	bpr << "<LFLAGS value=\"/P512 /C \"/>  "<<endl;
    }
    else {
    	bpr << "<CFLAG1 value=\"-O2 -H=$(BCB)\\lib\\CL_CORE.csm -Hc -VF -X- -a8 -5 -b -k- -vi -Oc -q -I. -c -tWM\"/> "<<endl;
    	bpr << "<PFLAGS value=\"-N2..\\Release\\Intermediate -N0..\\Release\\Intermediate -$Y- -$L- -$D- -tWM\"/> "<<endl;
    	bpr << "<RFLAGS value=\"/l 0x409 /d &quot;_DEBUG&quot; /i$(BCB)\\include; -tWM\"/>  "<<endl;
    	bpr << "<AFLAGS value=\"/mx /mv35 /w1 /zn -tWM\"/> "<<endl;
    	bpr << "<LFLAGS value=\"/P512 /C \"/>  "<<endl;
    }

    bpr << tail.rdbuf();
    bpr.flush();

    bpr.close();
}

void mak_create_file(Project &proj, bool debug)
{
	ofstream mak;
  ifstream head("Setup/Borland/mak_head.txt");
  ifstream body("Setup/Borland/mak_body.txt");
  ifstream tail("Setup/Borland/mak_tail.txt");

  if(debug)
  	mak.open(("Borland/"+(proj.libname)+"d.mak").c_str());
  else
  	mak.open(("Borland/"+(proj.libname)+".mak").c_str());

    //Mak_head here!
    mak << head.rdbuf();
    mak.flush();

		if(debug)
    	mak << "PROJECT = ..\\Debug\\" << proj.libname <<"d.lib" <<endl;
    else
    	mak << "PROJECT = ..\\Release\\" << proj.libname <<".lib" <<endl;

    mak << "OBJFILES =";

    for(std::list<string>::iterator str=proj.files.begin();str!=proj.files.end();str++) {
    	if(debug)
    		mak<<"..\\Debug\\Intermediate\\" << str->c_str() << ".obj";
      else
    		mak<<"..\\Release\\Intermediate\\" << str->c_str() << ".obj";
      str++;
      if(str++ != proj.files.end()) {
      	mak <<" \\";
      }
      str--;
      str--;
			mak << endl;
    }
    mak << endl << endl;

 		mak << "RESFILES =  " << endl;
    if(debug)
    	mak << "INT_DIR = ..\\Debug\\Intermediate" << endl;
    else
    	mak << "INT_DIR = ..\\Release\\Intermediate" << endl;
    mak << "MAINSOURCE = " << proj.libname << ".bpf" <<endl;

    mak << endl << "PATHCPP = ";
    for(std::list<string>::iterator str=proj.paths.begin();str!=proj.paths.end();str++)
    {
    	mak << "..\\"<< str->c_str() << ";";
    }

		//Mak_body here
    mak << body.rdbuf();
    mak.flush();

    mak << "USERDEFINES =";

    for(std::list<string>::iterator str=proj.defines_list.begin();str!=proj.defines_list.end();str++) {
    	mak << str->c_str() << ";";
    }
    if(debug)
    	mak << "DEUBG;_DEBUG" << endl;
    else
    	mak << "NDEBUG;" << endl;

      

    mak << "SYSDEFINES = _NO_VCL;_ASSERTE;NO_STRICT "<<endl;
    mak << "INCLUDEPATH = ..\\Sources\\GL\\GLX;..\\Sources\\GL\\WGL;..\\Sources\\GL\\Object3D;..\\Sources\\GL\\Generic;$(BCB)\\include;$(BCB)\\include\\mfc;$(BCB)\\include\\atl;..\\Sources "<<endl;
		//LIBPATH = ..\\Sources\\GL\\GLX;..\\Sources\\GL\\WGL;..\\Sources\\GL\\Object3D;..\\Sources\\GL\\Generic;$(BCB)\\lib;$(RELEASELIBPATH);$(BCB)\\lib\\psdk
    mak << "WARNINGS = -w-8026 -w-8027 -w-par "<<endl;
    mak << "LISTFILE = "<<endl;
    mak << "# ---------------------------------------------------------------------------"<<endl;

    if(debug)
    {
			mak << "CFLAG1 =-Od -H=$(BCB)\\lib\\CL_COREd.csm -w -xp -VF -X- -r- -a8 -5 -b -k -y -v -vi- -q -I. -c -tWM"<<endl;
    	mak << "PFLAGS = -N2..\\Debug\\Intermediate -N0..\\Debug\\Intermediate -$Y+ -$W -$O- -tWM "<<endl;

    }
    else
    {
    	mak << "CFLAG1 = -O2 -H=$(BCB)\\lib\\CL_CORE.csm -Hc -w -X- -a8 -5 -b -k- -vi -q -I. -c -tWM "<<endl;
    	mak << "PFLAGS = -N2..\\Release\\Intermediate -N0..\\Release\\Intermediate -$Y- -$L- -$D- -tWM  "<<endl;
		}

    //mak_tail here
    mak << tail.rdbuf();
    mak.flush();

    mak.flush();
    mak.close();


}
