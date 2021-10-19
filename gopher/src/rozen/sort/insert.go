package main
import "fmt"
func main() {
    msg := "A gopher named Desire"
    fmt.Printf("Hello gopher! %s!\n", msg)

    arr := [...]int{4, 42, 12, 762, 9, 1572, 235}
    fmt.Printf("arr before sort: %v\n", arr)

    l := len(arr)
    for i := 1; i < l; i++ {
        for j := i-1; j >= 0; j-- {
            if arr[j] > arr [j+1] {
                t := arr[j]
                arr[j] = arr[j+1]
                arr[j+1] = t
            }
        }
    }
    fmt.Printf("arr after sort: %v\n", arr)
}
