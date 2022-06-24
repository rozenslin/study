package main
import "fmt"
import "math"

func main() {
    bars := []float64 {0,1,0,2,1,0,1,3,2,1,2,1}
    // 9
    bars = []float64 {4,2,0,3,2,5}
    // max from left/right including this bar
    n := len(bars)
    left := make([]float64, n)
    right := make([]float64, n)
    left[0] = bars[0]
    right[n-1] = bars[n-1]

    for i := 1; i < n; i++ {
        // 1->n-1
        left[i] = math.Max(left[i-1], bars[i])

        // n-2 -> 0
        right[n-1-i] = math.Max(right[n-i], bars[n-1-i])
    }

    // now sum up, for each bar, trapped water is min(left, right)-bar
    sum := float64(0)
    for i := 0; i < n ; i++ {
        val := math.Min(left[i], right[i]) - bars[i]
        sum += val
    }
    fmt.Println("trapped : ", sum)
}
