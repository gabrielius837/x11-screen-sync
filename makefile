CC=gcc
CFLAGS=-Wall -O2
LDFLAGS=-lX11 -lXrandr
TARGET=output
SRC=main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

