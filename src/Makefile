BUILD_PATH=../bin/
CC=gcc
CFLAGS=-g -Wall -Wextra -Werror
EXTRAFLAGS=-fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector 

all: check rebuild

build: clean game_of_life game_of_life_extra  

rebuild: clean_all game_of_life game_of_life_extra clean

game_of_life: clean game_of_life.c game_of_life_engine.c
	$(CC) $(CFLAGS) game_of_life.c game_of_life_engine.c -o  $(BUILD_PATH)game_of_life -lncurses

game_of_life_extra: clean game_of_life.c game_of_life_engine.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) game_of_life.c game_of_life_engine.c -o  $(BUILD_PATH)game_of_life_extra -lncurses

clean_all:
	rm -rf *.o *.a *.so $(BUILD_PATH)*
	
clean:
	rm -rf *.o *.a

check:
	clang-format -n *.c
	clang-format -n *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

run:
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes $(BUILD_PATH)game_of_life

run_no_valgrind:
	$(BUILD_PATH)game_of_life_extra


