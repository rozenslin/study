// maze demo, 1 go 0 block, 9 target.
// 10:27AM 10:41am
package main
import "fmt"
import "container/list"

type Node struct {
    x int
    y int
    // dist to src node
    d int
}

func main() {
    const ROW int = 5
    const COL int = 5
    matrix := [ROW][COL]int {
        {1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 0, 1},
        {0, 0, 0, 1, 1},
        {1, 1, 9, 1, 0},
    }

    moves := [][]int{
        {-1, 0},
        {1,  0},
        {0,  -1},
        {0,  1},
    }
    visited := new([ROW][COL]bool)
    src := &Node{0, 0, 0}

    q := list.New()
    q.PushBack(src)
    visited[0][0] = true

    found := false
    for q.Len() != 0 {
        // pop out cur one
        f := q.Front()
        q.Remove(f)
        p := f.Value.(*Node)

        // if found
        if matrix[p.x][p.y] == 9 {
            found = true
            fmt.Printf("found target at (%d, %d) with dist as: %d\n", p.x, p.y, p.d)
            break
        }

        // check adj. nodes
        for _, v := range moves {
            x := p.x + v[0]
            y := p.y + v[1]

            if x < 0 || x > ROW - 1 || y < 0 || y > COL - 1 || visited[x][y] || matrix[x][y] == 0 {
                continue
            }

            // add new node
            node := &Node{x, y, p.d + 1}
            visited[x][y] = true
            q.PushBack(node)
        }
    }

    if !found  {
        fmt.Printf("cannot find target node\n")
    }
}
