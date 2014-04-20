#include "manager.h"

int main(int argc, char** argv){
    if(argc != 3){
        printf("Incorrect # of arguments\n");
        return 0;
    }
    if(!valid_file(argv[1]) || !valid_file(argv[2]) || !valid_file(argv[3])){
        printf("Please make sure all files exist and contain information\n");
        return 0;
    }

    //create the customer database
    parse_db(argv[1]);

    int num_cats = number_categories(argv[3]);
    //gets the categories and creates shared memory based on them
    parse_categories(argv[3], num_cats);

    //launch consumer processes
    for(int i = 0; i < num_cats; i++){
        pids[i] = fork();
        if(pids[i] == 0){
            char *args[4];
            args[0] = "consumer";
            args[1] = shm_keys[i];
            args[2] = CUSTDB_KEY; //macro for customer database shared memory key
            args[3] = 0;
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
    int count = 0;
    while((line_length = getline(&category, &sizeof_line, fp)) > 0){
        count++;
    }
    fclose(fp);
    return count;
}


