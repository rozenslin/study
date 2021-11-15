#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string.h>
#include <unistd.h>
using namespace std;

static time_t call_time = 0;

// check and return the timespan in seconds
size_t check_time_interval()
{
    time_t now = time(NULL);
    return size_t(now - call_time);
}

/*
   You have to implement a method.

   It should return true if it has been called 4 times or less
   in the last 7 seconds.
   */

bool checkStatus()
{
    static int called_times = 0;

    if (check_time_interval() <= 7)
    {
        if (called_times++ < 4)
            return true;
        else
            return false;
    }
    else
    {
        // now reset the time boundary
        call_time = time(NULL);

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
    call_time = time(NULL);

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
