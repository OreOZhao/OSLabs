#include <fcntl.h>
#include <unistd.h>
#include <copyfile.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>


int OSCopyFile(const char* source, const char* destination)
{
    int input, output;
    if ((input = open(source, O_RDONLY)) == -1)
    {
        printf("input error \n");
        return -1;
    }
    if ((output = creat(destination, 0660)) == -1)
    {
        printf("output error \n");
        close(input);
        return -1;
    }

    //Here we use kernel-space copying for performance reasons
#if defined(__APPLE__) || defined(__FreeBSD__)
    //fcopyfile works on FreeBSD and OS X 10.5+
    int result = fcopyfile(input, output, 0, COPYFILE_ALL);
#else
    //sendfile will work with non-socket output (i.e. regular file) on Linux 2.6.33+
    off_t bytesCopied = 0;
    struct stat fileinfo = {0};
    fstat(input, &fileinfo);
    int result = sendfile(output, input, &bytesCopied, fileinfo.st_size);
    #endif

        close(input);
        close(output);

        return result;
    }

int main(){
    char *from = "/Users/limingxia/desktop";
    char *to = "//Users/limingxia/desktop/study/cptest1.txt";
    struct timeval start;
    struct timeval end;
    double diff;

    gettimeofday(&start,NULL);
    printf("%d \n",OSCopyFile(from,to));
    gettimeofday(&end,NULL);
    gettimeofday(&end,NULL);
    diff = (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec);
    printf("%-15f s\n", diff/1000000);
    return 0;
}
