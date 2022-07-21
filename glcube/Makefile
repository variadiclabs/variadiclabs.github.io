OUTPUT = glcube
CXX = em++
CXXFLAGS = 
LFLAGS = -lGL -lglut -lglfw -s USE_SDL=2 -O3 --closure 1 -fPIC -Wno-implicit-function-declaration -o $(OUTPUT).html --shell-file shell_minimal.html -s EXPORTED_FUNCTIONS='["_main","_resizeWindow","_handleKey"]' -s EXPORTED_RUNTIME_METHODS='["cwrap","ccall","UTF8ToString"]' -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -I/usr/local/include -std=c++11
OBJECTS = glcube.cpp
all: $(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS)
clean:
	rm $(OUTPUT).html $(OUTPUT).js $(OUTPUT).wasm
