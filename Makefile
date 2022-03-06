all:
	clang++ -std=c++20 gasstation.cpp -pthread -o gasstation

clean:
	rm -f gasstation *~

test:
	clang++ -std=c++20 -fsanitize=thread gasstation.cpp -pthread -o gasstation
	