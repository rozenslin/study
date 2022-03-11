// demo for Dijkstra algo., as 'get shortest path from city A to city B'
// given a list of nodes and a list of edges connecting them with diff distance, 
// count the shortest path from node A to node B
package main
import "fmt"

const DIST_MAX int = 65536

type Node struct {
    // current Node/city id
    id string
    // distance to start Node/city
    dist int

    // prev node to get shortest path to start point
    prev *Node
}

// the road/edge btwn two nodes/cities
type Edge struct {
    id1 string
    id2 string
    dist int
}

// XXX dup for node.id and k, but the lazy way...
func gen_nodes(node_list []string) map[string]*Node {
    nodes := make(map[string]*Node)
    for _, v := range node_list {
        node := &Node{v, DIST_MAX, nil}
        if v == "a" {
            // start point
            node.dist = 0
        }

        nodes[v] = node
    }
    return nodes
}

// get the node from nodes that has smallest 'distance', and remove it from nodes.
func get_smallest(nodes *map[string]*Node) *Node {
    var smallest *Node = nil
    for _, v := range *nodes {
        if smallest == nil {
            smallest = v
        } else if smallest.dist > v.dist {
            smallest = v
        }
    }

    delete(*nodes, smallest.id)
    fmt.Printf("calc current node: %s, dist to start node:%d\n", smallest.id, smallest.dist)
    return smallest
}

// get the nodes connected to current one
// calc their smallest dist to start node and update
func calc_conn_nodes(nodes map[string]*Node, node *Node, roads[]Edge) {
    var conn_id string

    for _, e := range roads {
        if e.id1 == node.id {
            conn_id = e.id2
        } else if e.id2 == node.id {
            conn_id = e.id1
        } else {
            continue
        }

        // if the connected node still in map, update its dist
        if n, ok := nodes[conn_id]; ok {
            dist := e.dist + node.dist
            if dist < n.dist {
                n.dist = dist
                n.prev = node
            }
        }
    }
}

// get shortest path from this Node to start Node
func get_path(n *Node) {
    for n.prev != nil {
        fmt.Printf("Current node: %s, dist to start node:%d\n", n.id, n.dist)
        n = n.prev
    }
    fmt.Printf("Current node: %s, dist to start node:%d\n", n.id, n.dist)
}

// total time O(E*V^2)
// if get_smallest using min prior queue, then time will be
// O(E*LogV) (E >> V)
func do_dij(nodes *map[string]*Node, roads []Edge) {
    for len(*nodes) != 0 {
        node := get_smallest(nodes)
        calc_conn_nodes(*nodes, node, roads)
    }
}


func main() {
    node_list := []string{"a", "b", "c", "d", "e", "f"}
    roads := []Edge {
        {"a", "b", 3},
        {"a", "c", 3},
        {"b", "c", 2},
        {"b", "d", 1},
        {"b", "e", 2},
        {"b", "f", 7},
        {"d", "f", 1},
        {"d", "e", 2},
        {"e", "f", 1},
    }

    nodes := gen_nodes(node_list)

    node := nodes["f"]

    do_dij(&nodes, roads)

    get_path(node)
}
