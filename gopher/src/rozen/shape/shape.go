package shape
import "math"

type Shape interface {
    area() float64
}

func Total(shapes... Shape) float64 {
    var v float64
    for _, s := range shapes {
        v += s.area()
    }

    return v
}

type Circle struct {
    r float64
    string
}

func NewCircle(r float64) *Circle {
    c := Circle{r: r}
    return &c
}

// val receiver, so here always having a val copy,
// even caller using ptr, here got a copy, thus can accept both Circle and *Circle
func (c Circle) area() float64 {
    return math.Pi * c.r * c.r
}

// val receiver
func (c Circle) Area() float64 {
    return math.Pi * c.r * c.r
}

type Rect struct { l float64
    w float64
}

func NewRect(w, l float64) *Rect {
    r := Rect{w:w,  l:l}
    return &r
}

// ptr receiver
// so here is directly from the original ptr
func (r *Rect) area() float64 {
    return r.l * (*r).w
}

func (r *Rect) Area() float64 {
    return r.l * (*r).w
}
