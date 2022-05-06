// Mary loves binary strings.
// Given a binary string S, she defines the beauty of the string as the bitwise XOR of decimal representations of all substrings of S.
// Find the beauty of string S. Since the answer can be huge, print it modulo 998244353.
package main
import "fmt"
const MOD uint64 = 998244353
func main() {
    s := "1011"
    n := len(s)
    // c[i] holds the number of '1' at the the i-th bit
    c := make([]int, n)

    for i := 0; i < n; i++ {
        if s[i] == '1' {
            // means c[0] ... c[n-i-1] must be i+1, here keep only last one
            c[n-i-1] = i+1
        }
    }
    // now c[i]=x means c[0]...c[i-1] must have x too, so add back
    for i := n-2; i >= 0; i-- {
        c[i] += c[i+1]
    }
    // gen output
    ans, power := uint64(0), uint64(1)
    for i := 0; i < n; i++ {
        if c[i]%2 != 0 {
            ans += power
        }

        power *= 2
    }
    ans %= MOD
    fmt.Printf("bin str: %s\n", s)
    fmt.Println("count arr:")
    fmt.Println(c)
    fmt.Printf("final ans: %d\n", ans)
}
