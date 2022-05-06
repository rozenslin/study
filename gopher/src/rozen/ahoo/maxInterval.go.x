//  For a given array arr[], returns the maximum j – i such that arr[j] > arr[i] 
// for score (j-i)*min(arr[j], arr[i])
package main
import "fmt"
func min(a, b int) int {
    if a <= b {
        return a
    }
    return b
}
func max(a, b int) int {
    if a >= b {
        return a
    }
    return b
}

func main() {
    arr := []int{9, 2, 3, 4, 5, 6, 7, 8, 18, 0}
    n := len(arr)
    lmin := make([]int, n)
    rmax := make([]int, n)

    // lmin[i] stores the min of arr[0]...arr[i]
    // O(N)
    lmin[0] = arr[0]
    for i := 1; i < n; i++ {
        lmin[i] = min(lmin[i-1], arr[i])
    }
    // rmax[j] stores the max of arr[j]...arr[n-1]
    // O(N)
    rmax[n-1] = arr[n-1]
    for j := n-2; j >= 0; j-- {
        rmax[j] = max(rmax[j+1], arr[j])
    }
    fmt.Println("lmin:")
    fmt.Println(lmin)
    fmt.Println("rmax:")
    fmt.Println(rmax)

    maxDiff := -1
    // O(N)
    i, j := 0, 0
    for i < n && j < n {
        if lmin[i] <= rmax[j] {
            maxDiff = max(maxDiff, j - i)
            j++
        } else {
            i++
        }
    }

    fmt.Println("arr:")
    fmt.Println(arr)
    fmt.Printf("max diff %d\n", maxDiff)
}
