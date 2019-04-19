
GPP=g++ -Wall -std=c++17
LAUDIO=-I ~/Libs/portaudio/include ~/Libs/portaudio/lib/.libs/libportaudio.a -lasound -ljack -pthread
#LMAD=../libmad/*.o 
LPIXTERMLIBS=-pthread -lgdk_pixbuf_xlib-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lX11
#LPIXTERM=-L ../../PixelTerm -lPixelTerm $(LPIXTERMLIBS)
LIBS = -lpthread ../Libs/*.o $(LAUDIO) $(LPIXTERMLIBS)
CFLAGS = -O3 -I ../Libs/

all: program

program: main.o InputController.o
	$(GPP) $(CFLAGS) $^ $(LIBS) $(LPIXTERM) -o $@
	
main.o: main.cpp InputController.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
InputController.o: InputController.cpp InputController.h Spectrum.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
force: clean all

run:
	rm program* -f
	make all
	./program

clean: 
	rm *.o program* -f
