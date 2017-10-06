#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;
namespace cop4530
{

template <typename T>
class HashTable
{
	static const unsigned int default_capacity = 11;
	static const unsigned int max_prime = 1301081;
public:
	HashTable(size_t size = 101);
	~HashTable();
	bool contains (const T& x);
	bool insert (const T& x);
	bool insert (T && x);
	bool remove (const T &x);
	void clear();
	bool load (const char *filename);
	void dump();
	bool write_to_file(const char *filename);
	int getSize();


private:
	vector<std::list<T>> htable;
	int size;
	void makeEmpty();
	void rehash();
	size_t myhash(const T & x);
	unsigned long prime_below(unsigned long);
	void setPrimes(std::vector<unsigned long> &);

};
#include "hashtable.hpp"

}
#endif

