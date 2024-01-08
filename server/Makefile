CC=gcc

CFLAGS=-pthread

Sources=main.c pos_sockets/active_socket.c pos_sockets/active_socket.h pos_sockets/char_buffer.c pos_sockets/char_buffer.h pos_sockets/linked_list.h pos_sockets/passive_socket.c pos_sockets/passive_socket.h buffer.h hrac/hrac.h hrac/hrac.c hernaPlocha/hernaPlocha.c hernaPlocha/hernaPlocha.h

TARGET=main

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
