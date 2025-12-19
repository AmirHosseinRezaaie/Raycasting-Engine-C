CC = gcc
CFLAGS = -Wall -Wextra -std=c99
INCLUDES = -I D:\Program Files\raylib-5.5_win64_mingw-w64\raylib-5.5_win64_mingw-w64\include
LIBS = -L D:\Program Files\raylib-5.5_win64_mingw-w64\raylib-5.5_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm

SOURCES = main.c map.c player.c raycaster.c
OBJECTS = $(SOURCES:.c=.o)
EXEC = raycaster.exe

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	del $(OBJECTS) $(EXEC) 2>nul