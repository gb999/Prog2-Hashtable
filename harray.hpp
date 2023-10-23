/*****************************************************************
 * @file   harray.hpp
 * @brief  HArray class deklarációi és definíciói. Sajnos a templateknél ezeknek egy fordítási egységbe kell kerülnie.
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/

#ifndef HARRAY_H
#define HARRAY_H
#include <string>
#include "fixarray.hpp"
#include "linkedlist.hpp"
#include <exception>

#include "memtrace.h"


/**
 * Gernerikus Hash Array 
 * A HashTable belső működéséért felelős osztály.
 * @param T - Tárolt elemek típusa
 * @param keyType - Kulcs típusa. (default: std::string)
 * @param defSize - A tárolt tömbök alapártelmezett mérete. (default: 10)
 */
template <typename T, typename keyType = std::string, size_t defSize = 10>
class HArray {
public:

	/**
	 * HashItem-ek vannak tárolva a Láncolt listákban.
	 */
	struct HashItem {
		keyType key; //< Az elemhez tartozó kulcs
		T value; //< A tárolt elem
		/**
		 * Default konstruktor.
		 */
		HashItem():key(keyType()),value(T()) {};
		/**
		 * Konstruktor egy kulcsból és értékből.
		 * @param key a megadott kulcs
		 * @param a kulcshoz tartozó elem
		 */
		HashItem(keyType key, T value) :key(key), value(value) {};

		/** 
		 * Kulcs alapú keresésre használt konstruktor
		 * @param key a kulcs.
		 */
		HashItem(keyType key) :key(key),value(T()) {};

		/**
		 * Kulcsalapú egyenlőség 
		 */
		bool operator==(const HashItem& rhs) {
			return key == rhs.key;
		}

		/**
		 * Kulcsalapú egyenlőség. Elég kulccsal összehasonlítani
		 * @param rhs kulcs
		 */
		bool operator==(const keyType& rhs) {
			return key == rhs;
		}

		/**
		 * Nem egyenlőség
		 */
		bool operator !=(const HashItem& rhs) {
			return !(*this == rhs);
		}

		/**
		 * Kulcs alapú nem egyenlőség.
		 */
		bool operator !=(const keyType& rhs) {
			return !(*this == rhs);
		}

	};

	/**
	 * Konstruktor, ami megadott számú tömbbel hozza létre a HArray-t
	 * @param nArrays ennyi tömböt foglal
	 */
	HArray(size_t nArrays);

	/**
	 * Default konstruktor.
	 */
	HArray();
	/**
	 * @return Visszaadja a jelenlegi elemszámot
	 */
	size_t size() const; 

	/**
	 * @return Visszaadja a még tárolható elemek számát
	 */
	size_t capacity() const; 
		

	/** 
	 * Hozzáadja az elemet a megadott indexű láncolt listához, ha még nincs benne.
	 * @param i A láncolt lista indexe
	 * @param key Az elemhez tartozó kulcs
	 * @param value A tárolandó elem
	 */
	void add(size_t i, keyType key, const T& value); 

	/**
	 * Kitörli a megadott indexű láncolt listából az adott kulcsú elemet.
	 * @param i A láncolt lista indexe
	 * @param key A törlendő elemhez tartozó kulcs.
	 */
	void remove(size_t i, keyType key);		

	/**
	 * @param i a láncolt lista indexe
	 * @param key, a keresendő elemhez tartozó kulcs.
	 * @return  Visszaadja a megadott elem értékére mutató pointert, ha nem találja az elemet, nullptrt ad 
	 */
	T* get(size_t i, keyType key); 

	/**
	 * HArray iteratora. Csak a már feltöltött elemeken megy végig.
	 * Ezt fogja örökli a HashTable.
	 */
	class iterator {
	private:
		HArray<T, keyType, defSize>* pArr; //< Mutató a Tárolóra
		HashItem const * pItem; //< Mutató az éppen mutatott elemre
		size_t idx; //< A jelenlegi elem indexe
		keyType key; //< A jelenlegi elem kulcsa
	public:
		/**
		 * Konstruktor
		 * @param arr Mutató a tárolóra
		 * @param i Az elem indexe
		 * @param key Az elemhez tartozó kulcs
		 */
		iterator(HArray<T, keyType, defSize>* arr, size_t i, keyType key):pArr(arr),pItem((*pArr)[i].find(key)), idx(i), key(key){
		};

		/**
		 * Üres iterator konstruktora. Az end() létrehozásához kell.
		 * @param arr A tároló mutatója
		 */
		iterator(HArray<T, keyType, defSize>* arr, size_t i) :pArr(arr), pItem(nullptr), idx(i) {};

		/**
		 * Default konstruktor-szerű. A megadott tároló első elemére mutat
		 * @param arr A tároló mutatója
		 */
		iterator(HArray<T, keyType, defSize>* arr) :pArr(arr), pItem(nullptr), idx(0) {
			if ((*pArr)[idx].isEmpty())
				++(*this);
			else 
				pItem = (*pArr)[idx].getFirst();

			if (pItem == nullptr) // Üres az egész HArray
				*this = iterator(arr, idx); // = end()
			else
				key = pItem->key;
			
		};

		// Lehet kulcs alapjan keresni, mert van HashItem operator==(keyType)
		HashItem& operator*() {
			return *((*pArr)[idx].find(key)); 
		};
		HashItem* operator->() {
			return (*pArr)[idx].find(key); 
		};

		/** 
		 * @return Visszaadja a következő létező elem iterátorát, vagy az utolsó utánira mutatót
		 */
		iterator& operator++() { // pre-increment
			// Elértük a HArray végét 
			if (idx == pArr->nArrays * defSize) { 
				pItem = nullptr;
				return *this; // end()
			}

			HashItem const * next;
			//Ha a pItem nullptr, akkor tömböt léptünk és a lista első elemét kell megnéznünk
			if (pItem == nullptr) {
				next = (*pArr)[idx].getFirst();
			}
			else {
				// Megnézzük, hogy a jelenlegi listában van-e még elem.
				next = (*pArr)[idx].getNext(*pItem);
			}

			if (next != nullptr) { // Ha van, akkor boldogok vagyunk
				pItem = next;
				key = next->key;
				return *this;
			}
			else { // Ha nincs, akkor nézzük a következő tömböt.
				++idx;

				
				pItem = nullptr;
				return ++(*this);
			}
		}
		/**
		 * Post increment
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator==(const iterator& rhs) {
			return pItem == rhs.pItem;
		}
		bool operator!=(const iterator& rhs) {
			return !(*this == rhs);
		}

	};
	/**
	 * 
	 * @return első elemre mutató iterator
	 */
	iterator begin() {
		// Hívjuk meg az első létező elemet visszaadó konstruktort
		return iterator(this);
	}
	/**
	 * @return az utolsó utáni elemre mutató iterator
	 */
	iterator end() {
		return iterator(this, nArrays * defSize);

	};
	/**
	 * Értékadó operátor. 
	 */
	HArray& operator=(const HArray& rhs); 	// Privát értékadó
	/**
	 * Destruktor
	 */
	~HArray();
protected:
	size_t nArrays; //< A jelenleg tárolt tömbök száma. A HashTable függvényeinek el kell érni.
private:
	/**
	 * @return visszaadja az index alapján meghatározott láncolt listát.
	 */
	LinkedList<HashItem>& operator[](size_t i); 
	size_t nElements; //< A Jelenlegi elemszám, nyilván van tartva, hogy ne kelljen mindig kiszámolni
	typedef FixArray<LinkedList<HashItem>, defSize> fixarr; //= FixArray<LinkedList<HashItem>, size>
	fixarr* pData; //< Láncolt listákkal feltöltött fix tömbök tárolója.
	HArray(const HArray& rhs); 	//< Másoló konstruktor tiltása
};


template<typename T, typename keyType, size_t defSize>
inline HArray<T, keyType, defSize>::HArray(size_t nArrays): nArrays(nArrays), nElements(0), pData((nArrays > 0) ? new fixarr[nArrays] : nullptr)
{
}

template<typename T, typename keyType, size_t defSize>
inline HArray<T, keyType, defSize>::HArray() : HArray(1) 
{
}

template<typename T, typename keyType, size_t defSize>
inline size_t HArray<T, keyType, defSize>::size() const
{
	return nElements;
}

template<typename T, typename keyType, size_t defSize>
inline size_t HArray<T, keyType, defSize>::capacity() const
{
	return nArrays * defSize - nElements;
}



template<typename T, typename keyType, size_t defSize>
inline LinkedList<typename HArray<T,keyType,defSize>::HashItem>& HArray<T, keyType, defSize>::operator[](size_t i)  
{
	size_t idx_in_array = i % defSize;
	size_t nArray = (i-idx_in_array) / defSize;

	// Alulindexelésre nem kellene tesztelni a size_t típus miatt, de fő a biztonság.
	if (nArray >= nArrays || nArray < 0) 
		throw std::out_of_range("Out of range. Esetleg rossz a Hash fuggveny?");
	return pData[nArray][idx_in_array];
}

template<typename T, typename keyType, size_t defSize>
inline void HArray<T, keyType, defSize>::add(size_t i, keyType key, const T& value)
{	
	LinkedList<HashItem>& list = (*this)[i];

	if(list.find(key) != nullptr) return;

	list.push(HashItem(key, value));
	nElements++;
}

template<typename T, typename keyType, size_t defSize>
inline void HArray<T, keyType, defSize>::remove(size_t i, keyType key)
{
	LinkedList<HashItem>& list = (*this)[i];

	if (list.find(key) == nullptr) return;

	list.remove(key);
	nElements--;
}

template<typename T, typename keyType, size_t defSize>
inline T* HArray<T, keyType, defSize>::get(size_t i, keyType key)
{
	LinkedList<HashItem>& list = (*this)[i];
	HashItem* res = list.find(key);
	if (res == nullptr) return nullptr;
	return &(res->value);
}


template<typename T, typename keyType, size_t defSize>
inline HArray<T, keyType, defSize>& HArray<T, keyType, defSize>::operator=(const HArray& rhs)
{
	// Önértékadás
	if (this == &rhs) return *this;
	nArrays = rhs.nArrays;
	nElements = rhs.nElements;
	delete[] pData;
	pData = new fixarr[nArrays];
	for (size_t i = 0; i < nArrays; i++) {
		pData[i] = rhs.pData[i];
	}
	return *this;
}


template<typename T, typename keyType, size_t defSize>
inline HArray<T, keyType, defSize>::~HArray()
{
	delete[] pData;
}

#endif // !HARRAY_H