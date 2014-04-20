#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "uthash.h"

int producer(char* file_name){
    FILE *fp = fopen(file_name, "r");
    size_t sizeof_line = 0;
    char* buffer = NULL;
    char *token = NULL;
    char *delim = "\"|\n";
    char *key;
    int count=1;
    ssize_t line_length = 0;
    /*Taking the next line from fp, allocating space for it in buffer and returns
     * the length of the line
     */
    while ((line_length = getline(&buffer, &sizeof_line, fp)) > 0) {
        printf("%s\n",buffer);
        token = strtok(buffer,delim);
        if(token == NULL){  //case where there is no token
            continue;       //go to next line

        }else{
            //token = strtok(NULL,delim);         //token should now be the key
            //printf("This is the token: %s\n",token);
            key = malloc(strlen(token)*sizeof(char));   //store this token as key
            sprintf(key,"%s",token);
            //count++;
            Node customer = createNode(NULL,NULL,NULL,NULL,NULL,0);

            printf("%s\n", token);
            while(token != NULL && strlen(token) > 0){
                if(strcmp(token,"")==0){
                    token = strtok(NULL,delim);
                    printf("%s\n", token);
                    continue;
                }
                else if(count == 1){
                    //customer->name = token;
                    customer->name = malloc((strlen(token)+1));
                    strcpy(customer->name,token);
                    printf("%s, %s\n", token, customer->name);
                }
                else if(count == 2){
                    //customer->id = token;
                    customer->id= malloc((strlen(token)+1));
                    strcpy(customer->id,token);
                }
                else if(count == 3){
                    customer->credit = atof(token);
                }
                else if(count == 4){
                    //customer->address = token;
                    customer->address = malloc((strlen(token)+1));
                    strcpy(customer->address,token);

                }
                else if(count == 5){
                    //customer->state = token;
                    customer->state = malloc((strlen(token)+1));
                    strcpy(customer->state,token);
                }
                else{
                    //customer->zip = token;
                    customer->zip= malloc((strlen(token)+1));
                    strcpy(customer->zip,token);
                }
                token = strtok(NULL, delim);
                count++;
            }
            add_node(key,customer);
            count = 1;
        }
    }
    if(buffer) free(buffer);
    fclose(fp);
    return 1;

}
