CC = g++
CFLAGS = -std=c++17 -Wall -Wextra `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_ttf
PROGRAM = pool
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

$(PROGRAM): $(OBJECTS)
	$(CC) $^ $(CFLAGS) $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(PROGRAM)

