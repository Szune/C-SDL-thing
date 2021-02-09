#include <stdlib.h>

int main(int argc, char* argv[]) {
	system("gcc main.c -o test -lSDL2main -lSDL2"); // not a safe way to do it, but a very lazy one
	return 0;
}
