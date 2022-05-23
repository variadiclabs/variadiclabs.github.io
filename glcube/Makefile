OUTPUT = glcube
CXX = em++
CXXFLAGS = 
LFLAGS = -lGL -lglut -lglfw -s USE_SDL=2 -O3 -o $(OUTPUT).html --shell-file shell_minimal.html -s EXPORTED_FUNCTIONS='["_main","_checkbox","_setRotationSpeed","_setAngleX","_setAngleY","_setAngleZ","_resizeWindow","_handleKey"]' -s EXPORTED_RUNTIME_METHODS='["cwrap","ccall","UTF8ToString"]' -s ASSERTIONS=1 -I/usr/local/include -std=c++11
OBJECTS = glcube.cpp
all: $(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS)
clean:
	rm $(OUTPUT).html $(OUTPUT).js $(OUTPUT).wasm
