SRC_DIR = src
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OUT = build/presento
LFLAGS = -lSDL2main -lSDL2 -lSDL2_ttf

$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(OUT)