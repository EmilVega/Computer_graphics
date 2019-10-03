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

# all: exam2-0
# exam2-0: exam2-0.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o exam2-0 shaders.o exam2-0.o $(INCFLAGS) $(LDFLAGS)
# exam2-0.o: exam2-0.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c exam2-0.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o exam2-0


# all: exam3-0
# exam3-0: exam3-0.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o exam3-0 shaders.o exam3-0.o $(INCFLAGS) $(LDFLAGS)
# exam3-0.o: exam3-0.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c exam3-0.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o exam3-0

all: exam4-0
exam4-0: exam4-0.o shaders.o shaders.h geometry3.h
	$(CC) $(CFLAGS) -o exam4-0 shaders.o exam4-0.o $(INCFLAGS) $(LDFLAGS)
exam4-0.o: exam4-0.cpp shaders.h geometry3.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c exam4-0.cpp
shaders.o: shaders.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
clean:
	$(RM) *.o exam4-0

# all: final
# final: final.o shaders.o shaders.h geometry3.h
# 	$(CC) $(CFLAGS) -o final shaders.o final.o $(INCFLAGS) $(LDFLAGS)
# final.o: final.cpp shaders.h geometry3.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c final.cpp
# shaders.o: shaders.cpp
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
# clean:
# 	$(RM) *.o final
