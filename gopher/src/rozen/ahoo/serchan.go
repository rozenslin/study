package main

import (
    "bufio"
    "fmt"
    "time"
    "io"
    "os"
    "strconv"
    "strings"
)


/*
* Complete the 'Server' function below and missing types and global variables.
*
* The function is void.
*/

var serverChan = make(chan in, 5000)

type in struct {
    // int
    val int32
    // resp chan
    ch chan int32
}

// this will get intput from serverChan
// update the val and send result to respChan??
func Server() {
    // 
    for true {
        select {
            //  recv data from serverChan, including a respChan
            // update the val and send back to respChan
        case in_data := <- serverChan:
            in_data.val *= 2
            // send back ,direction
            in_data.ch <- in_data.val

            // exit when found/detected chan closed
            // timeout after 1s
        case <-time.After(time.Second):
            return
        }
    }
}

func main() {
    reader := bufio.NewReaderSize(os.Stdin, 16 * 1024 * 1024)

    stdout, err := os.Create(os.Getenv("OUTPUT_PATH"))
    checkError(err)

    defer stdout.Close()

    writer := bufio.NewWriterSize(stdout, 16 * 1024 * 1024)

    arrCount, err := strconv.ParseInt(strings.TrimSpace(readLine(reader)), 10, 64)
    checkError(err)

    var arr []int32

    for i := 0; i < int(arrCount); i++ {
        arrItemTemp, err := strconv.ParseInt(strings.TrimSpace(readLine(reader)), 10, 64)
        checkError(err)
        arrItem := int32(arrItemTemp)
        arr = append(arr, arrItem)
    }

    go Server()
    respChans := make([]chan int32, len(arr))
    for idx := 0; idx < len(arr); idx++ {
        i := idx
        respChans[i] = make(chan int32)
        serverChan <- in{arr[i], respChans[i]}
    }
    for i := range respChans {
        arr[i] = <- respChans[i]
    }

    for i, resultItem := range arr {
        fmt.Fprintf(writer, "%d", resultItem)

        if i != len(arr) - 1 {
            fmt.Fprintf(writer, "\n")
        }
    }

    fmt.Fprintf(writer, "\n")

    writer.Flush()
}

func readLine(reader *bufio.Reader) string {
    str, _, err := reader.ReadLine()
    if err == io.EOF {
        return ""
    }

    return strings.TrimRight(string(str), "\r\n")
}

func checkError(err error) {
    if err != nil {
        panic(err)
    }
}
