S=src/
B=build/
H=src/headers/

CC=gcc
FLAGS=-lncursesw
OBJECTS=$(B)main.o $(B)gui.o $(B)utils.o $(B)logic.o $(B)player.o \
	$(B)mapgen.o $(B)standard_objects.o $(B)monsters.o $(B)guiutils.o

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(B)game $(FLAGS)

$(B)main.o: $(S)main.c $(H)main.h $(H)gui.h $(H)utils.h \
  	    $(H)symbol.h $(H)logic.h $(H)guiutils.h $(H)standard_objects.h
	$(CC) -c $(S)main.c -o $(B)main.o

$(B)gui.o: $(S)gui.c $(H)gui.h $(H)symbol.h $(H)main.h
	$(CC) -c $(S)gui.c -o $(B)gui.o

$(B)utils.o: $(S)utils.c $(H)utils.h $(H)symbol.h $(H)mapgen.h
	$(CC) -c $(S)utils.c -o $(B)utils.o

$(B)logic.o: $(S)logic.c $(H)logic.h $(H)symbol.h $(H)player.h \
 	     $(H)standard_objects.h $(H)utils.h $(H)mapgen.h $(H)guiutils.h
	$(CC) -c $(S)logic.c -o $(B)logic.o

$(B)player.o: $(S)player.c $(H)player.h $(H)symbol.h  \
  	      $(H)mapgen.h $(H)standard_objects.h $(H)logic.h
	$(CC) -c $(S)player.c -o $(B)player.o

$(B)mapgen.o: $(S)mapgen.c $(H)mapgen.h $(H)standard_objects.h \
  	      $(H)player.h $(H)main.h $(H)utils.h $(H)symbol.h
	$(CC) -c $(S)mapgen.c -o $(B)mapgen.o

$(B)standard_objects.o: $(S)standard_objects.c $(H)standard_objects.h $(H)symbol.h
	$(CC) -c $(S)standard_objects.c -o $(B)standard_objects.o

$(B)monsters.o: $(S)monsters.c $(H)monsters.h $(H)symbol.h $(H)mapgen.h $(H)utils.h \
  		$(H)monsters.h $(H)player.h
	$(CC) -c $(S)monsters.c -o $(B)monsters.o

$(B)guiutils.o: $(S)guiutils.c $(H)guiutils.h $(H)symbol.h $(H)utils.h $(H)player.h \
  		$(H)mapgen.h
	$(CC) -c $(S)guiutils.c -o $(B)guiutils.o

run: all
	build/game

clean:
	-rm game.core
	-rm build/*.o
	-rm build/game
	-clear

style:
	astyle --options=.astyle -n --recursive 'src/*.c' 'src/headers/*.h'
