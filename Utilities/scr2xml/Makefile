OBJS=main.o resource_manager.o resourceoptions.o resource_tokenizer.o resource_tree.o
CXXFLAGS=-O2 -g -Wall

scr2xml: $(OBJS) Makefile
	$(CXX) -g $(OBJS) -lclanCore -lclanSignals -o scr2xml

clean:
	rm -f $(OBJS) scr2xml

.PHONY: clean
