# Presento
## Install
### Requirements
```
mingw-w64 (on windows)
make
gcc
SDL2
SDL2_ttf
```
### Build
Just run make. Will be built to build/main (or build/main.exe on windows)

NOTE: I haven't tested this on windows yet; there will probably be errors with missing DLLs.

## Implemented Controls
### Program Operation
- Ctrl+P - start slideshow from current slide
- Ctrl+Shift+P - start slideshow from first slide
- Ctrl+S - save
- Ctrl+L - load

### Slide Operation
- PgUp - go to previous slide
- PgDn - go to next slide
- Ctrl+PgUp - swap current and previous slide
- Ctrl+PgDn - swap current and next slide
- Ctrl+N - new slide
- Ctrl+Delete - delete slide

### Text Box Operation
- Arrow Keys - move active text box within slide*
- Ctrl+Arrow Keys - resize active text box within slide*
- Tab - switch active text box
- Ctrl+T - new normal text box
- Ctrl+B - new bold text box
- Del - remove active text box

*Holding Shift will speed up operation

### Slideshow Operation
- Left Arrow - go back one slide
- Right Arrow/Space - go forward one slide
- Esc - end slideshow

## Features
### Required Features
- [x] Adding normal or bold text boxes
- [x] Deleting text boxes
- [x] Moving text boxes anywhere within a slide
- [x] Resizing text boxes in a slide
- [x] At least a font for normal and bold text
- [x] Adding slides
- [x] Deleting slides
- [x] Slideshow mode (button for start from beginning and start from current slide)
- [x] Saving and loading files

### Extra Features (if time)
- [ ] Changing slide background colour (probably really easy)
- [ ] Adding images to slides