// problem:
// an int array {a[0], a[1], a[2]..., a[n]}
// find two index: i < j, which will have the value of
// X = max((j-i) * min(a[i], a[j])...)
// input:  the array of int a[n];
// output: the value of X

package main
import "fmt"

// return the min for two
func min(a int, b int) int {
    if a <= b {
        return a
    }

    return b
}

// get the max of X from arr, and output
func get_max(arr []int) {
    fmt.Printf("arr got: %v\n", arr)
    l := len(arr)
    max := 1
    var s []int
    // XXX now O = n(n-1)/2 = n^2, how to O(n)?
    // j is from l-1 to 1
    for j := l-1; j > 0; j-- {
        // i is from 0 to j-1
        for i := 0; i < j; i++ {
            // now get X value
            x := (j-i) * min(arr[i], arr[j])
            s = append(s, x)
            if x > max {
                max = x
            }
        }
    }
    fmt.Printf("result arr : %v, size: %d\n\n", s, len(s))

    // now get the max one from array s
    fmt.Printf("Max container size: %v\n\n", max)
}

func main() {
    msg := "A gopher named Desire"
    fmt.Printf("Hello gopher!\n %s\n\n", msg)
    arr := []int{1, 8, 6, 2, 5, 4, 8, 3, 7}
    get_max(arr)

    arr = []int{4,3,2,1,4}
    get_max(arr)

    arr = []int{1,1}
    get_max(arr)

    arr = []int{1, 2, 1}
    get_max(arr)

    arr = []int{22, 39, 5, 7, 3, 3, 3, 3}
    get_max(arr)

    arr = []int{2, 8, 15, 12, 1, 7}
    get_max(arr)
}
