package main
import "fmt"
import "rozen/wahoo"
func main() {
    msg := "A gopher named Desire"
    msg, _ = wahoo.Double_msg(msg)

    fmt.Printf("Hello gopher!\n %s\n", msg)
}
