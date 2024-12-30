.PHONY: windows linux

linux: main
windows: main.exe

main: main.c
	gcc -o build/main main.c -lSDL2main -lSDL2 -lSDL2_ttf

main.exe: main.c
	x86_64-w64-mingw32-gcc -I/usr/x86_64-w64-mingw32/include/ -L/usr/x86_64-w64-mingw32/lib/ -o build/main main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf