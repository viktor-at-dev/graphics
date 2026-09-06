CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm
TARGET = draw_line

# Direct paths matching your folder structure
SRCS = source/main.c source/canvas.c
OBJS = build/main.o build/canvas.o

.PHONY: all run clean

all: run

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source/main.c -> build/main.o
build/main.o: source/main.c include/canvas.h | build
	$(CC) $(CFLAGS) -c source/main.c -o build/main.o

# Compile source/canvas.c -> build/canvas.o
build/canvas.o: source/canvas.c include/canvas.h | build
	$(CC) $(CFLAGS) -c source/canvas.c -o build/canvas.o

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