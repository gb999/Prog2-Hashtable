/*****************************************************************
 * @file   neptun_teszt.h
 * @brief  3. Teszteset: Neptun szimuláció 
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/

#ifndef NEPTUN_H
#define NEPTUN_H
#include <string>
#include "hashtable.hpp"

using std::string;

/**
 * Absztrakt Ember class.
 */
class Ember {
	string nev;
	int szulev;
	string neptun;
public:
	Ember(string nev, int szulev, string neptun) :nev(nev), szulev(szulev), neptun(neptun) {};
	string getNeptun() {
		return neptun;
	}
	string getNev() {
		return nev;
	}
	virtual ~Ember() {};
};

class Hallgato : public Ember {
	string jelszo; // Bekesen kilisazhato jelszo
public:
	Hallgato(string nev, int szulev, string neptun, string jelszo) :Ember(nev, szulev, neptun), jelszo(jelszo) {};
	string getJelszo() {
		return jelszo;
	}
};

class Oktato : public Ember {
	string* targyak; // Targyak tombje
	size_t nTargyak;
public:
	Oktato(string nev, int szulev, string neptun) :Ember(nev,szulev, neptun),targyak(nullptr),nTargyak(0) {
	};
	void addTargy(string nev) {
		string* nArr = new string[nTargyak+1];
		for (size_t i = 0; i < nTargyak;++i) {
			nArr[i] = targyak[i];
		}
		nArr[nTargyak++] = nev;
		delete[] targyak;
		targyak = nArr;
	}
	~Oktato() {
		delete[] targyak;
	}
};

/**
 * Az előző hashfüggvényekhez hasonlóan jól működő függvény
 * @return végez egy pár műveletet a neptun kod karaktereivel, aztán veszi a modulusát a max mérettel.
 */
size_t neptunHash(string key, size_t defSize) {
	size_t res = 0;
	for (size_t i = 0; i < key.size(); ++i) {
		res += (size_t)key[i] * 46368611 + i; 
	}
	return res %= defSize;
}

/**
 * Berak néhány hallgató és oktató pointert a Hash Táblába. Majd kilistázza a neptunkódokat és neveket.
 * Fontos megjegyezni, hogy a memória felszabadítása nem a HashTable felelőssége.
 */
void neptun_teszt() {
	Oktato a("a", 1, "asd");
	a.addTargy("bela");
	std::vector<Ember*> emberek;
	HashTable<Ember*, string, neptunHash, 5> neptun; // Egy hatékony és biztonságos 5 fős neptun
	emberek.push_back(new Hallgato("Kis Bela", 2000, "ABC123", "kis8ela"));
	emberek.push_back(new Hallgato("Nagy Bela", 2001, "DEF123", "NagyBela"));
	emberek.push_back(new Hallgato("Kozepes Bela", 1990, "GHI123", "kozepesBela"));
	emberek.push_back(new Oktato("Nagytudasu Bela", 1842, "JKL123"));
	emberek.push_back(new Oktato("ifj. Nagytudasu Bela", 1950, "MNO123"));

	for (size_t i = 0; i < emberek.size(); ++i) {
		neptun.put(emberek[i]->getNeptun(), emberek[i]);
	}
	Ember** oktato = neptun.get("MNO123");
	if (oktato == nullptr) throw std::runtime_error("Elveszett ifj. Nagytudasu Bela");
	((Oktato*)(*oktato))->addTargy("Mersekelten Megszegyenito Matematika");
	((Oktato*)(*oktato))->addTargy("Mersekelten Megalazo Matematika");
	if (neptun.size() != emberek.size()) 
		throw std::runtime_error("Hibas a neptun merete");

	for (auto iter = neptun.begin(); iter != neptun.end(); ++iter) {
		std::cout <<iter->value->getNev() << " - "<< iter->value->getNeptun() << std::endl;
	}
	// Fel kell szabadítani
	for (size_t i = 0; i < emberek.size(); ++i) {
		delete emberek[i];
	}

}

#endif