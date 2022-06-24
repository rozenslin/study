package main
import "fmt"
import "sort"
/*
Given an array of integers and a value k, find the length of largest subarray having a sum greater than k.
Examples: 
Input : arr[] = {-2, 1, 6, -3}, k = 5
Output : 2
Largest subarray with sum greater than
5 is  {1, 6}.

Input : arr[] = {2, -3, 3, 2, 0, -1}, k = 3
Output : 5
Largest subarray with sum greater than
3 is {2, -3, 3, 2, 0}.
*/

// items for auxillary slice
type Item struct {
    // sum so far
    val int
    // index so far
    ind int
}
type Items []Item
// implement interface for sort use

func (l Items) Len() int {
    return len(l)
}

func (l Items) Less(i, j int) bool {
    if l[i].val == l[j].val {
        return l[i].ind < l[j].ind
    }

    return l[i].val < l[j].val
}

func (l Items) Swap(i, j int) {
    l[i], l[j] = l[j], l[i]
}
func min(a, b int) int {
    if a <= b {
        return a
    }

    return b
}

// this works by:
// if sum of arr[0:i] <= k;
//    to get arr[x+1:i] > k; which means to get sumx of arr[0:x] <= sum-k-1
//    so we need to find from sorted aux for such Item
// also if found such aux[x].val <= sum-k-1, which shall be the largest one qualified,
// it means, all aux[0...x-1] qualified too, now the minIndex[x] will be 
// 'the min index, such as y, that as aux[z] == sum(arr[0:y]) qualified, and 
//  this y will be smallest index, to get the longest subarray range for caller'
func findIndex(aux []Item, v int) int {
    // do binary search for sorted items
    start := 0
    end := len(aux)-1

    var mid int
    ans := -1
    for start <= end {
        mid = (end+start)/2
        if aux[mid].val > v {
            end = mid-1
        } else {
            // cannot return now, need to go on 
            // since there may be multi same vals in aux
            ans = mid
            start = mid+1
        }
    }

    return ans
}

// return the index range of found subarray for such requirement
func find_sub(arr []int, k int) (int, int) {
    // first, gen the aux slice
    sum := 0
    aux := make([]Item, len(arr))
    for i, v := range arr {
        sum += v
        aux[i] = Item{sum, i}
    }
    // second, sort the slice
    sort.Sort(Items(aux))
    for i, v := range aux {
        fmt.Printf("aux item:%d,  val:%d, ind:%d\n", i, v.val, v.ind)
    }

    // minIndex[i] stores the min index from aux array so far,
    // see findIndex for usage.
    minIndex := make([]int, len(arr))
    minIndex[0] = aux[0].ind
    for i := 1; i < len(aux); i++ {
        minIndex[i] = min(minIndex[i-1], aux[i].ind)
    }

    // then, check the sum against k 
    sum = 0
    a, b := -1, -1
    max_len := 0
    for i, v := range arr {
        sum += v
        // already found one qualified subarray 
        if sum > k {
            max_len = i+1
            a, b = 0, i
        } else {
            // found the qualified subarray by findIndex
            ind := findIndex(aux, sum-k-1)

            if ind != -1 && minIndex[ind] < i && max_len < i-minIndex[ind] {
                max_len = i-minIndex[ind]
                a, b = minIndex[ind]+1, i
            }
        }
    }

    // return a, b range...
    return a, b
}

func main () {
     arr := []int {-2, 1, 6, -3} // k 5
     arr = []int {2, -3, 3, 2, 0, -1}
     k := 3

     a, b := find_sub(arr, k)
     fmt.Printf("array:\t")
     fmt.Println(arr)
     fmt.Printf("k:\t")
     fmt.Println(k)

     if a != -1 {
         fmt.Printf("found max subarray range: len:%d, index:[%d:%d] as\t", b-a+1, a, b)
         fmt.Println(arr[a:b+1])
     } else {
         fmt.Printf("found no such subarray\n")
     }
}
