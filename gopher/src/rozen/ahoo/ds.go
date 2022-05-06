// [1, [2,4], [3,[5]]] => Depth-sum: 1*1 + 2*2 + 4*2 + 3*2 + 5*3 = 34
// 4:51 458
package main
import "fmt"
func get_ds(item interface{}, sum *int, depth int) {
    switch item.(type) {
    case int:
        *sum += depth * item.(int)
    case []interface{}:
        s := item.([]interface{})
        for _, v := range s {
            get_ds(v, sum, depth+1)
        }
    }
}

func main() {
    items := []interface{} {
        1,
        []interface{} {2, 4},
        []interface{} {
            3,
            []interface{} {5},
        },
    }
    sum := 0
    get_ds(items, &sum, 0)

    fmt.Println("items:")
    fmt.Println(items)
    fmt.Println("Depth-sum ans:")
    fmt.Println(sum)
}
