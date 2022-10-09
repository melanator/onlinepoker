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
	Table() {
		first = nullptr;
	}
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
		if (size == 0) {
			return;
		}
		else {
			Node* first_elem = first;
			do {
				Node* tmp = first->next;
				delete first->val;
				delete first;
				first = tmp;
			} while (first_elem != first->next);
		}
	}

	void push_front(Type val) {
		if (size == 0) {
			first = new Node({ new Type(val), nullptr });
			first->next = first;
			size = 1;
		}
		else {
			Node* tmp = first->next;
			first->next = new Node({ new Type(val), nullptr });
			first->next->next = tmp;
			size++;
		}
	}
	void push_front(Type* ptr){	
		if (size == 0) {
			first = new Node({ ptr, nullptr });
			first->next = first;
			size = 1;
		}
		else {
			Node* tmp = first->next;
			first->next = new Node({ ptr, nullptr });
			first->next->next = tmp;
			size++;
		}
	}

	void push_back(Type val) {
		if (size == 0) {
			first = new Node({ new Type(val), nullptr });
			first->next = first;
			size = 1;
		}
		else {
			this->operator[](size - 1).next = new Node({ new Type(val), first });
			size++;
		}
	}
	void push_back(Type* ptr) {
		if (size == 0) {
			first = new Node({ ptr, nullptr });
			first->next = first;
			size = 1;
		}
		else {
			this->operator[](size - 1).next = new Node({ ptr, first });
			size++;
		}
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

	void pop_front() {
		if (size == 1) {
			pop_last_element();
		}
		else if (size == 0) {
		}
		else {
			Node* tmp = first;
			first = first->next;
			size--;
			this->operator[](size - 1).next = first;	// Last element to point to new first
			delete tmp;
		}
	}
	
	void pop_back() {
		if (size == 1) {
			pop_last_element();
		}
		else if (size == 0) {
		}
		else {
			delete &(this->operator[](size - 1));
			size--;
			this->operator[](size - 1).next = first;
		}
	}

	void pop(Type* elem) {
		Node* search = first;
		Node* previous;
		for (int i = 0; i < size; i++) {
			if (search->val == elem) {
				previous->next = search->next;
				delete search;
				size--;
				return;
			}
			previous = search;
			search = search->next;
		}
		throw;
	}

	void pop(size_t pos) {
		if (size == 1) {
			pop_last_element();
		}
		else if (size == 0) {}
		else {	
			Node* tmp = &(this->operator[](pos));
			this->operator[](pos-1).next = &(this->operator[](pos+1));
			delete tmp;
			size--;
		}
	}

	void shift(size_t n = 1) {
		for (int i = 0; i < n; i++) {
			first = first->next;
		}
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
	size_t size = 0;

private:
	void pop_last_element() {
			delete first;
			first = nullptr;
			size = 0;
	}

	Node* GetNodePointer(size_t pos) {
		Node* result = first;
		for (int i = 0; i < pos; i++) {
			result = first->next;
		}
		return result;
	}
};

template <typename Type>
std::ostream& operator<< (std::ostream& os, Table<Type>& table) {
	if (table.size > 1) {
		const typename Table<Type>::Node* first_elem = table.first;
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

