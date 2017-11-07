#ifndef NITROGEN_LIST_H
#define NITROGEN_LIST_H

#include <cstdlib>

namespace Nitrogen {

	template <class T>
	class List {
	private:
		T* data;
		int ptr;
		int alloc_size;
		
	public:
		List(int size);
		~List();
		
		void add(T data);
		T get(int ptr);
		void set(int ptr, T data);
		
		int getSize() const { return ptr; }
	};
	
	template class List<unsigned char>;
	template class List<char*>;
	template class List<int>;
	
	template <class T>
	List<T>::List(int size) {
		this->data = (T*) malloc(sizeof(T) * size);
		this->alloc_size = size;
		this->ptr = 0;
	}
	
	template <class T>
	List<T>::~List() {
		free(this->data);
	}
	
	template <class T>
	void List<T>::add(T data) {
		if (this->ptr >= this->alloc_size) {
			this->alloc_size *= 2;
			this->data = (T*) realloc(this->data, sizeof(T) * this->alloc_size);
		}
		
		this->data[this->ptr++] = data;
	}
	
	template <class T>
	T List<T>::get(int ptr) {
		return this->data[ptr];
	}
	
	template <class T>
	void List<T>::set(int ptr, T data) {
		this->data[ptr] = data;
	}

}

#endif // NITROGEN_LIST_H