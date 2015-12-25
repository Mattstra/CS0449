#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>


void * requestHandler(void * threadSfd);
char * ip = 0;
int cport = 0;

int main()
{
	pthread_t wThread;
	int id = 0;		
	int connfd = 0;
	int binded = 0;
	
	struct sockaddr_in addr;
	int sfd = socket(PF_INET, SOCK_STREAM, 0);

	if(sfd == -1)
        {
                printf("Socket Error");
                exit(1);
        }

	memset(&addr.sin_zero, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(50039);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//bind
	binded = bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
	if(binded < 0)
        {
                printf("Error on bind");
                exit(1);
        }

	while(1)
	{
		if(listen(sfd, 10) == -1)
        	{
                	printf("Error on listen");
                	exit(1);
        	}

		//Accept connection
		connfd = accept(sfd, NULL, NULL);
		if(connfd == -1)
        	{
                	printf("Error on accept");
        		exit(1);
        	
		}
		else
		{
			printf("Connected \n");
			break;
		}
	}

	//get ip address and port of client
	ip = inet_ntoa(addr.sin_addr);
	cport = ntohs(addr.sin_port);	
	
	//create worker thread to handle requests
	pthread_create(&wThread, NULL, requestHandler, &connfd);
	
	//join threads
	pthread_join(wThread, NULL);

		
	

	close(connfd);
	close(sfd);

	return 0;
}

void * requestHandler(void * threadSfd)
{
	int tSfd = *(int *)threadSfd;
	char request[1024];
	char statInfo[1024];
	int recieved = 0;

	//read sent request
	memset(request, 0, 1024);
	while(1)
	{
		recieved = recv(tSfd, request, 1024, 0);
		
		if(recieved < 0)
		{
			printf("Error recieving request\n");
			exit(1);
		}
		
		//see if the end of the header has been read
		while(strstr(request, "\r\n\r\n") != 0)
		{
			recieved = recv(tSfd, request, 1024, 0);
		}

		if(strstr(request, "\r\n\r\n") == 0)
			break;
	}


	//lenghth of request
	int reqLen = strlen(request);

	//parse info from header
	char* host;
	char hostBuff[1024];
	host = strstr(request, "HOST");
	strncpy(hostBuff, host, 1024);

	int hostLen = strlen(hostBuff);			//length of host section of header
	
	//extract just host name
	char hostInfo[1024];
	strncpy(hostInfo, host, (hostLen-10));

	//parse get request for filename
	char* get;
	char getBuff[1024];
	get = strstr(request, "/");
	
	strncpy(getBuff, get, (reqLen - (hostLen + 17)));
	memmove(getBuff, getBuff+1, strlen(getBuff));
	char* filename = getBuff;
	
	printf("Requested %s\n", filename);

	//copy get request for stats.txt
	char getInfo[1024];
	strncpy(getInfo, request, reqLen-(hostLen + 4));
	
		
	//determine status of requested file
	struct stat status;
	int fStat = stat(filename, &status);
	
	if(fStat < 0)
	{	
		char* nofile = "HTTP/1.1 404 NOT FOUND";
		send(tSfd, nofile, strlen(nofile), 0);
		exit(1);
	}
	else
	{
		//found file
		
		//get current time
		time_t t = time(NULL);
		struct tm *info;
		time(&t);
		info = localtime(&t);

		//get content length
		int fSize = status.st_size;
		char size[sizeof(fSize)];
		sprintf(size, "%d", fSize);

		char response[1024];
		strcpy(response, "HTTP/1.1 200 OK\n");
		strcpy(response+strlen(response), asctime(info));
		strcpy(response+strlen(response), "Content-Length: ");
		strcpy(response+strlen(response), size);
		strcpy(response+strlen(response), "\n");
		strcpy(response +strlen(response), "Connection: close\n");
		strcpy(response+strlen(response), "Content-Type: text/html\n");
		strcpy(response+strlen(response), "\n");
		
		//now open and send file
		FILE * file;
		file = fopen(filename, "r");
		
		//copy contents of file to buffer
		fgets(response+strlen(response), 1024, file);
		
		//send response
		send(tSfd, response, strlen(response), 0);
                printf("Response sent \n");

		//create stats.txt
		FILE * stats;
		stats = fopen("stats.txt", "w");

		fputs(getInfo, stats);
		fprintf(stats, "\n");
		fputs(hostInfo, stats);
		fprintf(stats, "\n");
		
		//copy client info to statInfo buffer
		strcpy(statInfo+strlen(statInfo), "Client: ");
		strcpy(statInfo+strlen(statInfo), ip);		

                char port[sizeof(cport)];
                sprintf(port, "%d", cport);
		strcpy(statInfo+strlen(statInfo),":");
		strcpy(statInfo+strlen(statInfo), port);		

		fputs(statInfo, stats);
				
	}
}

