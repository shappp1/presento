.PHONY: windows linux

srcfiles = main.c sdl_functions.c operations.c
builddir = build

linux: $(builddir)/main
windows: $(builddir)/main.exe

$(builddir)/main: $(srcfiles)
	gcc -o $(builddir)/main $(srcfiles) -lSDL2main -lSDL2 -lSDL2_ttf

$(builddir)/main.exe: $(srcfiles)
	x86_64-w64-mingw32-gcc -I/usr/x86_64-w64-mingw32/include/ -L/usr/x86_64-w64-mingw32/lib/ -o $(builddir)/main $(srcfiles) -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf