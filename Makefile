SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OUT = $(BUILD_DIR)/presento
ifeq ($(OS),Windows_NT)
	FLAGS = -mwindows
	LFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	DLL = $(patsubst dll/%, $(BUILD_DIR)/%, $(wildcard dll/*))
else
	LFLAGS = -lSDL2main -lSDL2 -lSDL2_ttf
endif

$(OUT): $(OBJ) | $(BUILD_DIR) $(BUILD_DIR)/fonts $(DLL)
	$(CC) $(FLAGS) -o $@ $^ $(LFLAGS)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BUILD_DIR)/fonts:
	cp -r fonts/ build/

$(DLL): $(BUILD_DIR)/% : dll/%
	cp $< $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)