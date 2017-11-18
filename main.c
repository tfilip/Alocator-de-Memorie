#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0


void intialize(int n){
	printf("intialize CALLED %d\n",n);
	//TBC
}

void finalize(){
	printf("finalize CALLED\n");
	//TBC
}

void dump(){
	printf("dump CALLED\n");
	//TBC
}

void alloc(int size){
	printf("alloc CALLED %d\n",size);
	//TBC
}

void custom_free(int index){
	printf("free CALLED %d\n",index);
	//TBC
}

void fill(int index, int size, int value){
	printf("fill CALLED %d %d %d\n", index, size, value);
	//TBC
}

void show(){
	printf("show CALLED\n");
	//TBC
}


void read(){

	char command[20];
	const char delim[]=" ";
	while(TRUE){
		
		if(fgets(command,20,stdin)==NULL) break;
		
		char *token = strtok(command,delim);
		while(token != NULL){
			
			if(!strcmp(token, "DUMP")){
				printf("%s", token);
				dump();
			}
			else if(!strcmp(token, "FINALIZE")){
				printf("%s", token);
				finalize();
			}
			else if(!strcmp(token, "INITIALIZE")){
				int n;
				printf("%s ", token);
				token = strtok(NULL, delim);
				printf("%s", token);
				n = atoi(token);
				intialize(n);
			}
			else if(!strcmp(token, "ALLOC")){
				int size;
				printf("%s ", token);
				token = strtok(NULL, delim);
				printf("%s", token);
				size = atoi(token);
				alloc(size);
			}
			else if(!strcmp(token, "FREE")){
				int index;
				printf("%s ", token);
				token = strtok(NULL, delim);
				printf("%s", token);
				index = atoi(token);
				custom_free(index); 
			}
			else if(!strcmp(token, "FILL")){
				int index,size,value;
				printf("%s ", token);
				token = strtok(NULL, delim);
				printf("%s ", token);
				index = atoi(token);
				token = strtok(NULL, delim);
				printf("%s ", token);
				size = atoi(token);
				token = strtok(NULL, delim);
				printf("%s", token);
				value = atoi(token);
				fill(index, size, value);
			}

			token = strtok(NULL, delim);


		}
	}
}


int main(){
	read();
	return 0;
}