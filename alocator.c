#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned char *arena;
unsigned int n;
int arena_index;

struct block{
    uint32_t next_index;
    uint32_t prev_index;
    uint32_t current_index;
    uint32_t block_size;
};


void intialize(int n){
    arena = calloc(n, sizeof(unsigned char *));
}

void finalize(){    
    free(arena);
    printf("\n");
}

void dump(){
    unsigned char *i=arena;
    unsigned int k=0;
    while(k<n){

        if(k%16==0)
            printf("%08X \t", k);


        printf("%02X ",*i);
        i++;
        k++;

        if (k%8==0&&k!=0)
            printf(" ");
        if (k%16==0&&k!=0)
            printf("\n");
        
    }

}

void alloc(int size){

    int schimbat;
    uint32_t *int_arena=(uint32_t*)arena;
    schimbat=0;
    if(arena_index==0){
        while(!schimbat){
            //daca este primul bloc de alocat din memorie
            if(*int_arena==0&&*(int_arena+2)==0){
                *(int_arena+2)=size;
                schimbat=1;
                printf("%d\n", 12);
                break;
            }/*
            //verific daca este ultima posibiltate si tot nu are loc
            if(*(int_arena)-(arena[*(int_arena+1)])-*(int_arena+2)<size && *int_arena==0 && *int_arena+2!=0){
                printf("TRACTOR\n" );
                break;
            }*/
            //verific daca are loc
            if(*(int_arena)-(arena[*(int_arena+1)])-*(int_arena+2)>=size){
            

                //calculez octetul la care incepe blocul pe care il aloc
                printf("%d\n", arena[*(int_arena+1)]);
                int octet=(arena[*(int_arena+1)])+*(int_arena+2);

                //aloc zona de memorie
                arena[octet]=*int_arena;
                arena[octet+sizeof(int)]=arena[*(int_arena+1)];
                arena[octet+2*sizeof(int)]=size;
                //schimb zonele de memorie in stanga si in dreapta
                *int_arena=octet;
                arena[*(int_arena+1)]=octet;
                //afisez octetul la care este zona libera de memorie
                printf("%d\n", octet);
                schimbat=1;
                break;

            }else{
                //setez int_arena pe byte-ul urmatorului bloc
                unsigned char* ptr = (unsigned char*) int_arena;
                ptr+=*int_arena;
                int_arena=(uint32_t*)ptr;

            }
        }
    }


}

void custom_free(int index){
    
}

void fill(int index, int size, int value){
    
}

void show(){

}

void parse_command(char* cmd)
{
    const char* delims = " \n";

    char* cmd_name = strtok(cmd, delims);
    if (!cmd_name) {
        goto invalid_command;
    }

    if (strcmp(cmd_name, "INITIALIZE") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        n=size;
        intialize(size);

    } else if (strcmp(cmd_name, "FINALIZE") == 0) {
        finalize();

    } else if (strcmp(cmd_name, "DUMP") == 0) {
        dump();

    } else if (strcmp(cmd_name, "ALLOC") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        alloc(size);

    } else if (strcmp(cmd_name, "FREE") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        custom_free(index);

    } else if (strcmp(cmd_name, "FILL") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        char* value_str = strtok(NULL, delims);
        if (!value_str) {
            goto invalid_command;
        }
        uint32_t value = atoi(value_str);
        fill(index, size, value);

    } else if (strcmp(cmd_name, "ALLOCALIGNED") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        char* align_str = strtok(NULL, delims);
        if (!align_str) {
            goto invalid_command;
        }
        uint32_t align = atoi(align_str);
        // TODO - ALLOCALIGNED

    } else if (strcmp(cmd_name, "REALLOC") == 0) {
        printf("Found cmd REALLOC\n");
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
       // TODO - REALLOC

    } else {
        goto invalid_command;
    }

    return;

invalid_command:
    printf("Invalid command: %s\n", cmd);
    exit(1);
}


int main(void)
{
    size_t read;
    char* line = NULL;
    size_t len;

    /* parse input line by line */
    while ((read = getline(&line, &len, stdin)) != -1) {
        /* print every command to stdout */
        printf("%s", line);

        parse_command(line);
    }

    free(line);

    return 0;
}
