/* Top K Frequent Elements
 Input: nums = [1,1,1,2,2,3], k = 2
Output: [1,2]
    arr = []int {7, 10, 11, 5, 2, 5, 5, 7, 11, 8, 9}
    k := 4
*/
// hashmap to store item,count
// then sort the map by value, desc, and get the first k items.item
package main
import "fmt"
import "sort"
func find_topk(arr []int, k int, ret *[]int) {
    // k,v as value,count from arr
    items := make(map[int]int)
    // gen the map from arr
    for _, v := range arr {
        items[v]++
    }

    // check invalid k
    if len(items) < k {
        fmt.Errorf("Invalid k:%d\n", k)
        return
    }
    fmt.Printf("map: \t")
    fmt.Println(items)

    // sort the keys of map by the count value
    keys := make([]int, 0)
    for key := range items {
        keys = append(keys, key)
    }

    fmt.Printf("keys: \t")
    fmt.Println(keys)

    sort.SliceStable(keys,
               func (i, j int) bool {
                   // > for desc sorting
                   return items[keys[i]] > items[keys[j]]
               })

    fmt.Printf("sorted keys: \t")
    fmt.Println(keys)
    // now output the initial/top k (0~k-1) from keys
    *ret = keys[0:k]
}

func main() {
    // Array: [1, 3, 2, 6, -1, 4, 1, 8, 2], K=5
    arr := []int { 3, 1, 4, 4, 5, 2, 6, 1 }
    arr = []int {7, 10, 11, 5, 2, 5, 5, 7, 11, 8, 9}
    k := 4
    fmt.Printf("arr: \t")
    fmt.Println(arr)
    fmt.Printf("k: \t%d\n", k)

    ret := new([]int)
    find_topk(arr, k, ret)

    fmt.Printf("result arr: \t")
    fmt.Println(*ret)
}

