// Subject to the terms of the Mozilla Public License, v. 2.0, see https://mozilla.org/MPL/2.0/.
// Free for non-commercial use, contact owner for commercial use.

// a local tcp server serving multi msgs for multi conns concurrently
package main

import (
	"bufio"
	"log"
	"net"
	"os"
)

const (
	CONN_HOST = "127.0.0.1"
	CONN_PORT = "2222"
	CONN_TYPE = "tcp"
)

func main() {
	// listen
	l, err := net.Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)
	if err != nil {
		log.Println("error listening:", err.Error())
		os.Exit(1)
	}
	defer l.Close()

	log.Println("listening on " + CONN_HOST + ":" + CONN_PORT)

	// work loop
	for {
		// accept the conn
		conn, err := l.Accept()
		if err != nil {
			log.Println("error accepting: ", err.Error())
			os.Exit(1)
		}

		// handle conn in a new goroutine to work concurrently
		go handleConn(conn)
	}
}

func handleConn(conn net.Conn) {
	clientAddr := conn.RemoteAddr().String()
	log.Printf("accepted connection from %s", clientAddr)

	defer func() {
		_ = conn.Close()
		log.Printf("closed connection from %s", clientAddr)
	}()

	// handle multi msgs of same conn
	// NB: one buf scanner/reader for each conn, not for each msg,
	// or else it will discard the buffered data after reading first one
	scanner := bufio.NewScanner(conn)

	// or:
	// reader := bufio.NewReader(conn)

	for scanner.Scan() {
		// NB: also client shall use newline for msg ending,
		// or else here cannot delimit them
		message := scanner.Text()

		// or:
		// input, err := reader.ReadString('\n')
		// message := string(input)

		log.Println("msg got from ", clientAddr, " : ", message)
	}

	if err := scanner.Err(); err != nil {
		log.Println("error: ", err)
	}
}
