#include "../include/table.h"

int main() {
	int a = 5;
	Table<int> table(a);
	int b = 2;
	table.push_front(b);
	table.shift();
}