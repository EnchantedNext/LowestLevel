CC = clang

SRC = union.c
DEST = union

all: $(DEST)

union: union.c
	$(CC) $(SRC) -o $(DEST)

run:
	./$(DEST)

clean:
	rm -f $(DEST)

.PHONY: all run clean
