CC     = gcc
CFLAGS = -lmingw32 -lSDL2main -lSDL2 -std=c99 -lm
# modify the flags here ^

EXE    = chip8
OBJ    = main.o chip8.o file.o cycle.o graphics.o


# top (default) target
all: $(EXE)

debug: CFLAGS += -DDEBUG -g
debug: $(EXE)

release: CFLAGS += -O3
release: $(EXE)

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
.PHONY: clean cleanwin

clean:
	rm -f $(OBJ)

cleanwin: 
	del $(OBJ)