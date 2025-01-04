SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OUT = $(BUILD_DIR)/presento
LFLAGS = -lSDL2main -lSDL2 -lSDL2_ttf

$(OUT): $(OBJ) | $(BUILD_DIR)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c -o $@ $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(OUT)