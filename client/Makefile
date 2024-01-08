CC=g++
CFLAGS=-Wall -std=c++14
LFLAGS=-lwsock32 -lws2_32

EXE=Client.exe
SOURCES=main.cpp my_socket.cpp hrac/Hrac.cpp hernaPlocha/HernaPlocha.cpp

all: $(EXE)

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) $(LFLAGS)

clean:
	del *.obj *.exe *.o

run: $(EXE)
	$(EXE)
