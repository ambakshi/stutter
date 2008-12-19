stutter: stutter.cpp
	g++ $(CXXFLAGS) -o stutter stutter.cpp -lGL `sdl-config --cflags --libs`
