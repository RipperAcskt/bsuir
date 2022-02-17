#include <stdio.h>

#include <string>
#include <vector>

#include <dirent.h>
#include <sys/types.h>

std::vector<char*> dirs;

// Флаги
struct Flags{
    bool dir; //директории
    bool file; //файлы
    bool link; //символические ссылки
    bool sort; //сортировка
};

bool createPath(char **s, int argc, char **argv, Flags &f);
bool parsFlags(int argc, char *argv[], Flags &f);
void dirWalk(char *s, Flags flags);
void sort();

int main(int argc, char * argv[]) {
    char *s = nullptr;
    Flags f = {false};
    
    if(!createPath(&s, argc, argv, f))
        return 0;
    dirWalk(s, f);
    
    // удаление из вывода части строки содержащую введенный путь
    if(strcmp(s, ".") || f.dir)
        for(int i = 0; i < dirs.size(); i++)
            while(std::string(dirs[i]).find(s) != std::string::npos)
                strcpy(dirs[i], (std::string(dirs[i]).substr(std::string(dirs[i]).find(s) + std::string(s).length()).c_str()));

    if(f.sort)
        sort();

    for(int i = 0; i < dirs.size(); i++) puts(dirs[i]);
    
    return 0;
}

bool parsFlags(int argc, char *argv[], Flags &f){
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-f"))
            f.file = true;
        else if(!strcmp(argv[i], "-d"))
            f.dir = true;
        else if(!strcmp(argv[i], "-l"))
            f.link = true;
        else if(!strcmp(argv[i], "-s"))
            f.sort = true;
        else if(argc > 1 && argv[1][0] != '/'){
            return false;
        }
    }
    return true;
}

bool createPath(char **s, int argc, char **argv, Flags &f){
    if(!parsFlags(argc, argv, f)){
        printf("\ndirwalk [dir] [options]\ndir - directory\noptions:\n\t-l - symbol links only\n\t-d - directorys only\n\t-f - files only\n\t-s - sort\n\n");
        return false;
    }
    else{
        if((argc > 1 && argv[1][0] != '/') || argc == 1){ //если отсутсвует введенная директория
            *s = (char*)malloc(1);
            strcpy(*s, ".");
        }
            
        else{
            *s = (char*)malloc(strlen(argv[1]) * sizeof(char));
            strcpy(*s, argv[1]);
            if(!(opendir(*s))){
                char *b = (char*)malloc((strlen(*s)+1)*sizeof(char));
                b[0] = '.';
                strcat(b, *s);
                strcpy(*s, b);
                if(!(opendir(*s))){
                    printf("\nNo such directory\n\n");
                    return false;
                }
                
            }
        }
    }
    return true;
}

void dirWalk(char *s, Flags flags){
    DIR *d;
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
            
            if((flags.dir && f->d_type == DT_DIR) || (flags.file && f->d_type == DT_REG) || (flags.link && f->d_type == DT_LNK)
               || (!flags.file && !flags.dir && !flags.link))
                dirs.push_back(s);
            
            
            dirWalk(s, flags);
        }
        closedir(d);
    }
    else return;
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
