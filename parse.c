#include "parse.h"

Node createNode(void* name, void* address, void* state, void* zip, void* id, int credit){
    Node llNode = malloc(sizeof(Node));
    llNode->name = name;
    llNode->address = address;
    llNode->state = state;
    llNode->zip = zip;
    llNode->id = id;
    llNode->credit = credit;
    return llNode;
}

struct hash *tokenHash = NULL;

void add_node(void*key, void* customer){
    struct hash *h;
    HASH_FIND_STR(tokenHash, key, h); //search token hash for the key
    if(h == NULL){  //if the token doesn't exist
        h = (struct hash*)malloc(sizeof(struct hash));
        h->key = key;  //the key for this new hash is the token
        //printf("Inserting %s file into key %s\n",fileName,token);
        h->customer = customer;
        HASH_ADD_STR(tokenHash, key, h); //add this hash node to the hash table
    }
    else{   //the token exists in the hash table
        //printf("Inserting %s into key %s\n",fileName,token);
        h->customer = customer;   //add this node to the sorted list associated with the token
    }
}

int parse_db(char* file_name){
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

int parse_categories(char* file_name){
    FILE *fp = fopen(file_name, "r");
    size_t sizeof_line = 0;
    char* category = NULL;
    int count=0;
    ssize_t line_length = 0;
    /*Taking the next line from fp, allocating space for it in buffer and returns
     * the length of the line
     */
    while ((line_length = getline(&category, &sizeof_line, fp)) > 0) {
        //create shared memory
        //create consumer process
    }
    if(category) free(category);
    fclose(fp);
    return 1;

}

void print_files() {
    struct hash *h;
    Node ptr;
    for(h=tokenHash; h != NULL; h=(struct hash*)(h->hh.next)) {
        printf("This is the token: %s\n", h->key);  //print the token
        ptr = h->customer;  //points to hash node
        if(ptr){     //while the pointer is not null
            printf("This is the customers name: %s\n", (char*)ptr->name);
            printf("This is the customers id#: %s\n", (char*)ptr->id);
            printf("This is the customers credit: %.2f\n", ptr->credit);
            printf("This is the customers address %s\n", (char*)ptr->address);
            printf("This is the customers state: %s\n", (char*)ptr->state);
            printf("This is the customers zip code: %s\n", (char*)ptr->zip);
        }
    }
}

int main(int argc, char** argv){
    parse_db("database.txt");
    print_files();
}
