package main
import "fmt"

func swap(a, b *int) {
    if a == b {
        return
    }
    *a, *b = *b, *a
}

// make root largest in subtrees
func heapify(arr []int, root int, n int) {
    left := 2 * root + 1
    right := 2 * root + 2
    largest := root

    if left < n && arr[left] > arr[largest] {
        largest = left
    }
    if right < n && arr[right] > arr[largest] {
        largest = right
    }

    if largest != root {
        // swap
        swap(&arr[largest], &arr[root])
        // heapify subtrees
        heapify(arr, largest, n)
    }
}

// gather the root nodes to gen the sorted arr
func sort(arr []int) {
    for i := len(arr)-1; i > 0; i-- {
        swap(&arr[0], &arr[i])
        // then heapify the remaining ones and go on
        heapify(arr, 0, i)
    }
}

func main() {
    arr := []int{7, 3, 1, 7, 8, 9, 5}
    fmt.Println("arr:")
    fmt.Println(arr)

    for i := len(arr)/2 - 1; i >= 0; i-- {
        heapify(arr, i, len(arr))
    }

    fmt.Println("arr after heapify:")
    fmt.Println(arr)

    sort(arr)

    fmt.Println("sorted:")
    fmt.Println(arr)
}
