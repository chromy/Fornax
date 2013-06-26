CFLAGS= -g -m32 -fno-rtti
LDFLAGS= -lglfw -framework OpenGL -framework Cocoa -framework Awesomium
BINARIES= main

all: $(BINARIES)

clean: 
	 -rm *.o $(BINARIES)

main: main.o UserInterface.o GlfwToUiBridge.o
	 $(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.cc

%.o: %.cc
	$(CXX) $(CFLAGS) -c $< -o $@
