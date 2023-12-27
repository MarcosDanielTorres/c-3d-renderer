run:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o renderer
	./renderer

build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o renderer

clean:
	rm ./renderer
