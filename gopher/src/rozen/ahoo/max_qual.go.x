/* Question: You have n packets of some size, n[i] that need to be transferred optimally over m channels.
Rules:

    Every channel must transfer at least one packet.
    The optimal solution is one where quality of transfer is highest
    Quality is defined as the sum of medians of packets(if float, take the next highest integer) transferred over each channel
    Median is defined as the middle element in a non-decreasing array of elements. If even, it is the average of the middle two elements.
    Return the maximum possible quality of transfer.
*/
// examples: p{1, 1, 2, 2, 3} ch=3; max:6
//           p{5, 2, 2, 1, 5, 3} ch=2; max:8
// way: simply, for any packet, if shared with others in same channel, the medians val from that chan must become less, 
// so to get max, the bigger ones must be single in channel as possible,
// so the way is: sort the packets desc, then add packets[0:m-2] together, then get the median of packets[m-1:end], total is the max

// XXX limitation: the max val must the rouding at last, not 'rouding then add' see case 1 1 2 2 3
// 2PM 2:15PM
package main
import "fmt"
import "math"
func swap(a, b *int) {
    if a == b {
        return
    }
    *a, *b = *b, *a
}

func partition(arr []int, low, high int) int {
    pivot := arr[high]
    // i holds bigger ones
    i := low - 1
    for j := low; j < high; j++ {
        if arr[j] >= pivot {
            // found bigger, swap
            i++
            swap(&arr[i], &arr[j])
        }
    }
    // now swap pivot
    i++
    swap(&arr[i], &arr[high])

    return i
}

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

    fmt.Println("packets:")
    fmt.Println(packets)
    fmt.Printf("channels: %d\n", ch)

    // sort the packets desc, quick sort time: O(NLogN)
    qs(packets, 0, len(packets)-1)
    max := 0
    i := 0
    // get max from packets[0:ch-2]
    for ; i < ch - 1; i++ {
        max += packets[i]
        fmt.Printf("val for channel:%d is:%d\n", i, packets[i])
    }
    // get the median of packets[i:] ones
    rem := 0
    rem_size := len(packets[i:])
    for _, v := range packets[i:] {
        rem += v
    }

    // XXX ugly cast as type(val)
    rem_med := int(math.Round(float64(rem)/float64(rem_size)))
    fmt.Printf("val for channel:%d is:%d\n", i, rem_med)

    max += rem_med
    fmt.Printf("max quality is:%d\n", max)
}
