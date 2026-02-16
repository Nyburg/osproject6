CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
TARGET = speed

all: $(TARGET)

$(TARGET): speed.c
	$(CC) $(CFLAGS) -o $(TARGET) speed.c

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean