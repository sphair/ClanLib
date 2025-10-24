
#pragma once

#include <list>

class IRCConnection;

class Document
{
//! Construction:
public:
	Document();

	~Document();

//! Attributes:
public:
	std::list<IRCConnection *> connections;

//! Operations:
public:

//! Implementation:
private:
};
