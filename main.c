#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define TRUE 1
#define FALSE 0


unsigned char *arena;



void intialize(int n){

	arena=(unsigned char *) calloc(n,sizeof(unsigned char));


}

void finalize(){	
	free(arena);
}

void dump(){
	
}

void alloc(int size){
 
}

void custom_free(int index){

}

void fill(int index, int size, int value){

}

void show(){

}


void read(){

	char command[20];
	const char delim[]=" ";
	while(TRUE){
		
		if(fgets(command,20,stdin)==NULL) break;
		
		char *token = strtok(command,delim);

		while(token != NULL){
			
			if(!strcmp(token, "DUMP\n")){
				printf("%s", token);
				dump();
			}
			else if(!strcmp(token, "FINALIZE\n")){
				printf("%s", token);
				finalize();
			}
			else if(!strcmp(token, "FINALIZE\0")){
				printf("%s\n", token);
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