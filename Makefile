CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro

SRC_DIR = src
BIN_DIR = bin

TARGETS = \
	$(BIN_DIR)/01_basic_structure.exe \
	$(BIN_DIR)/02_graphics.exe \
	$(BIN_DIR)/03_input_keyboard.exe \
	$(BIN_DIR)/03_input_mouse.exe \
	$(BIN_DIR)/04_sound.exe \
	$(BIN_DIR)/05_gameplay.exe

all: $(BIN_DIR) $(TARGETS) copy_assets

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(BIN_DIR)/%.exe: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

copy_assets:
	cp assets/* $(BIN_DIR)/

clean:
	rm -f $(BIN_DIR)/*.exe

.PHONY: all clean copy_assets
