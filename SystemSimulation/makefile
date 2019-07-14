
CC=g++ -Wall -std=c++17

GENERALLIBS=../Libs/General.o ../Libs/Colour.o
#LAUDIO=../libportaudio.a -lasound -ljack -pthread
#LMAD=../libmad/*.o 
LXTERMLIBS=-pthread #-lgdk_pixbuf_xlib-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lX11
#LPIXTERM=-L ../../PixelTerm -lPixelTerm $(LXTERMLIBS)
LIBS= -lpthread $(GENERALLIBS) $(LXTERMLIBS)
OBJS= main.o TextScreen.o HeadSystem.o PseudoInterface.o Devices.o
CFLAGS= -O3 -I ../Libs/

all: program

program: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
	
%.o: %.cpp clean #make sure clean is called first
	$(CC) $(CFLAGS) $< -c -o $@

force: clean all

clean: 
	rm *.o program* -f
