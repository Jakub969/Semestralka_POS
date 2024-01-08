CC=cl
CFLAGS=/EHsc /std:c++14
LFLAGS=wsock32.lib ws2_32.lib

EXE=Client.exe
SOURCES=main.cpp my_socket.cpp hrac/Hrac.cpp hernaPlocha/HernaPlocha.cpp

all: $(EXE)

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) /Fe$(EXE) $(SOURCES) $(LFLAGS)

clean:
	del *.obj *.exe

run: $(EXE)
	$(EXE)
