#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
void main()
{
    int sid;
    char s[36]={},buff[36]={};
    int s1,flag=0;
    char *str,*substr;
	char del[2]=";";
    struct sockaddr_in ssock,csock;
    sid=socket(AF_INET,SOCK_STREAM,0);
    ssock.sin_family=AF_INET;                              //ipv4
    ssock.sin_addr.s_addr=inet_addr("127.0.0.1");          //127.0.0.1 ip adderess for loop back testing
    ssock.sin_port=htons(5000);                            //5000 is a port no.
    connect(sid,(struct sockaddr *)&ssock,sizeof(ssock)); //connect client with server through socket file descripter (sid)
    while(1)
    {
        printf("\n Enter the string;keyword");
        fgets(s,256,stdin);       //get std input
        for(int i=0;i<strlen(s);i++){                     //copying input string into a buffer
        	buff[i]=s[i];
        	if(s[i]==';')
        		flag=1;
        }
        if(flag==0){
        	printf("incorrect input format");
        	exit(0);
        }
        //printf("%d",strlen(s));
        if(strlen(s)==0||strlen(s)==1||s[0]==';'){         //check whether input string according to its standard
        	printf("empty string");
        	exit(0);                   
        }
        str=strtok(buff,del); //tokenize the string
		substr=strtok(NULL,del);   
		substr[strlen(substr)-1]='\0';                  //put null at the end of string
		if(strlen(str)<strlen(substr)){
			printf("keyword always less then or equal to words");
			exit(0);
		}
        if(strlen(str)==0||strlen(substr)==0){          //check whether input string according to its standard
        	printf("empty string or keyword");
        	exit(0);
        }
        else{
        	if(strlen(str)>30||strlen(substr)>5){      //check whether input string according to its standard
        		printf("string length must be <=30 or keyword must be <=5");
        		exit(0);
        	}
        }
        //printf("%s",s);
        for(int i=0;i<strlen(s);i++){    //byte stuffing put '0' at the space loc.
				if(s[i]==' '){
					s[i]='0';
				}
			}
			//printf("%s",s);
        write(sid,(void*)s,strlen(s)-1);  //write into the socket
        if(strlen(s)==0)
            break;
        //sleep(1);
        read(sid,&s1,sizeof(s1));        //read from the server socket
        printf("\n The received number is:%d\n",s1);
    }
    close(sid);   //closed connection
}

