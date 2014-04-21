#include "manager.h"

extern int num_cats;

int main(int argc, char** argv){
    int i;
    if(argc != 4){
        printf("Incorrect # of arguments\n");
        return 0;
    }
    if(!valid_file(argv[1]) || !valid_file(argv[2]) || !valid_file(argv[3])){
        printf("Please make sure all files exist and contain information\n");
        return 0;
    }
    int id = shmget(1000, sizeof(int), IPC_CREAT | 0666);
    int* total_money = shmat(id, NULL, 0);
    memset(total_money, 0, sizeof(int));

    //create the customer database
    parse_db(argv[1]);

    num_cats = number_categories(argv[3]);
    //gets the categories and creates shared memory based on them
    parse_categories(argv[3]);

    //launch consumer processes
    for(i = 0; i < num_cats; i++){
        pids[i] = fork();
        if(pids[i] == 0){
            char buffer1[128];
            char buffer2[128];
            char buffer3[128];
            char buffer4[128];

            sprintf(buffer1,"%d",shm_keys[i]);
            sprintf(buffer2,"%d",customer_db_key);
            sprintf(buffer3,"%d",num_customers);
            sprintf(buffer4,"%d",1000);

            char *args[6];
            args[0] = "consumer";
            args[1] = buffer1;
            args[2] = buffer2; //macro for customer database shared memory key
            args[3] = buffer3;
            args[4] = buffer4;
            args[5] = 0;
            execvp("./consumer", args);
            fprintf(stderr,"An error has occurred, could not start proccess\n");
            _exit(0);
        }
    }

    //launch producer thread
    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, producer, argv[2]);

    //wait for producer to finish
    pthread_join(producer_thread, NULL);

    //close processes
    for(i = 0; i < num_cats; i++){
        kill(pids[i], SIGINT);
        waitpid(pids[i], NULL, 0);
    }
    return 0;
}

int valid_file(char* file_name){
    FILE* fp = fopen(file_name, "r");
    fseek (fp, 0, SEEK_END);
    int size = ftell(fp);
    if(fp){
        if(size == 0){
            fclose(fp);
            return 0;
        }else{
            fclose(fp);
            return 1;
        }
    }
    return 0;
}

int number_categories(char* file_name){
    FILE* fp = fopen(file_name, "r");
    size_t sizeof_line = 0;
    char* category = NULL;
    ssize_t line_length = 0;
    int count = 0;
    while((line_length = getline(&category, &sizeof_line, fp)) > 0){
        count++;
    }
    fclose(fp);
    return count;
}


