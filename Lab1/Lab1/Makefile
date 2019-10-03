CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses -Wno-parentheses-equality
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -L./lib/mac/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage 
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I./glm-0.9.7.1 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L./lib/nix -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lfreeimage
endif
RM = /bin/rm -f
all: prac1A
prac1A: prac1A.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o prac1A shaders.o prac1A.o $(INCFLAGS) $(LDFLAGS) 
prac1A.o: prac1A.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c prac1A.cpp 
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp 
clean: 
	$(RM) *.o prac1A

all: prac1b
prac1b: prac1b.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o prac1b shaders.o prac1b.o $(INCFLAGS) $(LDFLAGS) 
prac1b.o: prac1b.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c prac1b.cpp 
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp 
clean: 
	$(RM) *.o prac1b
 
all: prac1C
prac1C: prac1C.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o prac1C shaders.o prac1C.o $(INCFLAGS) $(LDFLAGS) 
prac1C.o: prac1C.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c prac1C.cpp 
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp 
clean: 
	$(RM) *.o prac1C
 

