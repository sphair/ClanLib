LIBS = clanCore clanApp clanNetwork

all: server client

include ../Makefile.conf

server: server.o
	$(CXX) $(CXXFLAGS) server.o -o server $(LDFLAGS)

client: client.o
	$(CXX) $(CXXFLAGS) client.o -o client $(LDFLAGS)

# EOF #