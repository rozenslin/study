// Subject to the terms of the Mozilla Public License, v. 2.0, see https://mozilla.org/MPL/2.0/.
// Free for non-commercial use, contact owner for commercial use.
//
// KMP algo.
// problem:
//   given a patterns P, len = n
//   also a text string T, len = m, get all the matches of P in T
// main steps:
// 1. build the auxilliary array lps[n] to store the jump index for current failed/ended match
// 2. do the search with that lps.
// So total time is O(n+m)

#include <iostream>
#include <vector>
using namespace std;

// build the lps array from pattern, time: O(n)
// lps[i] = x means for pattern P[0:n-1]
// P[0:x-1] == P[i+1-x:i]
// thus x holds the longest proper prefix that's also the suffix for substring P[0:i] (len = i+1)
int* build_lps(const string& p)
{
    int n = p.size();
    int* lps = new int[n];

    lps[0] = 0; // lps[0] is always 0
  
    // the iteration set lps[i] for i = 1:n-1
    for (int i = 0; i < n; i++)
    {
        // best case is this
        lps[i+1] = lps[i] + 1;
        // this is checked by if p[x] ?= p[i+1]
        // if yes, go on, 
        // if not, then next best case is lps[i+1] = lps[x] + 1 and checked by p[lps[i+1] -1] ?= p[i+1], again...
        // till lps[i+1] is 0
        // here lps[i+1] -1 is the 'x'
        while (lps[i+1] && p[lps[i+1] -1] != p[i+1])
        {
            // next best case
            int next = lps[lps[i+1] -1] + 1;
            if (lps[i+1] == next)
                // no more chance
                lps[i+1] = 0;
            else
                lps[i+1] = next;
        }
    }

    cout << "lps: \t";
    for (int i = 0; i < n; i++)
    {
        cout << "[" << i << "]: " << lps[i] << ", ";
    }
    cout << endl; 
    return lps;
}

// do search, time: O(m)
void kmp_search(const int* lps, const string& t, const string& p, vector<int>& result)
{
    int n = p.size();
    for (int i = 0, j = 0; i < t.size(); i++)
    {
        cout << "current states: i :" << i << " j: " << j << endl;
        // if keeping found, go on
        if (t[i] == p[j])
        {
            j++;
            // one match found, store the result, reset j 
            if (j == n) 
            {
                result.push_back(i);
                j = lps[n-1];
            }
        }
        // if not, and j not 0, jump to next lps
        else if (j)
        {
            // it's current j to match, so we need to find lps for 'before j': P[0:j-1] (len = j)
            j = lps[j-1];
            // check current i with new j again
            i--;
        }
        // else already checked P0, current i cannot match, just move on
    }
}

int main()
{
    string t = "ADAABAACAADAABAABA";
    string p =  "AABA";

    cout << "text:\t\t" << t << endl;
    cout << "pattern:\t" << p << endl;
    int* lps = build_lps(p);


    vector<int> result;
    kmp_search(lps, t, p, result);


    cout << "pattern matches at: \t";
    for (const auto& pos : result)
    {
        cout << pos << ", ";
    }
    cout << endl;

    delete [] lps;
    return 0;
}
