#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define LISTEN_BACKLOG  32
#define SERVER_PORT  12345
#define PROG_FAILURE  -1
#define BUF_SIZE  128
#define FLG_ON  1
const char* MESSAGE = "Welcome to toy server~~ Say something!~~ \r\n";

// prepare the socket for conn and return the fd
int initsocket()
{
    int on = FLG_ON;
    // create a AF_INET stream socket for connections.
    int listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sd < 0) {
        printf("Error: socket() failed\n");
        exit(PROG_FAILURE);
    }

    // set sd reuseable to allow multi conns
    int rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0) {
        printf("Error: setsockopt() failed\n");
        close(listen_sd);
        exit(PROG_FAILURE);
    }

    // set socket nonblocking
    rc = fcntl(listen_sd, F_SETFL, O_NONBLOCK);
    if (rc < 0) {
        printf("Error: fcntl() failed\n");
        close(listen_sd);
        exit(PROG_FAILURE);
    }

    // bind the socket to ip address
    struct sockaddr_in   addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port   = htons(SERVER_PORT);

    rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0) {
        printf("Error: bind() failed\n");
        close(listen_sd);
        exit(PROG_FAILURE);
    }

    // setup the listen with backlog
    rc = listen(listen_sd, LISTEN_BACKLOG);
    if (rc < 0) {
        printf("Error: listen() failed\n");
        close(listen_sd);
        exit(PROG_FAILURE);
    }

    return listen_sd;
}

// setup new connections on socket
bool newconn(const int listen_sd, fd_set* active_fd_set, int* max_sd)
{
    int new_sd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    bool end_server = false;
    // foreach for all new connections
    while (true) 
    {
        // accept new connection, if got EWOULDBLOCK, means all done
        // has to set SOCK_NONBLOCK or by fcntl, since the nonblocking not inherited from listen_sd...
        new_sd = accept4(listen_sd, (struct sockaddr *)&address, (socklen_t*)&addrlen, SOCK_NONBLOCK);

        if (new_sd < 0) {
            if (errno != EWOULDBLOCK) {
                printf("Error:  accept() failed\n");
                end_server = true;
            }
            // now all done
            break;
        }

        // inform user of socket number - used in send and receive commands
        printf("FD: %d - New connection, ip: %s, port: %d\n" , new_sd, inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        // send new connection greeting message
        if (send(new_sd, MESSAGE, strlen(MESSAGE), 0) != strlen(MESSAGE)) 
        {
            printf("Error: send() failed\n");
        }

        FD_SET(new_sd, active_fd_set);
        // update max_sd
        if (new_sd > *max_sd)
            *max_sd = new_sd;
    }

    return end_server;
}
          
// receive all data from socket and echo back to client
bool communicate(const int fd, fd_set* active_fd_set, int* max_sd)
{
    // receive all data until EWOULDBLOCK/done or got failure.
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE];
    bool shall_close = false;
    while (true)
    {
        if (!(fcntl(fd, F_GETFL) & O_NONBLOCK)) {
            printf("Error:  ahoooooooooooooooooooo fd: %d is blocking\n", fd);
        }

        // no need for MSG_DONTWAIT here, fd shall be already nonblocking
        int rc = recv(fd, buffer, sizeof(buffer), 0);
        if (rc < 0) {
            if (errno != EWOULDBLOCK) {
                printf("Error:  recv() failed\n");
                shall_close = true;
            }
            printf("FD: %d - Msg drained\n", fd);
            break;
        } else if (rc == 0) {
            getpeername(fd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
            printf("FD: %d - Host disconnected, ip: %s, port: %d\n", fd, inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
            shall_close = true;
            break;
        } else {
            // got data
            int len = rc;
            buffer[len] = '\0';
            printf("FD: %d - Msg received, size: %d, msg: %s\n", fd, len, buffer);

            // echo data to client
            rc = send(fd, buffer, len, 0);
            if (rc < 0) {
                printf("Error:  send() failed\n");
                shall_close = true;
                break;
            }
        }
    }

    // cleanup for the current conn if bad
    if (shall_close) {
        close(fd);
        FD_CLR(fd, active_fd_set);
        if (fd == *max_sd) {
            while (!FD_ISSET(*max_sd, active_fd_set))
                (*max_sd)--;
        }
    }
    return shall_close;
}

// into work loop, in each turn:
// 1. select will wait for specified fds ready or timeout
// 2. then check for ready fds:
// 2.1 if it's listen_sd, means new conn, need to accept all, and add new fds into set
// 2.2 if fds added by 2.1, means ready for read, so receive all data, and echo to clients
void workloop(const int listen_sd, fd_set* active_fd_set, int* max_sd)
{
    // set select timeout as 10s
    struct timeval timeout;
    timeout.tv_sec  = 60;
    timeout.tv_usec = 0;

    bool end_server = false;

    // init the active_fd_set
    // add the listen_sd to active_fd_set
    FD_ZERO(active_fd_set);
    FD_SET(listen_sd, active_fd_set);

    fd_set read_fd_set;
    while (!end_server) 
    {
        // copy current active_fd_set into read_fd_set for select
        memcpy(&read_fd_set, active_fd_set, sizeof(fd_set));

        // do select() which will block till specified fd ready or timeout.
        printf("SD: %d - Waiting on select()...\n", listen_sd);
        int rc = select(*max_sd + 1, &read_fd_set, NULL, NULL, &timeout);

        if (rc < 0) {
            printf("Error:  select() failed\n");
            break;
        } else if (rc == 0) {
            printf("Info:  select() timed out.  End program.\n");
            break;
        }

        // some fds readable now
        int ready_fd_num = rc;

        // for the ready listening socket, means incoming conn.
        if (FD_ISSET(listen_sd, &read_fd_set))
        {
            ready_fd_num--;
            printf("SD: %d - Listening socket is readable\n", listen_sd);
            end_server = newconn(listen_sd, active_fd_set, max_sd);
        }

        // for others, means I/O incoming
        for (int i = 0; i <= *max_sd  &&  ready_fd_num > 0; ++i) 
        {
            if (!FD_ISSET(i, &read_fd_set) || i == listen_sd)
                continue;

            ready_fd_num--;

            printf("FD: %d - fd is readable\n", i);
            bool close_conn = communicate(i, active_fd_set, max_sd);

            if (close_conn)
                printf("FD: %d - fd is closed \n", i);
        }
    }
}

// create a service which
// 1. is listening on a socket,
// 2. and can accept multi client connections simultaneously,
// 3. and will echo back all msg from clients,
// 4. and all ops nonblocking.
//
// when idle > 60s, will auto-exit.
int main (int argc, char *argv[])
{
    // create and setup the socket for connection
    int listen_sd = initsocket();

    // set up the max set size, or use the FD_SETSIZE
    int max_sd = listen_sd;

    fd_set active_fd_set;

    workloop(listen_sd, &active_fd_set, &max_sd);

    // cleanup, close all open fd
    for (int i=0; i <= max_sd; ++i) {
        if (FD_ISSET(i, &active_fd_set))
            close(i);
    }

    return 0;
}
