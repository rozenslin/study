// Subject to the terms of the Mozilla Public License, v. 2.0, see https://mozilla.org/MPL/2.0/.
// Free for non-commercial use, contact owner for commercial use.

// demo for the local tcp client using connection pool implemented by golang buf chan only
package main

import (
	"log"
	"net"
	"time"
)

// pool for tcp conn, aka, net.Conn
type Pool struct {
	// buffered channel for connection pooling
	c chan net.Conn

	// factory to create new connection
	f func() (net.Conn, error)
}

// init a new pool,
// args:
//
//	s: the capability of pool
//	f: the factory to create new connection
func NewPool(s int, f func() (net.Conn, error)) *Pool {
	return &Pool{
		c: make(chan net.Conn, s),
		f: f,
	}
}

// get one idle conn from pool, if pool empty, create a new one
func (p *Pool) Get() (net.Conn, error) {
	select {
	case c := <-p.c:
		log.Printf("Got connection from pool: %p", c)
		return c, nil

	default:
		log.Printf("Creating new connection")
		c, err := p.f()
		if c == nil || err != nil {
			log.Printf("Failed to create new connection: %v", err)
			return nil, err
		}

		return c, nil
	}
}

// put conn into pool, if the pool full, close the conn instead
func (p *Pool) Put(c net.Conn) {
	select {
	case p.c <- c:
		log.Printf("Connection idle, joined pool: %p", c)

	default:
		log.Printf("Pool full: closing current connection: %p", c)
		c.Close()
	}
}

// close the pool, and close all conns in it
func (p *Pool) Close() {
	close(p.c)
	for c := range p.c {
		log.Printf("Closing connection: %p", c)
		c.Close()
	}
}

// create new conn by net.Dial() to local server
func NewConn() (net.Conn, error) {
	server := "localhost:2222"
	return net.Dial("tcp", server)
}

func SendMsg(p *Pool, msg string) {
	// get conn from pool
	c, err := p.Get()

	// failed to get valid conn
	if c == nil || err != nil {
		log.Printf("Cannot connec to server, message discarded. Err: %v", err)
		return
	}

	// send out msg
	// if failed, close the conn and don't add to pool
	if _, err := c.Write([]byte(msg)); err != nil {
		log.Printf("Failed to send msg to server. Err: %v", err)
		log.Printf("Closing connection: %p", c)
		c.Close()

		return
	}

	// put to pool after conn success
	p.Put(c)
}

func main() {
	// demo caller of conn pool
	s := 16
	f := NewConn
	p := NewPool(s, f)

	// ensure pool full by this
	for i := 2 * s; i > 0; i-- {
		go SendMsg(p, "1st batch msgs, ahooooooooooooooooooooooooooo\n")
	}

	time.Sleep(3 * time.Second)

	// ensure all conns from pool are reused, exhausted, and extra created conns closed after use,
	// with pool size within limit always
	for i := 100; i > 0; i-- {
		go SendMsg(p, "2nd batch msgs, wahooooooooooooooooooooooooooo\n")
	}

	time.Sleep(3 * time.Second)

	p.Close()
}
