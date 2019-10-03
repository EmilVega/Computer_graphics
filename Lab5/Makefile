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
# all: prac5A
# prac5A: prac5A.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o prac5A shaders.o prac5A.o $(INCFLAGS) $(LDFLAGS)
# prac5A.o: prac5A.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c prac5A.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o prac5A

# all: prac5A_solutionDrawObject
# prac5A_solutionDrawObject: prac5A_solutionDrawObject.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o prac5A_solutionDrawObject shaders.o prac5A_solutionDrawObject.o $(INCFLAGS) $(LDFLAGS)
# prac5A_solutionDrawObject.o: prac5A_solutionDrawObject.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c prac5A_solutionDrawObject.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o prac5A_solutionDrawObject

# all: prac5B
# prac5B: prac5B.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o prac5B shaders.o prac5B.o $(INCFLAGS) $(LDFLAGS)
# prac5B.o: prac5B.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c prac5B.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o prac5B
#
all: prac5C
prac5C: prac5C.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o prac5C shaders.o prac5C.o $(INCFLAGS) $(LDFLAGS)
prac5C.o: prac5C.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c prac5C.cpp
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
clean:
	$(RM) *.o prac5C
