#include "hash.h"
#include <iostream>
#include <cmath>
#include <typeinfo>
#include <string>
using namespace std;

hashTable::hashTable(int size)
{
    unsigned int primeSize = getPrime(size);
    data.resize(primeSize);
    capacity = primeSize;
    filled = 0;
    //clear data before hash table is created
    for (int i = 0; i < data.size(); i++)
    {
        data[i].isOccupied = false;
        data[i].isDeleted = false;
    }
}

//Insert values into hash table. Returns: sucesss -> 0, key already exists -> 1, failure -> 2.
int hashTable::insert(const std::string &key, void *pv)
{
	int hashVal = hash(key);
	// search for location of key / search for first avaliable spot
	while (data[hashVal].isOccupied && (data[hashVal].key != key))
    {
		++hashVal %= capacity;
	}
	// if key already exists in hash table, return 1
	if (data[hashVal].key == key)
    {
		return 1;
	}
	// rehash when half the capacity has been reached
	else if (capacity / (capacity-filled) >= 2)
    {
		// if rehashing fails, return 2
		if (rehash() == false)
        {
			return 2;
		}
	}
	// Insert a new key @ data[hashVal]
	data[hashVal].key = key;
	data[hashVal].isOccupied = true;
	data[hashVal].isDeleted = false;
	filled++;
	return 0;
}

//returns true if key exists in hashtable, returns false otherwise
bool hashTable::contains(const std::string &key)
{
    if (findPos(key) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//Given a string, return a numerical hash value
int hashTable::hash(const std::string &key)
{
    long sum = 0, x = 1;
    for (int i = 0; i < key.length(); i++)
    {
        x = (i % 8 == 0) ? 1 : x * 256;
        sum += tolower(key[i]) * x;
    }
    return (int)(abs(sum) % capacity);
}

// search for the key in the hashtable and return position. Return -1 if not found.
int hashTable::findPos(const std::string &key)
{
    // Search for key
    int pos = hash(key);
    while (data[pos].isOccupied || data[pos].isDeleted)
    {
        if (data[pos].key == key)
        {
            return pos;
        }
        ++pos %= capacity;
    }
    return -1;
}


bool hashTable::rehash()
{
	// create temporary vector, and get new data capacity
	int newCap = getPrime(capacity);
	vector<hashItem> tempVector;
	data.swap(tempVector);
	try 
    { 
        data.resize(newCap); 
    }
	catch (std::bad_alloc) 
    {
		cout << "Rehash failed!" << endl;
		return false;
	}
	capacity = newCap;
	filled = 0;
	for (auto i : tempVector) 
    {
		if ( (i.isOccupied == true) && (i.isDeleted == false) ) 
        {
			insert(i.key);
		}
	}
	return true;
}

//return first prime number from the list of primes that is larger than the given capacity
unsigned int hashTable::getPrime(int size)
{
    // large prime numbers to be used for hashing
    int primes[] = { 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741 };
    for (int i : primes)
    {
        if (i > size)
        {
            return i;
        }
    }
    //if i < size, return largest prime number in primes (@ index 14)
    return primes[14];
}