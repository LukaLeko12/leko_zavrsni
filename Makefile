CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
LDFLAGS = -lsqlite3

TARGET = onkoc
SRCS = main.c database.c validation.c utils.c ui.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
