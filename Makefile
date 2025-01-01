.PHONY: windows linux

linux: build/main
windows: build/main.exe

build/main: main.c
	gcc -o build/main main.c -lSDL2main -lSDL2 -lSDL2_ttf

build/main.exe: main.c
	x86_64-w64-mingw32-gcc -I/usr/x86_64-w64-mingw32/include/ -L/usr/x86_64-w64-mingw32/lib/ -o build/main main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf