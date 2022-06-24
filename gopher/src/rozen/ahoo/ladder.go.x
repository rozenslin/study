package main
import "fmt"
import "container/list"

type Word struct {
    word string
    level int
}
func new_word(word string, level int) *Word {
    w := new(Word)
    w.word = word
    w.level = level
    return w
}

func get_ladder_len(start, target string, dict map[string]int) int {
    if start == target {
        return 0
    }
    if _, ok := dict[target]; !ok {
        return 0
    }

    // do bfs check start against the dict, till target found or dict traversed.
    q := list.New()
    q.PushBack(new_word(start, 0))

    for q.Len() != 0 {
        // pop out current one
        f := q.Front()
        q.Remove(f)
        w := f.Value.(*Word)
        s := []rune(w.word)

        // new level
        level := w.level + 1

        // alter each char to found matches from dict
        for i := 0; i < len(s); i++ {
            ch := s[i]
            for l := 'a'; l <= 'z'; l++ {
                if l == ch {
                    continue
                }

                s[i] = l
                alter := string(s)

                if alter == target {
                    fmt.Printf("ahoooooooooo, found target   '%s' in dict for '%s', level: %d\n", alter, w.word, level+1)
                    return level+1
                }

                if _, ok := dict[alter]; ok {
                    // found adjacent one, remove from dict and push to queue
                    delete(dict, alter)

                    fmt.Printf("ahoooooooooo, found adjacent '%s' in dict for '%s', level: %d\n", alter, w.word, level)
                    q.PushBack(new_word(alter, level))
                }

                // revert ch
                s[i] = ch
            }
        }
    }


    // here means cannot found the ladder
    return 0
}

// if dict is not huge, this better
// if dict is huge, this is not...
/*
bool isOneDiff(a, b string) {
    if len(a) != len(b) {
        return false
    }
    diff := 0
    for i := 0; i < len(a); i++ {
        if a[i] != b[i] {
            diff++
        }
        if diff > 1 {
            return false
        }
    }

    if diff == 1 {
        return true
    }

    return false
}
*/

func main() {
    /*
    dict :{poon, plee, same, poie, plea, plie, poin}, start = toon, target = plea
    Output: 7
    */
    words := []string {
        "hot","dot","dog","lot","log","cog",
        // "hot","dot","dog","lot","log",
    }
    start := "hit"
    target := "cog"
    /*
    words := []string {"poon", "plee", "same", "poie", "plea", "plie", "poin"}
    start := "toon"
    target := "plea"
    */
    // map it as dict[string] = level
    dict := make(map[string]int)
    for _, v := range words {
        dict[v] = 0
    }

    lad_len := get_ladder_len(start, target, dict)
    fmt.Printf("dict:\t")
    fmt.Println(words)
    fmt.Printf("start:\t")
    fmt.Println(start)
    fmt.Printf("target:\t")
    fmt.Println(target)
    fmt.Printf("ladder len:\t")
    fmt.Println(lad_len)
}
