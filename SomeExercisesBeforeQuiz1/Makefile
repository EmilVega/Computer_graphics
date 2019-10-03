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
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lfreeimage -no-pie
endif
RM = /bin/rm -f
# all: prac3C-exampleL
# prac3C-exampleL: prac3C-exampleL.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o prac3C-exampleL shaders.o prac3C-exampleL.o $(INCFLAGS) $(LDFLAGS)
# prac3C-exampleL.o: prac3C-exampleL.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c prac3C-exampleL.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o prac3C-exampleL

all: prac3D-example
prac3D-example: prac3D-example.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o prac3D-example shaders.o prac3D-example.o $(INCFLAGS) $(LDFLAGS)
prac3D-example.o: prac3D-example.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c prac3D-example.cpp
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
clean:
	$(RM) *.o prac3D-example
