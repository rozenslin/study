// quick sort demo time: 4:48PM
package main
import "fmt"
func swap(a, b *int) {
    if a == b {
        return
    }

    t := *b
    *b = *a
    *a = t
}

func bs(arr []int, low, high, needle int) int {
    if low > high {
        return -1
    }
    mid := (int)(low+high)/2
    if arr[mid] == needle {
        return mid
    } else if arr[mid] > needle {
        return bs(arr, low, mid-1, needle)
    } else {
        return bs(arr, mid+1, high, needle)
    }
    return -1
}

func partition(arr []int, low, high int) int {
    pivot := arr[high]
    // i holds max index of smaller ones
    i := low-1
    // foreach arr, put all smaller ones left
    for j := low; j < high-1; j++ {
        if arr[j] <= pivot {
            // found smaller one, swap left
            i++
            swap(&arr[i], &arr[j])
        }
    }
    // now swap the pivot one left
    i++
    swap(&arr[i], &arr[high])

    return i
}

func qs(arr []int, low, high int) {
    if low >= high {
        return
    }

    pi := partition(arr, low, high)
    qs(arr, low, pi-1)
    qs(arr, pi+1, high)
}

func main() {
    arr := []int{7, 3, 1, 7, 8, 9, 5}
    fmt.Println("arr:")
    fmt.Println(arr)

    qs(arr, 0, len(arr)-1)
    fmt.Println("sorted:")
    fmt.Println(arr)
    fmt.Println("pos for 5:")
    fmt.Println(bs(arr, 0, len(arr)-1, 5))
}
