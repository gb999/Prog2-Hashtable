/*****************************************************************
 * @file   HashTable_test.cpp
 * @brief  
 * 
 * @author Pallos Gábor György 
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/

#include <iostream>
#include "memtrace.h"

#include "fixarray.hpp"
#include "linkedlist.hpp"
#include "harray.hpp"
#include "hashtable.hpp"
#include "gtest_lite.h"


// TESTCASE
// 1: FixArray
// 2: LinkedList 
// 3: FixArray<LinkedList>
// 4: HArray
// 5: charCodeHash 
// 6: HashTable
// 7: HArray iterator
// 8: HashTable iterator
// 9: HashTable rehash
// 10: TESZT1: felhasznalok.
// 11: TESZT2: programozasi nyelvek
// 12: TESZT3: az uj neptun

#define TESTCASE 12

#if TESTCASE > 9 
#include "felhasznalo_teszt.h"
#endif
#if TESTCASE > 10
#include "programnyelvek_teszt.h"
#endif
#if TESTCASE > 11
#include "neptun_teszt.h"
#endif

/**
* Segédclass teszteléshez
 */
class TextStruct {
public:
	std::string txt;
	TextStruct(std::string txt = "hello world") :txt(txt) {};
	bool operator==(const TextStruct& rhs) {
		return txt == rhs.txt;
	} 
	bool operator!=(const TextStruct& rhs) {
		return !(*this == rhs);
	}
	TextStruct& operator=(const TextStruct& rhs) {
		txt = rhs.txt;
		return *this;
	}
};
int main() { 
#if TESTCASE > 0 
TEST(FixArray, fixarray_tests) {
	FixArray<int, 10> a;
	EXPECT_EQ(10, a.size());
	a[3] = 340;
	EXPECT_EQ(340, a[3]) << "";

	EXPECT_THROW(a[-1], std::out_of_range);
	EXPECT_THROW(a[10], std::out_of_range);
 } END
#endif
#if TESTCASE > 1
TEST(LinkedList, LinkedList_int) {
	 LinkedList<int> ll;
	 int a = 14;
	 ll.push(a);
	 EXPECT_EQ(a, *(ll.find(14)));
	 EXPECT_FALSE(ll.isEmpty());
	 *ll.find(14) = 15; 
	 EXPECT_EQ(15, *(ll.find(15)));

 } END
TEST(LinkedList, nulltest) {
	
	LinkedList<int> ll;
	const int* a = ll.find(3);
	EXPECT_TRUE(a == nullptr) << "baj van, nem nullptr-t ad vissza" << std::endl;
	EXPECT_TRUE(ll.isEmpty());
 } END
TEST(LinkedList, dinamikusadat) {
	 //Dinamikus adattal
	 LinkedList<TextStruct> txtll;

	 TextStruct b("Hello");
	 TextStruct c("World");
	 txtll.push(b);
	 
	 const TextStruct *hell = txtll.find(TextStruct("Hell"));
	 EXPECT_TRUE(hell == nullptr) << "nincs talalat, de nem nullptrt ad vissza" << std::endl;
	 const TextStruct *hello = txtll.find(TextStruct("Hello"));
	 EXPECT_EQ(std::string("Hello"), (*hello).txt); 
 } END
TEST(LinkedList, remove) {
	 LinkedList<int> ll;
	 ll.push(1);
	 ll.push(2);
	 ll.push(3);
	 ll.push(4);
	 ll.remove(3);
	 const int* a = ll.find(1);
	 EXPECT_EQ(1, *a);
	 ll.remove(3); // Nem kéne bajt okoznia
	 EXPECT_EQ(2, *(ll.find(2)));
	 EXPECT_EQ(4, *(ll.find(4)));
	 EXPECT_EQ(1, *(ll.find(1)));
	 EXPECT_TRUE(nullptr== (ll.find(3)));
 } END
#endif
#if TESTCASE > 2
TEST(FixArray<LinkedList>, fixarray_of_linkedlists) {
	 FixArray<LinkedList<int>, 10> fa;
	 EXPECT_TRUE(fa[0].isEmpty()) << "Uresen hozza letre a listat?" << std::endl;
	 fa[0].push(30);
	 EXPECT_FALSE(fa[0].isEmpty()) << "ures maradt push utan" << std::endl;
	 auto b = fa[0].find(30);
	 EXPECT_EQ(30, *b);
 } END
#endif
#if TESTCASE > 3
TEST(HArray, size) {
	 HArray<int, char, 4> ha;
	 EXPECT_EQ(ha.size(), 0);
	 EXPECT_EQ(ha.capacity(), 4);

	 
	 EXPECT_THROW(ha.add(5, 'a', 1), std::out_of_range); 
	 EXPECT_THROW(ha.add(-1, 'b', 1), std::out_of_range);
	 EXPECT_THROW(ha.remove(-1, 'c'), std::out_of_range);
	 EXPECT_THROW(ha.remove(-1, 'd'), std::out_of_range);
 } END

#endif
#if TESTCASE > 4
TEST(charCodeHash, nehany_kiiratas) {
	 std::cout << "abc " << charCodeHash(std::string("abc"), 26) << std::endl;
	 std::cout << "bcd " << charCodeHash(std::string("bcd"), 26) << std::endl;
	 std::cout << "def " << charCodeHash(std::string("def"), 26) << std::endl;
 } END
#endif
#if TESTCASE > 5 
 TEST(HashTable, tests){
	 HashTable<int, std::string> ht;
	 EXPECT_EQ(0, ht.size());
	 std::string str("Nagy Lajos");
	 ht.put(str, 14);
	 EXPECT_EQ(1, ht.size());
	 int* a = ht.get(str);
	 EXPECT_FALSE(a == nullptr);
	 if(a != nullptr) 
		EXPECT_EQ(14, *a);
	 ht.remove(str);
	 EXPECT_EQ(0, ht.size());

	 size_t cap1 = ht.capacity();
	 std::cout << "kapacitas put elott: "<< cap1 << std::endl;
	 
	 ht.put("belabacsi", 46);
	 int* b = ht["belabacsi"];
	 EXPECT_FALSE(b == nullptr);
	 if(b != nullptr)
		EXPECT_EQ(46, *b);
	 ht.remove("valami ami nincs benne");

	 std::cout << "kapacitas put utan: " << ht.capacity() << std::endl;
	 EXPECT_EQ(cap1 - 1, ht.capacity());
 } END
#endif
#if TESTCASE > 6
TEST(HArray, iterator) {
	 HArray<int> ha;
	 auto it = ha.begin();
	 EXPECT_TRUE(ha.begin() == ha.end());
	 ha.add(0, std::string("a"), 1);
	 it = ha.begin();
	 EXPECT_EQ(1, it->value);
	 ha.add(0, "b", 2);
	 ha.add(0, "c", 3);
	 ha.add(0, "d", 4);
	 EXPECT_EQ(4, (*(ha.begin())).value);
	 std::stringstream ss;
	 for (it = ha.begin(); it != ha.end(); ++it) {
		 ss<< it->key << it->value;
	 }
	 EXPECT_STREQ("d4c3b2a1", ss.str().c_str());

	 EXPECT_NO_THROW(++it);
	 EXPECT_NO_THROW(it++);
	 EXPECT_THROW(*it, std::exception);
	 EXPECT_THROW(it->value, std::exception);
	 EXPECT_THROW(it->key, std::exception);
 } END
#endif
#if TESTCASE > 7
TEST(HashTable, iterator) {
	 HashTable<int> ht;
	 HashTable<int>::iterator it = ht.begin();
	 HashTable<int>::iterator ie = ht.end();
	 EXPECT_TRUE(ht.begin() == ht.end());

	 // Ez a teszt azért működik, mert egy db fix tömbbel indul a hashtable
	 ht.put("a", 1);
	 ht.put("b", 2);
	 ht.put("c", 3);
	 ht.put("d", 4);
	 std::stringstream ss;
	 for (it = ht.begin(); it != ht.end(); ++it) {
		 ss << it->key << it->value;
	 }
	 EXPECT_STREQ("d4c3b2a1", ss.str().c_str());

 } END
#endif
#if TESTCASE > 8
TEST(HashTable, rehash) {
	 HashTable<int, int, linHash, 3> ht;  
	 ht.put(0, 0);
	 ht.put(1, 1);
	 ht.put(2, 2);

	 //Megszámolja az elemeket
	 int c = 0;
	 for (auto it = ht.begin(); it != ht.end(); ++it) {
		 c++;
	 }
	 EXPECT_EQ(3, c);

	 ht.put(3, 3); // itt kell újrahashelődnie a táblának 
	 // Az összes elemnek meg kell lennie
	 EXPECT_FALSE(ht.get(0) == nullptr);
	 EXPECT_FALSE(ht.get(1) == nullptr);
	 EXPECT_FALSE(ht.get(2) == nullptr);
	 EXPECT_FALSE(ht.get(3) == nullptr);

	 std::stringstream ss;
	 for (auto it = ht.begin(); it != ht.end(); ++it) {
		 ss << it->value;
	 }
	 //Sorban kell lenniük a linHash függvény miatt
	 EXPECT_STREQ("0123", ss.str().c_str());

 } END

#endif
#if TESTCASE > 9 
TEST(TEST1, felhasznalok) {
	 EXPECT_NO_THROW(felhasznalo_teszt());

 }END
#endif
#if TESTCASE > 10 
TEST(TEST2, programnyelvek) {
	 EXPECT_NO_THROW(programnyelvek_teszt());
 } END
#endif
#if TESTCASE > 11 
TEST(TEST3, AzUjNeptun) {
	 EXPECT_NO_THROW(neptun_teszt());
 } END
#endif


	 return 0;
}

