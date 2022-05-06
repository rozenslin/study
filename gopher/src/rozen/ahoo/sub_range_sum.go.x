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

// trade space with time
// O(3n): O(n)
func sub_min_max(arr []int) int {
    n := len(arr)

    // use as stack 
    qmin := list.New()
    qmax := list.New()

    lmin := make([]int, n)
    rmin := make([]int, n)
    lmax := make([]int, n)
    rmax := make([]int, n)

    for i := 0; i < n ; i++ {
        // lmin
        cnt := 1
        for qmin.Len() != 0 {
            f := qmin.Back()
            n := f.Value.(*Node)
            if n.val > arr[i] {
                qmin.Remove(f)
                cnt += n.cnt
                continue
            } else {
                break
            }
        }

        // add this one
        lmin[i] = cnt
        node := &Node{arr[i], cnt}
        qmin.PushBack(node)

        // lmax
        cnt = 1
        for qmax.Len() != 0 {
            f := qmax.Back()
            n := f.Value.(*Node)
            // found all smallers left
            if n.val < arr[i] {
                qmax.Remove(f)
                cnt += n.cnt
                continue
            } else {
                break
            }
        }

        // add this one
        lmax[i] = cnt
        node = &Node{arr[i], cnt}
        qmax.PushBack(node)
    }

    qmin = list.New()
    qmax = list.New()
    for i := n - 1; i >= 0; i-- {
        // rmin
        cnt := 1
        for qmin.Len() != 0 {
            f := qmin.Back()
            n := f.Value.(*Node)
            // only one side >=
            if n.val >= arr[i] {
                qmin.Remove(f)
                cnt += n.cnt
                continue
            } else {
                break
            }
        }

        // add this one
        rmin[i] = cnt
        node := &Node{arr[i], cnt}
        qmin.PushBack(node)

        // rmax
        cnt = 1
        for qmax.Len() != 0 {
            f := qmax.Back()
            n := f.Value.(*Node)
            // only one side <=
            if n.val <= arr[i] {
                qmax.Remove(f)
                cnt += n.cnt
                continue
            } else {
                break
            }
        }

        // add this one
        rmax[i] = cnt
        node = &Node{arr[i], cnt}
        qmax.PushBack(node)
    }

    fmt.Println("left min arr:")
    fmt.Println(lmin)
    fmt.Println("right min arr:")
    fmt.Println(rmin)

    fmt.Println("left max arr:")
    fmt.Println(lmax)
    fmt.Println("right max arr:")
    fmt.Println(rmax)

    // now the result is from a[i]*left[i]*right[i]
    sum_min := 0
    sum_max := 0
    for i := 0; i < n; i++ {
        sum_min += arr[i] * lmin[i] * rmin[i]
        sum_max += arr[i] * lmax[i] * rmax[i]
    }

    fmt.Printf("min sum: %d\n", sum_min)
    fmt.Printf("max sum: %d\n", sum_max)

    return sum_max - sum_min
}

func main() {
    // 4
    // arr := []int{1, 2, 3}
    // 5 arr := []int{1, 3, 2}
    arr := []int{3, 5, 8, 2}
    fmt.Println("array:")
    fmt.Println(arr)

    ret := sub_min_max(arr)

    fmt.Printf("ret: %d\n", ret)
}
