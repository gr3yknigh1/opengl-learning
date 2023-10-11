.PHONY: fmt lint clean


fmt:
	clang-format -i ./src/*.cpp || true
	clang-format -i ./include/**/*.hpp || true

lint:
	clang-format -Werror -n ./src/*.cpp || true
	clang-format -Werror -n ./include/**/*.hpp || true
	clang-tidy -p ./build ./src/*.cpp || true
	clang-tidy -p ./build ./include/**/*.hpp || true

clean:
	rm -rf build/
