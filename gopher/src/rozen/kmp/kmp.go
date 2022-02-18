// Subject to the terms of the Mozilla Public License, v. 2.0, see https://mozilla.org/MPL/2.0/.
// Free for non-commercial use, contact owner for commercial use.
//
// KMP algo. demo
// Problem: given a pattern P, len=n, and a text T, len=m, find the matches of P in T.
// KMP way: the key is for the cases of 'retry new match, either after a complete one or failed one',
//          we don't need to start from P[0], instead, based on our knowledge of P,
//          we can jump to start from P[x>0] directly,
//          the jump index of each index in P is in an array called lps[n]
//          for the proving, see blog.
// So the total time will be just: O(n+m)
package main
import "fmt"

//  build lps, iteration way, time:O(n)
func build_lps(P string) []int {
    n := len(P)
    lps := make([]int, n)
    // init of iteration, jump index of 0 is of cause 0.
    lps[0] = 0

    for i := 0; i < n - 1; i++ {
        // use ptr for clearer code
        ptr := &lps[i + 1]
        // current best case, let lps[i] = x
        *ptr = lps[i] + 1
        // case true? determined by: p[x] ?= p[i+1]
        for *ptr > 0 && P[*ptr - 1] != P[i + 1] {
            // next best case lps[x]+1
            next := lps[*ptr - 1] + 1
            if *ptr > next {
                *ptr = next
            } else {
                // no hope, has to be p[0]
                *ptr = 0
            }
        }
    }
    return lps
}

// do search with lps, just linear, so: O(m)
func kmp_search(T, P string, lps []int, result *[]int) {
    m := len(T)
    n := len(P)

    // i for T, j for P
    for i, j := 0, 0; i < m; i++ {
        // if matches go on
        if T[i] == P[j] {
            j++
            // found pattern, reset j
            if j == n {
                *result = append(*result, i)
                j = lps[n - 1]
            }
        } else if j > 0 {
            // not matches, reset j
            // check the blog, the trick here is lps[j-1] is to get the jump index
            // for string P[0:j-1] (len=j), thus enable the next matching of P[x] vs T[i]
            j = lps[j - 1]
            // revist i
            i--
        }
        // else i cannot match any
    }
}

func main() {
    T := "ABAABAACAADAABAABA"
    P := "AABA"
    result := new([]int)

    lps := build_lps(P)
    kmp_search(T, P, lps, result)

    fmt.Printf("text:\t")
    fmt.Println(T)
    fmt.Printf("pattern:\t")
    fmt.Println(P)
    fmt.Printf("lps:\t")
    fmt.Println(lps)
    fmt.Printf("result:\t")
    fmt.Println(*result)
}
