SRC_FILES = $(wildcard src/*.c)
OBJ_FILES := $(subst .c,.o,$(SRC_FILES))
OBJ_FILES := $(subst src,build, $(OBJ_FILES))
TARGET = ./bin/app

.PHONY: run clean

$(TARGET): $(OBJ_FILES)
	gcc $^ -o $@ -lSDL2 -lGLEW -lGL -lm
build/%.o: src/%.c
	gcc -c $^ -o $@ -I./lib/include
run:
	$(TARGET)
clean:
	rm $(TARGET)
