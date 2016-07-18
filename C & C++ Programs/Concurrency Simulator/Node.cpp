//-----------------------------------------------------------------------------
// Filename:    Node.cpp
// Classes:     Node
//
// Methods:     Node() Default Constructor
//              Node(const T&) Constructor #2
//              Node(const T&, Node<T>*) Constructor #3
//              Node(const Node&) Copy Constructor
//              setNext()
//              getNext()
//              setItem()
//              getItem()
//-----------------------------------------------------------------------------
#include "Node.h"
#include <cstddef>

//---Default Constructor-----------------------------------------------
// Pre:  None
// Post: Node is created with nextNode set as NULL
//---------------------------------------------------------------------
template<class T>
Node<T>::Node() : nextNode(NULL)
{
}

//---Constructor w/ Data Parameter-------------------------------------
// Pre:  None
// Post: Node is created with nextNode set as NULL and theItem set
//       to value of the parameter passed
//
// Parameter:
//      const T&: A constant reference to type T to set theItem with
//---------------------------------------------------------------------
template<class T>
Node<T>::Node(const T& newItem) : theItem(newItem), nextNode(NULL)
{
}

//---Constructor w/ Data and Pointer Parameter-------------------------
// Pre:  None
// Post: Node is created with the nextNode and theItem set to value
//       of parameters passed
//
// Parameters: 
//      const T&: A constant reference to type T to set theItem with
//      Node<T> *: A pointer to a Node to set nextNode to
//---------------------------------------------------------------------
template<class T>
Node<T>::Node(const T& newItem, Node<T> * next) : theItem(newItem), 
    nextNode(next)
{
}

//---Copy Constructor--------------------------------------------------
// Pre:  None
// Post: Node is created with nextNode set as NULL and theItem set
//       to the same value as the theItem in Node parameter passed
//
// Parameters:
//      Node<T>&: A reference to the Node to be copied
//---------------------------------------------------------------------
template<class T>
Node<T>::Node(const Node<T>& newNode)
{
    theItem = newNode.getItem();
    nextNode = NULL;
}

//---Destructor--------------------------------------------------------
// Pre:  None
// Post: nextNode is deleted and set to NULL
//---------------------------------------------------------------------
template<class T>
Node<T>::~Node()
{
    if(nextNode != NULL)
    {
        delete nextNode;
        nextNode = NULL;
    }
}

//---setNext-----------------------------------------------------------
// Pre:  None
// Post: nextNode is set to point to the Node pointed to by parameter
//       User is responsible to make sure no memory leaks are created
//       by reassigning nextNode without proper deletion
//
// Parameters:
//      Node<T> *: Pointer to a Node to set nextNode to
//---------------------------------------------------------------------  
template<class T>
void Node<T>::setNext(Node<T> * newNode)
{
    nextNode = newNode;
}

//---getNext-----------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: The pointer to a Node stored in nextNode
//---------------------------------------------------------------------
template<class T>
Node<T> * Node<T>::getNext() const
{
    return nextNode;
}

//---setItem-----------------------------------------------------------
// Pre:  None
// Post: theItem is set to the value of parameter passed
//
// Parameter:
//      const T&: A reference to the object to copy to theItem
//---------------------------------------------------------------------
template<class T>
void Node<T>::setItem(const T& newItem)
{
    theItem = newItem;
}

//---getItem-----------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: The data item of type T stored in theItem private member
//---------------------------------------------------------------------
template<class T>
T Node<T>::getItem() const
{
    return theItem;
}