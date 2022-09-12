// Data structure imitating table
// Realization - linked list, but last node links to first node
#include <iostream>

template <typename Type>
class Table {
public:
	struct Node {
		Type* val;
		Node* next = nullptr;
	};
	Table(Type val) {
		first = new Node({ new Type(val), nullptr });
		first->next = first;
		size = 1;
	};
	Table(Type* ptr) {
		first = new Node({ ptr, nullptr });
		first->next = first;
		size = 1;
	}

	~Table() {
		Node* first_elem = first;
		do {
			Node* tmp = first->next;
			delete first->val;
			delete first;
			first = tmp;
		} while (first_elem != first->next);
	}

	void push_front(Type val) {
		Node* tmp = first->next;
		first->next = new Node({ new Type(val), nullptr });
		first->next->next = tmp;
		size++;
	}
	void push_front(Type* ptr){	
		Node* tmp = first->next;
		first->next = new Node({ ptr, nullptr });
		first->next->next = tmp;
		size++;
	}

	void push_back(Type val) {
		this->operator[](size - 1).next = new Node({ new Type(val), first});
		size++;
	}
	void push_back(Type* ptr) {
		this->operator[](size - 1).next = new Node({ ptr, first });
		size++;
	}

	void push(Type val, size_t pos) {
		// Push after pos element
		this->operator[](pos).next = new Node({ new Type(val), first});
		size++;
	}
	void push(Type* ptr, size_t pos) {
		// Push after pos element
		this->operator[](pos).next = new Node({ ptr, first });
		size++;
	}

//	Node pop_front();
//	Node pop_back();
//	Node pop(Type* elem);

	void shift() {
		first = first->next;
	}
	const Node* GetFirst() const {
		return first;
	}
	size_t get_size();
	
	Node& operator[] (int index) {
		Node* result = first;
		for (int i = 0; i < index; i++) {
			result = result->next;
		}
		return *result;
	}

	friend std::ostream& operator<< (std::ostream& os, const Node& node) {
		os << *(node.val);
		return os;
	}

	Node* first;
	size_t size;
};

template <typename Type>
std::ostream& operator<< (std::ostream& os, Table<Type>& table) {
	if (table.size > 1) {
		const Table<Type>::Node* first_elem = table.first;
		do {
			std::cout << *(table.first->val) << " ";
			table.first = table.first->next;
		} while (first_elem != table.first);	
	}
	else if (table.size == 1) {
		os << *(table.first->val);
	}
	
	return os;
}
