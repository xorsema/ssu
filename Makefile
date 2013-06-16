CC = g++
SDL_CFLAGS = $(shell sdl-config --cflags)
SDL_LDFLAGS = $(shell sdl-config --libs)
CPPFLAGS = -g $(SDL_CFLAGS)
LINK = -lGL -lGLU -lBox2D $(SDL_LDFLAGS) -lSDL_ttf
VPATH = src/
OUT = bin/
SRCS = main.cpp renderer.cpp input.cpp entity.cpp game.cpp texture.cpp text.cpp timer.cpp

include $(SRCS:.cpp=.d)

.DEFAULT_GOAL := ssu

ssu: $(SRCS:.cpp=.o)
	$(CC) $(CPPFLAGS) $(LINK) -o $(OUT)$@ $^ 

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -rf bin/ssu \
	rm -rf *.o
