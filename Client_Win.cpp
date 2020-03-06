#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

using namespace std;

int main(int argc, char* argv[])
{	if(argc<3)
        {       cout << "!!!usage: " << argv[0] << " port address" << endl;
				return 0;
        }
	WSADATA info;
	SOCKET sockd;
	struct addrinfo *res, server_addr;
	int n, bufsize = 2048;
	char buf[bufsize];
	n = WSAStartup(MAKEWORD(2,2), &info);
    if (n != 0) {
		       cout << ">>>WSAStartup failed with error: "<< n<< endl;
		       return 0;
		    }
	ZeroMemory((char*) &server_addr, sizeof(server_addr) );
	server_addr.ai_family = AF_INET;
    server_addr.ai_socktype = SOCK_STREAM;
    server_addr.ai_protocol = IPPROTO_TCP;
    n = getaddrinfo(argv[2], argv[1], &server_addr, &res);
    if ( n != 0 ) {
			    cout << ">>>Getaddrinfo failed with error: " << n << endl;
	    	    return 0;
    			  }    
    sockd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockd < 0) {
				cout << ">>>Socket failed with error: " << WSAGetLastError() << endl;
        	    return 0;
    			   }     
	cout << ">>>Socket created..."<< endl;   
    n = connect( sockd, (*res).ai_addr, (*res).ai_addrlen);
    if (n< 0) {
				cout << "<<<<<Unable to connect: "<< WSAGetLastError() << endl;
    	       	closesocket(sockd);
    	       	return 0;
        	  }
    cout << ">>>Connected with server..."<< endl;
	while(1){
    			ZeroMemory(buf,2048);
    			cout << ">>>Client: ";
    			fgets( buf, 2048, stdin);
				n = send( sockd, buf, bufsize, 0 );
				if ((n < 0) || (*buf==42))
                        {	    cout<<"<<<Goodbye!>>>" <<endl;
                                closesocket(sockd);
                                return 0;
                        }
				ZeroMemory(buf,2048);
			    if (n < 0) {
        		cout << ">>>Send failed with error: " << WSAGetLastError() << endl;
       			closesocket(sockd);
       			return 0;
    					   }
		    	n = recv(sockd, buf, bufsize, 0);
    			if ( n < 0 )
        		{	cout << "Connection broken, recv error: " << WSAGetLastError()<<endl;
					closesocket(sockd);
				}
				cout << ">>>Server:\n"<< buf<< endl;
    }
    closesocket(sockd);
    return 0;
}
