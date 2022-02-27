#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <dirent.h>
#include <sys/types.h>

// Флаги
struct Flags{
    bool dir; //директории
    bool file; //файлы
    bool link; //символические ссылки
    bool sort; //сортировка
};

struct dirs{
    char **allDirs;
    int len;
};

bool createPath(char **s, int argc, char **argv, Flags &f);
bool parsFlags(int argc, char *argv[], Flags &f);
void dirWalk(char *s, Flags flags, struct dirs *dr);
void sort(struct dirs **dr);
void addPath(struct dirs **dr, char *path);
int find(char *s1, char *s2);
char* cut(char *s, int index);

int main(int argc, char * argv[]) {
    char *s = NULL;
    Flags f = {false};
    struct dirs *dr = (struct dirs*)malloc(sizeof(struct dirs));
    
    if(!createPath(&s, argc, argv, f))
        return 0;
    
    dr->len = 0;
    dirWalk(s, f, dr);
    
    // удаление из вывода части строки содержащую введенный путь
    if(strcmp(s, ".") || f.dir)
        for(int i = 0; i < dr->len; i++)
            while(find(dr->allDirs[i], s) != -1)
                strcpy(dr->allDirs[i], cut(dr->allDirs[i], find(dr->allDirs[i], s)+strlen(s)));

    if(f.sort)
        sort(&dr);

    for(int i = 0; i < dr->len; i++) puts(dr->allDirs[i]);
    
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

void addPath(struct dirs **dr, char *s){
    (*dr)->len++;
    (*dr)->allDirs = (char**)realloc((*dr)->allDirs, (*dr)->len*sizeof(char*));
    (*dr)->allDirs[(*dr)->len-1] = (char*)malloc(strlen(s));
    strcpy((*dr)->allDirs[(*dr)->len-1], s);
}

void dirWalk(char *path, Flags flags, struct dirs *dr){
    DIR *d;
    struct dirent *f;
    
    if((d = opendir(path))){
        while((f = readdir(d))){
            
            if(strcmp(f->d_name, ".") == 0 || strcmp(f->d_name, "..") == 0 || strcmp(f->d_name, "") == 0)
                continue;
            
            char *s = (char*)malloc((strlen(path)+2+strlen(f->d_name))*sizeof(char));
            strcpy(s, path);
            strcat(s, "/");
            strcat(s, f->d_name);
            strcat(s, "\0");
            
            if((flags.dir && f->d_type == DT_DIR) || (flags.file && f->d_type == DT_REG) || (flags.link && f->d_type == DT_LNK)
               || (!flags.file && !flags.dir && !flags.link))
                addPath(&dr, s);
            
            
            dirWalk(s, flags, dr);
        }
        closedir(d);
    }
    else return;
}

int find(char *s1, char *s2){
    int n = 0, first = -1;
    size_t len;
    
    if(strlen(s1) < strlen(s2)) len = strlen(s1);
    else len = strlen(s2);
    
    for(int i = 0; i < len; i++){
        if(s1[i] == s2[i]){
            if(n == 0) first = i;
            n++;
        }
    }
    
    if(n >= len) return first;
    return -1;
}

char* cut(char *s, int index){
    int k = 0;
    char *stringBack = (char*)malloc((strlen(s)-index)*sizeof(char));
    for(int i = index; i < strlen(s); i++){
        stringBack[k] = s[i];
        k++;
    }
    return stringBack;
}

void sort(struct dirs **dr){
    char *temp;
    for(int i = 0; i < (*dr)->len - 1; i++){
        for(int j = 0; j < (*dr)->len - 1 - i; j++){
            if(strcmp((*dr)->allDirs[j], (*dr)->allDirs[j+1]) > 0){
                temp = (*dr)->allDirs[j];
                (*dr)->allDirs[j] = (*dr)->allDirs[j+1];
                (*dr)->allDirs[j+1] = temp;
            }
        }
    }
}
