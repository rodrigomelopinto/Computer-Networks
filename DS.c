#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#define PORT "58008"

int errno;
int verbose = 0;
int groups = 1;
int n_groups = 0;


typedef struct GROUPLIST{
	int no_groups;
	char group_no[100][3];
	char group_name[100][25];
	char group_mess[100][5];
}GROUPLIST;


void swap(GROUPLIST *list, int i, int j){
	char temp[24];
    strcpy(temp, list->group_no[i]);
    strcpy(list->group_no[i], list->group_no[j]);
    strcpy(list->group_no[j], temp);
	bzero(temp,24);
	strcpy(temp, list->group_name[i]);
	strcpy(list->group_name[i], list->group_name[j]);
	strcpy(list->group_name[j], temp);
	bzero(temp,24);
	strcpy(temp, list->group_mess[i]);
	strcpy(list->group_mess[i], list->group_mess[j]);
	strcpy(list->group_mess[j], temp);
	bzero(temp,24);
}
 
// A function to implement bubble sort
void bubbleSort(GROUPLIST *list){
   int i, j;
   for (i = 0; i < list->no_groups-1; i++){
 
       // Last i elements are already in place
    	for (j = 0; j < list->no_groups-i-1; j++){
        	if (atoi(list->group_no[j])>atoi(list->group_no[j+1])){
            	swap(list,j, j+1);
			}
		}
   	}
}

void count_groups(){
	DIR *d;
    struct dirent *dir;
    d = opendir("GROUPS");
    if (d){
        while ((dir = readdir(d)) != NULL){
            if(dir->d_name[0]=='.'){
                continue;
            }
            if(strlen(dir->d_name)>2){
                continue;
            }
			n_groups++;
		}
		closedir(d);
	}
}

char* parseFlags(int argc, char* argv[]) {

	if(argc == 2){
        if(!strcmp(argv[1],"-v")){
            /* Verbose mode activated */
            verbose = 1;
            return PORT;
        }
    }
	
	if(argc == 3){
    	if(!strcmp(argv[1],"-p")){
			/*  FS port was specified */
			char *port=(char*)malloc(sizeof(char)*strlen(argv[2]));
			strcpy(port,argv[2]);

      		return port;
    	}
        if(!strcmp(argv[1],"-v")){
            /* Verbose mode activated */
            verbose = 1;
            return PORT;
        }
  	}
    
    if(argc == 4){
        char *port=(char*)malloc(sizeof(char)*strlen(argv[2]));
		strcpy(port,argv[2]);
        verbose = 1;
      	return port;
    }

  	else {
  		return PORT;
  	}

  	return NULL; /* Program should never get here*/
  
}

int CreateUserDir(char *UID, char *PASS){
    char user_dirname[20];
	char file_name[50];
    int ret;
	FILE *fp;
    sprintf(user_dirname,"USERS/%s",UID);
	sprintf(file_name, "%s/%s_pass.txt", user_dirname, UID);
    ret=mkdir(user_dirname,0700);
    if(ret==-1){
        return(0);
    }
	fp = fopen(file_name, "w");
	fwrite(PASS,1,strlen(PASS),fp);
	fclose(fp);
	memset(user_dirname,'\0',sizeof(char)*20);
	memset(file_name,'\0',sizeof(char)*50);
    return(1);
}

int CreateGroupDir(char *UID, char *GNAME, char *GID){
    char group_dirname[20];
	char file_name[50];
	char file_name2[50];
	char path[30];
	char group_id[3];
    int ret;
	FILE *fp;
	if(strcmp(GID, "00") == 0){
		groups = n_groups + 1;
		if(groups < 10){
			sprintf(group_id, "0%d", groups);
			sprintf(group_dirname,"GROUPS/%s",group_id);
			groups++;
		}

		else{
			sprintf(group_id, "%d", groups);
			sprintf(group_dirname,"GROUPS/%s",group_id);
			groups++;
		}
		ret=mkdir(group_dirname,0700);
    	if(ret==-1){
        	return(0);
		}
		n_groups++;
		sprintf(path, "%s/MSG", group_dirname);
		sprintf(file_name, "%s/%s_name.txt", group_dirname, group_id);
		ret=mkdir(path,0700);
		if(ret==-1){
        	return(0);
    	}
		fp = fopen(file_name, "w");
		fwrite(GNAME,1,strlen(GNAME),fp);
		fclose(fp);

		sprintf(file_name2, "%s/%s.txt", group_dirname, UID);
		fp = fopen(file_name2, "w");
		fclose(fp);
		memset(group_dirname,'\0',sizeof(char)*20);
		memset(file_name,'\0',sizeof(char)*50);
		memset(file_name2,'\0',sizeof(char)*50);
		memset(path,'\0',sizeof(char)*30);
		memset(group_id,'\0',sizeof(char)*3);
		return(2);
	}

	else{
		sprintf(group_dirname,"GROUPS/%s",GID);
		sprintf(file_name2, "%s/%s.txt", group_dirname, UID);
		fp = fopen(file_name2, "w");
		fclose(fp);
		memset(group_dirname,'\0',sizeof(char)*20);
		memset(file_name,'\0',sizeof(char)*50);
		memset(file_name2,'\0',sizeof(char)*50);
		memset(path,'\0',sizeof(char)*30);
		memset(group_id,'\0',sizeof(char)*3);
		return(1);
	}
	
	
	
}

int DelUserDir(char *UID){
    char user_dirname[20];
    sprintf(user_dirname,"USERS/%s",UID);
    if(rmdir(user_dirname)==0){
		memset(user_dirname,'\0',sizeof(char)*20);
        return(1);
    }
    else{
		memset(user_dirname,'\0',sizeof(char)*20);
        return(0);
    }
}

int DelPassFile(char *UID){
    char pathname[50];
    sprintf(pathname,"USERS/%s/%s_pass.txt",UID,UID);
    if(unlink(pathname)==0){
		memset(pathname,'\0',sizeof(char)*50);
        return(1);
    }
    else{
		memset(pathname,'\0',sizeof(char)*50);
        return(0);
    }
}

int DelLoginFile(char *UID){
    char pathname[50];
    sprintf(pathname,"USERS/%s/%s_login.txt",UID,UID);
    if(unlink(pathname)==0){
		memset(pathname,'\0',sizeof(char)*50);
        return(1);
    }
    else{
		memset(pathname,'\0',sizeof(char)*50);
        return(0);
    }
}

int CreateLoginFile(char *UID, char *PASS){
	char password[10];
	char file_name_pass[50];
	char file_name_login[50];
	char user_dirname[20];

	FILE *fp1;
	FILE *fp2;

	sprintf(user_dirname,"USERS/%s",UID);
	sprintf(file_name_pass, "%s/%s_pass.txt", user_dirname, UID);
	sprintf(file_name_login, "%s/%s_login.txt", user_dirname, UID);

	fp1 = fopen(file_name_pass, "r");
	fgets(password,9,fp1);
	fclose(fp1);

	if(strcmp(password,PASS) != 0){
		memset(password,'\0',sizeof(char)*10);
		memset(file_name_pass,'\0',sizeof(char)*50);
		memset(file_name_login,'\0',sizeof(char)*50);
		memset(user_dirname,'\0',sizeof(char)*20);
		return(0);
	}

	fp2 = fopen(file_name_login, "w");
	fwrite("login feito",1,strlen("login feito"),fp2);
	fclose(fp2);
	memset(password,'\0',sizeof(char)*10);
	memset(file_name_pass,'\0',sizeof(char)*50);
	memset(file_name_login,'\0',sizeof(char)*50);
	memset(user_dirname,'\0',sizeof(char)*20);
	return(1);
}

int ListGroupsDir(GROUPLIST *list){
    DIR *d;
	DIR *d2;
	struct dirent *entry;
    struct dirent *dir;
    int i=0;
    FILE *fp;
    char GIDname[528];
	char path[528];
    list->no_groups=0;
    d = opendir("GROUPS");
    if (d){
        while ((dir = readdir(d)) != NULL){
            if(dir->d_name[0]=='.'){
                continue;
            }
            if(strlen(dir->d_name)>2){
                continue;
            }
			int msg_count = 0;
            strcpy(list->group_no[i], dir->d_name);
            sprintf(GIDname,"GROUPS/%s/%s_name.txt",dir->d_name,dir->d_name);
			sprintf(path,"GROUPS/%s/MSG",dir->d_name);
            fp=fopen(GIDname,"r");
            if(fp){
                fscanf(fp,"%24s",list->group_name[i]);
                fclose(fp);
            }
			memset(GIDname,'\0',sizeof(char)*528);
			d2 = opendir(path);
			while ((entry = readdir(d2)) != NULL) {
				if(entry->d_name[0]=='.'){
                	continue;
            	}
				if(strlen(dir->d_name)>4){
                continue;
            	}
         		msg_count++;
			}
			closedir(d2);
			char msg[4];
			char tmp[4];
			memset(tmp,'\0',sizeof(char)*4);
			sprintf(msg,"%d", msg_count);
			if(strlen(msg) == 1){
				strcat(tmp, "000");
				strcat(tmp, msg);
			}
			if(strlen(msg) == 2){
				strcat(tmp, "00");
				strcat(tmp, msg);
			}
			if(strlen(msg) == 3){
				strcat(tmp, "0");
				strcat(tmp, msg);
			}
			if(strlen(msg) == 4){
				strcat(tmp, msg);
			}
			strcpy(list->group_mess[i], tmp);
			memset(tmp,'\0',sizeof(char)*4);
			memset(msg,'\0',sizeof(char)*4);
            i++;
            if(i==99){
                break;
            }
            
        }
		list->no_groups=i;
		closedir(d);
    }
	else{
        return(-1);
    }


	if(list->no_groups > 1){
		bubbleSort(list);
	}
	
	return(list->no_groups);
}

int ListGroupsUser(GROUPLIST *list, char *USER){
    DIR *d;
	DIR *d2;
	struct dirent *entry;
    struct dirent *dir;
	int n_groups_tmp = 0;
    int i=0;
	int sub = 0;
    FILE *fp;
    char GIDname[528];
	char USERname[528];
	char path[528];
    list->no_groups=0;
    d = opendir("GROUPS");
    if (d){
        while ((dir = readdir(d)) != NULL){
            if(dir->d_name[0]=='.'){
                continue;
            }
            if(strlen(dir->d_name)>2){
                continue;
            }
			int msg_count = 0;
			n_groups_tmp++;
            sprintf(GIDname,"GROUPS/%s/%s_name.txt",dir->d_name,dir->d_name);
			sprintf(USERname,"GROUPS/%s/%s.txt", dir->d_name, USER);
			sprintf(path,"GROUPS/%s/MSG",dir->d_name);
			fp=fopen(USERname, "r");
			if(fp){
				fclose(fp);
				strcpy(list->group_no[i], dir->d_name);
				fp=fopen(GIDname,"r");
				if(fp){
					fscanf(fp,"%24s",list->group_name[i]);
					fclose(fp);
				}
				sub++;
				d2 = opendir(path);
				while ((entry = readdir(d2)) != NULL) {
					if(entry->d_name[0]=='.'){
                		continue;
            		}
					if(strlen(dir->d_name)>4){
					continue;
					}
					msg_count++;
				}
				closedir(d2);
				char msg[4];
				char tmp[4];
				memset(tmp,'\0',sizeof(char)*4);
				sprintf(msg,"%d", msg_count);
				if(strlen(msg) == 1){
					strcat(tmp, "000");
					strcat(tmp, msg);
				}
				if(strlen(msg) == 2){
					strcat(tmp, "00");
					strcat(tmp, msg);
				}
				if(strlen(msg) == 3){
					strcat(tmp, "0");
					strcat(tmp, msg);
				}
				if(strlen(msg) == 4){
					strcat(tmp, msg);
				}
				strcpy(list->group_mess[i], tmp);
				memset(tmp,'\0',sizeof(char)*4);
				memset(msg,'\0',sizeof(char)*4);
				i++;
			}
			memset(GIDname,'\0',sizeof(char)*528);
			memset(USERname,'\0',sizeof(char)*528);
            if(i==99){
                break;
            }
            
        }
		list->no_groups=i;
		closedir(d);
    }
	else{
        return(-1);
    }


	if(sub > 1){
		bubbleSort(list);
	}
	
	n_groups = n_groups_tmp;
	return(sub);
}


int DelUserFile(char *UID, char *GID){
    char pathname1[50];
	char pathname2[50];
	DIR *d;
    sprintf(pathname1,"GROUPS/%s/%s.txt",GID,UID);
	sprintf(pathname2,"GROUPS/%s",GID);

	d = opendir(pathname2);
	if(d){
		closedir(d);
		if(unlink(pathname1)==0){
			memset(pathname1,'\0',sizeof(char)*50);
        	return(1);
		}

    	else{
			memset(pathname1,'\0',sizeof(char)*50);
        	return(0);
    	}
	}
	else{
		memset(pathname2,'\0',sizeof(char)*50);
		return(2);
	}
	
}


char* reg(char *input) {
	char *nok = (char*)malloc(sizeof(char)*9);
	char *dup = (char*)malloc(sizeof(char)*9);
  	char *ok = (char*)malloc(sizeof(char)*8);

	char user[100];
	char pass[100];

	nok = "RRG NOK\n";
	dup = "RRG DUP\n";
	ok = "RRG OK\n";

	sscanf(input, "%s %s", user, pass);

	if(strlen(user) != 5 || strlen(pass) != 8){
		return nok;
	}
	int res = CreateUserDir(user, pass);
	if(res == 0){
		return dup;
	}
	return ok;
}

char* unregister(char *input) {
	char *nok = (char*)malloc(sizeof(char)*9);
  	char *ok = (char*)malloc(sizeof(char)*8);

	char user[100];
	char pass[100];

	nok = "RUN NOK\n";
	ok = "RUN OK\n";

	sscanf(input, "%s %s", user, pass);

	if(strlen(user) != 5 || strlen(pass) != 8){
		return nok;
	}
	
	int del_pass = DelPassFile(user);
	DelLoginFile(user);
	if(del_pass == 1){
		int res = DelUserDir(user);
		if(res == 1){
			return ok;
		}
	}
	return nok;
}

char* login(char *input) {
	char *nok = (char*)malloc(sizeof(char)*9);
  	char *ok = (char*)malloc(sizeof(char)*8);

	char user[100];
	char pass[100];
	

	nok = "RLO NOK\n";
	ok = "RLO OK\n";

	sscanf(input, "%s %s", user, pass);

	if(strlen(user) != 5 || strlen(pass) != 8){
		return nok;
	}
	
	int res = CreateLoginFile(user, pass);
	if(res == 0){
		return nok;
	}
	
	return ok;
}

char* logout(char *input) {
	char *nok = (char*)malloc(sizeof(char)*9);
  	char *ok = (char*)malloc(sizeof(char)*8);

	char user[100];
	char pass[100];
	

	nok = "ROU NOK\n";
	ok = "ROU OK\n";

	sscanf(input, "%s %s", user, pass);

	if(strlen(user) != 5 || strlen(pass) != 8){
		return nok;
	}
	
	int res = DelLoginFile(user);
	if(res == 0){
		return nok;
	}

	return ok;
}

char* subscribe(char *input) {
	char *nok = (char*)malloc(sizeof(char)*9);
  	char *ok = (char*)malloc(sizeof(char)*8);
	char *e_user = (char*)malloc(sizeof(char)*12);
	char *e_grp = (char*)malloc(sizeof(char)*11);
	char *e_gname = (char*)malloc(sizeof(char)*13);
	char *e_full = (char*)malloc(sizeof(char)*12);
	char *new_group = (char*)malloc(sizeof(char)*14);
	GROUPLIST *list = (GROUPLIST*)malloc(sizeof(GROUPLIST)*100);

	char user[100];
	char gid[100];
	char gname[100];
	char group_number[3];
	
	nok = "RGS NOK\n";
	ok = "RGS OK\n";
	e_user = "RGS E_USER\n";
	e_grp = "RGS E_GRP\n";
	e_gname = "RGS E_GNAME\n";
	e_full = "RGS E_FULL\n";

	sscanf(input, "%s %s %s", user, gid, gname);

	if(strlen(user) != 5){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		return e_user;
	}
	

	if(atoi(gid) > n_groups){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		return e_grp;
	}
	
	if(groups == 100){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		return e_full;
	}

	if(strlen(gid) != 2){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		return e_grp;
	}

	if(strlen(gname) > 24){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		return e_gname;
	}

	int res = -1;
	int res2;
	if(strcmp(gid, "00") == 0){
		res = ListGroupsDir(list);
	}
	else{
		res2 = CreateGroupDir(user, gname, gid);
	}

	if(res == 0){
		res2 = CreateGroupDir(user, gname, gid);
	}

	if(res != 0){
		for(int i = 0; i<res; i++){
			if(strcmp(gname, list->group_name[i]) == 0){
				memset(user,'\0',sizeof(char)*100);
				memset(gid,'\0',sizeof(char)*100);
				memset(gname,'\0',sizeof(char)*100);
				free(list);
				return e_gname;
			}
		}
		res2 = CreateGroupDir(user, gname, gid);
	}


	if(res2 == 1){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		free(list);
		return ok;
	}

	if(res2 == 0){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		free(list);
		return nok;
	}

	if(res2 == 2){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		memset(gname,'\0',sizeof(char)*100);
		free(list);
		memset(group_number, '\0', sizeof(char)*3);
		if(n_groups < 10){
			sprintf(group_number, "0%d", n_groups);
		}
		else{
			sprintf(group_number, "%d", n_groups);
		}
		sprintf(new_group, "RGS NEW %s\n", group_number);
		return new_group;
	}
	return nok;
}

char* unsubscribe(char *input) {
	char *nok = (char*)malloc(sizeof(char)*9);
  	char *ok = (char*)malloc(sizeof(char)*8);
	char *e_user = (char*)malloc(sizeof(char)*12);
	char *e_grp = (char*)malloc(sizeof(char)*11);

	char user[100];
	char gid[100];
	
	nok = "RGU NOK\n";
	ok = "RGU OK\n";
	e_user = "RGU E_USER\n";
	e_grp = "RGU E_GRP\n";

	sscanf(input, "%s %s", user, gid);

	if(strlen(user) != 5){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		return e_user;
	}

	if(strlen(gid) != 2){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		return e_grp;
	}

	int res;
	res = DelUserFile(user,gid);

	if(res == 0){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		return e_user;
	}
	
	if(res == 1){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		return ok;
	}

	if(res == 2){
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		return e_grp;
	}

	else{
		memset(user,'\0',sizeof(char)*100);
		memset(gid,'\0',sizeof(char)*100);
		return nok;
	}
}

char* group_list(char *input) {
	char *zero = (char*)malloc(sizeof(char)*8);
  	char *ok = (char*)malloc(sizeof(char)*4000);
	GROUPLIST *list = (GROUPLIST*)malloc(sizeof(GROUPLIST)*100);
	zero = "RGL 0\n";
	sprintf(ok,"RGL ");


	int res;
	res = ListGroupsDir(list);

	if(res == 0){
		free(list);
		return zero;
	}
	
	char tmp[3];
	if(res != 0){
		sprintf(tmp,"%d", res);
		strcat(ok, tmp);
		for(int i = 0;i<res;i++){
			strcat(ok, " ");
			strcat(ok,list->group_no[i]);
			strcat(ok, " ");
			strcat(ok,list->group_name[i]);
			strcat(ok, " ");
			strcat(ok,list->group_mess[i]);
		}
		strcat(ok, "\n");
		free(list);
		return ok;
	}

	return zero;
}

char* my_group_list(char *input) {
	char *zero = (char*)malloc(sizeof(char)*8);
	char *e_user = (char*)malloc(sizeof(char)*12);
  	char *ok = (char*)malloc(sizeof(char)*4000);
	GROUPLIST *list = (GROUPLIST*)malloc(sizeof(GROUPLIST)*100);
	
	zero = "RGM 0\n";
	e_user = "RGM E_USR\n";
	sprintf(ok,"RGL ");

	char user[100];

	sscanf(input, "%s", user);

	if(strlen(user) != 5){
		memset(user,'\0',sizeof(char)*100);
		return e_user;
	}

	int res;
	res = ListGroupsUser(list, user);

	if(res == 0){
		free(list);
		return zero;
	}
	
	char tmp[3];
	if(res != 0){
		sprintf(tmp,"%d", res);
		strcat(ok, tmp);
		for(int i = 0;i<res;i++){
			strcat(ok, " ");
			strcat(ok,list->group_no[i]);
			strcat(ok, " ");
			strcat(ok,list->group_name[i]);
			strcat(ok, " ");
			strcat(ok,list->group_mess[i]);
		}
		strcat(ok, "\n");
		free(list);
		return ok;
	}

	return zero;
}



int main(int argc, char* argv[]) {

	char *input;
	char *cmd = (char*)malloc(sizeof(char) * 32);
	int i;

	

	char *default_port=(char*)malloc(sizeof(char)*30);

	char *tmp;

	struct stat st = {0};
	struct stat st2 = {0};

	if(stat("./USERS", &st) == -1){
		mkdir("./USERS", 0700);
	}

	if(stat("./GROUPS", &st2) == -1){
		mkdir("./GROUPS", 0700);
	}

	count_groups();

	if (fork() > 0) {
		/* UDP Process */

		int fd;
		ssize_t n;
		socklen_t addrlen;
		struct addrinfo hints,*res;
		struct sockaddr_in addr;
		char buffer[1000];


		memset(&hints,0,sizeof(hints));

		hints.ai_family=AF_INET;            //IPv4
		hints.ai_socktype=SOCK_DGRAM;       //UDP socket
		hints.ai_flags=AI_PASSIVE|AI_NUMERICSERV;


		default_port=parseFlags(argc,argv);
		
		n = getaddrinfo(NULL,default_port, &hints, &res);
		fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(fd==-1){/*error*/
		  exit(1);
		}

		if(n != 0) {
			printf("getaddrinfo returned an error while opening on localhost\n");
			printf("Error:%d\n",errno);
			exit(1);
		}

		n=bind(fd,res->ai_addr,res->ai_addrlen);
		if(n==-1)/*error*/
		  exit(1);

		

		while(1){

			addrlen=sizeof(addr);

			n=recvfrom(fd,buffer,1000,0,(struct sockaddr*)&addr,&addrlen);
			if(n==-1)/*error*/
				exit(1);
			

			if(verbose == 1){
				write(1,"Received: ",10);write(1,buffer,n);
			}
			

			buffer[n]='\0';

			i = 0;

			input=buffer;

			while(*input != ' ' && *input != '\0' && *input != '\n'){
				*(cmd + i) = *input;
				input++;
				i++;
			}

			if(*input != '\0'){
				input++;
			}

			if(!strcmp(cmd, "REG")){

				tmp=reg(input);
				
				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			if(!strcmp(cmd, "UNR")){

				tmp=unregister(input);
				
				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			if(!strcmp(cmd, "LOG")){

				tmp=login(input);
				
				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			if(!strcmp(cmd, "OUT")){

				tmp=logout(input);
				
				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			if(!strcmp(cmd, "GSR")){
				
				tmp=subscribe(input);
				
				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			if(!strcmp(cmd, "GUR")){

				tmp=unsubscribe(input);
				
				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			if(!strcmp(cmd, "GLS")){
				tmp=group_list(input);
				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			if(!strcmp(cmd, "GLM")){

				tmp=my_group_list(input);
				printf("%s", tmp);

				n = sendto(fd, tmp, strlen(tmp), 0, (struct sockaddr*)&addr, addrlen);
				if(n == -1)
	  				exit(1);

			}

			memset(cmd, '\0', sizeof(char) * 32);

		}



		freeaddrinfo(res);

		close(fd);
	}

	else {
		/* TCP Process */

		int fd_tcp,newfd, errorcode_tcp;
		ssize_t n_tcp;
		socklen_t addrlen_tcp;
		struct addrinfo hints_tcp,*res_tcp;
		struct sockaddr_in addr_tcp;
		char buffer_tcp[10000];
		char *cmd = malloc(sizeof(char) * 27);

		if((fd_tcp=socket(AF_INET,SOCK_STREAM,0))==-1)exit(1);//error

		memset(&hints_tcp,0,sizeof(hints_tcp));
		
		hints_tcp.ai_family=AF_INET;            //IPv4
		hints_tcp.ai_socktype=SOCK_STREAM;       //TCP socket
		hints_tcp.ai_flags=AI_PASSIVE;


		default_port=parseFlags(argc,argv);

		if((errorcode_tcp=getaddrinfo(NULL, default_port, &hints_tcp, &res_tcp))!=0)/*error*/exit(1);

		if(bind(fd_tcp,res_tcp->ai_addr,res_tcp->ai_addrlen)==-1)/*error*/exit(1);

		n_tcp=bind(fd_tcp,res_tcp->ai_addr,res_tcp->ai_addrlen);
		
		if(listen(fd_tcp,5)==-1)/*error*/ exit(1);

		while(1){

			addrlen_tcp = sizeof(addr_tcp);

			if((newfd=accept(fd_tcp,(struct sockaddr*)&addr_tcp,&addrlen_tcp))==-1)/*error*/exit(1);

			if(verbose == 1){
				printf("Accepted TCP connection. Reading...\n");
			}

			n_tcp=read(newfd,cmd,3);
  			if(n_tcp==-1)/*error*/exit(1);
			
			if(verbose == 1){
				printf("%s ", cmd);
			}
			
			if(!strcmp(cmd, "ULS")){
				memset(buffer_tcp,'\0',sizeof(char)*10000);

				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);

				memset(buffer_tcp,'\0',sizeof(char)*10000);

				char gid[100];

				n_tcp=read(newfd,gid,2);
  				if(n_tcp==-1)/*error*/exit(1);
				
				if(verbose == 1){
					printf("%s\n", gid);
				}
				
				
				char *nok = (char*)malloc(sizeof(char)*9);
				char *ok = (char*)malloc(sizeof(char)*8);
				
				nok = "RUL NOK\n";
				sprintf(ok,"RUL OK ");

				DIR *d;
				struct dirent *dir;
				FILE *fp;
				char GIDname[528];
				char new_line[3] = "\n";
				char group[528];
				char fname[528];
				char user[7];
				char group_name[26];
				sprintf(group,"GROUPS/%s",gid);
				sprintf(GIDname,"GROUPS/%s/%s_name.txt",gid,gid);
				sprintf(fname,"%s_name.txt",gid);
				d = opendir(group);
				if (d){
					n_tcp=write(newfd,ok,sizeof(char)*strlen(ok));
  					if(n_tcp==-1)/*error*/exit(1);
					
					fp = fopen(GIDname, "r");
					if(fp){
						fscanf(fp,"%24s",group_name);
                		fclose(fp);
						strcat(group_name, " ");
					}

					n_tcp=write(newfd,group_name,sizeof(char)*strlen(group_name));
  					if(n_tcp==-1)/*error*/exit(1);
					
					memset(group_name,'\0',sizeof(char)*26);
					
					while ((dir = readdir(d)) != NULL){
						if(dir->d_type == DT_REG){
							if(strcmp(dir->d_name, fname) == 0){
								continue;
							}
							else{
								sscanf(dir->d_name, "%5s", user);
								strcat(user, " ");

								n_tcp=write(newfd,user,sizeof(char)*strlen(user));
  								if(n_tcp==-1)/*error*/exit(1);
								
								memset(user,'\0',sizeof(char)*7);
							}
						}	
					}
					closedir(d);

				}
				else{
					n_tcp=write(newfd,nok,sizeof(char)*strlen(nok));
					if(n_tcp==-1)/*error*/exit(1);
				}
				n_tcp=write(newfd,new_line,sizeof(char)*strlen(new_line));
  				if(n_tcp==-1)/*error*/exit(1);

				//close(newfd);
				
			}
				

			if(!strcmp(cmd, "PST")){
				char *ok = (char*)malloc(sizeof(char)*10);
				char user[6];
				char gid[3];
				char tsize[4];
				char path[528];
				char dirname[528];
				char author[528];
				char text[528];
				char file[528];
				char fname[25];
				char fsize[11];
				char MID[4];
				char msg[4];
				int msg_count = 0;
				int ret;
				int t_size;
				int f_size;
				DIR *d;
				struct dirent *entry;
				FILE *fp;
				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);
				memset(buffer_tcp,'\0',sizeof(char)*10000);
				memset(user,'\0',sizeof(char)*6);
				n_tcp=read(newfd,user,5);
  				if(n_tcp==-1)/*error*/exit(1);
				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);
				memset(buffer_tcp,'\0',sizeof(char)*10000);
				memset(gid,'\0',sizeof(char)*3);
				n_tcp=read(newfd,gid,2);
  				if(n_tcp==-1)/*error*/exit(1);
				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);
				memset(buffer_tcp,'\0',sizeof(char)*10000);
				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);
				memset(tsize,'\0',sizeof(char)*4);
				while(*buffer_tcp != ' '){
					strcat(tsize, buffer_tcp);
					memset(buffer_tcp,'\0',sizeof(char)*10000);
					n_tcp=read(newfd,buffer_tcp,1);
  					if(n_tcp==-1)/*error*/exit(1);
				}
				memset(buffer_tcp,'\0',sizeof(char)*10000);
				t_size = atoi(tsize);
				if(verbose == 1){
					printf("%s %s %s\n", user, gid, tsize);
				}
				sprintf(path,"GROUPS/%s/MSG",gid);
				d = opendir(path);
				while ((entry = readdir(d)) != NULL) {
					if(entry->d_name[0]=='.'){
						continue;
					}
					if(strlen(entry->d_name)>4){
					continue;
					}
					msg_count++;
				}
				closedir(d);
				if(msg_count == 0){
					sprintf(dirname,"GROUPS/%s/MSG/0001",gid);
					ret=mkdir(dirname,0700);
					if(ret==-1){
						exit(1);
					}
					sprintf(author,"GROUPS/%s/MSG/0001/A U T H O R.txt",gid);
					sprintf(text,"GROUPS/%s/MSG/0001/T E X T.txt",gid);
					fp = fopen(author, "w");
					if(fp){
						fwrite(user,1 , strlen(user), fp);
						fclose(fp);
					}
					fp = fopen(text, "w");
					while(t_size > 0){
						n_tcp=read(newfd,buffer_tcp,1);
  						if(n_tcp==-1)/*error*/exit(1);
						fputc(*buffer_tcp,fp);
						t_size--;
						memset(buffer_tcp,'\0',sizeof(char)*10000);
					}
					fclose(fp);
					n_tcp=read(newfd,buffer_tcp,1);
  					if(n_tcp==-1)/*error*/exit(1);
					if(*buffer_tcp == '\n'){
						ok = "RPT 0001\n";
						n_tcp=write(newfd,ok,sizeof(char)*strlen(ok));
  						if(n_tcp==-1)/*error*/exit(1);
					}
					else{
						memset(buffer_tcp,'\0',sizeof(char)*10000);
						n_tcp=read(newfd,buffer_tcp,1);
  						if(n_tcp==-1)/*error*/exit(1);
						memset(fname,'\0',sizeof(char)*25);
						while(*buffer_tcp != ' '){
							strcat(fname,buffer_tcp);
							memset(buffer_tcp,'\0',sizeof(char)*10000);
							n_tcp=read(newfd,buffer_tcp,1);
  							if(n_tcp==-1)/*error*/exit(1);
						}
						memset(buffer_tcp,'\0',sizeof(char)*10000);
						n_tcp=read(newfd,buffer_tcp,1);
  						if(n_tcp==-1)/*error*/exit(1);
						memset(fsize,'\0',sizeof(char)*11);
						while(*buffer_tcp != ' '){
							strcat(fsize, buffer_tcp);
							memset(buffer_tcp,'\0',sizeof(char)*10000);
							n_tcp=read(newfd,buffer_tcp,1);
  							if(n_tcp==-1)/*error*/exit(1);
						}
						memset(buffer_tcp,'\0',sizeof(char)*10000);
						f_size = atoi(fsize);
						sprintf(file,"GROUPS/%s/MSG/0001/%s",gid, fname);
						fp = fopen(file, "w");
						while(f_size > 0){
							n_tcp=read(newfd,buffer_tcp,1);
							if(n_tcp==-1)/*error*/exit(1);
							fputc(buffer_tcp[0],fp);
							f_size--;
							memset(buffer_tcp,'\0',sizeof(char)*10000);
						}
						fclose(fp);
						n_tcp=read(newfd,buffer_tcp,1);
						if(n_tcp==-1)/*error*/exit(1);
						if(*buffer_tcp == '\n'){
							ok = "RPT 0001\n";
							n_tcp=write(newfd,ok,sizeof(char)*strlen(ok));
							if(n_tcp==-1)/*error*/exit(1);
						}
					}
				}
				else{
					sprintf(msg,"%d", (msg_count+1));
					memset(MID, '\0', sizeof(char)*4);
					if(strlen(msg) == 1){
						strcat(MID, "000");
						strcat(MID, msg);
					}
					if(strlen(msg) == 2){
						strcat(MID, "00");
						strcat(MID, msg);
					}
					if(strlen(msg) == 3){
						strcat(MID, "0");
						strcat(MID, msg);
					}
					if(strlen(msg) == 4){
						strcat(MID, msg);
					}
					sprintf(dirname,"GROUPS/%s/MSG/%s",gid, MID);
					ret=mkdir(dirname,0700);
					if(ret==-1){
						exit(1);
					}
					sprintf(author,"GROUPS/%s/MSG/%s/A U T H O R.txt",gid, MID);
					sprintf(text,"GROUPS/%s/MSG/%s/T E X T.txt",gid, MID);
					fp = fopen(author, "w");
					if(fp){
						fwrite(user,1 , strlen(user), fp);
						fclose(fp);
					}
					fp = fopen(text, "w");
					while(t_size > 0){
						n_tcp=read(newfd,buffer_tcp,1);
  						if(n_tcp==-1)/*error*/exit(1);
						fputc(*buffer_tcp,fp);
						t_size--;
						memset(buffer_tcp,'\0',sizeof(char)*10000);
					}
					fclose(fp);
					n_tcp=read(newfd,buffer_tcp,1);
  					if(n_tcp==-1)/*error*/exit(1);
					if(*buffer_tcp == '\n'){
						sprintf(ok,"RPT %s\n", MID);
						n_tcp=write(newfd,ok,sizeof(char)*strlen(ok));
  						if(n_tcp==-1)/*error*/exit(1);
					}
					else{
						memset(buffer_tcp,'\0',sizeof(char)*10000);
						n_tcp=read(newfd,buffer_tcp,1);
  						if(n_tcp==-1)/*error*/exit(1);
						
						memset(fname,'\0',sizeof(char)*25);
						while(*buffer_tcp != ' '){
							strcat(fname,buffer_tcp);
							memset(buffer_tcp,'\0',sizeof(char)*10000);
							n_tcp=read(newfd,buffer_tcp,1);
  							if(n_tcp==-1)/*error*/exit(1);
						}
						memset(buffer_tcp,'\0',sizeof(char)*10000);
						n_tcp=read(newfd,buffer_tcp,1);
  						if(n_tcp==-1)/*error*/exit(1);
						
						memset(fsize,'\0',sizeof(char)*11);
						while(*buffer_tcp != ' '){
							strcat(fsize, buffer_tcp);
							memset(buffer_tcp,'\0',sizeof(char)*10000);
							n_tcp=read(newfd,buffer_tcp,1);
  							if(n_tcp==-1)/*error*/exit(1);
						}
						memset(buffer_tcp,'\0',sizeof(char)*10000);
						f_size = atoi(fsize);
						sprintf(file,"GROUPS/%s/MSG/%s/%s",gid, MID, fname);
						fp = fopen(file, "w");
						while(f_size > 0){
							n_tcp=read(newfd,buffer_tcp,1);
							if(n_tcp==-1)/*error*/exit(1);
							fputc(buffer_tcp[0],fp);
							f_size--;
							memset(buffer_tcp,'\0',sizeof(char)*10000);
						}
						fclose(fp);
						n_tcp=read(newfd,buffer_tcp,1);
						if(n_tcp==-1)/*error*/exit(1);
						if(*buffer_tcp == '\n'){
							sprintf(ok,"RPT %s\n", MID);
							n_tcp=write(newfd,ok,sizeof(char)*strlen(ok));
							if(n_tcp==-1)/*error*/exit(1);
						}
					}
				}
				memset(buffer_tcp,'\0',sizeof(char)*10000);
				memset(user,'\0',sizeof(char)*6);
				memset(gid,'\0',sizeof(char)*3);
				memset(tsize,'\0',sizeof(char)*4);
				memset(path,'\0',sizeof(char)*528);
				memset(dirname,'\0',sizeof(char)*528);
				memset(author,'\0',sizeof(char)*528);
				memset(text,'\0',sizeof(char)*528);
				memset(file,'\0',sizeof(char)*528);
				memset(fname,'\0',sizeof(char)*25);
				memset(fsize,'\0',sizeof(char)*11);
				memset(MID,'\0',sizeof(char)*4);
				memset(msg,'\0',sizeof(char)*4);

				//close(newfd);
			}

			if(!strcmp(cmd, "RTV")){
				char *nok = (char*)malloc(sizeof(char)*9);
				char *eof = (char*)malloc(sizeof(char)*9);
				char *ok = (char*)malloc(sizeof(char)*10);

				nok = "RRT NOK\n";
				eof = "RRT EOF\n";

				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);

				memset(buffer_tcp,'\0',sizeof(char)*10000);

				char new_line[3] = "\n";
				char space[3] = " ";
				char barra[3] = "/";
				char user[6];
				char gid[3];
				char tsize[5];
				char path[528];
				char group[528];
				char user_file[528];
				char dirname[528];
				char author[528];
				char text[528];
				char file[528];
				char fname[528];
				char fsize[11];
				char N[4];
				char MID[4];
				char UID[7];
				char msg[258];
				int msg_count = 0;
				int count = 0;
				int retrieve = 0;
				int t_size;
				int f_size;
				DIR *d;
				DIR *d2;
				struct dirent *dir;
				FILE *fp;

				n_tcp=read(newfd,user,5);
  				if(n_tcp==-1)/*error*/exit(1);
				
				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);

				memset(buffer_tcp,'\0',sizeof(char)*10000);

				n_tcp=read(newfd,gid,2);
  				if(n_tcp==-1)/*error*/exit(1);

				n_tcp=read(newfd,buffer_tcp,1);
  				if(n_tcp==-1)/*error*/exit(1);

				memset(buffer_tcp,'\0',sizeof(char)*10000);

				n_tcp=read(newfd,MID,4);
  				if(n_tcp==-1)/*error*/exit(1);
				
				if(verbose == 1){
					printf("%s %s %s\n", user, gid, MID);
				}

				retrieve = atoi(MID);

				sprintf(path,"GROUPS/%s/MSG",gid);

				struct dirent **files;
				int n = scandir(path, &files, NULL, alphasort);
				if (n < 0) {
					fprintf(stderr,
						"Cannot open %s (%s)\n", dirname, strerror(errno));
						exit(EXIT_FAILURE);
				}

				sprintf(group,"GROUPS/%s",gid);
				d = opendir(group);
				if(d){
					sprintf(user_file,"GROUPS/%s/%s.txt",gid,user);
					fp = fopen(user_file, "r");
					if(fp){
						fclose(fp);
						if(n == 2){
							n_tcp=write(newfd,eof,sizeof(char)*strlen(eof));
							if(n_tcp==-1)/*error*/exit(1);
						}

						sprintf(ok,"RRT OK ");
						n_tcp=write(newfd,ok,sizeof(char)*strlen(ok));
						if(n_tcp==-1)/*error*/exit(1);
						msg_count = n - 2;
						count = (msg_count - retrieve) + 1;
						sprintf(N,"%d",count);

						n_tcp=write(newfd,N,sizeof(char)*strlen(N));
						if(n_tcp==-1)/*error*/exit(1);

						for(int i = retrieve+1; i<msg_count || i<retrieve+20; i++){
							n_tcp=write(newfd,space,sizeof(char)*strlen(space));
							if(n_tcp==-1)/*error*/exit(1);
							sprintf(dirname,"GROUPS/%s/MSG/%s",gid,files[i]->d_name);
							sprintf(msg,"%s ",files[i]->d_name);
							n_tcp=write(newfd,msg,sizeof(char)*strlen(msg));
							if(n_tcp==-1)/*error*/exit(1);

							memset(msg,'\0',sizeof(char)*258);

							d2 = opendir(dirname);

							if(d2){
								sprintf(author,"GROUPS/%s/MSG/%s/A U T H O R.txt",gid,files[i]->d_name);
								sprintf(text,"GROUPS/%s/MSG/%s/T E X T.txt",gid,files[i]->d_name);

								fp = fopen(author, "r");
								if(fp){
									fscanf(fp,"%5s",UID);
									fclose(fp);
								}
								strcat(UID, " ");
								n_tcp=write(newfd,UID,sizeof(char)*strlen(UID));
								if(n_tcp==-1)/*error*/exit(1);

								memset(UID,'\0',sizeof(char)*7);
								memset(author,'\0',sizeof(char)*528);

								fp = fopen(text, "r");
								if(fp){
									fseek (fp, 0, SEEK_END);
									t_size = ftell (fp);
									fseek (fp, 0, SEEK_SET);
									sprintf(tsize,"%d ",t_size);

									n_tcp=write(newfd,tsize,sizeof(char)*strlen(tsize));
									if(n_tcp==-1)/*error*/exit(1);

									while(t_size > 0){
										fread(buffer_tcp,1,1,fp);
										n_tcp=write(newfd,buffer_tcp,sizeof(char)*strlen(buffer_tcp));
										if(n_tcp==-1)/*error*/exit(1);
										t_size--;
										memset(buffer_tcp,'\0',sizeof(char)*10000);
									}
									fclose(fp);

									memset(text,'\0',sizeof(char)*528);
									memset(tsize,'\0',sizeof(char)*5);
								}
								while ((dir = readdir(d2)) != NULL){
									if(dir->d_name[0]=='.'){
										continue;
									}
									if(strcmp(dir->d_name, "A U T H O R.txt") != 0 && strcmp(dir->d_name, "T E X T.txt") != 0){
										char c;
										n_tcp=write(newfd,space,sizeof(char)*strlen(space));
										if(n_tcp==-1)/*error*/exit(1);
										n_tcp=write(newfd,barra,sizeof(char)*strlen(barra));
										if(n_tcp==-1)/*error*/exit(1);
										n_tcp=write(newfd,space,sizeof(char)*strlen(space));
										if(n_tcp==-1)/*error*/exit(1);

										sprintf(fname,"%s ",dir->d_name);
										n_tcp=write(newfd,fname,sizeof(char)*strlen(fname));
										if(n_tcp==-1)/*error*/exit(1);

										memset(fname,'\0',sizeof(char)*528);

										sprintf(file,"GROUPS/%s/MSG/%s/%s",gid,files[i]->d_name,dir->d_name);
										fp = fopen(file, "rb");
										if(fp){
											fseek (fp, 0, SEEK_END);
											f_size = ftell (fp);
											fseek (fp, 0, SEEK_SET);
											sprintf(fsize,"%d ",f_size);

											n_tcp=write(newfd,fsize,sizeof(char)*strlen(fsize));
											if(n_tcp==-1)/*error*/exit(1);

											while(f_size > 0){
												c = fgetc(fp);
												n_tcp=write(newfd,&c,1);
												if(n_tcp==-1)/*error*/exit(1);
												f_size--;
												memset(&c,'\0',sizeof(char)*1);
											}
											fclose(fp);
										}
										memset(file,'\0',sizeof(char)*528);
										memset(fsize,'\0',sizeof(char)*11);	
									}
								}
								closedir(d2);
							}
						}
						n_tcp=write(newfd,new_line,sizeof(char)*strlen(new_line));
						if(n_tcp==-1)/*error*/exit(1);
					}
					else{
						n_tcp=write(newfd,nok,sizeof(char)*strlen(nok));
						if(n_tcp==-1)/*error*/exit(1);
					}
					closedir(d);
				}

				else{
					n_tcp=write(newfd,nok,sizeof(char)*strlen(nok));
					if(n_tcp==-1)/*error*/exit(1);
				}
				for (int i = 0; i < n; i++) {
					free(files[i]);
				}
				free(files);
				
				//close(newfd);

			}

			else{
				/* ERROR */
			}

			memset(cmd, '\0', sizeof(char) * 32);


		}



		freeaddrinfo(res_tcp);
		
		close(fd_tcp);
	}

	return 0;
}
