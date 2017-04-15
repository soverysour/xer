s=src/
b=build/
h=src/headers/

cc=gcc
flags=-lncurses
objects=$(b)main.o $(b)gui.o $(b)utils.o $(b)logic.o

all: $(objects)
	$(cc) $(objects) -o $(b)game $(flags)

$(b)main.o: $(s)main.c $(h)main.h $(h)gui.h $(h)utils.h $(h)symbol.h $(h)logic.h
	$(cc) -c $(s)main.c -o $(b)main.o

$(b)gui.o: $(s)gui.c $(h)gui.h $(h)symbol.h $(h)main.h
	$(cc) -c $(s)gui.c -o $(b)gui.o

$(b)utils.o: $(s)utils.c $(h)utils.h $(h)symbol.h
	$(cc) -c $(s)utils.c -o $(b)utils.o

$(b)logic.o: $(s)logic.c $(h)logic.h $(h)symbol.h
	$(cc) -c $(s)logic.c -o $(b)logic.o

run: all
	build/game

clean:
	-rm build/*.o
	-rm build/game
	-clear
