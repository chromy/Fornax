CFLAGS= -m32
LDFLAGS= -framework Awesomium

all: main

clean: 
	 -rm *.o main

main: main.o
	 $(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.cc
	$(CXX) $(CFLAGS) -c $< -o $@

