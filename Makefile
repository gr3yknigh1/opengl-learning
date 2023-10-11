.PHONY: fmt lint clean

SOURCES :=
SOURCES += $(wildcard ./src/*.cpp)
SOURCES += $(wildcard ./src/**/.cpp)
SOURCES += $(wildcard ./src/*.hpp)
SOURCES += $(wildcard ./src/**/.hpp)


fmt:
	clang-format -i $(SOURCES) || true

lint:
	clang-format -Werror -n $(SOURCES) || true
	clang-tidy -p ./build $(SOURCES) || true

clean:
	rm -rf build/
