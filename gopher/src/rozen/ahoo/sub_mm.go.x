// sum of the min of all subarrays
// for an array, get all continuous subarrays,
// for each, get the val of (max - min)
// then sum up for all subarrays, as 'sum of imbalance val'
// so same as:
// get the sum of the min of all subarrays
// get the sum of the max of all subarrays
// ret = ret2 - ret1, since all possible min must be in ret1, so as max ones.
package main
import "fmt"
import "container/list"

type Node struct {
    val int
    cnt int
}

func sub_mm(arr []int) {
    n := len(arr)

    // using as stack
    smin := list.New()
    smax := list.New()

    // get the min sum of all subarrays
    // lmin[i] holds the number of all subarrays that has arr[i] as min
    lmin := make([]int, n)
    lmax := make([]int, n)
    for i := 0; i < n; i++ {
        // at least 1
        count := 1

        for smin.Len() != 0 {
            b := smin.Back()
            n := b.Value.(*Node)
            if n.val > arr[i] {
                // bigger one on left
                count += n.cnt
                // check next
                smin.Remove(b)
            } else {
                break
            }
        }

        lmin[i] = count
        // add into stack for next item check.
        node := &Node{arr[i], count}
        smin.PushBack(node)

        // for lmax
        count = 1
        for smax.Len() != 0 {
            b := smax.Back()
            n := b.Value.(*Node)
            if n.val < arr[i] {
                count += n.cnt
                smax.Remove(b)
            } else {
                break
            }
        }

        lmax[i] = count
        // add into stack for next item check.
        node = &Node{arr[i], count}
        smax.PushBack(node)
    }

    // rmin[i] holds the number of all subarrays that has arr[i] as min, from right side
    rmin := make([]int, n)
    rmax := make([]int, n)
    smin = list.New()
    smax = list.New()

    for i := n-1; i >= 0; i-- {
        // at least 1
        count := 1

        for smin.Len() != 0 {
            b := smin.Back()
            n := b.Value.(*Node)
            // only this >=, not both to avoid overlapping
            if n.val >= arr[i] {
                // bigger one on right
                count += n.cnt
                // check next
                smin.Remove(b)
            } else {
                break
            }
        }

        rmin[i] = count
        // add into stack for next item check.
        node := &Node{arr[i], count}
        smin.PushBack(node)

        // for rmax
        count = 1
        for smax.Len() != 0 {
            b := smax.Back()
            n := b.Value.(*Node)
            if n.val <= arr[i] {
                count += n.cnt
                smax.Remove(b)
            } else {
                break
            }
        }

        rmax[i] = count
        // add into stack for next item check.
        node = &Node{arr[i], count}
        smax.PushBack(node)
    }

    // get sum of min
    min_sum := 0
    for i := 0; i < n; i++ {
        min_sum += lmin[i] * rmin[i] * arr[i]
    }
    fmt.Println("lmin array:")
    fmt.Println(lmin)
    fmt.Println("rmin array:")
    fmt.Println(rmin)
    fmt.Printf("min sum: %d\n", min_sum)

    // get sum of max
    max_sum := 0
    for i := 0; i < n; i++ {
        max_sum += lmax[i] * rmax[i] * arr[i]
    }
    fmt.Println("lmax array:")
    fmt.Println(lmax)
    fmt.Println("rmax array:")
    fmt.Println(rmax)
    fmt.Printf("min sum: %d\n", max_sum)

    fmt.Printf("sum of max-min for all subarrays: %d\n", max_sum - min_sum)
}

func main() {
    arr := []int{1, 3, 2}
    fmt.Println("array:")
    fmt.Println(arr)
    sub_mm(arr)
}
