#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

using namespace std;

int main(int argc, char* argv[])
{	if(argc < 2)
        	{
				cout << "!!!usage: " << argv[0] << " port" << endl;
				return 0;
        	}
    WSADATA info;
    SOCKET sockd = INVALID_SOCKET, clsock = INVALID_SOCKET;
    struct addrinfo *res = NULL, server_addr, clnt_addr;
	int n, bufsize = 2048, serv;
	char buf[bufsize],rsp[bufsize];
    n = WSAStartup(MAKEWORD(2,2), &info);
    if (n != 0) {
        cout << ">>>WSAStartup failed with error: "<< n<< endl;
        return 0;
    }
    ZeroMemory(&server_addr, sizeof(server_addr));
    server_addr.ai_family = AF_INET;
    server_addr.ai_socktype = SOCK_STREAM;
    server_addr.ai_protocol = IPPROTO_TCP;
    server_addr.ai_flags = AI_PASSIVE;
    n = getaddrinfo( NULL, argv[1], &server_addr, &res);
    if ( n == SOCKET_ERROR) {
        cout << ">>>Getaddrinfo() failed with error: " << n<< endl;
        WSACleanup();
        return 0;
    }
	sockd = socket((*res).ai_family, (*res).ai_socktype, (*res).ai_protocol);
    if (sockd < 0) 
	{
    	cout << ">>>Error establishing socket, error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    cout << "\n---Server socket has been created...---" << endl;
    n = bind( sockd, (*res).ai_addr, (int)(*res).ai_addrlen);
    if (n == SOCKET_ERROR) 
	{
		cout << ">>>Error binding, error: " << WSAGetLastError() << endl;
        closesocket(sockd);
        WSACleanup();
        return 0;
    }
    listen(sockd,1);
    cout << "---Looking for client...---"<< endl;
    socklen_t size = sizeof(clnt_addr);
    clsock = accept( sockd, (struct sockaddr*)&clnt_addr.ai_addr, &size);
    if (clsock == INVALID_SOCKET) {
        cout << ">>>Error on accepting, error: " << WSAGetLastError() << endl;
        closesocket(sockd);
        WSACleanup();
        return 0;
    }
    cout << "---Connected with client---" << endl;
    FILE *fp;
    for(;;)
       	{	shutdown(sockd,SD_BOTH);
   	        ZeroMemory(buf, bufsize);
			n = recv( clsock, buf, bufsize, 0);
   	        if ((n < 0) || (*buf==42)) 
			{
   	                cout<<"<<<<<Goodbye!>>>>>  " << endl;
   	                closesocket(sockd);
					return 0;
			}    	        
			cout << "Recevied: " << strlen(buf)<<endl;
			buf[strlen(buf)-1]='\0';
			if(strlen(buf)==0)
			buf[0]='0';
   	        strcat(buf,">c.txt");
   	        system(buf);
			if((fp = fopen("c.txt", "r")) == 0 )
        	{       cout << "Impossible!!!"<<endl;
					closesocket(sockd);
					return 0;
			}
        	ZeroMemory( buf, bufsize);
        	ZeroMemory( rsp, bufsize);
        	while(fgets( buf, bufsize, fp))
        	    {               strcat(rsp, buf);
                }
			if(strlen(rsp)==0)
			rsp[0]='0';
			n = send( clsock, rsp, strlen(rsp), 0);
           	if (n < 0) {
                		cout << "<<<<<Write < 0 : " << n  << endl;
						fclose(fp);
			        	closesocket(sockd);
						return 0;
				   }
			cout <<"Sent: "<<strlen(rsp)<<endl;
            fclose(fp);
           	system("del c.txt");
        	}
	
}
