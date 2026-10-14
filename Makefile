.PHONY: all clean rebuild
BUILD_DIR="build/"

all: $(BUILD_DIR)rlgol

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)rlgol: src/main.c $(BUILD_DIR)
	$(CC) $< -o $@ -ggdb -lraylib

clean:
	-rm src/*.o
	-rm -rd $(BUILD_DIR)

rebuild: clean all
