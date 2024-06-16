CC := gcc
CFLAGS := $(shell sdl2-config --cflags) -Wall -O2
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf

TARGET := sdl_game
SRC := new.c
OBJ := new.o uchow.o

# 編譯 .c 檔案成 .o 檔案
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 鏈接所有 .o 檔案成最終的執行檔
$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(TARGET) $(OBJ)