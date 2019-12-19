//
//  minishell.cpp
//  minishell
//
//  Created by 15803450188 on 2019/12/9.
//  Copyright © 2019 zy. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <stddef.h>
#include <ctype.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;

char curdir[256];
char username[256];
char curname[256];
char prompt[512] = "";

void SplitInput(const string& s, vector<string>& v, const string& c)
{
    size_t pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    size_t npos = -1;
    while(npos != pos2)
    {
        if(pos1!=pos2)v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

int checklogin(char *aoUserPara)
{
    uid_t uid = getuid();

    char *LoginId;
    struct passwd *pwdinfo=getpwuid(uid);
    if (aoUserPara == NULL)
        return -9;
    if ((LoginId = getlogin()) == NULL)
    {
        perror("getlogin");
        aoUserPara[0] = '\0';
        return -8;
    }
    if ((pwdinfo = getpwnam (LoginId)) == NULL) {
        perror ("getpwnam");
        return -7;
    }
    return 0;
}

// get user default dir
int getuserdir(char *aoUserDir)
{
    uid_t uid=getuid();
    struct passwd *pwdinfo = getpwuid(uid);
    int check = checklogin(aoUserDir);
    if (check != 0)
    {
        return check;
    }
    
    strcpy(aoUserDir, pwdinfo->pw_dir);
    return 0;
}

// get user name
int getusername(char *aoUserName)
{
    int check = checklogin(aoUserName);
    if (check != 0)
    {
        return check;
    }
    strcpy(aoUserName, getlogin());
    return 0;
}

// get current dir
int getusercwd(char *aoUserCwd)
{
    char buf[256];
    getcwd(buf, sizeof(buf));
    strcpy(aoUserCwd, buf);
    return 0;
}

// get current dir name
int getcurname(char *curname)
{
    char *dir = curdir;
    if (dir == NULL)
    {
        return -9;
    }
    size_t len = strlen(dir);
    int j = 0;
    for (j = len - 1; j > 0; j--)
    {
        if (dir[j] == '/')
        {
            j++;
            for (int i = 0; j < len; i++)
            {
                curname[i] = dir[j];
                j++;
            }
            return 0;
        }
    }
    if (j == 0)
    {
        curname = dir;
    }
    return 0;
}

// change dir
int chuserdir(char *newDir)
{
    int cd = chdir(newDir);
    return cd;
}

void initprompt()
{
    memset(username,0,sizeof(username));
    memset(curdir,0,sizeof(curdir));
    memset(curname,0,sizeof(curname));
    getusername(username);
    getusercwd(curdir);
    getcurname(curname);
    memset(prompt,0,sizeof(prompt));
    strcat(prompt, "[");
    strcat(prompt, username);
    strcat(prompt, " ");
    strcat(prompt, curname);
    strcat(prompt, "]$");
}

// initialize global vars
void initshell()
{
    getusername(username);
    getusercwd(curdir);
    getcurname(curname);
    initprompt();
    printf("**************welcome to ruby's mini shell**************\n");
}

int main(){
    initshell();
    while(1) {
        printf("%s", prompt);
        fflush(stdout);
        string tmp;
        getline(cin,tmp);
        vector<string> result;
        SplitInput(tmp,result,";");
        for(vector<string>::size_type i = 0;i<result.size();i++)
        {
            char *ptr=new char[strlen(result[i].c_str())+1];
            strcpy(ptr,result[i].c_str());
            int argc=0;
            char *argv[32]={NULL};
            while(*ptr != '\0') {//没有到达输入末尾
                if(!isspace(*ptr)) {
                    argv[argc]=ptr;
                    argc++;
                    while(!isspace(*ptr) && *ptr != '\0')ptr++;
                    *ptr='\0';
                    ptr++;
                    continue;
                }
                ptr++;
            }
            argv[argc]=NULL;
            if(!strcmp(argv[0],"cd")){
                if(argv[1]==NULL){
                    char* userdir;
                    getuserdir(userdir);
                    chdir(userdir);
                }
                chdir(argv[1]);
                initprompt();
                continue;
            }
            int pid=fork();
            if(pid==0) {
                execvp(argv[0],argv);
                //若子进程替换程序失败，则直接退出，因为不需要多个shell
                exit(0);
            }
            wait(NULL);
        }
    }
    return 0;
}
