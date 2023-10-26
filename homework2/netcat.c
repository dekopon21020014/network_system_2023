#include <arpa/inet.h>  /* htons */
#include <stdio.h>      /* fprintf, perror */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset, strlen */
#include <sys/select.h> /* FD_ISSET, FD_SET, FD_ZERO, select */
#include <sys/types.h>  /* accept, bind, read, setsockopt, socket, write */
#include <sys/socket.h> /* accept, bind, listen, setsockopt, shutdown, socket */
#include <unistd.h>     /* close, read, write */
// #include <sys/uio.h>    /* read, write */

int main (int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "argument error. expected more than 2, have %d\n", argc-1);
        return 1;
    }
    
    int dist_port = atoi (argv[2]);    
    if (dist_port <= 0 | dist_port >= (1 << 16)) {
        fprintf (stderr, "port number should be specified 0-65535\n");
        exit(1);
    }
    
    int sockfd, nbytes;
    char buf[BUFSIZ]; /* 自分の環境ではBUFSIZ=1024っぽい */
    
    struct sockaddr_in servaddr;
	memset (&servaddr, 0, sizeof (servaddr)); /*構造体の初期化*/
    
    if ((sockfd = socket (AF_INET/*ip v4*/, SOCK_STREAM/*two way connection*/, 0)) < 0) {
        perror ("sockert error\n"); exit (1);
    }
    
    servaddr.sin_family = AF_INET; /* ip v4 */
    servaddr.sin_port = htons (dist_port);

    if (inet_pton (AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        perror ("inet_pton"); exit (1);
    } else if (inet_pton (AF_INET, argv[1], &servaddr.sin_addr) == 0){
        fprintf (stderr, "Invalid ip address format\n"); exit (1);
    }

    if (connect (sockfd, (struct sockaddr *)&servaddr, sizeof (servaddr)) < 0) {
        perror ("connect\n"); exit (1);        
    }

	int maxfd;
    fd_set read_fds, write_fds;

    FD_ZERO (&read_fds);
    FD_SET (STDIN_FILENO, &read_fds);
    FD_SET (sockfd,       &read_fds);
    maxfd = sockfd;
        
    if (select (maxfd+1, &read_fds, &write_fds, NULL, NULL) < 0) {
        perror("select");
        exit(1);
    }

    if (FD_ISSET (sockfd, &read_fds)) {
        if ((nbytes = read (sockfd, buf, sizeof (buf) - 1)) == 0) {
            printf ("EOF\n");
            return 0;
        }
        buf[nbytes] = '\0';
		fputs (buf, stdout);
    }
    
    if (FD_ISSET (STDIN_FILENO, &read_fds)) {
        nbytes = read (STDIN_FILENO, buf, sizeof (buf) - 1);
        buf[nbytes] = '\0';            
        nbytes = write (sockfd, buf, strlen (buf));
    }	
    
    close(sockfd);
    return 0;
}
