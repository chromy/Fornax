LDFLAGS= -lglfw -framework OpenGL -framework Cocoa

all: main

clean: 
	 -rm *.o main

main: main.o 
	 clang $(LDFLAGS) $^ -o $@

main.o: main.c
	 clang -c $(CFLAGS) $^

