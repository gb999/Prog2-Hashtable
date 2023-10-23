/*****************************************************************
 * @file   hashtable.hpp
 * @brief  HashTable class 
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "harray.hpp"
#include <string>

/**
 * Karakterkod sorrend alapján hashel.
 * @param key kulcs
 * @param maxSize hash tabla jelenlegi merete
 * @return szumma(i=0...l) {(key[i] + i) * i * l} a kulcs elejetol a vegeig haladva 
 */
size_t charCodeHash(const std::string key, const size_t maxSize);



/**
 * @param key kulcs
 * @param maxSize a hashtabla jelenlegi merete
 * @return a modulo maxSize 
 */
size_t linHash(const int key, const size_t maxSize);

/**
 * Generikus Hash tábla.
 * @tparam T A tárolt adat típusa
 * @tparam keyType A kulcs típusa.  
 * @tparam hashFunction Hash függvény, ami a megadott kulcstípusból előállít egy indexet a hashtábla mérettartományán belül.
 * @tparam defSize A tábla alapértelmezett tömbmérete. Ekkora blokkokban növekszik a tábla, ha a kapacitás 90% fölé érne.
 */
template<typename T, typename keyType = std::string, size_t hashFunction(keyType key, const size_t maxSize) = charCodeHash, size_t defSize = 100>
class HashTable : private HArray<T, keyType, defSize> {
	
	typedef HArray<T, keyType, defSize> harray;
	
	/**
	 * Újra hashel minden elemet. Akkor hívódik, ha a kapacitás elérte a 90%-ot.
	 * Megnöveli a HashTable méretét egy újabb defSize méretű tömbbel.
	 */
	void rehash(); 


	/**
	 * Meghívja a hash függvényt a jelenlegi mérettel.
	 */
	size_t hash(keyType key) const; 

	/**
	 * Privát konstruktor megadott számú tömbbel.
	 * Csak a rehash() használja
	 */
	HashTable(size_t nArrays) :harray(nArrays) {};

	/**
	 * Privát értékadás.
	 * Csak a rehash() használja.
	 */
	HashTable& operator=(const HashTable& rhs); 
public:
	/**
	 * Default konstruktor.
	 */
	HashTable();

	// Örökölt függvények
	using harray::size;
	using harray::capacity;


	/**
	* Berakja a megadott elemet a HashTable-be.
	 * @param key az elemhez tartozó kulcs
	 * @param value Tárolandó elem
	 */
	void put(keyType key, const T& value); 

	/**
	 * @param key Az elemhez tartozó kulcs. 
	 * @return Visszaadja a kulcshoz tartozó adatra mutató pointert, ha nem találja nullptr-t
	 */
	T* get(keyType key);

	/**
	 * Kitörli a kulcs által jelölt elemet a HashtTable-ből. Ha nincs benne, nem csinál semmit.
	 * @param key Az elemhez tartozó kulcs.
	 */
	void remove(keyType key);

	/** 
	 * @return Visszaadja a kulcshoz tartozó elemre mutató ptrt, ha nincs a táblában nullptr-t ad. 
	 */
	T* operator[](keyType key); 

	/**
	 * Konstans indexelő operátor.
	 */
	T* const operator[](keyType key) const;
	
	/**
	 * Örökölt iterator
	 */
	class iterator : public harray::iterator {
	public:
		/**
		 * Megörökli az összes konstruktort.
		 */
		using harray::iterator::iterator; 
	};

	/**
	 * @return A hashtable elejére mutató iterator
	 */
	iterator begin() {
		return iterator(this);
	};
	/**
	 * @return A hashtable utolsó utáni elemére mutató iterator
	 */
	iterator end() {
		return iterator(this, this->nArrays * defSize);
	};
};

template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline size_t HashTable<T, keyType, hashFunction, defSize>::hash(keyType key) const
{
	return hashFunction(key, this->nArrays * defSize);
}


template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline void HashTable<T, keyType, hashFunction, defSize>::rehash()
{
	HashTable nTable(this->nArrays + 1);
	for (HashTable::iterator iter = begin(); iter != end(); ++iter) {
		nTable.put(iter->key, iter->value);
	}

	*this = nTable;
}

template<typename T, typename keyType, size_t hashFunction(keyType, size_t), size_t defSize>
inline HashTable<T, keyType, hashFunction, defSize>::HashTable() :harray()
{
}

template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline HashTable<T, keyType, hashFunction, defSize>& HashTable<T, keyType, hashFunction, defSize>::operator=(const HashTable& rhs)
{
	harray::operator=(rhs);
	return *this;
}


template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline void HashTable<T, keyType, hashFunction, defSize>::put(keyType key, const T& value)
{
	if ((double)size() / (double)(this->nArrays * defSize) >= 0.9) {
		rehash();
	}
	this->add(hash(key), key, value);
}

template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline T* HashTable<T, keyType, hashFunction, defSize>::get(keyType key) 
{
	return harray::get(hash(key), key);
}

template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline void HashTable<T, keyType, hashFunction, defSize>::remove(keyType key)
{
	harray::remove(hash(key), key);
}

template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline T* HashTable<T, keyType, hashFunction, defSize>::operator[](keyType key)
{
	return get(key);
}

template<typename T, typename keyType, size_t hashFunction(keyType key, const size_t maxSize), size_t defSize>
inline T* const HashTable<T, keyType, hashFunction, defSize>::operator[](const keyType key) const
{
	return get(key);
}


#endif // HASHTABLE_H