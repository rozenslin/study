package main
import "fmt"
func main() {
    fmt.Println("ahoooooooooooooooooooo go!")
    for i := 1; i <= 12; i++ {
        for j := 1; j <= 12; j++ {
            fmt.Printf("%d\t", i*j)
        }
        fmt.Println("")
    }
}
