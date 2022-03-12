.PHONY: run clean

app: main.c
	gcc $^ -o $@ -lSDL2
run:
	./app
clean:
	rm app
