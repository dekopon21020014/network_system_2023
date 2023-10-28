#include <unistd.h>     /* close, read, write */
#include <stdlib.h>     /* exit */
#include <stdio.h>      /* fprintf, perror */
#include <string.h>     /* memset, strlen */
#include <netdb.h>      /* struct addrinfo, */
#include <fcntl.h>
#include <errno.h>

// #include <sys/select.h> /* FD_ISSET, FD_SET, FD_ZERO, select */
// #include <sys/types.h>  /* accept, bind, read, setsockopt, socket, write */
// #include <sys/socket.h> /* accept, bind, listen, setsockopt, shutdown, socket */
// #include <netinet/in.h>

#define BUFFER_SIZE 1024

int read_from (int, char*, int);


int main (int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "argument error. expected more than 2, have %d\n", argc - 1);
        return 1;
    }
    
    int dist_port = atoi (argv[2]);
    if (dist_port <= 0 | dist_port >= (1 << 16)) {
        fprintf (stderr, "port number should be specified 0-65535\n");
        exit(1);
    }

    int sockfd, read_size, written_size;
    char buf[BUFFER_SIZE]; /* 自分の環境ではBUFSIZ=1024っぽい */
    int error;
    struct addrinfo hints, *res, *r;
    memset (&hints, 0, sizeof (hints)); /* 構造体の初期化 */
    hints.ai_family   = PF_UNSPEC;      /* IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    if ((error = getaddrinfo (/*server name*/ argv[1], /*port*/ argv[2], &hints, &res))) {
        fprintf(stderr, "getadddrinfo:\n%s\n", gai_strerror (error));
        exit (1);
    }

    for (r = res; r; r = r->ai_next) {
        if ((sockfd = socket (r->ai_family, r->ai_socktype, r->ai_protocol)) < 0) 
            continue;

        if (connect (sockfd, r->ai_addr, r->ai_addrlen) == 0) 
            break;

        close (sockfd);
        sockfd = -1;
    }
    freeaddrinfo (res);

    if (sockfd < 0) {
        fprintf (stderr, "couldn't create connection\nname: %s\nport: %s\n", argv[1], argv[2]);
        exit (1);
    }
    
    for (;;) {
		int maxfd;
	    fd_set read_fds;

    	FD_ZERO (&read_fds);
	    FD_SET (STDIN_FILENO, &read_fds);
	    FD_SET (sockfd,       &read_fds);
    	maxfd = sockfd;

    	if (select (maxfd+1, &read_fds, NULL, NULL, NULL) < 0) {
        	perror("select");
        	exit(1);
    	}

	    if (FD_ISSET (sockfd, &read_fds)) {
            
    	    if ((read_size = read (sockfd, buf, sizeof (buf) - 1)) == 0) {
        	    fprintf (stderr, "EOF\n"); break;
            }
            
            /*
            printf("-------\n");
            if ((read_size = read_from (sockfd, buf, sizeof (buf) - 1)) < 0) {
                fprintf (stderr, "read EOF\n");
                break;
            }
            */
            printf ("read size = %d\n", read_size);
            
            printf("-------\n");
        	buf[read_size] = '\0';
			fputs (buf, stdout);
    	}

    	if (FD_ISSET (STDIN_FILENO, &read_fds)) {
        	// read_size = read (STDIN_FILENO, buf, sizeof (buf) - 1);
            if ((read_size = read_from (STDIN_FILENO, buf, sizeof (buf) - 1)) < 0) {
                fprintf (stderr, "read EOF\n");
                break;
            }
	        buf[read_size] = '\0';
            printf ("buf is : %s\n", buf);
	        written_size = write (sockfd, buf, sizeof (buf) - 1);
	    }	
    }

    close(sockfd);
    return 0;
}

/* read(2)のラッパー */
int read_from (int fd, char* buf, int size) {
	int val;
	val = fcntl (fd, F_GETFL);
	fcntl (fd, F_SETFL, val | O_NONBLOCK);
    
    int read_size = 0;
    int offset = 0;
    int nbyte;
	
    for (;;) {
        if (size == 0) break;
    	if ((nbyte = read (fd, buf + offset, size)) == 0) {
            read_size = -1;
            fprintf (stderr, "here EOF\n");
        	break;
    	} else if (errno == EAGAIN) {
            printf("eagain\n");
            /* readするものがなかっただけ*/
            break;
        } else {
            printf ("nbyte = %d\n", nbyte);
            size   -= nbyte;
        	offset += nbyte;
            read_size += nbyte;
        }
    }
    return read_size;
}
