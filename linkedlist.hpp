/*****************************************************************
 * @file   LinkedList.hpp
 * @brief  
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <iostream>

#include "memtrace.h"

/**
 * Generikus Láncolt Lista. (nem sorrendtartó)
 * @tparam T a tárolt elemek típusa
 */
template<typename T>
class LinkedList {
	/**
	 * Láncoltlistaelem: Segédclass az elemek tárolásához. 
	 */
	struct LinkedListItem {
		T data; //< A tárolt adat 
		LinkedListItem* next; //< A következő elemre mutató pointer. Ha nincs következő, akkor nullptr
		/**
		 * Konstruktor.
		 * @param data eltárolandó adat
		 */
		LinkedListItem(T data):data(data), next(nullptr) {};
	};

	/**
	 * Az első láncoltlistaelem.
	 */
	LinkedListItem* first;
	LinkedList(const LinkedList&); //< Másoló konstzruktor tiltása
public:
	/**
	 * Default konstruktor.
	 */
	LinkedList() :first(nullptr) {};

	/**
	 * Lemásolja a listát, (de fordított sorrendben), nem elvárt, hogy sorrendtartó legyen a lista.
	 */
	LinkedList& operator=(const LinkedList&); 

	/**
	 * Hozzáad egy elemet a lista elejéhez
	 * @param item a tárolandóü elem
	 */
	void push(T item); 

	/**
	 * Kitörli a megadott elemet a listából
	 * @param item A törlendő elem referenciája.
	 */
	void remove(const T& item);

	/**
	 * Megkeresi a megadott elemet.
	 * @param item Az elem referenciája.
	 * @return Az elemre mutató pointer, ha nincs a listában, nullptr-t ad
	 */
	T* find(const T& item);

	/**
	 * Megkeresi a lista megadott elem utáni elemét. 
	 * @param item Az elem referenciája.
	 * @return Visszaadja a következő elemre mutató pointert, ha nem létezik, nullptr-t ad vissza.
	 */
	T* getNext(const T& item); 

	/**
	 * @return visszaadja az első elemére mutató ptr-t, vagy nullptr-t, ha üres a lista.
	 */
	T* getFirst();

	/**
	 * @return visszaadja, hogy üres-e a láncolt lista
	 */
	bool isEmpty() const;

	/**
	 * Destruktor
	 */
	~LinkedList();
};


template<typename T>
inline LinkedList<T>& LinkedList<T>::operator=(const LinkedList& rhs)
{
	if (this == &rhs) return *this; // önértékadás
	this->~LinkedList(); // Meg kell szüntetni a lista tartalmát, mielőtt feltöltjük.
	LinkedListItem* iter = rhs.first;
	while (iter != nullptr)
	{
		push(iter->data);
		iter = iter->next;
	}
	return *this;
}

template<typename T>
inline void LinkedList<T>::push(T item)
{
	LinkedListItem* tmp = first;
	first = new LinkedListItem(item);
	first->next = tmp;
}



template<typename T>
inline void LinkedList<T>::remove(const T& item)
{
	LinkedListItem* iter = first;
	T* res = find(item);
	if (res == nullptr) return;

	LinkedListItem* prev = nullptr;
	while (iter != nullptr) {
		if (&(iter->data) == res) break;
		prev = iter;
		iter = iter->next;
	}
	// Újralinkel
	if (prev != nullptr)
		prev->next = (iter == nullptr) ? nullptr : iter->next;
	else
		first = nullptr;

	// Ha nullptr-t töröl nincs hatás
	delete iter;
}


template<typename T>
inline T* LinkedList<T>::find(const T& item)
{
	if (isEmpty()) return nullptr;
	LinkedListItem* iter = first;

	while (iter->data != item) {
		iter = iter->next;
		if (iter == nullptr) return nullptr;
	}
	return &(iter->data);
}

template<typename T>
inline T* LinkedList<T>::getNext(const T& item) {
		if (&item == nullptr) return nullptr;
	if (isEmpty()) return nullptr;
	LinkedListItem* iter = first;

	while (iter->data != item) {
		iter = iter->next;
		if (iter == nullptr) return nullptr;
	}
	if (iter->next == nullptr) return nullptr;
	return &(iter->next->data);
}

template<typename T>
inline T* LinkedList<T>::getFirst() 
{
	if (isEmpty()) return nullptr;
	return &(first->data);
}

template<typename T>
inline bool LinkedList<T>::isEmpty() const
{
	return first == nullptr;
}

template<typename T>
inline LinkedList<T>::~LinkedList()
{
	while (!isEmpty()) {
		LinkedListItem* next = first->next;
		delete first;
		first = next;
	}
}


#endif // !LINKEDLIST_H