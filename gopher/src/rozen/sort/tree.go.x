// binary tree sort
package main
import "fmt"

type Node struct {
    val int
    left *Node
    right *Node
}

func add_node(v int) *Node {
    node := new(Node)
    node.val = v
    return node
}

func insert(node *Node, v int) *Node {
    // new node
    if node == nil {
        return add_node(v)
    }

    if v < node.val {
        // less, to left
        node.left = insert(node.left, v)
    } else {
        // or to right
        node.right = insert(node.right, v)
    }

    // updated node
    return node
}

// sort by in-order traverse
// i as shared index
// arr as shared result arr
func sort(node *Node, arr []int, i *int) {
    if node != nil {
        // do left
        sort(node.left, arr, i)
        // left done, now root
        arr[*i] = node.val
        *i++
        // now right
        sort(node.right, arr, i)
    }
}

// btree, with in-order as left < root <= right
func btree(arr []int) *Node {
    // create root
    root := insert(nil, arr[0])
    for _, v := range arr[1:] {
        // insert from root
        insert(root, v)
    }

    return root
}

func main() {
    arr := []int{7, 3, 1, 7, 8, 9, 5}
    root := btree(arr)
    ans := make([]int, len(arr))
    i := 0
    sort(root, ans, &i)
    fmt.Println("arr:")
    fmt.Println(arr)
    fmt.Println("sorted:")
    fmt.Println(ans)
}
