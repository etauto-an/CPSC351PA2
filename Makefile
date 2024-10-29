# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -pthread

# Executable name
TARGET = sleeping_ta

# Source files
SRCS = main.c ta.c student.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target: compile the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile individual .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)
