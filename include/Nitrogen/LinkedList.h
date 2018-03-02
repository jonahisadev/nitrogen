#ifndef NITROGEN_LINKEDLIST_H
#define NITROGEN_LINKEDLIST_H

#include <cstdio>

namespace Nitrogen {

	template <class T>
	struct LinkData {
		LinkData<T>* parent;
		LinkData<T>* child;
		T data;

		LinkData(LinkData<T>* parent, LinkData<T>* child, T data) {
			this->parent = parent;
			this->child = child;
			this->data = data;
		}

		bool hasChild() {
			if (child) {
				return true;
			}
			return false;
		}

		void insertBefore(LinkData<T>* data) {
			LinkData<T>* psave = this->parent;

			// Set link between current and new
			this->parent = data;
			data->child = this;

			// Set link between old parent and new
			data->parent = psave;
			psave->child = data;
		}

		void insertAfter(LinkData<T>* data) {
			LinkData<T>* csave = this->child;

			// Set link between current and new
			this->child = data;
			data->parent = this;

			// Set link between old child and new
			data->child = csave;
			csave->parent = data;
		}

		void remove() {
			child->parent = parent;
			parent->child = child;
			this->parent = nullptr;
			this->child = nullptr;
		}
	};

	template <class T>
	class LinkedList {
	private:
		LinkData<T>* root;
		LinkData<T>* tail;
		LinkData<T>* current;

	public:
		LinkedList(T root);
		~LinkedList();

		void add(T data);
		LinkData<T>* get(int index);
		T getData(int index);
		LinkData<T>* getRelative(int offset);
		LinkData<T>* child(LinkData<T>* current);
		LinkData<T>* parent(LinkData<T>* current);
		void setCurrent(LinkData<T>* current);

		LinkData<T>* getCurrent() const { return current; }
	};

	template class LinkedList<int>;
	
	template <class T>
	LinkedList<T>::LinkedList(T root) {
		this->root = new LinkData<T>(nullptr, nullptr, root);
		this->tail = this->root;
	}

	template <class T>
	LinkedList<T>::~LinkedList() {
		LinkData<T>* current = this->root;
		int count = 0;
		while (current->child != nullptr) {
			current = child(current);
			count++;
		}

		LinkData<T>* temp;
		for (int i = 0; i < count; i++) {
			temp = current->parent;
			delete current;
			current = temp;
		}
	}

	template <class T>
	void LinkedList<T>::add(T data) {
		// LinkData<T>* tailSave = this->tail;
		this->tail->child = new LinkData<T>(this->tail, nullptr, data);
		this->tail = this->tail->child;
		// this->tail->parent = tailSave;
	}

	template <class T>
	LinkData<T>* LinkedList<T>::get(int index) {
		LinkData<T>* current = this->root->child;
		for (int i = 0; i < index; i++) {
			current = current->child;
		}
		this->current = current;
		return current;
	}

	template <class T>
	T LinkedList<T>::getData(int index) {
		LinkData<T>* ld = get(index);
		return ld->data;
	}

	template <class T>
	LinkData<T>* LinkedList<T>::getRelative(int offset) {
		LinkData<T>* current = this->current;
		if (offset < 0) {
			offset = -offset;
			for (int i = 0; i < offset; i++) {
				current = current->parent;
			}
		} else if (offset > 0) {
			for (int i = 0; i < offset; i++) {
				current = current->child;
			}
		}
		return current;
	}

	template <class T>
	LinkData<T>* LinkedList<T>::child(LinkData<T>* current) {
		LinkData<T>* child = current->child;
		this->current = child;
		return child;
	}

	template <class T>
	LinkData<T>* LinkedList<T>::parent(LinkData<T>* current) {
		LinkData<T>* parent = current->parent;
		this->current = parent;
		return parent;
	}

	template <class T>
	void LinkedList<T>::setCurrent(LinkData<T>* current) {
		this->current = current;
	}

}

#endif // NITROGEN_LINKEDLIST_H