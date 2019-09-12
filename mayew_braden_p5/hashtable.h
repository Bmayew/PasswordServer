//**********************************************************************************************//
// Braden Mayew                                                                                 //
// COP 4530                                                                                     //
// Project 5 - Hash Tables and its Applications													//
// Contains HashTable class template declaration                                                //
// --------------------------------------- hashtable.h ---------------------------------------- //
//**********************************************************************************************//
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include<vector>
#include<list>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<functional>
#include<cctype>

namespace cop4530 {
	// K and V are template parameters (generic data types), which represent the key and value types 
	// for a table entry, respectively
	template<typename K, typename V>
	class HashTable {
		public:
			// Constructor -
			explicit HashTable(size_t size = 101);

			// Destructor -
			~HashTable();

			// See if pair kv is in the hash table -
			bool match(const std::pair<K, V> &kv);

			// Check if key k is in the hash table -
			bool contains(const K & k) const;

			// add kv pair into hash table (copy) -
			bool insert(const std::pair<K, V> & kv);

			// add kv pair into hash table (move) -
			bool insert(std::pair<K, V> && kv);

			// Clear function for destructor, clears hash table, -
			// calls private interface makeEmpty()
			void clear(); 

			// return index of vector entry where k should be stored -
			size_t myhash(const K &k) const;

			// show all hash table entries -
			void dump() const;

			// delete key k and corresponding value if it is in the hashtable
			bool remove(const K &k);

			// write all elements to a file
			bool write_to_file(const char *filename) const;

			// load file contents to the hashtable
			bool load(const char *filename);

			// return the currentSize of the hashtable
			int getSize() const;

			// setSize used for adapter class to set table size to passed value
			void setSize(size_t);

			bool updatePassword_Check(const std::pair<K, V> &kv, const std::string password) const;

		private:
			// array of linked lists
			std::vector< std::list< std::pair<K,V> > > Vector;

			// size of hash table
			int currentSize;

			// when number of elements in the hash table is > vector.size()
			void rehash();

			// Delete all elements in the hash table 
			// The public interface clear() will call this function
			void makeEmpty();

			// determine the proper underlying vector size
			unsigned long prime_below( unsigned long );

			// set the calculated size the current hash table
			void setPrimes( std::vector<unsigned long> & );

			// max_prime is used by the helpful functions provided
			// to you.
			static const unsigned int max_prime = 1301081;
			// the default_capacity is used if the initial capacity 
			// of the underlying vector of the hash table is zero. 
			static const unsigned int default_capacity = 11;
	};

#include "hashtable.hpp"
}
#endif
 

