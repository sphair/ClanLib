EXAMPLE_BIN=shadertest

include ../Makefile.conf

shadertest: Intermediate/main.o
	$(CXX) $(COMP_OPTIONS) Intermediate/main.o -o shadertest -lclanCore -lclanApp -lclanDisplay -lclanGL
