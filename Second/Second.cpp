#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

int sockfd;
socklen_t len;
struct sockaddr_in servaddr, cliaddr;

#define PORT     8080
#define MAXLINE 1024

int sum_length(int n) {
	if (n == 0) return 1;
	int _length = 0;
	while (n) {
		_length++;
		n /= 10;
	}
	return _length;
}

union exchange_line{
	unsigned u;
	char c[4];
};

void create_server(){

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
	    exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
    	perror("bind failed");
    	exit(EXIT_FAILURE);
    }

    len = sizeof(cliaddr);

	return;
}

unsigned get_data(){
	exchange_line buffer;
	int n;
	n = recvfrom(sockfd, buffer.c, 4, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);

	printf("Client : %d\n", buffer.u);
	return buffer.u;
}

int main() {
	create_server();
	while (1) {
		auto sum = get_data();
			if (sum % 32 && sum_length(sum)> 2)
				cout << "Data received\n";
			else cout << "Error\n";
	}

	return 0;
}
