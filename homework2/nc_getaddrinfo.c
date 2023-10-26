#include <unistd.h>     /* close, read, write */
#include <stdlib.h>     /* exit */
#include <stdio.h>      /* fprintf, perror */
#include <string.h>     /* memset, strlen */
#include <netdb.h>      /* struct addrinfo, */

// #include <sys/select.h> /* FD_ISSET, FD_SET, FD_ZERO, select */
// #include <sys/types.h>  /* accept, bind, read, setsockopt, socket, write */
// #include <sys/socket.h> /* accept, bind, listen, setsockopt, shutdown, socket */
// #include <netinet/in.h>

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

    int sockfd, nbytes;
    char buf[BUFSIZ]; /* 自分の環境ではBUFSIZ=1024っぽい */
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
    	    if ((nbytes = read (sockfd, buf, sizeof (buf) - 1)) == 0) {
        	    printf ("EOF\n"); break;
        	}
        	buf[nbytes] = '\0';
			fputs (buf, stdout);
    	}

    	if (FD_ISSET (STDIN_FILENO, &read_fds)) {
        	nbytes = read (STDIN_FILENO, buf, sizeof (buf) - 1);
	        buf[nbytes] = '\0';            
	        nbytes = write (sockfd, buf, strlen (buf));
	    }	
    }

    close(sockfd);
    return 0;
}
