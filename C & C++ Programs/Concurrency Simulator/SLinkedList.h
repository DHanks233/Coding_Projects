//-----------------------------------------------------------------------------
// Filename:    SLinkedList.h
// Classes:     SLinkedList
//
// Methods:     SLinkedList() Default Constructor
//              SLinkedList(const SLinkedList<T>& ) Copy Constructor
//              ~SLinkedList()
//              isEmpty()
//              getItemCount()
//              getItemAt()
//              clear()
//              addToFront()
//              addToBack()
//              remove()
//              mergeSort()
//              printItems()
//              contains()
//              setTempList()
//-----------------------------------------------------------------------------
#ifndef SLINKEDLIST_H
#define SLINKEDLIST_H
#include "Node.h"

//-----------------------------------------------------------------------------
// SLinkedList Template Class:
// This is a template class which uses the Node template class to create a 
// singly-linked list of Node objects. It contains public operations to observe
// the data in the list or clear all Nodes in the list. All methods to that add
// or remove Nodes are protected. The idea of SLinkedList is to be the super
// class for any data structure class wanting to use a linked implementation,
// ie: a stack, queue, priority queue classes
//
// An SLinkedList object contains two private data members:
//
//      Node<T>* headNode: A pointer to the first Node in the linked list
//
//      int itemCount:     A count of the number of Node objects contained in
//                         the linked list
//
// Implementation and assumptions:
//   -- After a SLinkedList object is created, Nodes are added or removed by 
//      value passed using add() and remove() methods
//   -- The add() method adds a Node containing the parameter data to the end
//      of the linked list
//   -- clear() deletes the entire linked list and is called by the destructor
//   -- mergeSort() sorts the Nodes into ascending order, assuming the data
//      items of each Node can be compared using < and > operators
//   -- Output using printItems() currently outputs to a .txt file
//
// Written by:      Daniel P. Hanks (ID 0024879)
//-----------------------------------------------------------------------------
template<class T>
class SLinkedList
{
    private:
        //Pointer to first Node in the linked list or NULL
        Node<T> * headNode;

        //Pointer to last Node in the linked list
        Node<T> * tailNode;

        //Count representing the number of Nodes in the linked list
        int itemCount;

    protected:
        
        //---addToFront--------------------------------------------------------
        // Pre:  None
        // Post: Adds a new Node to the front of the linked list containing
        //       the data passed as parameter
        //
        // Returns: If the memory could be allocated and new Node is created,
        //          false otherwise
        // Parameter:
        //      const T&: The item to be added to the list
        //---------------------------------------------------------------------
        bool addToFront(const T& );

        //---addToEnd----------------------------------------------------------
        // Pre:  None
        // Post: Adds a new Node to the end of the linked list containing
        //       the data passed as parameter
        //
        // Returns: If the memory could be allocated and new Node is created,
        //          false otherwise
        // Parameter:
        //      const T&: The item to be added to the list
        //---------------------------------------------------------------------
        bool addToEnd(const T& );

        //---removeItem--------------------------------------------------------
        // Pre:  Linked list contains the item to be removed
        // Post: Deletes the Node containing the data passed in as parameter
        //       and adjust the previous Node to point to the new next Node
        //
        // Returns: A bool that is true is the Node is found and deleted, false
        //          if the list is empty or the data is not found in the list
        // Parameter:
        //      const T&: The item to be removed from the list, if present
        //---------------------------------------------------------------------
        bool removeItem(const T& );

        //---popFront----------------------------------------------------------
        // Pre: The linked list is not empty
        // Post: The first item in the linked list, headNode, is deleted
        //
        // Returns: A bool that is false if list is empty, true if it has at
        //          least one Node that can be deleted
        //---------------------------------------------------------------------
        bool popFront();

        //---contains----------------------------------------------------------
        // Pre:  The list is not empty and contains the item searched for
        // Post: None
        //
        // Returns: A Node * containing a pointer to the Node containing data
        //          equal to the parameter, or NULL if the data is not found
        //          in the list
        //
        // Parameter:
        //      const T&: Reference to the item being searched for in the list
        //---------------------------------------------------------------------
        Node<T> * contains(const T& ) const;

    public:

        //---Default Constructor-----------------------------------------------
        // Pre:  None
        // Post: SLinkedList object is created with headNode set to NULL and
        //       itemCount set to 0
        //---------------------------------------------------------------------
        SLinkedList();

        //---Copy Constructor--------------------------------------------------
        // Pre:  None
        // Post: SLinkedList object is created with headNode set to NULL and
        //       itemCount set to 0
        //
        // Parameter:
        //      const SLinkedList<T>&: A reference to the object to be copied
        //---------------------------------------------------------------------
        SLinkedList(const SLinkedList<T>& );

        //---Destructor--------------------------------------------------------
        // Pre:  None
        // Post: The linked list is deleted and headNode is set to NULL
        //---------------------------------------------------------------------
        ~SLinkedList();

        //---Assignment Operator Overload--------------------------------------
        // Pre: None
        // Post: The linked list is copied from the SLinkedList object
        //       parameter, as is itemCount.
        //
        // Returns: The SLinkedList object to be copied
        // Parameters:
        //      const SLinkedList<T>&: A reference to the object to be copied
        //---------------------------------------------------------------------
        SLinkedList<T>& operator=(const SLinkedList<T> & );

        //---isEmpty-----------------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: A bool that is true if itemCount is 0, false otherwise
        //---------------------------------------------------------------------
        bool isEmpty() const;

        //---getItemCount------------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: The value of itemCount, representing number of Nodes in
        //          the linked list
        //---------------------------------------------------------------------
        int getItemCount() const;

        //---getItemAt---------------------------------------------------------
        // Pre: The list is not empty, and the position provided as parameter
        //      is in valid range.
        // Post: None
        // 
        // Parameter: An int representing the position of the Node in the list.
        //            A position of 1 is the first item in the list.
        // Returns: T type representing data stored in the Node at position
        //          provided as parameter.
        //
        // THROWS:  out_of_range exception if the position to get is higher
        //          than the number of items in the list, or list is empty
        //---------------------------------------------------------------------
        T getItemAt(int) const;

        //---clear-------------------------------------------------------------
        // Pre:  None
        // Post: All Node objects in the linked list are deleted and headNode
        //       is set to NULL
        //
        // Returns: bool that is true if itemCount reaches 0, false otherwise
        //---------------------------------------------------------------------
        bool clear();

        //---printItems--------------------------------------------------------
        // Pre:  None
        // Post: An output file named "output.txt" is created and receives
        //       output from the SLinkedList object in the format of the data
        //       item of each Node printed on a separate line
        //---------------------------------------------------------------------
        void printItems() const;

};

#include "SLinkedList.cpp"
#endif