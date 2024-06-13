CC := gcc
CFLAGS := `sdl2-config --cflags` -Wall -O2
LDFLAGS := `sdl2-config --libs` -lSDL2_image -lSDL2_ttf

TARGET := sdl_game
SRC := test.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
