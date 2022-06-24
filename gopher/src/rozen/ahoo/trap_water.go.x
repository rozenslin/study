package main
import "fmt"
import "math"
func main() {
    // 6
    bars := []float64 {0,1,0,2,1,0,1,3,2,1,2,1}
    // 9
    bars = []float64 {4,2,0,3,2,5}
    n := len(bars)
    // this holds the max on left side including current one
    left := make([]float64, n)
    // this holds the max on right side including current one
    right := make([]float64, n)
    left[0] = bars[0]
    right[n-1] = bars[n-1]

    for i := 1; i < n ; i++ {
        // 1 -> n-1
        left[i] = math.Max(bars[i], left[i-1])
        // n-2 -> 0
        right[n-i-1] = math.Max(bars[n-i-1], right[n-i])
    }

    // now for each bar, the trapped water is min(left[i], right[i]) - bars[i]
    // sum up to get total value
    sum := float64(0)
    for i := 0; i < n ; i++ {
        val := math.Min(left[i], right[i]) - bars[i]
        sum += val
    }
    fmt.Println("bars:\t", bars)
    fmt.Println("final ret:\t", sum)
}
