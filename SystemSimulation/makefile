
GPP=g++ -std=c++17 #c11
GCC=gcc -std=c11

#GENERALLIBS=../Libs/General.o ../Libs/Colour.o
#LAUDIO=../libportaudio.a -lasound -ljack -pthread
#LMAD=../libmad/*.o 
#LXTERMLIBS=-pthread #-lgdk_pixbuf_xlib-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lX11
#LPIXTERM=-L ../../PixelTerm -lPixelTerm $(LXTERMLIBS)
LIBS= -lpthread #$(GENERALLIBS) $(LXTERMLIBS)
OBJS= Global.o TextScreen.o HeadController.o EmotionController.o PseudoInterface.o Devices.o Messaging.o UI.o Expressions.o ExpressionData.o FrameData.o ExpressionData.o
CFLAGS= -Wall -O3 #-I ../Libs/

all: program test

program: main.o $(OBJS)
	@echo
	$(GPP) $(CFLAGS) $^ $(LIBS) -o $@

test: test.o FrameData.o
	@echo
	$(GCC) $(CFLAGS) $^ $(LIBS) -o $@
	
%.o: %.cpp clean #make sure clean is called first
	$(GPP) $(CFLAGS) $< -c -o $@

%.o: %.c clean #make sure clean is called first
	$(GCC) $(CFLAGS) $< -c -o $@

%.o: %.bin clean
	ld -r -b binary $< -o $@

ExpressionData.bin FrameData.bin: clean
	python GenExpressionFrames.py

force: clean all

clean: 
	rm *.o *.bin program test -f
