// 253pm 3:07PM
// max score from good sub arr.
// good sub arr contains with i <= k <= j, score as min * j-i+1
// given arr := []int {1,4,3,7,4,5}, k = 3; output: 15
package main
import "fmt"

func min(a, b int) int {
    if a < b {
        return a
    }
    return b
}

func max_score(arr []int, k int) int {
    // gen the aux. arr from arr, this is the possible val to gen the max scores
    n := len(arr)
    scores := make([]int, n)
    scores[k] = arr[k]

    // 1. O(n)
    for i := k - 1; i >= 0; i-- {
        scores[i] = min(arr[i], scores[i + 1])
    }
    for i := k + 1; i < n; i++ {
        scores[i] = min(arr[i], scores[i - 1])
    }
    fmt.Println("aux. scores arr:")
    fmt.Println(scores)

    // now gen the max_score, each step get the max known so far.
    // O(n)
    left, right := k, k
    max_score := 0
    for true {
        // get the new max_score
        cur := min(scores[left], scores[right])
        if max_score < cur * (right - left + 1) {
            max_score = cur * (right - left + 1)
        }

        fmt.Printf("current pos, left: %d, right: %d, cur score: %d, cur max_score:%d\n", left, right, cur, max_score)

        // end
        if left == 0 && right == n - 1 {
            break
        }

        // enlarge the subarray for next 
        if left == 0 {
            // go right
            right++
        } else if right == n - 1 {
            // go left
            left--
        } else if scores[left - 1] > scores[right + 1] {
            // go left
            left--
        } else {
            // go right
            right++
        }
    }

    return max_score
}

func main() {
    arr := []int {5,5,4,5,4,1,1,1}
    k := 0
    // arr := []int {1,4,3,7,4,5}
    // k := 3

    fmt.Println("arr:")
    fmt.Println(arr)
    fmt.Printf("k: %d\n", k)

    max_score := max_score(arr, k)
    fmt.Printf("max_score: %d\n", max_score)
}
