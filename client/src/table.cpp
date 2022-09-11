#include "../include/table.h"
using namespace std;

int main() {

	{
		int a = 5;
		Table<int> table(3);
		table.push_front(4);
		table.push_front(5);
		std::cout << table << std::endl;
		std::cout << *table.first;
		
	}
}