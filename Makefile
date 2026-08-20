# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = draw_line
SRCS = main.c canvas.c
OBJS = $(SRCS:.c=.o)

# Default target runs the pipeline automatically
.PHONY: all run clean

all: run

# Build the final binary executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compile C source files into object files
%.o: %.c canvas.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program and convert PPM output to PNG
run: $(TARGET)
	./$(TARGET)
	convert output.ppm output.png
	@echo "Rendered output.ppm and converted to output.png successfully!"

# Clean up build artifacts and generated images
clean:
	rm -f $(OBJS) $(TARGET) output.ppm output.png
	@echo "Cleaned build directory."