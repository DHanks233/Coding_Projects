//-----------------------------------------------------------------------------
// Filename:    SLinkedList.cpp
// Classes:     SLinkedList
//
// Methods:     SLinkedList() Default Constructor
//              SLinkedList(const SLinkedList<T>& ) Copy Constructor
//              ~SLinkedList()
//              isEmpty()
//              getItemCount()
//              getItemAt()
//              clear()
//              add()
//              remove()
//              mergeSort()
//              printItems()
//              contains()
//              setTempList()
//-----------------------------------------------------------------------------
#include "SLinkedList.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>

//---Default Constructor-----------------------------------------------
// Pre:  None
// Post: SLinkedList object is created with headNode set to NULL and
//       itemCount set to 0
//---------------------------------------------------------------------
template<class T>
SLinkedList<T>::SLinkedList() : headNode(NULL), tailNode(NULL), 
    itemCount(0)
{
}

//---Copy Constructor--------------------------------------------------
// Pre:  None
// Post: SLinkedList object is created with headNode set to NULL and
//       itemCount set to 0
//
// Parameter:
//      const SLinkedList<T>&: A reference to the object to be copied
//---------------------------------------------------------------------
template<class T>
SLinkedList<T>::SLinkedList(const SLinkedList<T>& newList)
{
    Node<T> * currPtr = NULL;
    Node<T> * newNodePtr = NULL;
    Node<T> * origPtr = newList.headNode;

    itemCount = newList.getItemCount();
    if(origPtr == NULL)
    {
        headNode = NULL;
    }
    else
    {
        //Set headNode to the first item in newList
        headNode = new Node<T>();
        headNode->setItem(origPtr->getItem());
        currPtr = headNode;
        origPtr = origPtr->getNext();
        while(origPtr != NULL)
        {
            //Traverse the list copying all Nodes
            newNodePtr = new Node<T>();
            newNodePtr->setItem(origPtr->getItem());
            currPtr->setNext(newNodePtr);
            origPtr = origPtr->getNext();
            currPtr = currPtr->getNext();
        }
        tailNode = currPtr;
        currPtr->setNext(NULL);
    }
    currPtr = NULL;
    newNodePtr = NULL;
    origPtr = NULL;
}

//---Destructor--------------------------------------------------------
// Pre:  None
// Post: The linked list is deleted and headNode is set to NULL
//---------------------------------------------------------------------
template<class T>
SLinkedList<T>::~SLinkedList()
{
    if(headNode != NULL)
    {
        clear();
    }
}

//---Assignment Operator Overload--------------------------------------
// Pre: None
// Post: The linked list is copied from the SLinkedList object
//       parameter, as is itemCount.
//
// Returns: The SLinkedList object to be copied
// Parameters:
//      const SLinkedList<T>&: A reference to the object to be copied
//---------------------------------------------------------------------
template<class T>
SLinkedList<T>& SLinkedList<T>::operator=(const SLinkedList<T> & newList)
{
    Node<T> * currPtr = NULL;
    Node<T> * newNodePtr = NULL;
    Node<T> * origPtr = newList.headNode;

    //If setting the object to itself, return the invoking object
    if(this == &newList)
    {
        return * this;
    }
    else
    {
        itemCount = newList.getItemCount();
        if(origPtr == NULL)
        {
            headNode = NULL;
        }
        else
        {
            //Set headNode to the first item in newList
            headNode = new Node<T>();
            headNode->setItem(origPtr->getItem());
            currPtr = headNode;
            origPtr = origPtr->getNext();
            while(origPtr != NULL)
            {
                //Traverse the list copying all Nodes
                newNodePtr = new Node<T>();
                newNodePtr->setItem(origPtr->getItem());
                currPtr->setNext(newNodePtr);
                origPtr = origPtr->getNext();
                currPtr = currPtr->getNext();
            }
            tailNode = currPtr;
            currPtr->setNext(NULL);
        }
        currPtr = NULL;
        newNodePtr = NULL;
        origPtr = NULL;
    }
    return * this;
}

//---isEmpty-----------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: A bool that is true if itemCount is 0, false otherwise
//---------------------------------------------------------------------
template<class T>
bool SLinkedList<T>::isEmpty() const
{
    return (itemCount == 0);
}

//---getItemCount------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: The value of itemCount, representing number of Nodes in
//          the linked list
//---------------------------------------------------------------------
template<class T>
int SLinkedList<T>::getItemCount() const
{
    return itemCount;
}

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
template<class T>
T SLinkedList<T>::getItemAt(int position) const
{
    Node<T> * currPtr = headNode;

    if(isEmpty() || (position > itemCount))
    {
        throw std::out_of_range("The position entered is larger than the number"
            " of items in the list.");
    }
    for(int i = 1; i < position; i++)
    {
        currPtr = currPtr->getNext();
    }
    return currPtr->getItem();
}

//---clear-------------------------------------------------------------
// Pre:  None
// Post: All Node objects in the linked list are deleted and headNode
//       is set to NULL
//
// Returns: bool that is true if itemCount reaches 0, false otherwise
//---------------------------------------------------------------------
template<class T>
bool SLinkedList<T>::clear()
{
    while(!isEmpty())
    {
        popFront();
    }
    if(isEmpty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

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
template<class T>
bool SLinkedList<T>::addToFront(const T& newItem)
{
    Node<T> * currPtr = headNode;

    //Set headNode as a new Node with the parameter value as Node data
    headNode = new Node<T>();
    headNode->setItem(newItem);
    headNode->setNext(currPtr);
    //If headNode is the only Node in the list, set tailNode to headNode
    if(currPtr == NULL)
    {
        tailNode = headNode;
    }

    //If headNode could not be allocated new memory, return false
    if(headNode == NULL)
    {
        return false;
    }
    itemCount++;
    currPtr = NULL;
    return true;
    
}

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
template<class T>
bool SLinkedList<T>::addToEnd(const T& newItem)
{
    Node<T> * newNode = NULL;

    if(headNode == NULL)
    {
        headNode = new Node<T>();
        headNode->setItem(newItem);
        tailNode = headNode;
    }
    else
    {
        newNode = new Node<T>();
        newNode->setItem(newItem);
        tailNode->setNext(newNode);
        tailNode = tailNode->getNext();
        newNode = NULL;
    }

    //If memory could not be allocated, return false
    if(headNode == NULL)
    {
        return false;
    }
    itemCount++;
    return true;
}

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
template<class T>
bool SLinkedList<T>::removeItem(const T& removeItem)
{
    Node<T> * currPtr = headNode;
    Node<T> * prevPtr = NULL;

    if(isEmpty())
    {
        return false;
    }
    while((currPtr != NULL) && (currPtr->getItem() != removeItem))
    {
        prevPtr = currPtr;
        currPtr = currPtr->getNext();
    }
    //Return false if the search item is not found
    if(currPtr == NULL)
    {
        return false;
    }
    //If the headNode contains the data to be removed
    else if(currPtr == headNode)
    {
        headNode = headNode->getNext();
        currPtr->setNext(NULL);
        delete currPtr;
        if(headNode == NULL)
        {
            tailNode = NULL;
        }
    }
    //Delete the Node containing the search item and re-link list
    else
    {
        if(currPtr == tailNode)
        {
            tailNode = prevPtr;
        }
        prevPtr->setNext(currPtr->getNext());
        currPtr->setNext(NULL);
        delete currPtr;
    }
    currPtr = NULL;
    itemCount--;
    return true;
}

//---printItems--------------------------------------------------------
// Pre:  None
// Post: An output file named "output.txt" is created and receives
//       output from the SLinkedList object in the format of the data
//       item of each Node printed on a separate line
//---------------------------------------------------------------------
template<class T>
void SLinkedList<T>::printItems() const
{
    Node<T> * currPtr = headNode;

    while(currPtr != NULL)
    {
        std::cout << currPtr->getItem() << std::endl;
        currPtr = currPtr->getNext();
    }
}

//---contains----------------------------------------------------------
// Pre:  None
// Post: None
//
// Returns: A Node * containing a pointer to the Node containing data
//          equal to the parameter, or NULL if the data is not found
//          in the list
//
// Parameter:
//      const T&: Reference to the item being searched for in the list
//---------------------------------------------------------------------
template<class T>
Node<T> * SLinkedList<T>::contains(const T& searchItem) const
{
    Node<T> * temp = headNode;

    //Traverse list until item is found or end of the list is reached
    while((temp != NULL) && (temp->getItem() != searchItem))
    {
        temp = temp->getNext();
    }
    return temp;
}

//---popFront----------------------------------------------------------
// Pre: The linked list is not empty
// Post: The first item in the linked list, headNode, is deleted
//
// Returns: A bool that is false if list is empty, true if it has at
//          least one Node that can be deleted
//---------------------------------------------------------------------
template<class T>
bool SLinkedList<T>::popFront()
{
    Node<T> * currPtr = headNode;

    if(isEmpty())
    {
        return false;
    }
    else
    {
        headNode = headNode->getNext();
        currPtr->setNext(NULL);
        delete currPtr;
        if(headNode == NULL)
        {
            tailNode = NULL;
        }
        itemCount--;
    }
    currPtr = NULL;
    return true;
}