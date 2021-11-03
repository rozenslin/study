package main
import (
    "fmt"
    . "rozen/shape"
)

func main() {
    circle := NewCircle(2.5)
    rect := NewRect(2.5, 4)
    fmt.Printf("Total area: %f\n", Total(circle, rect))
    // fmt.Printf("Total area: %f\n", Total(*circle, *rect))
    fmt.Printf("Total area: %f\n", (*circle).Area() + (*rect).Area())
    fmt.Printf("Total area: %f\n", circle.Area() + rect.Area())
    return
}
