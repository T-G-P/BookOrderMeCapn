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

    parse_db(argv[1]);      //creating the customer database

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
