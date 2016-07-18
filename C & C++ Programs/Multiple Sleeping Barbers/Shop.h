//-----------------------------------------------------------------------------
// File:        Shop.h
// Classes:     Shop
//
// Methods:     Shop()
//              Shop(int nBarbers, int nChairs)
//              ~Shop
//              visitShop()
//              leaveShop()
//              helloCustomer()
//              byeCustomer()
//
// Written by:  Daniel Hanks
// Date:        5/6/2015 - last updated
//----------------------------------------------------------------------------

#ifndef SHOP_H_
#define SHOP_H_
#include <pthread.h>
#include <queue>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

#define DEFAULT_CHAIRS 3
#define DEFAULT_BARBERS 1
#define DEFAULT_CUSTOMERS 10
const string WAIT_EXCEPT = "Error thrown by wait condition ";
const string SIGNAL_EXCEPT = "Error thrown by signal condition ";
const string MUTEX_EXCEPT = "Error thrown by mutex lock call ";

//-----------------------------------------------------------------------------
// Class:       Shop
// Description: This class acts as a monitor to allow synchronization between
//              all threads entering. The threads are either "barber" threads,
//              or "customer threads." The barber threads service the customer
//              threads with a haircut, with performance depending on the
//              number of barbers, number of customers and number of waiting
//              room chairs (a semaphore-type resources).
//-----------------------------------------------------------------------------
class Shop
{
  public:
  //---------------------------------------------------------------------------
  // Default Shop constructor
  // Sets the chair and barber-related variables, creates condition variable
  // arrays, then initializes them and the mutex lock
  //
  // @pre:    None
  // @post:   Shop object is created and fully initialized
  //---------------------------------------------------------------------------
    Shop();

    //-------------------------------------------------------------------------
    // Shop constructor with number barbers and chairs
    // Sets the chair and barber-related variables, creates condition variable
    // arrays, then initializes them and the mutex lock
    //
    // @pre:    nBarbers is > 0
    // @post:   Shop object is instantiated and fully initialized
    // @param nBarbers: The number of barbers threads running
    // @param nChairs:  The number of available "waiting room" chairs
    //-------------------------------------------------------------------------
    Shop(int nBarbers, int nChairs, int nCustomers);

    //-------------------------------------------------------------------------
    // Shop destructor
    // Deletes all dynamically allocated condition variable arrays
    //
    // @pre:    None
    // @post:   All dynamically allocated memory in the Shop instance are deleted
    //-------------------------------------------------------------------------
    ~Shop();

    //-------------------------------------------------------------------------
    // visitShop
    // Customer thread enters the "shop," leaves (terminates) if all barbers are
    // busy and no waiting chairs are available, takes a waiting chair if barbers.
    // are busy. When a barber is available, customer signals barber customer
    // thread is ready
    //
    // @pre:    id is a positive int value generated sequentially from 1 to N
    // @post:   Customer thread links to barber ID, which is returned, and enters
    //          a service chair for a haircut
    // @param id: The sequential customer number, starting at 0 and up to N
    // @returns barberID: ID of barber that will give customer a haircut
    //-------------------------------------------------------------------------
    int visitShop(int id);

    //-------------------------------------------------------------------------
    // leaveShop
    // Customer thread waits for haircut to finish, then exits
    //
    // @pre:    customerAssignedTo[barberID] holds customer ID. barberID and
    //          customerID are positive int values representing each thread
    // @post:   Customer thread outputs to console and finishes execution
    // @param barberID:   ID assigned to barber thread
    // @param customerID: ID assigned to the customer thread
    //-------------------------------------------------------------------------
    void leaveShop(int customerID, int barberID);

    //-----------------------------------------------------------------------------
    // helloCustomer
    // If no customers threads are waiting, barber thread waits on signal from a
    // new customer, then begins a haircut service for customer thread
    //
    // @pre:    id parameter is a positive int representing barber number 0 - N
    // @post:   Barber is assigned to a customer thread and begins haircut service
    // @param id: The value representing barber's ID
    //-----------------------------------------------------------------------------
    void helloCustomer(int id);

    //-------------------------------------------------------------------------
    // byeCustomer
    // Barber thread finishes haircut service for customer thread, outputs to
    // console and returns to helloCustomer to service next customer thread
    //
    // @pre:    id is a positive int value representing barber's ID
    // @post:   Barber finishes haircut is output to console
    // @param id: Barber's ID
    //-------------------------------------------------------------------------
    void byeCustomer(int id);

    int nDropsOff; //Number of customers who leave due to no waiting chairs

  private:

    pthread_mutex_t shopLock;
    pthread_cond_t * signalNext;    //For wait and signal once haircut begins
    pthread_cond_t * barberIsReady; //Signal to customer that barber is ready
    int numberChairs;         //The current number of waiting chairs available
    int maxChairs;            //The total number of waiting chairs in the shop
    int * customerAssignedTo; //Array to hold customer ID indexed by barber ID
    queue<int> barberQueue;   //Queue holding barbers waiting for customers
    queue<int> customerQueue; //Queue holding customers waiting for service

};

#endif /* SHOP_H_ */
