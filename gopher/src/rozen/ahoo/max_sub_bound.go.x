// nums = [2, 1, 4, 3] left = 2 right = 3
// find all subarrs that has max with [left, right] range, count the number
package main
import "fmt"

func find_max_bound(arr []int, left, right int) int {
    // mid to sum up subarrs <= right
    // low to exclude subarrs < left
    ans, low, mid := 0, 0, 0
    for _, v := range arr {
        if v > right {
            mid = 0
        } else {
            mid++
            ans += mid
        }
        if v >= left {
            low = 0
        } else {
            low++
            ans -= low
        }
    }

    return ans
}

func main() {
    // nums := []int {2, 1, 4, 3}
    // left, right := 2, 3
    nums := []int {2,9,2,5,6}
    left, right := 2, 8
    count := find_max_bound(nums, left, right)
    fmt.Println("arr:")
    fmt.Println(nums)
    fmt.Printf("ans:%d \n", count)
}
