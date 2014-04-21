#include "parse.h"

shmap *queue;
char **cat_names;
int *pids;
int *shm_keys;
int *queueid;
Node customerdb_shm;
int customer_db_key;
int num_customers;
int num_cats;

/*Node createNode(void* name, void* address, void* state, void* zip, void* id, int credit){
    Node llNode = malloc(sizeof(Node));
    llNode->name = name;
    llNode->address = address;
    llNode->state = state;
    llNode->zip = zip;
    llNode->id = id;
    llNode->credit = credit;
    sem_init(&llNode->ssem, 1, 1);
    sem_init(&llNode->rsem, 1, 1);
    return llNode;
}
*/

int parse_db(char* file_name){
    FILE *fp = fopen(file_name, "r");
    size_t sizeof_line = 0;
    char* buffer = NULL;
    char *token = NULL;
    char *delim = "\"|\n";
    char *key;
    int count=1;
    int index;
    ssize_t line_length = 0;

    /*Taking the next line from fp, allocating space for it in buffer and returns
     * the length of the line
     */

    num_customers = number_customers(file_name);
    customer_db_key = 1100;
    customerdb_shm = shmat(shmget(customer_db_key, num_customers * sizeof(struct Node_), IPC_CREAT | 0666), NULL, 0);

    memset(customerdb_shm, 0, sizeof(struct Node_) * num_customers);

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
            struct Node_ customer;

            printf("%s\n", token);
            while(token != NULL && strlen(token) > 0){
                if(strcmp(token,"")==0){
                    token = strtok(NULL,delim);
                    printf("%s\n", token);
                    continue;
                }
                else if(count == 1){
                    //customer->name = token;
                    strcpy(customer.name,token);
                    printf("%s, %s\n", token, (char *)customer.name);
                }
                else if(count == 2){
                    //customer->id = token;
                    strcpy(customer.id,token);
                }
                else if(count == 3){
                    customer.credit = atof(token);
                }
                else if(count == 4){
                    //customer->address = token;
                    strcpy(customer.address,token);
                }
                else if(count == 5){
                    //customer->state = token;
                    strcpy(customer.state,token);
                }
                else{
                    //customer->zip = token;
                    strcpy(customer.zip,token);
                }
                token = strtok(NULL, delim);
                count++;
            }
            sem_init(&customer.success,1,1);
            sem_init(&customer.rejection,1,1);

            //inserting customer node into customer into database here
            if(index < num_customers*sizeof(struct Node_)){
                customerdb_shm[index] = customer;
                index+=sizeof(struct Node_);
            }
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
    ssize_t line_length = 0;
    /*Taking the next line from fp, allocating space for it in buffer and returns
     * the length of the line
     */
    pids = malloc(sizeof(int) * num_cats);
    queue = malloc(sizeof(shmap) * num_cats);
    cat_names = malloc(sizeof(char*) * num_cats);
    queueid = malloc(sizeof(int)*num_cats);

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
        queueid[i] = shmget(shm_keys[i], sizeof(struct shm_map_), IPC_CREAT|0666);
        queue[i] = shmat(queueid[i],NULL,0);
        memset(queue[i], 0, sizeof(struct shm_map_));
    }

    return 1;
}

/*void print_files() {
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
  */

int number_customers(char* file_name){
    FILE* fp = fopen(file_name, "r");
    size_t sizeof_line = 0;
    char* customer = NULL;
    ssize_t line_length = 0;
    int count = 0;
    while((line_length = getline(&customer, &sizeof_line, fp)) > 0){
        count++;
    }
    fclose(fp);
    return count;
}
