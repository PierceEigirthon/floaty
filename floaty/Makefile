
.PHONY: format
format:
	clang-format src/* include/floaty/* -i --style="{BasedOnStyle: Google, IndentWidth: 4 }"


.PHONY: test
test:
	g++ -std=c++2a -I src -I include tests/all.cpp src/main.cpp -o tests/out
	./tests/out
	rm tests/out

