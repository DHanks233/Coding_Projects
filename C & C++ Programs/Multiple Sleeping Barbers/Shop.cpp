//-----------------------------------------------------------------------------
// File:        Shop.cpp
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
// Date:        5/8/2015 - last updated
//----------------------------------------------------------------------------

#include "Shop.h"

//-----------------------------------------------------------------------------
// Default Shop constructor
// Sets the chair and barber-related variables, creates condition variable
// arrays, then initializes them and the mutex lock
//
// @pre:    None
// @post:   Shop object is created and fully initialized
//----------------------------------------------------------------------------
Shop::Shop()
{
  nDropsOff = 0;
  numberChairs = DEFAULT_CHAIRS;
  maxChairs = DEFAULT_CHAIRS;
  customerAssignedTo = new int[DEFAULT_BARBERS];
  barberIsReady = new pthread_cond_t[DEFAULT_CUSTOMERS];
  signalNext = new pthread_cond_t[DEFAULT_BARBERS];
  pthread_mutex_init(&shopLock, NULL);
  for(int i = 0; i < DEFAULT_CUSTOMERS; i++)
  {
    pthread_cond_init(&barberIsReady[i], NULL);
  }
  for(int i = 0; i < DEFAULT_BARBERS; i++)
  {
    pthread_cond_init(&signalNext[i], NULL);
  }
}

//-----------------------------------------------------------------------------
// Shop constructor with number barbers and chairs
// Sets the chair and barber-related variables, creates condition variable
// arrays, then initializes them and the mutex lock
//
// @pre:    nBarbers is > 0
// @post:   Shop object is instantiated and fully initialized
// @param nBarbers: The number of barbers threads running
// @param nChairs:  The number of available "waiting room" chairs
//-----------------------------------------------------------------------------
Shop::Shop(int nBarbers, int nChairs, int nCustomers)
{
  nDropsOff = 0;
  numberChairs = nChairs;
  maxChairs = nChairs;
  customerAssignedTo = new int[nBarbers];
  barberIsReady = new pthread_cond_t[nCustomers];
  signalNext = new pthread_cond_t[nBarbers];
  pthread_mutex_init(&shopLock, NULL);
  for(int i = 0; i < nCustomers; i++)
  {
    pthread_cond_init(&barberIsReady[i], NULL);
  }
  for(int i = 0; i < nBarbers; i++)
  {
    pthread_cond_init(&signalNext[i], NULL);
  }
}

//-----------------------------------------------------------------------------
// Shop destructor
// Deletes all dynamically allocated condition variable arrays
//
// @pre:    None
// @post:   All dynamically allocated memory in the Shop instance are deleted
//-----------------------------------------------------------------------------
Shop::~Shop()
{
  if(customerAssignedTo != NULL)
  {
    delete [] customerAssignedTo;
    customerAssignedTo = NULL;
  }
  if(barberIsReady != NULL)
  {
    delete [] barberIsReady;
    barberIsReady = NULL;
  }
  if(signalNext != NULL)
  {
    delete [] signalNext;
    signalNext = NULL;
  }
}

//-----------------------------------------------------------------------------
// visitShop
// Customer thread enters the "shop," leaves (terminates) if all barbers are
// busy and no waiting chairs are available, takes a waiting chair if barbers.
// are busy. When a barber is available, customer signals barber customer thread
// is ready
//
// @pre:    id is a positive int value generated sequentially from 1 to N
// @post:   Customer thread links to barber ID, which is returned, and enters
//          a service chair for a haircut
// @param id:         The sequential customer number, starting at 0 and up to N
// @returnsmbarberID: ID of barber that will give customer a haircut
//-----------------------------------------------------------------------------
int Shop::visitShop(int id)
{
  int barberID = 0;
  if(pthread_mutex_lock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in customer thread " << id << endl;
  }
  //Customer leaves if there are no available waiting chairs or available
  //or if there are no chairs in the shop at all and barbers are busy
  if((numberChairs < 1 && maxChairs != 0)
     || (maxChairs == 0 && barberQueue.empty()))
  {
    cout << "customer[" << id << "]: left the shop because of no chairs"
        << endl;
    nDropsOff++;
    if(pthread_mutex_unlock(&shopLock) != 0) {
      cerr << MUTEX_EXCEPT << "in customer thread " << id << endl;
    }
    return -1;
  }
  //Customer waits for barber to be available
  else if(barberQueue.empty())
  {
    cout << "customer[" << id << "]: takes a waiting chair. # chairs available "
    << --numberChairs << endl;
    customerQueue.push(id - 1);
    if(pthread_cond_wait(&barberIsReady[id - 1], &shopLock) != 0) {
      cerr << WAIT_EXCEPT << "in customer thread " << id << endl;
    }
    ++numberChairs;
  }
  //Customer is assigned a barber
  else if(!barberQueue.empty()) {
    barberID = barberQueue.front();
    barberQueue.pop();
    customerAssignedTo[barberID] = id;
  }
  cout << "customer[" << id << "]: moves to service chair ["
  << barberID << "]. " << "# waiting seats available = "
  << numberChairs << endl;

  if(pthread_cond_signal(&signalNext[barberID]) != 0) {
    cerr << SIGNAL_EXCEPT << "in customer thread " << id << endl;
  }
  if(pthread_mutex_unlock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in customer thread " << id << endl;
  }
  return barberID;
}

//-----------------------------------------------------------------------------
// leaveShop
// Customer thread waits for haircut to finish, then exits
//
// @pre:    customerAssignedTo[barberID] holds customer ID. barberID and
//          customerID are positive int values representing each thread
// @post:   Customer thread outputs to console and finishes execution
// @param barberID:   ID assigned to barber thread
// @param customerID: ID assigned to the customer thread
//-----------------------------------------------------------------------------
void Shop::leaveShop(int customerID, int barberID)
{
  if(pthread_mutex_lock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in customer thread " << customerID << endl;
  }
  cout << "customer[" << customerID << "]: waits for "
  << "Barber[" << barberID << "] to be done with haircut."<< endl;
  if(pthread_cond_wait(&signalNext[barberID], &shopLock) != 0) {
    cerr << WAIT_EXCEPT << "in customer thread " << customerID << endl;
  }
  cout << "customer[" << customerID << "]: says goodbye to Barber["
  << barberID << "]." << endl;
  if(pthread_cond_signal(&signalNext[barberID]) != 0) {
    cerr << SIGNAL_EXCEPT << "in customer thread " << customerID << endl;
  }
  if(pthread_mutex_unlock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in customer thread " << customerID << endl;
  }
}

//-----------------------------------------------------------------------------
// helloCustomer
// If no customers threads are waiting, barber thread waits on signal from a
// new customer, then begins a haircut service for customer thread
//
// @pre:    id parameter is a positive int representing barber number 0 - N
// @post:   Barber is assigned to a customer thread and begins haircut service
// @param id: The value representing barber's ID
//-----------------------------------------------------------------------------
void Shop::helloCustomer(int id)
{
  if(pthread_mutex_lock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in barber thread " << id << endl;
  }
  //If no customers, go to sleep and wait for signal from customer
  if(customerQueue.empty()) {
    cout << "barber[" << id << "]: sleeps because of no customers" << endl;
    barberQueue.push(id);
    if(pthread_cond_wait(&signalNext[id], &shopLock) != 0) {
      cerr << WAIT_EXCEPT << "in barber thread " << id << endl;
    }
  }
  else {
    int nextCustomer = customerQueue.front();
    customerQueue.pop();
    customerAssignedTo[id] = nextCustomer;
    if(pthread_cond_signal(&barberIsReady[nextCustomer]) != 0) {
      cerr << SIGNAL_EXCEPT << "in barber thread " << id << endl;
    }
  }
  if(pthread_mutex_unlock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in barber thread " << id << endl;
  }
}

//-----------------------------------------------------------------------------
// byeCustomer
// Barber thread finishes haircut service for customer thread, outputs to
// console and returns to helloCustomer to service next customer thread
//
// @pre:    id is a positive int value representing barber's ID
// @post:   Barber finishes haircut is output to console
// @param id: Barber's ID
//-----------------------------------------------------------------------------
void Shop::byeCustomer(int id)
{
  if(pthread_mutex_lock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in barber thread " << id << endl;
  }
  cout << "barber[" << id << "]: starts a haircut service for "
      << "Customer[" << customerAssignedTo[id] << "]." << endl;
  cout << "barber[" << id << "]: says he's done with a haircut service for "
  << "Customer[" << customerAssignedTo[id] << "]." << endl;
  if(pthread_cond_signal(&signalNext[id]) != 0) {
    cerr << SIGNAL_EXCEPT << "in barber thread " << id << endl;
  }
  if(pthread_cond_wait(&signalNext[id], &shopLock) != 0) {
    cerr << WAIT_EXCEPT << "in barber thread " << id << endl;
  }
  cout << "barber[" << id << "]: calls in another customer" << endl;
  if(pthread_mutex_unlock(&shopLock) != 0) {
    cerr << MUTEX_EXCEPT << "in barber thread " << id << endl;
  }
}
