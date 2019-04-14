
GPP=g++ -Wall -std=c++17
CFLAGS = -O3 -I .

all: WindowController.o Colour.o General.o

WindowController.o: WindowController.cpp WindowController.h Colour.h
	$(GPP) $(CFLAGS) $< -c -o $@

Colour.o: Colour.cpp Colour.h
	$(GPP) $(CFLAGS) $< -c -o $@

General.o: General.cpp General.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
force: clean all

clean: 
	rm *.o -f
