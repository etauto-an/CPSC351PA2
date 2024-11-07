# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -pthread

# Executable name
TARGET = ta	

# Source files (.c files only)
SRCS = TA.c TA_Activity.c Student.c 
#SRCS = TA2.c

# Header files (not used for compiling to .o)
HDRS = TA_Activity.h Student.h

# Object files
OBJS = $(SRCS:.c=.o)

# Default target: compile the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile individual .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to format all source files using clang-format
format:
	clang-format -style=Google -i $(SRCS) $(HDRS)

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)
