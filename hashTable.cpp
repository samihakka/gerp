/*
 * Sam Miller & Sami Hakkarainen
 * 12/06/2022
 * 
 * Purpose - 
 */

#include <iostream>
#include <string>
#include <functional> 

using namespace std;

const int HASH_VALUE = 10000;


/*
 * name:      buildHashTable()
 * purpose:   TO DO
 * arguments: TO DO
 * returns:   TO DO
 * effects:   TO DO
 */
void buildHashTable() {


    //max number of keys: 171,200
    // 1.3 * maxWords = 222,560
    // prime number near this is 222561


    // fill hash table of this value with -1

    std::string dictionary[HASH_VALUE];

    for (int i = 0; i < HASH_VALUE; i++) {
        
        dictionary[i] = "-1";
    }

    std::string key = "blah";
    size_t slot = hash<std::string>()(key) % HASH_VALUE;

    std::cout << slot;

}