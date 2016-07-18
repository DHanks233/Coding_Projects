//-----------------------------------------------------------------------------
// Filename:    Node.h
// Classes:     Node
//
// Methods:     Node() Default Constructor
//              Node(const T&) Constructor #2
//              Node(const T&, Node<T>*) Constructor #3
//              Node(const Node&) Copy Constructor
//              ~Node()
//              setNext()
//              getNext()
//              setItem()
//              getItem()
//-----------------------------------------------------------------------------
#ifndef NODE_H
#define NODE_H

//-----------------------------------------------------------------------------
// Node Template Class:
// This is a template class which creates singly-linked nodes usable in a 
// linked list, and provides operations to get and assign both the data stored
// and the pointer to the next Node object.
//
// An SecurityList object contains two private data members:
//
//      T theItem:      The data item of the Node of type specified by user at
//                      creation time
//
//      Node<T>* nextNode: A pointer to the next Node in the linked list
//                         Set to NULL by default
//
// Implementation and assumptions:
//   -- 3 Constructors and a Copy constructor are implemented, and all set
//      nextNode to NULL unless the user provides a Node pointer explicitly to
//      act as nextNode.
//   -- The assignment operator is made private and not implemented so no user
//      can assign one Node to another Node. This is to prevent multiple
//      pointers to the same data
//
// Written by:      Daniel P. Hanks (ID 0024879)
//-----------------------------------------------------------------------------

template<class T>
class Node
{
    private:

        //The data item stored in the Node
        T theItem;

        //Pointer to next Node object in the linked list
        Node<T> * nextNode;

        //---Assignment Operator Overload--------------------------------------
        // Not implemented, made private to disallow its use and avoid multiple
        // pointers to the same data.
        //---------------------------------------------------------------------
        Node<T>& operator=(const Node<T>& );

    public:
        //---Default Constructor-----------------------------------------------
        // Pre:  None
        // Post: Node is created with nextNode set as NULL
        //---------------------------------------------------------------------
        Node();

        //---Constructor w/ Data Parameter-------------------------------------
        // Pre:  None
        // Post: Node is created with nextNode set as NULL and theItem set
        //       to value of the parameter passed
        //
        // Parameter:
        //      const T&: A constant reference to type T to set theItem with
        //---------------------------------------------------------------------
        Node(const T&);

        //---Constructor w/ Data and Pointer Parameter-------------------------
        // Pre:  None
        // Post: Node is created with the nextNode and theItem set to value
        //       of parameters passed
        //
        // Parameters: 
        //      const T&: A constant reference to type T to set theItem with
        //      Node<T> *: A pointer to a Node to set nextNode to
        //---------------------------------------------------------------------
        Node(const T&,  Node<T> * );

        //---Copy Constructor--------------------------------------------------
        // Pre:  None
        // Post: Node is created with nextNode set as NULL and theItem set
        //       to the same value as the theItem in Node parameter passed
        //
        // Parameters:
        //      Node<T>&: A reference to the Node to be copied
        //---------------------------------------------------------------------
        Node(const Node<T>& );

        //---Destructor--------------------------------------------------------
        // Pre:  None
        // Post: nextNode is deleted and set to NULL
        //---------------------------------------------------------------------
        ~Node();

        //---setNext-----------------------------------------------------------
        // Pre:  None
        // Post: nextNode is set to point to the Node pointed to by parameter
        //       User is responsible to make sure no memory leaks are created
        //       by reassigning nextNode without proper deletion
        //
        // Parameters:
        //      Node<T> *: Pointer to a Node to set nextNode to
        //---------------------------------------------------------------------        
        void setNext(Node<T> * );

        //---getNext-----------------------------------------------------------
        // Pre:  None
        // Post: None
        //
        // Returns: The pointer to a Node stored in nextNode
        //---------------------------------------------------------------------
        Node<T> * getNext() const;

        //---setItem-----------------------------------------------------------
        // Pre:  None
        // Post: theItem is set to the value of parameter passed
        //
        // Parameter:
        //      const T&: A reference to the object to copy to theItem
        //---------------------------------------------------------------------
        void setItem(const T& );

        //---getItem-----------------------------------------------------------
        // Pre:  theItem has an assigned value, so a useful value os returned
        // Post: None
        //
        // Returns: The data item of type T stored in theItem private member
        //---------------------------------------------------------------------
        T getItem() const;
        
};
#include "Node.cpp"
#endif