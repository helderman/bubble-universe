# Requires SDL2
# Ubuntu: sudo apt-get install libsdl2-dev

bu: bu.c
	$(CC) -Wall -Wextra -Wpedantic -Werror $< -lm -lSDL2 -o $@

clean:
	$(RM) bu
