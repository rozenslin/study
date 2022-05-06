// [1, [2,4], [3,[5]]] => Depth-sum: 1*1 + 2*2 + 4*2 + 3*2 + 5*3 = 34
package main
import "fmt"
type ItemArray []interface{}
func get_depsum(item interface{}, sum *int, depth int) {
    switch item.(type) {
    case int:
        // got the number, calc and sum up
        *sum += depth * item.(int)

    case ItemArray:
        // got the array, increase the depth and go into array
        depth++

        s := item.(ItemArray)
        for _, v := range s {
            get_depsum(v, sum, depth)
        }

    default:
        fmt.Errorf("unknown item got, type: %T, value: %V\n", item, item)
    }
}

func main() {
    // using interface{} as auto type
    arr := ItemArray {
        1,
        ItemArray {2, 4},
        ItemArray {
            3,
            ItemArray {5},
        },
    }

    fmt.Println("orig nested array:")
    fmt.Println(arr)
    // this shared by all calling
    sum := 0

    // this from callers into callees 
    depth := 0

    get_depsum(arr, &sum, depth)

    fmt.Printf("final Depth sum: %d\n", sum)
}
