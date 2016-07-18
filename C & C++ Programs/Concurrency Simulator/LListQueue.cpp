//-----------------------------------------------------------------------------
// Filename:    LListQueue.cpp
// Classes:     LListQueue
//
// Methods:     LListQueue() Default Constructor
//              LListQueue() Constructor #2
//              isEmpty()
//              enqueue()
//              dequeue()
//              peek()
//-----------------------------------------------------------------------------
#include "LListQueue.h"
#include <cstddef>
#include <iostream>

template<class T>
//---Default Constructor-----------------------------------------------
// Pre:  None
// Post: LListQueue obejct is created with an empty list
//---------------------------------------------------------------------
LListQueue<T>::LListQueue()
{
}

//---Constructor w/ Data Parameter-------------------------------------
// Pre:  None
// Post: LListQueue obejct is created with a list of one, containing
//       the data object passed in
//
// Parameter:
//      const T&: The object or data value to used in head Node
//---------------------------------------------------------------------
template<class T>
LListQueue<T>::LListQueue(const T& newItem)
{
    SLinkedList<T>::addToEnd(newItem);
}

//---enqueue-----------------------------------------------------------
// Pre:  None
// Post: The item passed in as parameter is added to the end of the 
//       list, or at the front if the list is empty
//
// Returns: A bool that is true if added successfully, false otherwise
// Parameters:
//      const T&: The object or data value to add to queue
//---------------------------------------------------------------------
template<class T>
bool LListQueue<T>::enqueue(const T& newItem)
{
    return (SLinkedList<T>::addToEnd(newItem));
}

//---dequeue-----------------------------------------------------------
// Pre:  The list contains at least one item
// Post: The item at the front of the list is removed
//
// Returns: A bool that is true if list is not empty, false otherwise
//---------------------------------------------------------------------
template<class T>
bool LListQueue<T>::dequeue()
{
    return (SLinkedList<T>::popFront());
    
}

//---peek--------------------------------------------------------------
// Pre:  The list contains at least one item
// Post: None
//
// Returns: An item of type T, the first item in the list
// THROWS:  An out_of_range exception if the list is empty
//---------------------------------------------------------------------
template<class T>
T LListQueue<T>::peek() const
{
    return SLinkedList<T>::getItemAt(1);
}