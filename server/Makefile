CC=gcc
CFLAGS=-pthread

SOURCES=main.c pos_sockets/active_socket.c pos_sockets/char_buffer.c pos_sockets/passive_socket.c hrac/hrac.c hernaPlocha/hernaPlocha.c

TARGET=main

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
