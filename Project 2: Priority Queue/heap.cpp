#include "heap.h"
#include <iostream>
using namespace std;

//initialize heap
heap::heap(int capacity)
{
    data.resize(capacity + 1);
    hashtable = new hashTable(capacity * 2);
    this->capacity = capacity;
    size = 0;
}

//insert node into the tree at the bottom. Fix any violations by percolating up
int heap::insert(const string &id, int key, void *pv)
{
    //if heap is filled to capacity
    if (size == capacity)
    {
        return 1;
    }
    if (hashtable->contains(id) == false)
    {
        size++;
        data[size].id = id;
        data[size].key = key;
        data[size].pData = pv;
        hashtable->insert(id, &data[size]);
        percolateUp(size);
    }
    //if node with the given id already exists & heap is not filled to capacity
    else
    {
        return 2;
    }
    return 0;
}

// set the key of the specificed node to the specified value
// return 0 on success and 1 on failure
int heap::setKey(const string &id, int key)
{
    if (!(hashtable->contains(id)))
    {
        return 1;
    }
    node *px = static_cast<node *>(hashtable->getPointer(id));
    //get position of node in data vector
    int pos = px - &data[0];
    int oldVal = px->key;
    px->key = key;
    if (key > oldVal)
    {
        percolateDown(pos);
    }
    else if (key < oldVal)
    {
        percolateUp(pos);
    }
    return 0;
}

//delete min value, which is at the root. Place last node in the data vector at the root.
//fix any violations by percolating down
int heap::deleteMin(string *pId, int *pKey, void *ppData)
{
    if (size == 0)
    {
        return 1;
    }
    hashtable->remove(data[1].id);
    if (pId != NULL)
    {
        *pId = data[1].id;
    }
    if (pKey != NULL)
    {
        *pKey = data[1].key;
    }
    if (ppData != NULL)
    {
        *(static_cast<void **>(ppData)) = data[1].pData;
    }
    data[1] = data[size--];
    percolateDown(1);
    return 0;
}

//set the id of the key in question to be a really small value (chosen to be -100000000)
//the key will percolate up to the root, and deletemin() will delete it
int heap::remove(const string &id, int *pKey, void *ppData)
{
    if (!(hashtable->contains(id)))
    {
        return 1;
    }
    node *px = static_cast<node *>(hashtable->getPointer(id));
    if (ppData != NULL)
    {
        *(static_cast<void **>(ppData)) = px->pData;
    }
    if (pKey != NULL)
    {
        *pKey = px->key;
    }
    setKey(id, -100000000);
    deleteMin();
    return 0;
}

//percolate up function
//move key up in tree to fix any violations
void heap::percolateUp(int index)
{
    node temp = data[index];
    //while the index is not the root and the index's parent's value is greater
    //data at index/2 is the location of data[index]'s parent
    for (; index > 1 && temp.key < data[index/2].key; index /= 2)
    {
        data[index] = data[index/2];
        hashtable->setPointer(data[index].id, &data[index]);
    }
    data[index] = temp;
    hashtable->setPointer(data[index].id, &data[index]);
}

//percolate down 
//move key down in tree to fix any violations
void heap::percolateDown(int index)
{
    node temp = data[index];
    int child;
    //while the index has children
    for (; index * 2 <= size; index = child)
    {
        //data at index*2 is the locaton of the index's child to the left
        child = index * 2;
        //determine which child has the lower value
        if (child != size && data[child + 1].key < data[child].key)
        {
            child++;
        }
        if (data[child].key < temp.key)
        {
            data[index] = data[child];
            hashtable->setPointer(data[index].id, &data[index]);
        }
        else
        {
            break;
        }
    }
    data[index] = temp;
    hashtable->setPointer(data[index].id, &data[index]);
}