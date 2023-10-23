/*****************************************************************
 * @file   felhasznalo.h
 * @brief  1. Teszteset: Felhasználók.
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/

#ifndef FELHASZNALO_H
#define FELHASZNALO_H
#include <string>
#include <iostream>
#include <fstream>
#include "hashtable.hpp"
using std::string;

class Felhasznalo
{
	string username;
public:
	Felhasznalo(string username = string("")) :username(username) {}
	string getUserName() {
		return username;
	}
};

// Csinál sok szép műveletet a kulccsal, hátha jó titkosítást ér el...
size_t bitShiftHash(const string key, size_t defSize) {
	size_t l = key.length();
	size_t res = 0;
	// Készít egy számot a stringből
	for (size_t i = 0; i < l; ++i) {
		res += (int)key[i] * (i + 128);
		res = (size_t)pow(res, i);
	}
	return res % 33586151 % defSize; // Fun fact: a 33586151 egy prímszám
}

/**
 * Ez a teszt felhasználóneveket és jelszavakat tölt be egy fileból. 
 * A HashTable alapértelmezetten 100 elemű, ezért amikor a 91. felhasználó	bekerül, újra kell hashelődnie a táblának.
 * Ezután a teszt megpróbálja megtalálni az összes felhasználót, ha valamelyiket nem találta hibát dob.
 * A teszt működésének feltétele, hogy a passwords.txt fileban minden jelszó után álljon egy felhasználónév.
 */
void felhasznalo_teszt() {
	HashTable<Felhasznalo, string, bitShiftHash> felhasznalok;
	std::ifstream is;
	is.open("passwords.txt");
	string un;
	string pw;
	if (!is.is_open()) throw std::runtime_error("Nem nyilt meg a file.");
	while (is) {
		is >> pw;	
		is >> un;
		felhasznalok.put(pw, Felhasznalo(un));
	}
	is.close();
	
	felhasznalok.put("TEST1", Felhasznalo("TESZTELEK"));
	
	is.open("passwords.txt");
	if(!is.is_open()) throw std::runtime_error("Nem nyilt meg a file.");
	while (is) {
		is >> pw;
		Felhasznalo* felh = felhasznalok.get(pw);
		if (felh == nullptr) throw std::runtime_error("Valaki elveszett.");
		is >> un;
		if (felh->getUserName() != un) throw std::runtime_error("Megvaltozott a felhasznaloneve?");

	}
	if (felhasznalok.get("TEST1") == nullptr) throw std::runtime_error("Elveszett ELEK.");
	if (felhasznalok.get("TEST1")->getUserName() != "TESZTELEK") throw std::runtime_error("Elveszett ELEK neve.");
}


#endif // !FELHASZNALO_H