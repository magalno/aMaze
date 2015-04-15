FLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main.run : main.cpp
	g++ $< solver.cpp -o $@ $(CFLAGS) $(LIBS)
