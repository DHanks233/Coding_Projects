//-----------------------------------------------------------------------------
// Filename:    LListQueue.h
// Classes:     LListQueue
//
// Methods:     LListQueue() Default Constructor
//              LListQueue() Constructor #2
//              isEmpty()
//              enqueue()
//              dequeue()
//              peek()
//-----------------------------------------------------------------------------

#ifndef LLISTQUEUE_H
#define LLISTQUEUE_H
#include "Node.h"
#include "SLinkedList.h"

//-----------------------------------------------------------------------------
// LListQueue Template Class
// This is a template class which inherits publicly from SLinkedList<T>
// template class. It adds all the operations you would expect for a queue to
// a linked implementation, so it can be used without knowledge of how the list
// is stored
//
// An LListQueue object contains no private data members, only public
// operations
//
// Implementation and assumptions:
//   -- After a LListQueue object is created, Nodes are added by value passed 
//      using enqueue() and removed from the front of the list using dequeue()
//   -- enqueue() puts Nodes at the end of the list, dequeue() removes from the
//      front of the list
//   -- peek() returns the first item in the list, or throws an out_of_range 
//      exception if the list is empty
//
// Written by:      Daniel P. Hanks (ID 0024879)
//-----------------------------------------------------------------------------

template<class T>
class LListQueue : public SLinkedList<T>
{
    public:

        //---Default Constructor-----------------------------------------------
        // Pre:  None
        // Post: LListQueue obejct is created with an empty list
        //---------------------------------------------------------------------
        LListQueue();

        //---Constructor w/ Data Parameter-------------------------------------
        // Pre:  None
        // Post: LListQueue obejct is created with a list of one, containing
        //       the data object passed in
        //
        // Parameter:
        //      const T&: The object or data value to used in head Node
        //---------------------------------------------------------------------
        LListQueue(const T& );

        //---enqueue-----------------------------------------------------------
        // Pre:  None
        // Post: The item passed in as parameter is added to the end of the 
        //       list, or at the front if the list is empty
        //
        // Returns: A bool that is true if added successfully, false otherwise
        // Parameters:
        //      const T&: The object or data value to add to queue
        //---------------------------------------------------------------------
        bool enqueue(const T&);

        //---dequeue-----------------------------------------------------------
        // Pre:  The list contains at least one item
        // Post: The item at the front of the list is removed
        //
        // Returns: A bool that is true if list is not empty, false otherwise
        //---------------------------------------------------------------------
        bool dequeue();

        //---peek--------------------------------------------------------------
        // Pre:  The list contains at least one item
        // Post: None
        //
        // Returns: An item of type T, the first item in the list
        // THROWS:  An out_of_range exception if the list is empty
        //---------------------------------------------------------------------
        T peek() const;
};

#include "LListQueue.cpp"
#endif