/*****************************************************************
 * @file   FixArray.h
 * @brief  Fix méretű tömb, a memória kezelés elrejtésére
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/

#ifndef FIXARRAY_H
#define FIXARRAY_H
#include <exception>
#include "memtrace.h"

/**
 * Generikus fix méretú tömb.
 * @tparam T a tárolt típus.
 * @tparam N a tömb mérete
 */
template <typename T, size_t N>
class FixArray
{
	T* data; //< A tárolt adatok tömbje
public:
	/**
	 * Default konstruktor.
	 */
	FixArray() {
		data = new T[N];
	};
	/**
	 * Másoló konstruktor
	 */
	FixArray(const FixArray& fa) {
		data = new T[N];
		for (size_t i = 0; i < N; i++) {
			data[i] = fa.data[i];
		}
	}
	FixArray& operator=(const FixArray& rhs) {
		if (this == &rhs) return *this;
		for (size_t i = 0; i < N; i++) {
			data[i] = rhs.data[i]; 
		}
		return *this;
	}

	T& operator[](size_t i) {
		// Alulindexelésre nem kellene tesztelni a size_t típus miatt, de fő a biztonság.
		if (i < 0 || i >= N) throw std::out_of_range("Index out of range.");
		return data[i];
	}
	const T& operator[](size_t i) const {
		// Alulindexelésre nem kellene tesztelni a size_t típus miatt, de fő a biztonság.
		if (i < 0 || i >= N) throw std::out_of_range("Index out of range.");
		return data[i];
	}
	/**
	 * @return Visszaadja a  tömb maximális méretét.
	 */
	size_t size() const {
		return N;
	}
	~FixArray() {
		delete[] data;
	}
};

#endif // !FIXARRAY_H