
default: compile

configure:
	rm ./build -rf
	cmake -B build -G "Ninja Multi-Config" -D CMAKE_EXPORT_COMPILE_COMMANDS=true

compile:
	cmake --build build

run:
	./build/Debug/opengl-triangle
