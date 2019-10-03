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
# all: prac2A
# prac2A: prac2A.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o prac2A shaders.o prac2A.o $(INCFLAGS) $(LDFLAGS)
# prac2A.o: prac2A.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c prac2A.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o prac2A

# all: prac2B
# prac2B: prac2B.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o prac2B shaders.o prac2B.o $(INCFLAGS) $(LDFLAGS)
# prac2B.o: prac2B.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c prac2B.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o prac2B

all: prac2C
prac2C: prac2C.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o prac2C shaders.o prac2C.o $(INCFLAGS) $(LDFLAGS)
prac2C.o: prac2C.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c prac2C.cpp
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
clean:
	$(RM) *.o prac2C
