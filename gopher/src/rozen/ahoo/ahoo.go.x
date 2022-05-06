// max qual thru channels
package main
import "fmt"
import "math"
func swap(a, b *int) {
    if a == b {
        return
    }
    *a, *b = *b, *a
}

// partition
func partition(arr []int, low, high int) int {
    pivot := arr[high]
    // i holds the bigger ones left
    i := low - 1
    for j := low; j < high; j++ {
        if arr[j] >= pivot {
            // bigger, move left
            i++
            // check dup. XXX
            swap(&arr[i], &arr[j])
        }
    }
    // final, move pivot left
    i++
    swap(&arr[i], &arr[high])

    return i
}

// qs, desc
func qs(arr []int, low, high int) {
    if low >= high {
        return
    }
    p := partition(arr, low, high)
    qs(arr, low, p - 1)
    qs(arr, p + 1, high)
}

func main() {
    packets := []int{1, 1, 2, 2, 3}
    ch := 3
    // quick sort desc for packets

    fmt.Println("packets :")
    fmt.Println(packets)
    fmt.Printf("chan : %d\n", ch)

    qs(packets, 0, len(packets) -1)
    fmt.Println("packets after qs:")
    fmt.Println(packets)

    // sum up [0:ch-2]
    max := 0
    i := 0
    for ; i < ch - 1; i++ {
        max += packets[i]
        fmt.Printf("pkg size for chan: %d, is %d\n", i, packets[i])
    }
    // get the median for packets[ch-1:]
    rem := 0
    for _, v := range packets[ch-1:] {
        rem += v
    }
    rem_size := len(packets[ch-1:])
    rem_median := int(math.Round(float64(rem)/float64(rem_size)))
    fmt.Printf("pkg size for chan: %d, is %d\n", i, rem_median)
    max += rem_median

    fmt.Printf("final max quality: %d\n", max)
}
