CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = mb_top
SRCS = mb_top.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o