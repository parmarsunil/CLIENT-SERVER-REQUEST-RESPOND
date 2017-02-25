o#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include<string.h>
#include<stdlib.h>

int main(){
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	char buff[36];
	size_t nbytes;
	int i,j,l,buffsize;
	int count=0;
	char *str,*substr;
	char del[2]=";";
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(5000);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address,server_len);

	//Create a connection queue, ignore child exit details and wait for clients.

	listen(server_sockfd, 2);

	signal(SIGCHLD, SIG_IGN);
	int client=3;
	while(--client) {
		printf("server waiting\n");
		// Accept connection
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		bzero(buff,sizeof(buff));//put null in buff
		//Fork to create a process for this client and perform a test to see whether we're the parent or the child. 
		if(fork() == 0) {

			//If we're the child, we can now read/write to the client on client_sockfd.The five second delay is just for this demonstration.
			nbytes=sizeof(buff);
			read(client_sockfd, buff, nbytes);
			sleep(5);
			
			//printf("%s",buff);
			str=strtok(buff,del);
			substr=strtok(NULL,del);
			i=0;
			j=0;
			//printf("%s\t%s",str,substr);
			while(i!=strlen(str)+1){
				if(substr[j]!='\0'){
					if(str[i]==substr[j]){
						i++;
						j++;
					}
					else{
						i++;
					}
				}
				else{
					j=0;
					count++;
					//printf("count++");
				}
			}
			//printf("%d",count);
			write(client_sockfd, &count, sizeof(int));
			close(client_sockfd);
			exit(0);
		}
		//Otherwise, we must be the parent and our work for this client is finished. 
		else {
			close(client_sockfd);
		}
	}
}
