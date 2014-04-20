#include "parse.h"

#define start_key 1200

extern shm_t *queue;
extern char **cat_names;
extern int *pids;
extern int *shm_keys;
extern shm_t customerdb_shm;
extern customer_db_key;

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

int parse_db(char* file_name){
    FILE *fp = fopen(file_name, "r");
    size_t sizeof_line = 0;
    char* buffer = NULL;
    char *token = NULL;
    char *delim = "\"|\n";
    char *key;
    int count=1;
    int num_customers;
    ssize_t line_length = 0;

    /*Taking the next line from fp, allocating space for it in buffer and returns
     * the length of the line
     */

    num_customers = get_num_customers(file_name);
    customer_db_key = 1100;
    customerdb_shm = shmget(customer_db__key, num_customers *sizeof(struct Node_), IPC_CREAT | 0666);

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
            //add_node(key,customer);
             count = 1;
        }
    }
    if(buffer) free(buffer);
    fclose(fp);
    return 1;
}

int parse_categories(char* file_name, int num_cats){
    FILE *fp = fopen(file_name, "r");
    size_t sizeof_line = 0;
    char* category = NULL;
    ssize_t line_length = 0;
    /*Taking the next line from fp, allocating space for it in buffer and returns
     * the length of the line
     */
    pids = malloc(sizeof(int) * num_cats);
    queue = malloc(sizeof(shm_t) * num_cats);
    cat_names = malloc(sizeof(char*) * num_cats);

    int i = 0;
    while ((line_length = getline(&category, &sizeof_line, fp)) > 0){
        cat_names[i] = malloc(strlen(category) + 1);
        strcpy(cat_names[i], category);
        i++;
    }
    if(category) free(category);
    fclose(fp);

    shm_keys = malloc(sizeof(int) * num_cats);
    for(i = 0; i < num_cats; i++){
        shm_keys[i] = start_key + i;
    }

    for(i = 0; i < num_cats; i++){
        //taking a key, creating an shm, setting permission to r/w for the user
        queue[i] = shmget(shm_keys[i], sizeof(struct shm_map_), IPC_CREAT|0666);
    }

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

/*int main(int argc, char** argv){
    parse_db("database.txt");
    print_files();
}
*/
