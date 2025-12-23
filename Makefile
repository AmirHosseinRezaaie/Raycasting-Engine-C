# Compiler
CC = gcc

# Compiler flags: warnings and C99 standard
CFLAGS = -Wall -Wextra -std=c99

# Path to raylib headers
INCLUDES = -I "D:\Program Files\raylib-5.5_win64_mingw-w64\raylib-5.5_win64_mingw-w64\include"

# Path to raylib library and required Windows libs
LIBS = -L "D:\Program Files\raylib-5.5_win64_mingw-w64\raylib-5.5_win64_mingw-w64\lib" -lraylib -lopengl32 -lgdi32 -lwinmm -lm

# Source files
SOURCES = main.c map.c player.c raycaster.c

# Object files (auto generated from sources)
OBJECTS = $(SOURCES:.c=.o)

# Executable name
EXEC = raycaster.exe

# Default target: build executable
all: $(EXEC)

# Link object files to create executable
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files (Windows)
clean:
	del $(OBJECTS) $(EXEC) 2>nul