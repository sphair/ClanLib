TESTS = \
Collision \
Disp2Test \
FontTest \
GfxBenchmark \
GUITest \
InputButtonTest \
InputTest \
ListBoxTest \
NetSessionsTest \
PrimitivesTest \
ProviderTest \
SocketTestClient \
SocketTestServer \
SpriteTest \
StateTreeTest \
TextStylerTest \
TextureSizeTest \
PointerTest \
NetworkTest \
StringTest

all: 
	@for i in $(TESTS); do \
          $(MAKE) -C $$i all; \
        done
	@echo ""
	@echo "The following examples where build: " 
	@for i in $(TESTS); do \
          echo "     "$$i; \
        done

clean:
	@for i in $(TESTS); do \
           $(MAKE) -C $$i clean; \
         done

# EOF #
