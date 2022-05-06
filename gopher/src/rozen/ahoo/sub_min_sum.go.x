// given an int array, get the sum of the min of all subarrays
package main
import "fmt"
import "container/list"
type Node struct {
    val int
    cnt int
}

func get_sub_min_sum(arr []int) int {
    n := len(arr)
    left := make([]int, n)
    right := make([]int, n)

    // use this as stack
    q := list.New()

    // left[i] holds number of items > arr[i]
    // using stack as:
    // like 321 when checking for 1, must check back to 2 then 3
    // to enlarge the possible count of subarrays that contains 1.
    for i := 0; i < n; i++ {
        cnt := 1
        for q.Len() != 0 {
            b := q.Back()
            n := b.Value.(*Node)
            if n.val > arr[i] {
                // found bigger one, add the cnt
                q.Remove(b)
                cnt += n.cnt
                continue
            } else {
                break
            }
        }

        n := &Node{arr[i], cnt}
        q.PushBack(n)
        left[i] = cnt
    }

    // right[i] holds number of right items >= arr[i]
    // only one side be >= to avoid overlapping like 311 into both left[] and right[] 
    q = list.New()
    for i := n-1; i >= 0; i-- {
        cnt := 1
        for q.Len() != 0 {
            b := q.Back()
            n := b.Value.(*Node)
            if n.val >= arr[i] {
                // found bigger one, add the cnt
                q.Remove(b)
                cnt += n.cnt
                continue
            } else {
                break
            }
        }

        n := &Node{arr[i], cnt}
        q.PushBack(n)
        right[i] = cnt
    }

    fmt.Println("left arr:")
    fmt.Println(left)
    fmt.Println("right arr:")
    fmt.Println(right)

    // total sum of subarrays min that has arr[i] as min is:
    // left[i]*right[i]*arr[i]
    // tricky as [31, 1] v [1, 12], in total 4 subarrays...
    ret := 0
    for i := 0; i < n ; i++ {
        ret += arr[i] * left[i] * right[i]
    }

    return ret
}

func main() {
    // arr := []int {3, 1, 2}
    arr := []int {3, 1, 2, 4}
    fmt.Println("arr:")
    fmt.Println(arr)
    ret := get_sub_min_sum(arr)

    fmt.Printf("ret: %d\n", ret)
}
