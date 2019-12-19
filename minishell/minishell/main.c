int substr(char *dest, char *src, int start, int len)
{
    int i = 0;
    int j = start;
    while (i < len)
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    return 0;
}

int isletter(char a)
{
    if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
    {
        return 1;
    }
    return 0;
}
/*
int main(){
    inishell();
    
    printf("%s", prompt);
    fflush(stdout);
    char tmp[1024] = {0};
    scanf("%[^\n]%*c",tmp);
    printf("[%s]\n",tmp);
    //解析命令行
    char* ptr = tmp;
    int argc = 0;
    char* argv[32] = {NULL};
    int i;
    for(i=0;i<strlen(tmp);i++){
        while(*ptr!=';'){
            if(!isspace(*ptr)){
                argv[argc] = ptr;
                argc++;
                while(!isspace(*ptr)&&*ptr!='\0'){
                    ptr++;
                }
                *ptr = '\0';
                ptr++;
                continue;
            }
            ptr++;
        }
        for(int i=0; i<argc;++i){
            printf("argv[%d] = [%s]\n",i,argv[i]);
        }
        argv[argc] = NULL;
        execvp(argv[0],argv);
    }
    

    
}
*/
/*
int main()
{
    inishell();
    printf("%s", prompt);
    fflush(stdout);
    int size;
    char cmd[300][1000]={0};
    char input[1000];
    fgets(input, 256, stdin);
    size=(int)strlen(input);
    int i = 0;          // for scan
    int start = 0;      // flags
    int len = 0;        // word length
    int pos_word = 0;   // NO. k word
    for (; i < size; i++)
    {
        if(input[i]==' ')
        {
            if(len!=0)
            {
                substr(cmd[pos_word], input, start, len);
                pos_word++;
            }
            
            len=0;
        }
        else if(input[i]==';')
        {
            substr(cmd[pos_word], input, start, len);
            pos_word++;
            char* argv[1000]={NULL};
            
            argv[pos_word]=NULL;
            printf("start execvp\n");
            int temp=execvp((char *)cmd[0],(char **)cmd);
            if(temp == -1)
                perror("execl error");

            for(int j=0;j<=pos_word;j++){
                printf("argv[%d]: %s\n", j, cmd[j]);
                memset(cmd[j],0,sizeof(cmd[j])/sizeof(char));
                printf("argv[%d]: %s\n", j, cmd[j]);
            }
            
            pos_word=0;
            len=0;
        }
        else if(input[i]=='\n')
        {
            if(len!=0)
            {
                substr(cmd[pos_word], input, start, len);
            }
            pos_word++;
            printf("start execvp\n");
            int temp=execvp((char *)cmd[0],(char **)cmd);
            if(temp == -1)
                perror("execl error");
            for(int j=0;j<=pos_word;j++){
                printf("argv[%d]: %s\n", j, cmd[j]);
                memset(cmd[j],0,sizeof(cmd[j])/sizeof(char));
                printf("argv[%d]: %s\n", j, cmd[j]);
            }
            break;
        }
        else if(input[i]=='\t')
        {
            continue;
        }
        else
        {
            len++;
            if (i >= 1 && (input[i - 1] == ';' || input[i - 1] == ' ' || input[i-1]=='\t'))
            {
                start = i;
                
            }
        }
    }
}
*/
        /*
        if (input[i] == ' ' && hasspace == 0 && len != 0)
        {
            substr(cmd[pos_word], input, start, len);
            hasspace = 1;
            pos_word++;
            len = 0;
        }
        else if (input[i] == ';' || input[i+1]=='\n')
        {
            if(input[i+1]=='\n'){
                substr(cmd[pos_word], input, start, len+1);
                end=TRUE;
            }
            else{
                substr(cmd[pos_word], input, start, len);
            }
            hasspace = 0;
            pos_word++;
            break;
        }
        else if (input[i] == '\t')
        {
            continue;
        }
        else
        {
            len++;
            hasspace = 0;
            if (i >= 1 && (input[i - 1] == ';' || input[i - 1] == ' '))
            {
                start = i;
            }
        }
        */
    
    /*
    int ret;
    char *argv[] = {"ls","-l",NULL};
    ret = execvp("ls",argv);
    if(ret == -1)
        perror("execl error");
     */
    
//execve
/*

#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include<unistd.h>
#include <sys/wait.h>
#include<stdlib.h>
int main(){
    while(1){
        printf("[dev@localhost ~]$ ");
        fflush(stdout);
        char tmp[1024] = {0};
        scanf("%[^\n]%*c",tmp);
        printf("[%s]\n",tmp);
        //解析命令行
        char* ptr = tmp;
        int argc = 0;
        char* argv[32] = {NULL};
        while(*ptr!='\0'){
            if(!isspace(*ptr)){
                argv[argc] = ptr;
                argc++;
                while(!isspace(*ptr)&&*ptr!='\0'){
                    ptr++;
                }
                *ptr = '\0';
                ptr++;
                continue;
            }
            ptr++;
        }
        for(int i=0; i<argc;++i){
            printf("argv[%d] = [%s]\n",i,argv[i]);
        }
        argv[argc] = NULL;
        
        if(!strcmp(argv[0],"cd")){
            //改变当前工作路径
            chdir(argv[1]);
            continue;
        }
        //建立一个子进程，替换子进程
        
        int pid = fork();
        if(pid == 0){
            execvp(argv[0],argv);
            exit(0);
        }
        //等待子进程退出，避免出现僵尸进程
        wait(NULL);
    }
    return 0;
    
}
*/
