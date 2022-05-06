// input 'a1b2' output ['a1b2', 'A1b2', 'a1B2', 'A1B2']
package main
import "fmt"
import . "unicode"

// XXX ans ptr??
func dfs(arr []rune, ans *[]string, i int) {
    if i < len(arr) {
        dfs(arr, ans, i+1)

        ch := arr[i]
        // if letter add altered case
        if IsLetter(ch) {
            arr[i] = func (c rune) rune {
                if IsUpper(c) {
                    return ToLower(c)
                } else {
                    return ToUpper(c)
                }
            } (ch)

            dfs(arr, ans, i+1)
        }
    } else {
        // complte one string, add it
        *ans = append(*ans, string(arr))
    }
}

func main() {
    // str is read-only
    s := "a1b2"
    // into rune slice to alter
    arr := []rune(s)

    ans := []string{}
    dfs(arr, &ans, 0)
    fmt.Printf("orig string: %s\n", s)
    fmt.Println("result:")
    fmt.Println(ans)
}
