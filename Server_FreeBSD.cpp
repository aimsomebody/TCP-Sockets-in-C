#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
using namespace std;
int main(int argc, char* argv[])
{	if(argc < 2)
        	{       cout << "usage: " << argv[0] << " port" << endl;
			return 0;
        	}
	int port, serv;
	const int bufsize = 2048;
	char buf[bufsize], rsp[bufsize];
	struct sockaddr_in server_addr, clnt_addr;
	socklen_t size;
	port = atoi(argv[1]);
	int sockd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockd < 0)
        	{
        	        cout << "<<<Error establishing socket!!!>>>" << endl;
               		return 0;
        	}
    cout << "\n---Server socket has been created...---" << endl;
	bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockd,(struct sockaddr*)&server_addr,sizeof(server_addr)) <0 )
		{   cout << "<<<Error binding...>>>"<< endl;
			close(sockd);
			return 0;
        }
	cout << "---Looking for client...---"<< endl;
	size = sizeof(clnt_addr);
    listen(sockd,1);
    serv = accept(sockd,(struct sockaddr*)&clnt_addr,&size);
    if(serv < 0)
      	{       cout << "<<<Error on accepting!!!>>> return: " << serv << endl;
        	        close( sockd);
        	        return 0;
        }
    char* ip = inet_ntoa(clnt_addr.sin_addr);
    cout << "---Connected with client IP: " << ip << endl;
    int n;
    FILE *fp;
    for(;;)
       	{
   	        bzero(buf, bufsize);
			n = read( serv, buf, bufsize );
   	        if ((n < 0) || (*buf==42)) 
			{
   	                cout<<"<<<<<Goodbye!>>>>>" <<endl;
   	                close(sockd);
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
					close(sockd);
					return 0;
			}
        	bzero( buf, bufsize);
        	bzero( rsp, bufsize);
        	while(fgets( buf, bufsize, fp))
        	    {               strcat(rsp, buf);
                }
			cout << "  " <<buf << " \n";
			if(strlen(rsp)==0)
			rsp[0]='0';
			n = write( serv, rsp, strlen(rsp));
           	if (n < 0) {
                		cout << "<<<<<Write < 0 : " << n  << endl;
						fclose(fp);
			        	close(sockd);
						return 0;
				   }
			cout <<"Sent: "<<strlen(rsp)<<endl;
            fclose(fp);
           	system("rm c.txt");
        	}
}

