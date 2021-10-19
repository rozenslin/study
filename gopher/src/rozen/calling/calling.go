package main
import "fmt"
import "time"

/*
   You have to implement a method.

   It should return true if it has been called 4 times or less
   in the last 7 seconds.
*/

var start = time.Now()
var callings = 0

func checkStatus() bool {
    // if elapsed time < 7s since first call, and callings <= 4 times
    // increase callings and return true
    // all else, return false
    t := time.Now()
    elapsed := int(time.Since(start).Seconds())
    if elapsed <= 7 {
        if callings < 4 {
            callings++
            return true
        } else {
            callings++
            return false
        }
    // also if > 7s, reset the start/callings, and return false
    } else {
        // now is new beginning
        start = t
        // this is 1st calling
        callings = 0
        // in such case, false
        return false
    }
}

func main() {
    msg := "A gopher named Desire"
    fmt.Printf("Hello gopher!\n%s\n", msg)

    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // false
    fmt.Printf("Status shall be false: %v\n", checkStatus())
    // false
    fmt.Printf("Status shall be false: %v\n", checkStatus())
    time.Sleep(8 * time.Second)
    // false
    fmt.Printf("Status shall be false: %v\n", checkStatus())
    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // true
    fmt.Printf("Status shall be true: %v\n", checkStatus())
    // false
    fmt.Printf("Status shall be false: %v\n", checkStatus())
    // false
    fmt.Printf("Status shall be false: %v\n", checkStatus())
}
