//constructor, initialize hashtable
template <typename T>
HashTable<T>::HashTable(size_t size): size(0)
{
	htable.resize(prime_below(size));
}
//destructor
template <typename T>
HashTable<T>::~HashTable()
{
	clear();
}

//check if a value is in hashtable
template <typename T>
bool HashTable<T>::contains(const T& x)
{
	auto & whichList = htable[myhash(x)];
	return find (begin(whichList), end (whichList), x) != end (whichList);
}

//insert a value into the hashtable
template <typename T>
bool HashTable<T>::insert(const T& x)
{
	auto & whichList = htable[myhash(x)];
	if (find(begin(whichList), end(whichList), x) != end(whichList))
		return false;
	whichList.push_back(x);
	if (++size > htable.size())
		rehash();
	return true;
}

//move version
template <typename T>
bool HashTable<T>::insert (T&&x)
{
	auto & whichList = htable[myhash(x)];
	if (find (begin (whichList), end (whichList), x) != end(whichList))
		return false;
	whichList.push_back(std::move(x));
	if (++size > htable.size())
		rehash();
	return true;
}

//remove a value from hashtable
template <typename T>
bool HashTable<T>::remove(const T& x)
{
	auto & whichList = htable[myhash(x)];
	auto itr = find (begin (whichList), end(whichList), x);
	if (itr == end(whichList))
		return false;
	whichList.erase(itr);
	--size;
	return true; 
}

//clear hashtable
template <typename T>
void HashTable<T>::clear()
{
	makeEmpty();
}


//load file to hashtable
template <typename T>
bool HashTable<T>::load (const char *filename)
{
	ifstream ifile(filename);
	string line;
	while (getline(ifile, line))
	{
		T x;
		istringstream is(line);
		is>>x;
		insert(x);
	}
}

//print hashtable
template <typename T>
void HashTable<T>::dump()
{
	if (size!= 0)
	{
		for (auto & whichList:htable)
			for (auto itr = whichList.begin(); itr != whichList.end(); ++itr)
				cout  << *itr << endl;
	}
	
}

//write to hashtable
template <typename T>
bool HashTable<T>::write_to_file(const char *filename)
{
	//change buffer stream, call dump, reset back to standard input
	ofstream out;
	out.open(filename);
	streambuf *old = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	dump();
	cout.rdbuf(old);
	out.close(); 
	return true;
}
//simple getsize function
template <typename T>
int HashTable<T>::getSize()
{	return size;	}

//private function called by clear
template <typename T>
void HashTable<T>::makeEmpty()
{
	for (auto & thisList: htable)
	{
		thisList.clear();
	}
}

//rehash function
template <typename T>
void HashTable<T>::rehash()
{
	vector<list<T>> temp = htable;
	
	htable.resize(prime_below(2*htable.size()));
	for (auto & thisList: htable)
		thisList.clear();

	size = 0;
	for (auto & thisList : temp)
		for (auto & x : thisList)
			insert(std::move(x));
}


template <typename T>
size_t HashTable<T>::myhash(const T& x)
{
	static hash <T> temp;
	return temp(x) % htable.size();
}
// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename T>
unsigned long HashTable<T>::prime_below (unsigned long n)
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
template <typename T>
void HashTable<T>::setPrimes(std::vector<unsigned long>& vprimes)
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

