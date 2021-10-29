#include<iostream>
#include<cstring>
using namespace std;
const int LEN = 33;
int main()
{
    char * arr = new char[LEN + 1];
    arr[LEN] = '\0';
    int mid = LEN/2;
    memset(arr, ' ', LEN);
    for (int i = 1; i <= mid; i++)
    {
        for (int j = 0; j < i; j++)
        {
            arr[mid+j] = arr[mid-j] = '*';
        }

        cout << arr <<  endl;
        memset(arr, ' ', LEN);
    }
    for (int i = mid; i >= 1; i--)
    {
        for (int j = 0; j < i; j++)
        {
            arr[mid+j] = arr[mid-j] = '*';
        }

        cout << arr <<  endl;
        memset(arr, ' ', LEN);
    }

    return 0;
}
