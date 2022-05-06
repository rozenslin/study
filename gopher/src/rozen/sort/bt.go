package main
import "fmt"
import "container/list"

type Node struct {
    val     int
    left    *Node
    right   *Node
}

func new_node(v int) *Node {
    node := new(Node)
    node.val = v
    return node
}

func insert(arr []int, node *Node, i int) *Node {
    if i < len(arr) {
        t := new_node(arr[i])

        t.left = insert(arr, t.left,  2*i+1)
        t.right = insert(arr, t.right, 2*i+2)

        // new node
        return t
    }

    // no new node, hit end
    return node
}

func invert(node *Node) {
    if node != nil {
        invert(node.left)
        invert(node.right)
        node.left, node.right = node.right, node.left
    }
}

// traverse nodes till specified one
func traverse(node *Node, v int, q *list.List) *Node {
    if node != nil {
        left := traverse(node.left, v, q)
        if left != nil {
            q.PushBack(node)
            return left
        }
        if node.val == v {
            q.PushBack(node)
            return node
        }
        right := traverse(node.right, v, q)
        if right != nil {
            q.PushBack(node)
            return right
        }
    }

    return nil
}

// reverse the tree path till node
func do_reverse(root *Node, v int) {
    q := list.New()
    traverse(root, 5, q)
    // get the path contents for reverse use
    contents := new([]int)
    // value from end/target to root
    for i := q.Front(); i != nil; i = i.Next() {
        val := i.Value.(*Node).val
        *contents = append(*contents, val)
    }
    i := 0
    for q.Len() != 0 {
        b := q.Back()
        q.Remove(b)
        n := b.Value.(*Node)
        // reverse the node vals
        n.val = (*contents)[i]
        i++
        // fmt.Printf("node addr: %p, val:%d, left: %p, right: %p\n", n, n.val, n.left, n.right)
    }
}

// build arr into complete bin tree
func bt(arr []int) *Node {
    root := insert(arr, nil, 0)

    return root
}

// order naming: where is root?
func inorder(node *Node, arr []int, i *int) {
    if node != nil {
        inorder(node.left, arr, i)
        arr[*i] = node.val
        *i++
        inorder(node.right, arr, i)
    }
}

func preorder(node *Node, arr []int, i *int) {
    if node != nil {
        arr[*i] = node.val
        *i++
        preorder(node.left, arr, i)
        preorder(node.right, arr, i)
    }
}

func postorder(node *Node, arr []int, i *int) {
    if node != nil {
        postorder(node.left, arr, i)
        postorder(node.right, arr, i)
        arr[*i] = node.val
        *i++
    }
}



func bfs(root *Node) {
    i := 0
    q := list.New()
    q.PushBack(root)
    for q.Len() != 0 {
        f := q.Front()
        q.Remove(f)
        n := f.Value.(*Node)

        fmt.Printf("bfs: got node :%d, addr: %p, val:%d, left: %p, right: %p\n", i, n, n.val, n.left, n.right)
        i++

        if n.left != nil {
            q.PushBack(n.left)
        }
        if n.right != nil {
            q.PushBack(n.right)
        }
    }
}

func main() {
    arr := []int{1, 2, 3, 4, 5, 6}
    root := bt(arr)

    fmt.Println("arr:")
    fmt.Println(arr)

    bfs(root)

    ans := make([]int, len(arr))
    i := 0
    inorder(root, ans, &i)

    fmt.Println("inorder ans:")
    fmt.Println(ans)

    i = 0
    preorder(root, ans, &i)
    fmt.Println("preorder ans:")
    fmt.Println(ans)

    i = 0
    postorder(root, ans, &i)
    fmt.Println("postorder ans:")
    fmt.Println(ans)

    fmt.Println("bfs after invert:")
    invert(root)
    bfs(root)

    // invert back
    invert(root)

    do_reverse(root, 5)
    fmt.Println("bfs after reverse to 5:")
    bfs(root)
}
