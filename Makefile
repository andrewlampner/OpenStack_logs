CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = main.exe

SRCS = main.c log.c hashmap.c dyn_array.c ComputeLogFunctions.c APIMicroFunctions.c print.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)