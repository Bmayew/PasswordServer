//**********************************************************************************************//
// Braden Mayew                                                                                 //
// COP 4530                                                                                     //
// Project 5 - Hash Tables and its Applications                                                 //
// Contains the implementation of member functions for hashtable.h                              //
// -------------------------------------- hashtable.hpp --------------------------------------- //
//**********************************************************************************************//

// Constructor
template<typename K, typename V>
HashTable<K,V>::HashTable(size_t size)
{
  // set the vector size to prime_below(size)
  Vector.resize(prime_below(size));
  // set current hash size
  currentSize = 0;
}

//Destructor
template<typename K, typename V>
HashTable<K,V>::~HashTable()
{
  clear();
}

template<typename K, typename V>
int HashTable<K,V>::getSize() const{
  return currentSize;
}

// set size of hashtable object with adapter class
template<typename K, typename V>
void HashTable<K,V>::setSize(size_t size)  {
  if (getSize() != size)  {
    Vector.resize(prime_below(size));
    currentSize = 0;
  }
}

//clear (public)
template<typename K, typename V>
void HashTable<K,V>::clear()
{
  makeEmpty();
} 

// makeEmpty (private)
template<typename K, typename V>
void HashTable<K,V>::makeEmpty()
{
  // iterates through the vector and deletes each elements (each linked list)
  for (int i = 0; i < Vector.size(); ++i) {
    Vector[i].clear();
  }
} 

// resize the hash table. called when the number of elements in the 
// hash table is greater than the size of the vector 
template<typename K, typename V>
void HashTable<K, V>::rehash() {
  // need a calling object
  auto newVector = Vector;
  Vector.resize(prime_below(2 * Vector.size()));
  clear();
  currentSize = 0;
  for (auto & itr : newVector) {
    for (auto & kv : itr) {insert(kv);}
  }
}

// (copy) insert operation 
template<typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv) {
  // if the pair is in the hash table, don't insert
  if (match(kv)) {
    return false;
  }
  
  // if the key is in the hash table, but has a different value, update the value to the new one with kv
  else if (contains(kv.first)) {
    //std::cout << "Updating the pair with new value.\n ";
    auto & matchedList = Vector[myhash(kv.first)];
    // create iterator, start at first node
    for (auto listItr = matchedList.begin(); listItr != matchedList.end(); ++listItr) {
      if (listItr->first == kv.first) { // same key?
          // erase the pair
          auto newNode = matchedList.erase(listItr);  // set newNode to empty value at correct Itr spot
          matchedList.insert(newNode, kv);
          return true;
      }
    }
  }

  else {
    currentSize++;
    if (Vector.size() < currentSize) {
      rehash();
      std::cout << "resizing the hashtable...\n";
    }
    Vector[myhash(kv.first)].push_back(kv);
    return true;
  }
}

// remove the node's data (pair) if the key is present in the hashtable
template<typename K, typename V>
bool HashTable<K, V>::remove(const K &k) {
  if (contains(k)) {
    auto & matchedList = Vector[myhash(k)];
    for (auto itr = matchedList.begin(); itr != matchedList.end(); ++itr) {
      if (k == itr->first) {
        matchedList.erase(itr);
        currentSize = currentSize - 1;
        //std::cout << "Node and corresponding pair deleted.\n";
        return true;
      }
    }
  }
  else return false;
}

// (move) version of insert operation 
template<typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> && kv) {
    std::pair<K, V> newPair = std::move(kv);
    insert(newPair);  
    return true; 
}

// Check if pair kv is in the hash table
template<typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv)
{
  // std::find -> #include <algorithm>
  // find returns itr to the first element in the range == to kv
  // If no elements match, the function returns last.
  // find requires a type that supports the equals operation
  auto & matchedList = Vector[myhash(kv.first)];
  return find(begin(matchedList), end(matchedList), kv) != end(matchedList);

}

// Check if key k is in the hash table
template<typename K, typename V>
bool HashTable<K, V>::contains(const K & k) const
{
  // go to linked list pull out the pair and get the key
  auto & matchedList = Vector[myhash(k)];
  for (auto itr = matchedList.begin(); itr != matchedList.end(); ++itr) {
    if (itr->first == k) {
      return true;  // the key is the same 
    }
  }
  return false; // if no key match was found
}

// Check if value is in the hash table based on key
template<typename K, typename V>
bool HashTable<K, V>::updatePassword_Check(const std::pair<K, V> &kv, const std::string password) const
{
  if (kv.second == password) {
    return true;
  }
}

// return the index of the vector entry where k should be stored 
template<typename K, typename V>
size_t HashTable<K, V>::myhash(const K & k) const {
  static std::hash<K> hIdx;  // using the hash class defined in std 
  return hIdx(k) % Vector.size();

}

// display all entries in the hash table 
template<typename K, typename V>
void HashTable<K, V>::dump() const {
  for (int i = 0; i < Vector.size(); i++) {
    std::cout << "Vector[" << i << "]= ";
    for (auto itr = Vector[i].begin(); itr != Vector[i].end(); ++itr) {
      if (itr == Vector[i].begin()) {
         std::cout << itr->first << " " << itr->second;
      }
      else {
        std::cout << " : "; // add semicolon for consecutive pairs only after the first pair has been printed
        std::cout << itr->first << " " << itr->second;
      }
    }
    std::cout << std::endl;
  }
}

// write all elements to a file
template<typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const{
  std::ofstream outputFile;
  outputFile.open(filename);
  if (filename) {
    for (int i = 0; i < Vector.size(); i++) {
      for (auto itr = Vector[i].begin(); itr != Vector[i].end(); ++itr) {
        outputFile << itr->first << " " << itr->second << "\n";
      }
    }
  }
  else {
    return false;
  }
  return true;
}

// load file contents to a hash table
template<typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
  // load the content of filename into hashtable. 
  // each line: single pair of key and value
  // read lines from file     
  std::ifstream inputFile;
  std::pair<K, V> kv;
  inputFile.open(filename);
  
  if (inputFile) {
    while (!inputFile.eof()) {
      // acquire k and v
      std::getline(inputFile, kv.first, ' ');
      //std::cout << "the key is:" << kv.first << std::endl;

      std::getline(inputFile, kv.second);
      //std::cout << "the value is:" << kv.second << std::endl;
      insert(kv);
    }
  }

  else {
    return false;
  }
  return true;
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
    std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
  return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

