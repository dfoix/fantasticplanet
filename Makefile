

CFLAGS =  $(shell sdl2-config --cflags)  -O2 
LIBS = -lSDL2_image -lSDL2_mixer $(shell sdl2-config --libs) -lm


main:	 
	gcc $(CFLAGS) -c Fixed_print.c
	g++ $(CFLAGS) demo.cpp \
				  objects/object.cpp \
				  creatures/creature.cpp \
				  player/player.cpp \
				  backgrounds/background.cpp \
				  objects/tile.cpp \
				  objects/objectmanager.cpp \
				  structures/ll.cpp \
				  weapons/weaponmanager.cpp \
				  weapons/weapon.cpp \
				  monsters/enemymanager.cpp \
				  monsters/monster.cpp \
				  monsters/slime.cpp \
				  monsters/face.cpp \
				  monsters/ape.cpp \
				  monsters/jumper.cpp \
				  Fixed_print.o -lm \
				  -o game $(LIBS)
	./game 

