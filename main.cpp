#include <stdio.h>

#include <string>
#include <vector>

#include <dirent.h>
#include <sys/types.h>

std::vector<char*> dirs;

struct Flags{
    bool dir = false;
    bool file;
    bool link;
    bool sort;
};

void dirWalk(DIR *d, char *s, Flags flags);
void parsFlags(int argc, char *argv[], Flags &f);
void sort();

int main(int argc, char * argv[]) {
    DIR *d = nullptr;
    char *s = (char*)malloc(strlen(argv[1]) * sizeof(char));
    Flags f;
    
    strcpy(s, argv[1]);
    parsFlags(argc, argv, f);
    dirWalk(d, s, f);
    
    for(int i = 0; i < dirs.size(); i++)
        while(std::string(dirs[i]).find(s) != std::string::npos)
            strcpy(dirs[i], (std::string(dirs[i]).substr(std::string(dirs[i]).find(s) + std::string(s).length()).c_str()));
    
    for(int i = 0; i < dirs.size(); i++) puts(dirs[i]);
    printf("\n");
    if(f.sort)
        sort();

    for(int i = 0; i < dirs.size(); i++) puts(dirs[i]);
    
    return 0;
}

void parsFlags(int argc, char *argv[], Flags &f){
    for(int i = 2; i < argc; i++){
        if(!strcmp(argv[i], "-f"))
            f.file = true;
        else if(!strcmp(argv[i], "-d"))
            f.dir = true;
        else if(!strcmp(argv[i], "-l"))
            f.link = true;
        else if(!strcmp(argv[i], "-s"))
            f.sort = true;
    }
}

void dirWalk(DIR *d, char *s, Flags flags){
    struct dirent *f;
    char *b = (char*)malloc(strlen(s) * sizeof(char));
    
    strcpy(b, s);
    
    if((d = opendir(s))){
        while((f = readdir(d))){
            
            if(strcmp(f->d_name, ".") == 0 || strcmp(f->d_name, "..") == 0 || strcmp(f->d_name, "") == 0)
                continue;
            
            s = (char*)malloc((strlen(b)+1+strlen(f->d_name))*sizeof(char));
            strcpy(s, b);
            strcat(s, "/");
            strcat(s, f->d_name);
            if((flags.dir && f->d_type == DT_DIR) || (flags.file && f->d_type == DT_REG) || (flags.link && f->d_type == DT_LNK) || (!flags.file && !flags.dir && !flags.link))
                dirs.push_back(s);
            
            dirWalk(d, s, flags);
        }
        closedir(d);
    }
    else{
        return;
    }
}

void sort(){
    char *temp;
    for(int i = 0; i < dirs.size()-1; i++){
        for(int j = 0; j < dirs.size() - 1 - i; j++){
            if(strcmp(dirs[j], dirs[j+1]) > 0){
                temp = dirs[j];
                dirs[j] = dirs[j+1];
                dirs[j+1] = temp;
            }
        }
    }
}
