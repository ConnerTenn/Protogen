
GPP=g++ -std=c++17 #c11
GCC=gcc -std=c11

#GENERALLIBS=../Libs/General.o ../Libs/Colour.o
#LAUDIO=../libportaudio.a -lasound -ljack -pthread
#LMAD=../libmad/*.o 
#LXTERMLIBS=-pthread #-lgdk_pixbuf_xlib-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lX11
#LPIXTERM=-L ../../PixelTerm -lPixelTerm $(LXTERMLIBS)
LIBS= -lpthread #$(GENERALLIBS) $(LXTERMLIBS)
HEADOBJS= Head/HeadController.o Head/UI.o Head/Expressions.o Head/FrameController.o Head/FrameData.o Head/ExpressionData.o Head/FrameData.o Head/ExpressionData.o
IFACEOBJS=PseudoInterface/PseudoInterface.o PseudoInterface/Devices.o PseudoInterface/Messaging.o
GLOBALOBJS= Global.o TextScreen.o
OBJS= $(GLOBALOBJS) $(HEADOBJS) $(IFACEOBJS)
CFLAGS= -Wall -O3 -I./ -IHead/ -IBack/ -IChest/ -IPseudoInterface/ #-I ../Libs/

.PHONY: bin

all: postclean

program: main.o $(OBJS)
	@echo
	$(GPP) $(CFLAGS) $^ $(LIBS) -o $@

test: test.o Head/FrameData.o
	@echo
	$(GCC) $(CFLAGS) $^ $(LIBS) -o $@
	
%.o: %.cpp clean
	$(GPP) $(CFLAGS) $< -c -o $@

%.o: %.c clean
	$(GCC) $(CFLAGS) $< -c -o $@

Head/%.o: Head/%.bin clean
	ld -r -b binary $< -o $@
	

Head/FrameData.bin Head/ExpressionData.bin: bin
bin: clean
	cd Head; python GenExpressionFrames.py

force: clean all

clean: 
	rm *.o $(OBJS) Head/*.bin program test -f

postclean: program test
	rm *.o $(OBJS) Head/*.bin -f
