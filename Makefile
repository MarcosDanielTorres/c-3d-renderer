run:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o renderer
	./renderer

build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o renderer

clean:
	rm ./renderer
