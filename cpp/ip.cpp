#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <string>
using namespace std;

// 1. gen random ip address, ipv4 only, as string not bits.
// 2. store a list of random ip adr into memory: no uniq required, no validation req.
// 3. print out all them

string gen_ip()
{
    char buffer[32];
    memset(buffer, 0, 32);

    // ignore the endian, just got the 'random' 4 bytes.
    uint32_t bits = rand();
    uint8_t* pbyte = (uint8_t*)&bits;
    string addr = "";
    for (int i = 0; i < 4; i++)
    {
        sprintf(buffer, "%d", *pbyte);
        addr += buffer;

        if (i < 3)
            addr += ".";

        pbyte++;
    }

    fprintf(stderr,"ahooooo:%s:%d---%s\n",__FUNCTION__,__LINE__, addr.c_str());

    return addr;
}


int main() {
    // the seed
    srand(time(NULL));
    string*  addrs = new string[4];
    addrs[0] = gen_ip();
    addrs[1] = gen_ip();
    addrs[2] = gen_ip();
    addrs[3] = gen_ip();

    for (int i=0; i<=3; i++)
        cout<<addrs[i]<<endl;

    delete[] addrs;

    return 0;
}
