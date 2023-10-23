/*****************************************************************
 * @file   programnyelvek_teszt.h
 * @brief  2. Teszteset: programnyelvek
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/
#ifndef PROGRAMNYELVEK_H
#define PROGRAMNYELVEK_H
#include "hashtable.hpp"
#include <string>
#include <iostream>
using std::string;

/**
 * A "languages.txt" file tartalmának forrása: (https://gist.github.com/turicas/d5f8ce3ceb99f43a11b1e4e7fb2a2bf9)
	name,wikipedia_url
 */
/**
 * Ez a teszt betölt egy táblába sok programozási nyelvet és megkeres néhány különlegeset, 
 * valamint kielégíti kíváncsiságunkat a size és capacity függvények működésével kapcsolatban.
 */
void programnyelvek_teszt() {
	HashTable<string, string, charCodeHash, 1000> nyelvek; // 1000 mereture foglalja, hogy ne kelljen ujrahashelni
	std::ifstream is;
	is.open("languages.txt");
	if (!is.is_open()) throw std::runtime_error("Nem nyilt meg a file.");
	string name;
	string url;
	size_t n = 0;
	while (is) {
		std::getline(is, name, ',');
		std::getline(is, url);
		if (nyelvek.get(name) == nullptr) { // Erre a feltetelre szukseg van, hogy helyesen tudjuk szamolni a betett nyelveket. A fileban lehetnek duplikaciok
			nyelvek.put(name, url);
			++n;
		}
		
	}
	is.close();
	// Most keresunk par nyelvet
	if (nyelvek.get("JavaScript") == nullptr) throw std::runtime_error("Nincs meg a kedvenc nyelvem.");
	if (nyelvek.get("Java") == nullptr) throw std::runtime_error("Nincs meg a 2. kedvenc nyelvem.");
	if (nyelvek.get("C") == nullptr) throw std::runtime_error("Nincs meg a 3. kedvenc nyelvem.");
	if (nyelvek.get("Python") == nullptr) throw std::runtime_error("Nem sikerult elovarazsolni a kigyocskakat.");
	if (n != nyelvek.size()) throw std::runtime_error("Nem annyi nyelv lett betoltve, mint amennyi be lett olvasva.");
	if (1000-n != nyelvek.capacity()) throw std::runtime_error("Rossz a kapacitas.");
}
#endif // PROGRAMNYELVEK_H
