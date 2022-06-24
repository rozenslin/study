package main
import "fmt"
import "container/heap"
import "sort"

// An IntHeap is a min-heap of ints.
type IntHeap []int

func (h IntHeap) Len() int           { return len(h) }
func (h IntHeap) Less(i, j int) bool { return h[i] < h[j] }
func (h IntHeap) Swap(i, j int)      { h[i], h[j] = h[j], h[i] }

// note the heap is doing heap sorting internally,
// so must use like heap.Push(h..) and heap.Pop(h)
// which will do the sorting work plus calling into here
func (h *IntHeap) Push(x interface{}) {
	// Push and Pop use pointer receivers because they modify the slice's length,
	// not just its contents.
	*h = append(*h, x.(int))
}

func (h *IntHeap) Pop() interface{} {
	old := *h
	n := len(old)
	x := old[n-1]
	*h = old[0 : n-1]
	return x
}

type Meeting struct {
    start int
    end int
}

func main() {
    // intervals := {{9,10},{4,9},{5,17}}
    meetings := []Meeting {
        {9, 10},
        {4, 10},
        {6, 9},
        {5, 8},
        {9, 14},
    }

    // sort asc by start time
    sort.Slice(meetings, func(i, j int) bool { return meetings[i].start <= meetings[j].start })

    // now go thru the slice, add the end time into the min heap
    h := new(IntHeap)
    heap.Init(h)
    for _, v := range meetings {
        if h.Len() != 0 {
            top := heap.Pop(h).(int)
            if v.start < top {
                // not done, push again
                heap.Push(h, top)
            }
        }
        heap.Push(h, v.end)
    }

    // the final heap size is the max meeting room needed.
    fmt.Println("total rooms needed: ", h.Len())
}
