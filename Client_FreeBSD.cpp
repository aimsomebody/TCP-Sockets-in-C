#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main(int argc, char* argv[]){
	if(argc<3){
		cout << "usage: " << argv[0] << " port address" << endl;
		return 0;
	}
	const int bufsize = 2048;
	char buf[bufsize];
	int portNo = atoi(argv[1]);
	char* ip = argv[2];
	struct sockaddr_in server_addr;
	int sockd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockd<0){
		cout << ">>>Enable to create socket..." << endl;
		return 0;
	}
	cout << ">>>Socket has been created..."<< endl;
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNo);
	inet_pton(AF_INET, ip, &server_addr.sin_addr);
	int serv = connect(sockd,(struct sockaddr*)&server_addr, sizeof(server_addr));
	if(serv < 0){
	    cout << ">>>Error on connecting!" << endl;
		close(sockd);
		return 0;
	}
	cout << ">>>Connected with server ip: : "<< ip<<", port number:"<< argv[1]<< endl;
	cout << ">>>>>Enter '*' to close connection<<<<<"<< endl;
	int n;
	for(;;)
	{	bzero( buf, bufsize);
		cout << ">>>Client: ";
		fgets( buf, bufsize, stdin);
		n = write(sockd,buf, bufsize);
		if ((n < 0) || (*buf==42))
            {   cout << "<<<<<Goodbye!>>>>>" <<endl;
                close(sockd);
                return 0;
            }
		bzero( buf, bufsize);
		n = read(sockd, buf, bufsize);
		cout << n<<endl;
		if (n < 0)
		{	cout << "<<<<<Impossible to read: " << n << endl;
			close(sockd);
			return 0;
        }
		cout << ">>>Server: " << endl;
		cout << buf;
		cout << n << endl;
	}
}