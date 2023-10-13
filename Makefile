.PHONY: fmt lint clean

SOURCES :=
SOURCES += $(shell find src/ -type f -regex ".*.[cpp,hpp]$$")


fmt:
	clang-format -i $(SOURCES) || true

lint:
	clang-format -Werror -n $(SOURCES) || true
	clang-tidy -p ./build $(SOURCES) || true

clean:
	rm -rf build/
