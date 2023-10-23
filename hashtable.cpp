/*****************************************************************
 * @file   hashtable.cpp
 * @brief  HashTable nem template függvényeinek megvalósítása
 * 
 * @author Pallos Gábor György
 * @neptun QN1SXN
 * @date   April 2023
 *********************************************************************/
#include "hashtable.hpp"

size_t charCodeHash(const std::string key, const size_t maxSize)
{
    size_t res = 0;
    size_t l = key.length();
    for (size_t i = 0; i < l; ++i) {
        res += (key[i] + i) * i * l ;
    }
    res %= maxSize;
    return res;
}

size_t linHash(const int a, const size_t maxSize) {
    return a % maxSize;
}
