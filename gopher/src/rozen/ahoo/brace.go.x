// '{}[]': true '{[}]' false, must matches, and in correct order
// 3:19PM 3:28pm
package main
import "fmt"
import "container/list"

func check_match(left, right rune) bool {
    var str string
    str += string(left)
    str += string(right)

    switch str {
    case "{}", "[]", "()":
        return true

    default:
        return false
    }

    return false
}

func check_str(str string) bool {
    // stack, LIFO
    stack := list.New()

    for _, ch := range str {
        switch ch {
        case '{', '[', '(':
            // open one, push in
            stack.PushBack(ch)
        case '}', ']', ')':
            // close one, check and pop out
            if stack.Len() == 0 {
                return false
            }

            b := stack.Back()
            left := b.Value.(rune)
            if check_match(left, ch) {
                stack.Remove(b)
            } else {
                return false
            }
        }
    }

    // check stack
    if stack.Len() == 0 {
        return true
    }

    return false
}

func main() {
    s := "}[{}]{}[[]]"
    ret := check_str(s)

    fmt.Printf("string: %s, result: %v\n", s, ret)
}
