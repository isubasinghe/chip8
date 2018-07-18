CC     = gcc
CFLAGS = -std=c99 -lm
# modify the flags here ^

EXE    = chip8
OBJ    = main.o chip8.o file.o cycle.o


# top (default) target
all: $(EXE)

debug: CFLAGS += -DDEBUG -g
debug: $(EXE)

release: CFLAGS += -O3
release: $(EXE)

web: CC = emcc
web: CFLAGS += -O3 -DWEB -s USE_SDL=2
web: EXE = chip8.html
web: $(EXE)

# how to link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

# other dependencies
main.o: chip8.h
chip8.o: chip8.h
file.o: file.h chip8.h
cycle.o: chip8.h cycle.h



# phony targets (these targets do not represent actual files)
.PHONY: clean cleanwin

clean:
	rm -f $(OBJ)

cleanwin: 
	del $(OBJ)