CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm
TARGET = draw_line

# Include 3d.c in sources and objects
SRCS = source/main.c source/canvas.c source/3d.c
OBJS = build/main.o build/canvas.o build/3d.o

.PHONY: all run clean

all: run

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile main.c
build/main.o: source/main.c include/canvas.h include/3d.h | build
	$(CC) $(CFLAGS) -c source/main.c -o build/main.o

# Compile canvas.c
build/canvas.o: source/canvas.c include/canvas.h | build
	$(CC) $(CFLAGS) -c source/canvas.c -o build/canvas.o

# Compile 3d.c
build/3d.o: source/3d.c include/3d.h include/canvas.h | build
	$(CC) $(CFLAGS) -c source/3d.c -o build/3d.o

# Ensure build directory exists
build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)
	convert output.ppm output.png
	@echo "Rendered output.ppm and converted to output.png successfully!"

clean:
	rm -rf build $(TARGET) output.ppm output.png
	@echo "Cleaned build directory."