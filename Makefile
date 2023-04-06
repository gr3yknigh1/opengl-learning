
default: compile

compile:
	cmake --build build

run:
	./build/Debug/opengl-triangle
