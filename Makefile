# Compiler
CC = gcc

# Compiler flags: warnings and C99 standard
CFLAGS = -Wall -Wextra -std=c99

# Path to raylib headers
INCLUDES = -I "C:\raylib\include"

# Path to raylib library and required Windows libs
LIBS = -L "C:\raylib\lib" -lraylib -lopengl32 -lgdi32 -lwinmm -lm -lmsvcrt

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