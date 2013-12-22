CC=clang
SOURCES=test.c queue.c
EXECUTABLE=test

all:
	$(CC) -o $(EXECUTABLE) $(SOURCES)

clean:
	rm $(EXECUTABLE)
