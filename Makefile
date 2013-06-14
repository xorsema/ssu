CC = g++
SDL_CFLAGS = $(shell sdl-config --cflags)
FT_CFLAGS = $(shell freetype-config --cflags)
SDL_LDFLAGS = $(shell sdl-config --libs)
FT_LDFLAGS = $(shell freetype-config --libs)
CPPFLAGS = -g $(SDL_CFLAGS) $(FT_CFLAGS)
LINK = -lGL -lGLU -lBox2D $(SDL_LDFLAGS) $(FT_LDFLAGS)
VPATH = src/
OUT = bin/
SRCS = main.cpp renderer.cpp input.cpp entity.cpp timer.cpp

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
