// Golang program to illustrate the
// use of strings.Join Function

package main

// importing fmt and strings
import (
    "fmt"
    "strings"
)

// calling main method
func main() {

    // array of strings.
    msg := []string{"we", "say", ":", "yes"}
    // encode
    encode := strings.Join(msg, ":;")
    decode := strings.Split(encode, ":;")

    fmt.Println("orig msg:\t", msg)
    fmt.Println("encoded msg:\t", encode)
    fmt.Println("dec msg:\t", decode)
}
