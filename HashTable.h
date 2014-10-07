//DO NOT CHANGE THIS FILE
//Author: Bo Brinkman
//Date: 2013/07/24
#include "USet.h"
//use hashprimes array constants 
/*
 * Note: Just above your template declaration when you use this class, you
 * must define method called "hash" that takes a Key as input, and returns
 * an unsigned long (which is the hash value)
 *
 * For example, you might do:
 * unsigned long hash(char c){ return 10*((unsigned long)c)%backingArraySize; }
 * HashTable<char,int> mySillyTable;
 *
 * If you don't define an appropriate hash function, the class won't compile.
 */

template <class Key, class T>
class HashTable : public USet <Key, T> {
 private:
  class HashRecord {
  public:
    Key k;
    T x;

    //If the slot in the hash table is totally empty, set this to true.
    bool isNull;

    //If the slot used to have something in it, but doesn't now, set
    // isDel to true, and isNull to false. isNull is only for slots
    // that have never been used
    bool isDel;

    HashRecord() { isNull = true; isDel = false; };
  };

 public:
  //See USet.h for documentation of these methods
  virtual unsigned long size();
  virtual void add(Key k, T x);
  virtual void remove(Key k);
  virtual T find(Key k);
  virtual bool keyExists(Key k);

  //Initialize all private member variables.
  HashTable();
  //Delete any dynamically allocated memory.
  virtual ~HashTable();

private:
  //A pointer to the array that holds the hash table data
  HashRecord* backingArray;

  //Whenever numItems + numRemoved >= backingArraySize/2, call
  // grow(). grow() should make a new backing array that is twice the
  // size of the old one, similar to what we did in the ArrayQueue
  // lab.
  //Note: You cannot just loop through the old array and copy it to the
  // new one! Since the backing array size has changed, each item will likely
  // map to a different slot in the array. You may just want to use add()
  // after initializing the new array.
  void grow();

  //This helper method should take a key, and return the index for that
  // item within the hash table. If the item already exists, return the
  // index of the existing item. If the item doesn't exist, return the index
  // where it OUGHT to be. This function can then be used as a helper method in
  // your other methods.
  unsigned long calcIndex(Key k);

  unsigned long numItems; //Number of items in the hash table
  unsigned long primeNum;
  //Note: Ordinarily, these OUGHT to be private. In this case I have
  // made them public for easy of testing.
 public:
  unsigned long numRemoved; //Number of slots that have been removed but not re-used. Those that have isDel == true
  unsigned long backingArraySize;
};



//You will need this so you can make a string to throw in
// remove
#include <string>

template <class Key, class T>
HashTable<Key,T>::HashTable(){
  numItems=0;
  numRemoved=0;
  primeNum=0;
  backingArraySize= hashPrimes[primeNum];
  backingArray = new HashRecord[backingArraySize];


}

template <class Key, class T>
HashTable<Key,T>::~HashTable() {
  //destructor - delete everything 
}

template <class Key, class T>
unsigned long HashTable<Key,T>::calcIndex(Key k){
	return hash(k) % backingArraySize;
	//basic idea - >This indicates failure, since it is an impossible value
	//find starting index. linear probing
}

template <class Key, class T>
void HashTable<Key,T>::add(Key k, T x){
  //check for numremoved- overwrite numitems+numremoved < backingarraysize
	//check to see if need to grow

	if((numRemoved + numItems)+1 > backingArraySize)
	{ grow(); }

	int start= calcIndex(k);
	while(!backingArray[start].isDel && !backingArray[start].isNull)
	{ start = (start+1) % backingArraySize; }
	if(backingArray[start].isNull || backingArray[start].isDel)
	{ 
				if(backingArray[start].isNull)
				{ numItems++; }
				else
				{numRemoved--; }

				backingArray[start].k=k;
				backingArray[start].x=x;
				backingArray[start].isNull =false;
				backingArray[start].isDel=false;
	}
	while(!backingArray[start].isDel && !backingArray[start].isNull)
	{ start = (start+1) % backingArraySize; }

}

template <class Key, class T>
void HashTable<Key,T>::remove(Key k){
  //don't delete anything here
 //flag things as isDel. so make isDel true
//3 possibilities - isNull, isDel
	int index= calcIndex(k);
	if(keyExists(k))
	{
		backingArray[index].isDel = true;
		numItems--;
		numRemoved++;
	}
}

template <class Key, class T>
T HashTable<Key,T>::find(Key k){
  int index=calcIndex(k);
  while(!backingArray[index].isNull  && !backingArray[index].isDel)
  {
	  if(backingArray[index].k==k)
	  { return true;}
	  index = (index+1)%backingArraySize;
  }
}

template <class Key, class T>
bool HashTable<Key,T>::keyExists(Key k){
  int index=calcIndex(k);
  while(!backingArray[index].isNull && !backingArray[index].isDel)
  {
	if(backingArray[index].k==k)
	 { return true;}
	index=(index+1)%backingArraySize;
  }
}

template <class Key, class T>
unsigned long HashTable<Key,T>::size(){
	return numItems;
}

template <class Key, class T>
void HashTable<Key,T>::grow(){
  //TODO check if numRemoved is greater than 0; 
	//making new backing array- copying 
	//delete old array
	//numItems + numRemoved >= backingArraySize/2

	primeNum++;
	unsigned long doubleSize = hashPrimes[primeNum];
	HashRecord* newArr = new HashRecord[doubleSize];

	for(unsigned long i=0; i<backingArraySize; i++)
	{
		if(!backingArray[i].isDel && !backingArray[i].isNull)
		{
			int objIndex=calcIndex(backingArray[i].k);
			while(!newArr[objIndex].isNull)
			{objIndex = (objIndex+1)%doubleSize; }
			newArr[objIndex].k=backingArray[i].k;
			newArr[objIndex].x=backingArray[i].x;
			newArr[objIndex].isDel=false;
			newArr[objIndex].isNull=false;
		}

	}
	backingArraySize= doubleSize;
	delete[] backingArray;
	backingArray=newArr;
	numRemoved=0;
}
