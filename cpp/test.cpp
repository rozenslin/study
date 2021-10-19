#include <iostream>
using namespace std;


// invalid calling to now(), just for POC
// single proc, no need for lock.
static time_t call_time = 0;
static int called_times = 0;

// check and return the timespan in seconds 
size_t check_time_interval(struct* tm_old, strunew_time)
{
  return size_t (new_time.tm_seconds - old_time.tm_seconds);
}

/*
You have to implement a method.

It should return true if it has been called 4 times or less
in the last 7 seconds.
*/

// XXX more doundary check.
bool checkStatus()
{ 
  time_t now = time(&now);
  struct tm tm_now;
  localtime_r(&now, &tm_now);

  struct tm tm_old;
  localtime_r(&call_time, &tm_old);

  if (check_time_interval(tm_old, tm_now) <= 7)
  {
    if (called_times <= 4)
       return true;
    else
      return false;
  }
  else 
  {
    // now reset the time boundary
    call_time = time_now;
    
    // also reset the call times to be the first
    called_times = 1;
    
    // this time must be true
    return true;
  }
  
  // here never reached.
}

// To execute C++, please define "int main()"
int main() {
  // init call time
  call_time = time(&call_time);
  
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False

  sleep(8);
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  
  sleep(8);
  cout << checkStatus() << endl; // True
  
  sleep(8);
  cout << checkStatus() << endl; // True
  
  sleep(8);
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // True
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  cout << checkStatus() << endl; // False
  
  return 0;
}

