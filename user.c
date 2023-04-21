#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#define PORT "58008"

typedef struct default_p {
	char *default_ip;
	char *default_port;
}default_param;

char uid[15];
int user = 0;
int active = 0;
char gid[15];
char FileName[25]; 

int TimerOn(int sd){
	struct timeval tmout;

	memset((char*)&tmout,0,sizeof(tmout));
	tmout.tv_sec=15;
	return(setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tmout,sizeof(struct timeval)));
}

int TimerOff(int sd){
	struct timeval tmout;

	memset((char*)&tmout,0,sizeof(tmout));
	return(setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tmout,sizeof(struct timeval)));
}



void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}



char* getStr(){
	/* Reads entire line from input */
	int i = 0;
	int c;
	char *tmp_str = malloc(sizeof(char) * 4000);

	while((c = getchar()) != '\n'){
		tmp_str[i++] = c;
	}
  	tmp_str[i] = '\0';

	return tmp_str;
}

default_param parseFlags(int argc, char* argv[]) {

	default_param dp;

	char *default_ip;
	char *default_port;

	if(argc == 1){
		/* 	No flags were specified */
    	default_ip=malloc(sizeof(char)*strlen("localhost")+1);
    	strcpy(default_ip,"localhost");

    	default_port=malloc(sizeof(char)*6);
    	strcpy(default_port,PORT);

    	dp.default_port=default_port;
    	dp.default_ip=default_ip;
		
	}

	if(argc == 3){
		 	/*1 flag was specified */
		if(!strcmp(argv[1],"-n")){
			/* 	DS IP was specified */

      		default_ip=malloc(sizeof(char)*strlen(argv[2]));
      		strcpy(default_ip,argv[2]);

      		default_port=malloc(sizeof(char)*6);
      		strcpy(default_port,PORT);


			dp.default_port=default_port;
    		dp.default_ip=default_ip;
		}

		if(!strcmp(argv[1],"-p")){
			/* 	DS port was specified */
      		default_ip=malloc(sizeof(char)*10);
      		strcpy(default_ip,"localhost");

      		default_port=malloc(sizeof(char)*strlen(argv[2]));
      		strcpy(default_port,argv[2]);

			dp.default_port=default_port;
    		dp.default_ip=default_ip;
		}
	}

	if(argc == 5){
		/* 	DS IP and port were specified */
    	default_ip=malloc(sizeof(char)*strlen(argv[2]));
	    default_port=malloc(sizeof(char)*strlen(argv[4]));

	    strcpy(default_ip,argv[2]);
    	strcpy(default_port,argv[4]);
		
		dp.default_port=default_port;
    	dp.default_ip=default_ip;
	}

	return dp;
}

char * reg(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*21);
	char new_line[2] = "\n";

	strcpy(cmd_send,  "REG ");

	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * unregister(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*21);
	char new_line[2] = "\n";

	strcpy(cmd_send,  "UNR ");

	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}
char * login(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*21);
	char new_line[2] = "\n";

	strcpy(cmd_send,  "LOG ");

	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * logout(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*21);
	char new_line[2] = "\n";

	strcpy(cmd_send,  "OUT ");
	
	strcat(cmd_send,input);
	strcat(cmd_send, uid);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * groups(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*21);
	char new_line[2] = "\n";

	strcpy(cmd_send,  "GLS");

	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * ulist(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*21);
	char new_line[2] = "\n";
	char copy[15];
	strcpy(copy,gid);

	strcpy(cmd_send,  "ULS ");

	strcat(cmd_send,copy);
	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * subscribe(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*34);
	char new_line[2] = "\n";
	char copy[15];
	strcpy(copy,uid);
	char *user = strtok(copy, " ");

	strcpy(cmd_send,  "GSR ");

	strcat(cmd_send,user);
	strcat(cmd_send," ");
	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * unsubscribe(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*34);
	char new_line[2] = "\n";
	char copy[15];
	strcpy(copy,uid);
	char *user = strtok(copy, " ");

	strcpy(cmd_send,  "GUR ");

	strcat(cmd_send,user);
	strcat(cmd_send," ");
	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * my_groups(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*21);
	char new_line[2] = "\n";
	char copy[15];
	strcpy(copy,uid);
	char *user = strtok(copy, " ");

	strcpy(cmd_send,  "GLM ");
	
	strcat(cmd_send,user);
	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}

char * post(char *input) {
	char *cmd_send = (char*)malloc(sizeof(char)*300);
	char new_line[2] = "\n";
	char copy[6];
	int tsize = 0;
	char copy2[4];
	char text[242];
	int aspas = 0;
	
	memset(copy,'\0',sizeof(char)*6);
	strncpy(copy,uid,5);
	strncpy(text,input+1,strlen(input));
	char resposta[242];
	sscanf(text,"%[^\"]", resposta);
	tsize = strlen(resposta);
	itoa(tsize,copy2);

	sprintf(cmd_send,"PST %s %s %s %s", copy,gid,copy2,resposta);

	for(int i = 0; i<strlen(input); i++){
		if(input[i] == '\"'){
			aspas += 1;
		}
		if(aspas == 2){
			if(input[i+1] == ' '){
				strcat(cmd_send, " ");
				strncpy(FileName,input+(i+2),strlen(input));
				memset(text, '\0', sizeof(char) * 242);
				memset(resposta, '\0', sizeof(char) * 242);
				memset(copy2, '\0', sizeof(char) * 4);
				return cmd_send;
			}

			else{
				
				strcat(cmd_send,new_line);
				memset(text, '\0', sizeof(char) * 242);
				memset(resposta, '\0', sizeof(char) * 242);
				memset(copy2, '\0', sizeof(char) * 4);
				return cmd_send;
			}
		}
	}
	return cmd_send;
}

char * retrieve(char *input) {	
	char *cmd_send = (char*)malloc(sizeof(char)*30);
	char new_line[2] = "\n";
	char copy[15];

	strcpy(copy,uid);
	char *user = strtok(copy, " ");

	strcpy(cmd_send,  "RTV ");

	strcat(cmd_send, user);
	strcat(cmd_send, " ");
	strcat(cmd_send, gid);
	strcat(cmd_send, " ");
	strcat(cmd_send,input);
	strcat(cmd_send,new_line);

	return cmd_send;

}


int main(int argc, char* argv[]){

	char *input;
	char *cmd = malloc(sizeof(char) * 32);
	char *tmp;
	int i;
	char *buffer = (char*)malloc(sizeof(char)*1000);
	char *bufferg = (char*)malloc(sizeof(char)*10000);

	memset(buffer, '\0', sizeof(char) * 1000);

	char *tmp_str = malloc(sizeof(char) * 1000);
	memset(tmp_str, '\0', sizeof(char) * 1000);
	char *size = malloc(sizeof(char) * 20);
	memset(size, '\0', sizeof(char) * 20);

	//char ok[8] = "RRG OK\n";
	char nok[9] = "RRG NOK\n";
	char dup[9] = "RRG DUP\n";
	char ok2[8] = "RUN OK\n";
	char nok2[9] = "RUN NOK\n";
	char ok3[8] = "RLO OK\n";
	char nok3[9] = "RLO NOK\n";
	char ok4[8] = "ROU OK\n";
	char nok4[9] = "ROU NOK\n";
	char ok5[8] = "RGS OK\n";
	char nok5[9] = "RGS NOK\n";
	char e_user[12] = "RGS E_USR\n";
	char e_grp[12] = "RGS E_GRP\n";
	char e_gname[14] = "RGS E_GNAME\n";
	char e_full[13] = "RGS E_FULL\n";
	char ok6[8] = "RGU OK\n";
	char nok6[9] = "RGU NOK\n";
	char e_user2[12] = "RGU E_USR\n";
	char e_grp2[12] = "RGU E_GRP\n";
	char no_groups[8] = "RGM 0\n";
	char ok7[8] = "RUL OK ";
	char nok7[9] = "RUL NOK\n";
	char nok8[9] = "RPT NOK\n";
	char new_line[2] = "\n";

	
	default_param dp;
	dp=parseFlags(argc,argv);
	
	while((input = getStr()) && strcmp(input,"exit") != 0) {

		i = 0;
		while(*input != ' ' && *input != '\0'){
			*(cmd + i) = *input;
			input++;
			i++;
		}

		if(*input != '\0')
			input++;

		if (!strcmp(cmd, "reg") || !strcmp(cmd, "unregister") || !strcmp(cmd, "unr") || !strcmp(cmd, "login") || !strcmp(cmd, "logout") || 
			!strcmp(cmd, "groups") || !strcmp(cmd, "gl") || !strcmp(cmd, "subscribe") || !strcmp(cmd, "s") || !strcmp(cmd, "unsubscribe") ||
			!strcmp(cmd, "my_groups") || !strcmp(cmd, "mgl") || !strcmp(cmd, "showuid") || !strcmp(cmd, "su") || !strcmp(cmd, "u") || !strcmp(cmd, "select") ||
			!strcmp(cmd, "sag") || !strcmp(cmd, "showgid") || !strcmp(cmd, "sg") ) {

			int fd, errcode;
			ssize_t n;
			socklen_t addrlen;
			struct addrinfo hints, *res;
			struct sockaddr_in addr;

			fd=socket(AF_INET,SOCK_DGRAM,0);
			if(fd==-1)/*error*/
				exit(1);


		  	memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_CANONNAME;


			errcode = getaddrinfo(dp.default_ip,dp.default_port, &hints, &res);
				if(errcode != 0) {
					printf("getaddrinfo returned an error while opening on host\n");
					exit(1);
				}
			
			if(!strcmp(cmd, "reg")){

				tmp=reg(input);

				n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
				/* Sends REG userID */
			  	if(n == -1)
			    	exit(1);

			    addrlen = sizeof(addr);

				TimerOn(fd);

				n = recvfrom(fd, buffer, 1000, 0, (struct sockaddr*)&addr, &addrlen);
				/* Receives RRG status */
				if(n == -1)
					exit(1);
				
				TimerOff(fd);
				
			 	buffer[n]='\0';
				char teste[8] = "RRG OK\n";

			 	if ((strcmp(buffer,teste))==0)
					printf("User \"%s\" registered successfully\n",input);

				else if ((strcmp(buffer,nok))==0 || (strcmp(buffer,dup))==0)
					printf("User \"%s\" registration failed\n",input);

				memset(buffer,'\0',sizeof(char)*1000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);

			}

			if(!strcmp(cmd, "unregister") || !strcmp(cmd, "unr")){

				tmp=unregister(input);

				n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
				if(n == -1)
					exit(1);

				addrlen = sizeof(addr);

				TimerOn(fd);

				n = recvfrom(fd, buffer, 1000, 0, (struct sockaddr*)&addr, &addrlen);
				if(n == -1)
					exit(1);
				
				TimerOff(fd);

				buffer[n]='\0';

				if ((strcmp(buffer,ok2))==0){
					active = 0;
					printf("User \"%s\" unregistered successfully\n",input);
				}

				else if ((strcmp(buffer,nok2))==0)
					printf("User \"%s\" unregistration failed\n",input);

				memset(buffer,'\0',sizeof(char)*1000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);

			}

			if(!strcmp(cmd, "login")){
				if(user == 1){
					printf("nao e possivel fazer login\n");
				}
				else{
					tmp=login(input);

					n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
					if(n == -1)
						exit(1);

					addrlen = sizeof(addr);

					TimerOn(fd);

					n = recvfrom(fd, buffer, 1000, 0, (struct sockaddr*)&addr, &addrlen);
					if(n == -1)
						exit(1);

					TimerOff(fd);

					buffer[n]='\0';

					if ((strcmp(buffer,ok3))==0){
						strcpy(uid,input); // Store active user ID
						user = 1;
						printf("You are now logged in\n");
					}

					else if ((strcmp(buffer,nok3))==0)
						printf("Login failed\n");
					
				}
				memset(buffer,'\0',sizeof(char)*1000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);
			}

			if(!strcmp(cmd, "logout")){
				if(user == 0){
					printf("nao e possivel fazer logout\n");
				}
				else{
					tmp=logout(input);

					n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
					if(n == -1)
						exit(1);

					addrlen = sizeof(addr);

					TimerOn(fd);

					n = recvfrom(fd, buffer, 1000, 0, (struct sockaddr*)&addr, &addrlen);
					if(n == -1)
						exit(1);

					TimerOff(fd);

					buffer[n]='\0';

					if ((strcmp(buffer,ok4))==0){
						strcpy(uid,"00000000000000");
						user = 0;
						active = 0;
						printf("You are now logged out\n");
					}

					else if ((strcmp(buffer,nok4))==0)
						printf("Logout failed\n");
				}
				memset(buffer,'\0',sizeof(char)*1000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);
			}

			if(!strcmp(cmd, "showuid") || !strcmp(cmd, "su")){
				char copy[15];
				strcpy(copy,uid);
				char *user = strtok(copy, " ");
				printf("%s\n", user);
				freeaddrinfo(res);
				close(fd);
				memset(cmd,0,32);
			}

			if(!strcmp(cmd, "groups") || !strcmp(cmd, "gl")){
				tmp=groups(input);
				printf("%s", tmp);
				int e;
				n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
				if(n == -1)
					exit(1);

				addrlen = sizeof(addr);

				TimerOn(fd);

				n = recvfrom(fd, bufferg, 10000, 0, (struct sockaddr*)&addr, &addrlen);
				if(n == -1)
					exit(1);
				
				TimerOff(fd);

				bufferg[n]='\0';
				char bg[10000];
				char ng[3];
				int count = 1;
				strncpy(bg,bufferg+7,10000);
				strncpy(ng, bufferg+4, 3);
				if(ng[2] == '0'){
					memset(ng,'\0',sizeof(char)*3);
					memset(bg,'\0',sizeof(char)*10000);
					strncpy(bg,bufferg+6,10000);
					strncpy(ng, bufferg+4, 2);
				}
				printf("number of groups: %s\n",ng);
				for (e = 0; e < strlen(bg); e++) {
					if (bg[e] == ' ' && count == 3) {
						count = 1;
						printf("\n");
					}
					else if (bg[e] == ' ') {
						count++;
						printf("%c", bg[e]);
					}
					else {
						printf("%c", bg[e]);
					}
				}
				memset(bufferg,'\0',sizeof(char)*10000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);
			}

			if(!strcmp(cmd, "subscribe") || !strcmp(cmd, "s")){
				if(user == 0){
					printf("user must be logged in\n");
				}
				else {


					tmp=subscribe(input);
					n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
					if(n == -1)
						exit(1);

					addrlen = sizeof(addr);

					TimerOn(fd);

					n = recvfrom(fd, buffer, 1000, 0, (struct sockaddr*)&addr, &addrlen);
					if(n == -1)
						exit(1);
					
					TimerOff(fd);

					buffer[n]='\0';
					if ((strcmp(buffer,ok5))==0)
						printf("User subscribed successfully\n");

					else if ((strcmp(buffer,nok5))==0)
						printf("User subscription failed\n");

					else if ((strcmp(buffer,e_user))==0)
						printf("User is invalid\n");

					else if ((strcmp(buffer,e_grp))==0)
						printf("Group is invalid\n");

					else if ((strcmp(buffer,e_gname))==0)
						printf("Group name is invalid\n");

					else if ((strcmp(buffer,e_full))==0)
						printf("Group is full\n");

					else {
						printf("New group created and subscribed - %c%c\n", buffer[8], buffer[9]);
					}
				}
				memset(buffer,'\0',sizeof(char)*1000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);
			}

			if(!strcmp(cmd, "unsubscribe") || !strcmp(cmd, "u")){
				if(user == 0){
					printf("user must be logged in\n");
				}
				else {


					tmp=unsubscribe(input);
					n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
					if(n == -1)
						exit(1);

					addrlen = sizeof(addr);

					TimerOn(fd);

					n = recvfrom(fd, buffer, 1000, 0, (struct sockaddr*)&addr, &addrlen);
					if(n == -1)
						exit(1);
					
					TimerOff(fd);

					buffer[n]='\0';
					if ((strcmp(buffer,ok6))==0)
						printf("User unsubscribed successfully\n");

					else if ((strcmp(buffer,nok6))==0)
						printf("User unsubscription failed\n");

					else if ((strcmp(buffer,e_user2))==0)
						printf("User is invalid\n");

					else if ((strcmp(buffer,e_grp2))==0)
						printf("Group is invalid\n");
				}
				memset(buffer,'\0',sizeof(char)*1000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);
			}

			if(!strcmp(cmd, "my_groups") || !strcmp(cmd, "mgl")){
				if(user == 0){
					printf("user must be logged in\n");
				}
				else {
					tmp=my_groups(input);
					int j;
					n = sendto(fd, tmp, strlen(tmp), 0, res->ai_addr, res->ai_addrlen);
					if(n == -1)
						exit(1);

					addrlen = sizeof(addr);

					TimerOn(fd);

					n = recvfrom(fd, bufferg, 10000, 0, (struct sockaddr*)&addr, &addrlen);
					if(n == -1)
						exit(1);
					
					TimerOff(fd);

					bufferg[n]='\0';
					char bg[10000];
					char tmp[10000];
					int count = 1;
					if(strcmp(bufferg,no_groups) == 0){
						printf("No groups have been subscribed\n");
					}
					else{
						strncpy(bg,bufferg+6,10000);
						strncpy(tmp,bufferg+4,10000);
						char *ng = strtok(tmp, " ");
						printf("number of groups subscribed: %s\n",ng);
						for (j = 0; j < strlen(bg); j++) {
							if (bg[j] == ' ' && count == 3) {
								count = 1;
								printf("\n");
							}
							else if (bg[j] == ' ') {
								count++;
								printf("%c", bg[j]);
							}
							else {
								printf("%c", bg[j]);
							}
						}
					}
                    memset(bg,'\0',sizeof(char)*10000);
                    memset(tmp,'\0',sizeof(char)*10000);
				}
                
				memset(bufferg,'\0',sizeof(char)*10000);
				memset(cmd,0,32);
				freeaddrinfo(res);
				close(fd);
			}

			if(!strcmp(cmd, "select") || !strcmp(cmd, "sag")){
				if(user == 0){
					printf("user must be logged in\n");
				}
				else {
					strcpy(gid,input);
					active = 1;
					freeaddrinfo(res);
					close(fd);
					memset(cmd,0,32);
				}
			}

			if(!strcmp(cmd, "showgid") || !strcmp(cmd, "sg")){
				if(user == 0){
					printf("user must be logged in\n");
				}
				else {
					printf("%s\n", gid);
					freeaddrinfo(res);
					close(fd);
					memset(cmd,0,32);
				}
			}
		}

		else if(!strcmp(cmd, "ulist") || !strcmp(cmd, "ul") || !strcmp(cmd, "post") || !strcmp(cmd, "retrieve") || !strcmp(cmd, "r")) {

			int fd_tcp, errorcode_tcp;
			ssize_t n_tcp;
			struct addrinfo hints_tcp,*res_tcp;

			fd_tcp = socket(AF_INET, SOCK_STREAM, 0);
			if(fd_tcp==-1)/*error*/exit(1);

			memset(&hints_tcp,0,sizeof hints_tcp);
			hints_tcp.ai_family=AF_INET;            //IPv4
			hints_tcp.ai_socktype=SOCK_STREAM;      //TCP socket

			errorcode_tcp = getaddrinfo(dp.default_ip,dp.default_port,&hints_tcp,&res_tcp);
			if(errorcode_tcp!=0)/*error*/exit(1);

	  		n_tcp=connect(fd_tcp,res_tcp->ai_addr,res_tcp->ai_addrlen);
	  		if(n_tcp==-1){
				printf("%d\n", errno);
				exit(1);
			}

			if(!strcmp(cmd, "ulist") || !strcmp(cmd, "ul")){
				tmp=ulist(input);
				n_tcp = write(fd_tcp,tmp,strlen(tmp));
				if(n_tcp==-1){
					printf("Error writing to TCP socket\n");
					exit(1);
				}
                n_tcp = read(fd_tcp, buffer, 7);
				if(n_tcp==-1){
					printf("Error reading TCP socket\n");
					exit(1);
				}
				buffer[n_tcp]='\0';

				char gname[25];

				if((strcmp(buffer,ok7))==0){
					memset(buffer,'\0',sizeof(char)*1000);
					printf("Group name: ");
					while(*buffer != ' '){
                    	n_tcp = read(fd_tcp, buffer, 1);
						if(*buffer != ' '){
							printf("%s", buffer);
						}
					   
                    }
					printf("\n");
					
                    while(*buffer != '\n'){
                        memset(buffer,'\0',sizeof(char)*1);
                        n_tcp = read(fd_tcp, buffer, 1);
						if(*buffer == '\n'){
							break;
						}
                        if(*buffer == ' '){
                            printf("\n");
                            memset(buffer,'\0',sizeof(char)*1);
                        }
                        else { 
                            printf("%s", buffer);
                        }

                    }
                    memset(buffer,'\0',sizeof(char)*1);

                }
				else if((strcmp(buffer,nok7))==0){
					printf("That group does not exist\n");
                    memset(buffer,'\0',sizeof(char)*7);
				}
                memset(gname,'\0',sizeof(char)*25);
				freeaddrinfo(res_tcp);
				close(fd_tcp);
				memset(cmd,0,32);
			}
			if(!strcmp(cmd, "post")){
				if(user == 0){
					printf("You must be logged in\n");
					freeaddrinfo(res_tcp);
					close(fd_tcp);
					continue;
				}
				
				if(active == 0){
					printf("You have no selected group\n");
					freeaddrinfo(res_tcp);
					close(fd_tcp);
					continue;
				}
				tmp=post(input);
				int nleft = strlen(tmp);
				if(tmp[nleft-1] == '\n'){
					while(nleft > 0){
						n_tcp = write(fd_tcp,tmp,strlen(tmp));
						nleft -= n_tcp;
						if(n_tcp==-1){
							printf("Error writing to TCP socket\n");
							exit(1);
						}
					}
				}

				else{
					
					char len[12];
					int length;
					FILE *fp;

					strcat(tmp,FileName);
					strcat(tmp, " ");
					fp = fopen(FileName, "rb");

					if(fp==NULL){
						printf("File open error");
						exit(1);   
					}

					fseek (fp, 0, SEEK_END);
					length = ftell (fp);
					fseek (fp, 0, SEEK_SET);
					sprintf(len,"%d ",length);
					strcat(tmp, len);
					while(nleft > 0){
						n_tcp = write(fd_tcp,tmp,strlen(tmp));
						nleft -= n_tcp;
						if(n_tcp==-1){
							printf("Error writing to TCP socket\n");
							exit(1);
						}
					}   

					/* Read data from file and send it */
					while(1){
						/* First read file in chunks of 256 bytes */
						unsigned char data[1024]={0};
						int nread = fread(data,1,1024,fp);        

						/* If read was success, send data. */
						if(nread > 0){
							//printf("Sending \n");
							write(fd_tcp, data, nread);
						}
						if (nread < 1024){
							if (feof(fp)){
								//printf("End of file\n");
							}
							if (ferror(fp))
								printf("Error reading\n");
							break;
						}
					}

					fclose(fp);
					memset(len,'\0',sizeof(char)*11);
					n_tcp = write(fd_tcp, new_line, strlen(new_line));
				}

				n_tcp = read(fd_tcp, buffer, 1000);
				if(n_tcp==-1){
					printf("Error reading TCP socket\n");
					exit(1);
				}
				buffer[n_tcp]='\0';

				if((strcmp(buffer,nok8))==0){
					printf("Could not post your message\n");
				}
				else{
					char r[5];
					memset(r,'\0',sizeof(char)*5);
					strncpy(r,buffer+4,strlen(buffer));
					printf("New message created %s", r);
				}
				
				memset(buffer,'\0',sizeof(char)*1000);
				
				freeaddrinfo(res_tcp);
				close(fd_tcp);
				memset(cmd,0,32);
				memset(FileName,0,sizeof(FileName));
			}

			if(!strcmp(cmd, "retrieve") || !strcmp(cmd, "r")){
				if(user == 0){
					printf("You must be logged in\n");
					freeaddrinfo(res_tcp);
					close(fd_tcp);
					continue;
				}
				
				if(active == 0){
					printf("You have no selected group\n");
					freeaddrinfo(res_tcp);
					close(fd_tcp);
					continue;
				}

				tmp=retrieve(input);
				int nleft = strlen(tmp);
				while(nleft > 0){
					n_tcp = write(fd_tcp,tmp,strlen(tmp));
					nleft -= n_tcp;
					if(n_tcp==-1){
						printf("Error writing to TCP socket\n");
						exit(1);
					}
				}

				n_tcp = read(fd_tcp, buffer, 7);
				if(n_tcp==-1){
					printf("Error reading TCP socket\n");
					exit(1);
				}
				buffer[n_tcp]='\0';

				if((strcmp(buffer,"RRT OK "))==0){
					int n;
					char string[300];
					memset(string,'\0',sizeof(char)*300);
					memset(buffer,'\0',sizeof(char)*7);
					n_tcp = read(fd_tcp, buffer, 2); //read N
					
					if(buffer[1] == ' '){
						n = atoi(&buffer[0]); 
						memset(buffer,'\0',sizeof(char)*2);
					}
					else{
						n = atoi(buffer);
						memset(buffer,'\0',sizeof(char)*2);
						n_tcp = read(fd_tcp, buffer, 1);
						memset(buffer,'\0',sizeof(char)*1);
					}

					int message = 1;
					
					while(n>0){
                        int text_size = 0;
					    char tsize[4];
                        memset(tsize, '\0', sizeof(char) * 4);
						int spaces = 0;
						while(spaces != 2){
							memset(buffer, '\0', sizeof(char) * 1);
							n_tcp = read(fd_tcp, buffer, 1);
							if(*buffer == ' '){
								spaces++;
							}
							strcat(string,buffer);
						}
						//cheguei ao tsize
						memset(buffer, '\0', sizeof(char) * 1);

						while(*buffer != ' '){
							memset(buffer, '\0', sizeof(char) * 1);
							n_tcp = read(fd_tcp, buffer, 1);
							if(*buffer != ' '){
								strcat(tsize, buffer);
							}
						}
						text_size = atoi(tsize);
						//text_size--;
						memset(tsize, '\0', sizeof(char)*4);
						sprintf(tsize, "%d ", text_size);
						strcat(string, tsize);
						memset(buffer, '\0', sizeof(char) * 1);
						
						while(text_size > 0){
							memset(buffer, '\0', sizeof(char) * 1);
							n_tcp = read(fd_tcp, buffer, 1);
							text_size = text_size - n_tcp;
							if(*buffer != '\n'){
								strcat(string, buffer);
							}
						}

			
						memset(buffer, '\0', sizeof(char) * 1);
						n_tcp = read(fd_tcp, buffer, 1);	// read '\n'
						memset(buffer, '\0', sizeof(char) * 1);
						n_tcp = read(fd_tcp, buffer, 1);	// read '/' or next message
    

						if(*buffer != '/'){
							n--;
							printf("%d. %s\n", message, string);
							message++;
							text_size = 0;
							memset(tsize,'\0',sizeof(char)*4);
							memset(string,'\0',sizeof(char)*300);
							strcat(string,buffer);
							memset(buffer, '\0', sizeof(char) * 1);
						}
						else{
							memset(buffer, '\0', sizeof(char) * 1);
							char fname[25];
							char fsize[11];
							int size;
							int bytesReceived = 0;
							char recvBuff[1];
							memset(recvBuff, '0', sizeof(recvBuff));
                            memset(fname, '\0', sizeof(char) * 25);

							FILE *fp;

							n_tcp = read(fd_tcp, buffer, 1);
							memset(buffer, '\0', sizeof(char) * 1);
							while(*buffer != ' '){
								memset(buffer, '\0', sizeof(char) * 1);
								n_tcp = read(fd_tcp, buffer, 1);
								if(*buffer != ' '){
									strcat(fname,buffer); //get file name
								}
							}
							printf("%d. %s; file stored: %s\n", message, string, fname);

							fp = fopen(fname, "a");
							if(NULL == fp){
       	 						printf("Error opening file");
         						exit(1);
    						}

							n_tcp = read(fd_tcp, buffer, 1);
							strcat(fsize,buffer);
							while(*buffer != ' '){
								memset(buffer, '\0', sizeof(char) * 1);
								n_tcp = read(fd_tcp, buffer, 1);
								if(*buffer != ' '){
									strcat(fsize,buffer); //get file size
								}
							}
							size = atoi(fsize);
							n_tcp = read(fd_tcp, buffer, 1);
							fputc(*buffer,fp);

							while(size > 0){
								bytesReceived = read(fd_tcp, recvBuff, 1);
								if(size != 1){
									fputc(*recvBuff,fp);
								}
								size = size-1;
    						}
							

							if(bytesReceived < 0){
        						printf("\n Read Error \n");
    						}
							fclose(fp);
							n_tcp = read(fd_tcp, buffer, 1);
							memset(buffer,'\0',sizeof(char)*1);
							message++;
							n--;
                            
							memset(string,'\0',sizeof(char)*300);
							memset(recvBuff,'\0',sizeof(char)*1);
                            memset(fname, '\0', sizeof(char) * 25);
                            memset(fsize, '\0', sizeof(char) * 11);
							memset(buffer, '\0', sizeof(char) * 1);

						}
					}
				}

				else if((strcmp(buffer,"RRT NOK"))==0){
					memset(buffer,'\0',sizeof(char)*7);
					printf("There is a problem with the RTV request\n");
				}

				else if((strcmp(buffer,"RRT EOF"))==0){
					memset(buffer,'\0',sizeof(char)*7);
					printf("There are no messages available\n");
				}
				
				memset(buffer, '\0', sizeof(char) * 1);
				freeaddrinfo(res_tcp);
				close(fd_tcp);
				memset(cmd,0,32);
			}
		}
	}
	return 0;
}
