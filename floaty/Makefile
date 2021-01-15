
.PHONY: format
format:
	clang-format src/* include/floaty/* -i --style="{BasedOnStyle: Google, IndentWidth: 4 }" 

.PHONY: test
test:
	@g++ -std=c++17 -I src -I include src/converter32.cpp tests/case_generators.cpp tests/case_verifiers.cpp -Wall -pthread -o tests/out
	@./tests/out
	@rm tests/out
.PHONY: run
run:
	@g++ -std=c++17 -I src -I include src/*.cpp -O3 -o src/play
	@./src/play
	@rm src/play 
