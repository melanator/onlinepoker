// Data structure imitating table
// Realization - linked list, but last node links to first node

template <typename Type>
class Table {

	struct Node {
		Type* val;
		Node* next;
	};

public:
	Table(){};
	Table(Type& val) {
		first = { &val, nullptr };
		first.next = &first;
		size = 1;
	};
	void push_front(Type node) {	}
//	void push_back(Type node)
//  void push(Node* node, size_t pos);
//	Node pop_front();
//	Node pop_back();
//	Node pop();
	void shift() { first = *first.next; }
	size_t get_size();

//	int& operator[] (int index);

private:
	Node first;
	size_t size;
};
