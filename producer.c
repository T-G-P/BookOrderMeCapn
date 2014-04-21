#include "producer.h"

void * producer(void* file_name){
    FILE *fp = fopen((char*)file_name, "r");
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
            struct Order_ nWo;      //this is an order

            while(token != NULL && strlen(token) > 0){
                if(strcmp(token,"")==0){
                    token = strtok(NULL,delim);
                    continue;
                }
                else if(count == 1){
                    //customer->name = token;
                    strcpy(nWo.book_title,token);
                    //printf("%s, %s\n", token, customer->name);
                }
                else if(count == 2){
                    //customer->id = token;
                    nWo.price = atof(token);
                }
                else if(count == 3){
                    //customer->address = token;
                    strcpy(nWo.id,token);
                }
                else if(count == 4){
                    //customer->address = token;
                    strcpy(nWo.category,token);

                }
                else{
                    ;
                }
                token = strtok(NULL, delim);
                count++;
            }
            write_order(nWo);
            count = 1;
        }
    }
    if(buffer) free(buffer);
    fclose(fp);
    return NULL;
}
//in consumer use the key for shmget
//shmget returns an id. Get pointer from shmat

void write_order(struct Order_ cust_order){
    int i;
    for(i = 0; i<num_cats; i++){
        if(strcmp(cat_names[i],cust_order.category)==0){
            break;
        }
    }
    enqueue(cust_order,queue[i]);

}
