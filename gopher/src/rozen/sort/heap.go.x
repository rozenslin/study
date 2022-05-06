// heap sort, by build arr into complete bin tree
// comp bin tree can always be into the linear arr
package main
import "fmt"

func swap(a, b *int) {
    if a == b {
        return
    }
    *a, *b = *b, *a
}

// make root the largest in current heap/tree
func heapify(arr []int, root int, n int) {
    // complete btree index of left/right from root index
    l := 2*root + 1
    r := 2*root + 2
    largest := root

    if l < n && arr[l] > arr[largest] {
        largest = l
    }
    if r < n && arr[r] > arr[largest] {
        largest = r
    }

    if largest != root {
        swap(&arr[largest], &arr[root])

        // update subtree 
        heapify(arr, largest, n)
    }
}

func heap_sort(arr []int) {
    // build heap
    for i := len(arr)/2 - 1; i >= 0; i-- {
        heapify(arr, i, len(arr))
    }
    fmt.Println("now heap:")
    fmt.Println(arr)

    // do sort, get the cur root as largest, then re-heapify, and go on
    for i := len(arr)-1; i > 0; i-- {
        // top to end, asc
        swap(&arr[0], &arr[i])

        // re-heapify from beginning
        heapify(arr, 0, i)
    }
}

func main() {
    arr := []int{7, 3, 1, 7, 8, 9, 5}
    fmt.Println("arr:")
    fmt.Println(arr)

    heap_sort(arr)

    fmt.Println("ans:")
    fmt.Println(arr)
}
