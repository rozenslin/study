#include<iostream>
using namespace std;
void set_divide(char* arr, int front, int end);
const int LEN = 34;
static int LEV = 0;
void set_divide(char* arr, int front, int end)
{
    LEV++;
    int mid = (end + front)/2;
    if (mid <= front)
        return;
    arr[mid] = '|';
    cout << arr << endl;
    set_divide(arr, front, mid);
    set_divide(arr, mid, end);
}

void set_divide_lev(char* arr, int front, int end, int lev)
{
    if (!lev)
        return;
    int mid = (end + front)/2;
    if (mid <= front)
        return;
    arr[mid] = '|';
    set_divide_lev(arr, front, mid, lev - 1);
    set_divide_lev(arr, mid, end, lev -1);
}



int main()
{
    char * arr = new char[LEN];
    for (int i = 1; i < LEN - 2 ; i++)
    {
        arr[i] = ' ';
    }
    arr[0] = arr[LEN-2] = '|';
    arr[LEN-1] = '\0';
    cout << arr << endl;
    set_divide(arr, 0, LEN - 2);
    LEV = (LEV+1)/2;

    for (int i = 1; i < LEN - 2 ; i++)
    {
        arr[i] = ' ';
    }

    cout << arr << endl;
    for (int i = 1, exp = 2; exp  <= LEV; i++)
    {
        exp *= 2;
        set_divide_lev(arr, 0, LEN -2, i);
        cout << arr << endl;
    }

    delete [] arr;
    return 0;
}
