// Subject to the terms of the Mozilla Public License, v. 2.0, see https://mozilla.org/MPL/2.0/.
// Free for non-commercial use, contact owner for commercial use.
package main
import "container/list"
import "fmt"
import "strings"

// demo for the Aho-Corasic algorithm.
// Problem:
//   given a list of string patterns, (P1, P2, ...Pk), such as
//         'in', 'tin', 'tina', 'sting'; with total len = n
//   if input a string text T, len(T) = m,
//      output all the matched occurrence of patterns from it,
//      e.g, if T is 'stina', the output will show occurrence of 'in, tin, tina', not 'sting'
//
// Design:
//    Key Aim: after preprocess, one traverse of T to get all result.
// O. instead of naive way: O(n*m), and linear KMP way: O(k*m + n), we generate a trie
//    to store the patterns, and an automaton (algorithm robot) to work upon it 
//    to do the search work
//
// I. create an automaton (finite state machine) that contains 3 functions:
//    1. goto: build the trie and traverse it for the Text
//    2. suffix/failure: jump to the 'next longest suffix' when current goto path ended/failed,
//               thus, we don't need to restart from root again,
//               or else, the search time will be O(Lmax*m) (Lmax is the max len in patterns)
//    3. output: this is to show all possible match during 'goto', e.g, if Px is substring of Py,
//               when goto finds match of 'Py', it also means match of 'Px'
//               or else, the search time will be O(m + n)
// II. use that automaton to search T for matches of patterns, thus total time cost: O(n + m + z), 
//     with z as 'number of total match'
//
// Time cases:
// 1. naive way:                    O(n*m)
// 2. KMP way:                      O(k*m+n)
// 3. build trie:                   O(n)
// 4. search trie without suffix:   O(Lmax*m)
//    it's like: 
//             foreach the T over the trie;
//                  output the matched nodes
//                  break if hit path end or found not matched nodes
//             then restart foreach again for T[1:], then again for T[2:], ...
//    so the worst case is like one longest path/pattern always matches;
//    thus time will be Lmax * (m-Lmax) = O(Lmax * m) (suppose m >> Lmax)
// 5. search trie without output:   well, without this, the total algorithm does not work,
//    as we cannot do like when after 4, foreach T again for the unchecked path, means
//    the benefit of 4 does not work.
// 6. search trie with both:        O(m+z), z is number of matches (the jump number of output links)
// 7. total time:                   O(n+m+z)

// node for the trie
type Node struct {
    // child nodes info, each as {char, node address}, for goto function 
    child map[rune]*Node
    // link to the suffix node, for suffix function
    suffix *Node
    // link to the output node, for output function
    output *Node
    // pattern index, is current node is the pattern end?
    index int
}

func add_node() *Node {
    node := new(Node)
    node.child = make(map[rune]*Node)
    node.suffix = nil
    node.output = nil
    node.index = -1
    return node
}

// build the trie by adding patterns into nodes,
func build_trie(patterns []string) *Node {
    root := add_node()
    // go thru all chars in patterns, so in total: O(n)
    for i, str := range patterns {
        // 1. start from root
        cur := root
        // 2. foreach all chars in string
        for _, ch := range str {
            // 2.1 if char already in trie, follow it
            if node, ok := cur.child[ch]; ok {
                cur = node
                continue
            }

            // 2.2 else create a new node and add into trie
            node := add_node()
            cur.child[ch] = node
            cur = node
        }

        // 3. mark pattern ending
        cur.index = i
    }

    // return the root
    return root
}

// add the suffix and output links to trie
// time: O(n)
func suffix_n_output(root *Node) {
    // in go, use linked list as a queue (FIFO)
    q := list.New()

    // for root children 
    // the suffix links are just root
    // the output links are nil
    root.suffix = root
    for _, v := range root.child {
        v.suffix = root
        q.PushBack(v)
    }

    // do breadth first search (thru queue) for nodes in trie
    // 1. pop from node, remove from queue 
    // 2. set suffix links for children and push back them into queue
    // 3. set output link if found 
    // 1. to 1. again, till all nodes checked
    for q.Len() != 0 {
        // 1. pop out the front one
        front := q.Front()
        cur := front.Value.(*Node)
        q.Remove(front)
        // 2. add suffix for all children
        for k, v := range cur.child {
            // 2.0 start from parent suffix 
            temp := cur.suffix

            // 2.1 the child's suffix link must be:
            //       the child of parent's suffix link
            // or
            //       the child of (parent's suffix link)'s suffix link...
            node, ok := temp.child[k]
            for !ok && temp != root {
                temp = temp.suffix
                node, ok = temp.child[k];
            }

            // 2.2 if found suffix, add it, else link back to root
            if ok {
                v.suffix = node
            } else {
                v.suffix = root
            }

            // 2.3 enque the node
            q.PushBack(v)
        }

        // 3. add output, the output link must be:
        //    its suffix link or suffix link's output link...
        if cur.suffix.index >= 0 {
            cur.output = cur.suffix
        } else {
            cur.output = cur.suffix.output
        }
    }
}

// dump the trie
func bfs_dump(root *Node) {
    x := root
    i := 1
    q := list.New()

    // root
    fmt.Printf("Node %2d:    root: %p, \tsuffix: %p, \tindex: %d, \toutput: %p\n", i, x, x.suffix, x.index, x.output)
    fmt.Printf("\t\t\tchild:")
    for k, v := range x.child {
        fmt.Printf("(%s,  %p), ", string(k), v)
        q.PushBack(v)
    }
    fmt.Println()
    i++

    for q.Len() != 0 {
        n := q.Front()
        x := n.Value.(*Node)
        q.Remove(n)

        fmt.Printf("Node %2d:    node: %p, \tsuffix: %p, \tindex: %d, \toutput: %p\n", i, x, x.suffix, x.index, x.output)
        fmt.Printf("\t\t\tchild:")
        for k, v := range x.child {
            fmt.Printf("(%s,  %p), ", string(k), v)
            q.PushBack(v)
        }
        fmt.Println()
        i++
    }
}

// search the text in the trie, and store the match result
// time: O(m+z)
func search_match(text string, root *Node, result *map[int][]int) {
    // initial node/state
    cur := root
    // not use range, see 2.1 case
    for i := 0; i < len(text); i++ {
        ch := rune(text[i])
        // 1. 'goto(current, ch)': traverse the trie for text (state)
        if node, ok := cur.child[ch]; ok {
            cur = node
            // 3. 'output(cur, ch)': add all matches into result
            if node.index >= 0 {
                (*result)[node.index] = append((*result)[node.index], i)
            }

            t := node.output
            for t != nil {
                (*result)[t.index] = append((*result)[t.index], i)
                t = t.output
            }

            continue
        }

        // 2. 'suffix/failure(cur, ch)': follow the suffix links for next (state)
        cur = cur.suffix
        _, ok := cur.child[ch]
        for cur != root && !ok {
            cur = cur.suffix
            _, ok = cur.child[ch]
        }

        // 2.1 back one step for 'goto' on such ch again
        if ok {
            i--
        }
    }
}

func main() {
    // patterns, total len = n
    //patterns := []string{"in", "no", "tin", "tina", "tino", "sting"}
    // text, len = m
    //text := "stinoin"

    patterns := []string {"ab", "about",  "ate", "at", "bed", "edge", "get"} 
    text := "abedgeget"

    root := build_trie(patterns)

    suffix_n_output(root)

    bfs_dump(root)

    // result map, element as {pattern index: {text char pos1, pos2...}}
    result := make(map[int][]int)

    // finite state automaton, means, given the input, it outputs finite states/results
    search_match(text, root, &result)

    // print result
    fmt.Printf("Patterns: \t%s\n", strings.Join(patterns[:], ", "))
    fmt.Printf("Match text: \t%s\n", text)
    for k, v := range result {
        fmt.Printf("Pattern: \t%s occurs at pos:\t", patterns[k])
        for _, pos := range v {
            fmt.Printf(" %d, ", pos)
        }
        fmt.Println()
    }
}
