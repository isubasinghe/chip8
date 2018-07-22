CC     = gcc
CFLAGS = -std=c99
# modify the flags here ^

EXE    = chip8
OBJ    = main.o chip8.o file.o cycle.o graphics.o



debug: CFLAGS += -lSDL2main -lSDL2 -DDEBUG -g -lm
debug: $(EXE)

release: CFLAGS += -lSDL2main -lSDL2 -O3 -lm
release: $(EXE)

mingw-release: CFLAGS += -O3 -lmingw32 -lSDL2main -lSDL2 -std=c99 -lm
mingw-release: $(EXE)

mingw-debug: CFLAGS += -g -DDEBUG -lmingw32 -lSDL2main -lSDL2 -std=c99 -lm
mingw-debug: $(EXE)

web: CC = emcc
web: CFLAGS += --preload-file roms -s USE_SDL=2 -DWEB
web: EXE = chip8.html
web: $(EXE)

# how to link executable
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(CFLAGS)

# other dependencies
main.o: chip8.h
chip8.o: chip8.h
file.o: file.h chip8.h
cycle.o: chip8.h cycle.h
graphics.o: chip8.h graphics.h



# phony targets (these targets do not represent actual files)
.PHONY: clean cleanwin cleanweb

clean:
	rm -f $(OBJ)

cleanwin: 
	del $(OBJ)

cleanweb:
	rm -f $(OBJ)
	rm *.wasm *.js *.html