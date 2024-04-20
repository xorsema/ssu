CC = g++
SDL_CFLAGS = $(shell sdl-config --cflags) $(shell pkg-config box2d --cflags)
SDL_LDFLAGS = $(shell sdl-config --libs) $(shell pkg-config box2d --libs)
CPPFLAGS = -g -std=c++11 $(SDL_CFLAGS)
LINK = $(SDL_LDFLAGS) -lSDL_ttf -lGL -lGLU -lbox2d 
VPATH = src/
OUT = bin/
SRCS = main.cpp renderer.cpp input.cpp entity.cpp game.cpp texture.cpp text.cpp timer.cpp

include $(SRCS:.cpp=.d)

.DEFAULT_GOAL := ssu

ssu: $(SRCS:.cpp=.o)
	$(CC) $^ $(CPPFLAGS) $(LINK) -o $(OUT)$@  

%.o : %.cpp
	$(CC) $< $(CPPFLAGS) -c -o $@ 

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -rf bin/ssu \
	rm -rf *.o
