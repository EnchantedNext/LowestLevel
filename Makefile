CC = clang

SRC = clock.c
DEST = clock

all: $(DEST)

clock: clock.c
	$(CC) $(SRC) -o $(DEST)

run:
	./$(DEST)

clean:
	rm -f $(DEST)

.PHONY: all run clean
