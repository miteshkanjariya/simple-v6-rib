CC=g++

all:
	$(CC) -o ipv6 ipv6_main.cpp trie.cpp
test: all
	./ipv6
clean:
	rm -rf ipv6
