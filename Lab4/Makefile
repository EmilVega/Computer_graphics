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
# all: prac4
# prac4: prac4.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o prac4 shaders.o prac4.o $(INCFLAGS) $(LDFLAGS)
# prac4.o: prac4.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c prac4.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o prac4

all: prac4B
prac4B: prac4B.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o prac4B shaders.o prac4B.o $(INCFLAGS) $(LDFLAGS)
prac4B.o: prac4B.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c prac4B.cpp
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
clean:
	$(RM) *.o prac4B
