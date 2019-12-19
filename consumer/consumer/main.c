#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> //getpid()
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/syscall.h> //gettid() thread
#include <time.h>
#include <string.h> //for memset
//创建模式权限
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define BUFFER 3  // 缓冲区数量
#define PRO_NO 15 // PRODUCING NO
#define OVER (-1)
#define PSLEEP 10000 //
#define CSLEEP 10000 //

#define PRODUCER 5                  // 生产者线程数量
#define CONSUMER 4                  // 消费者线程数量
#define gettid() syscall(SYS_gettid) //获取线程ID

sem_t empty, full;     //同步信号量
pthread_mutex_t mutex; //互斥信号量

struct prodcons
{                          // 缓冲区相关数据结构
    int buf[BUFFER];       /* 实际数据存放的数组*/
    int tid[BUFFER];       //生产进程PID   getpid() 头文件
    char *time[BUFFER];    //写入时间
    int readpos, writepos; /* 读写指针*/
};
struct prodcons buffer;

/* 初始化缓冲区结构 */
void init(struct prodcons *b)
{
    b->readpos = 0;
    b->writepos = 0;
}
int producer_id = 0, consumer_id = 0; //生产者消费者ID

void *Producer() //生产者函数
{
    int n = 0;
    time_t timep;
    time(&timep);
    char *time;
    int nn = PRO_NO;
    while (nn--)
    {
        sleep(rand()%5+1);
        sem_wait(&empty);
        time = asctime(gmtime(&timep));
        printf("Time: %sProducer tries to enter the critical zone.\n", time);
        pthread_mutex_lock(&mutex);
        printf("Time: %sProducer enters the critical zone success.\n", time);
        buffer.buf[buffer.writepos] = n;
        buffer.tid[buffer.writepos] = gettid(); //threadid
        
        buffer.time[buffer.writepos] = time; //time
        //printf("data:%d ,tid: %ld ,time_in: %s --->\n", n, gettid(), tem);
        printf("Time: %sProducer produces NO. %d product success. \n", time, n);
        printf("Time: %sProducer NO. %d product writes on NO. %d buffer success. \n", time, n, buffer.writepos);
        buffer.writepos++;
        if (buffer.writepos >= BUFFER){
            buffer.writepos = 0;
        }
        pthread_mutex_unlock(&mutex);
        printf("Time: %sProducer leaves the critical zone success. \n", time);
        sem_post(&full);
        printf("Time: %sProducer sends signal to consumer success. \n", time);
        n++;
    }
    pthread_exit(0);
    
}

void *Consumer() //消费者函数
{
    int d;
    int gettid;
    time_t timep;
    time(&timep);
    char *time;
    int nn = PRO_NO;
    while (nn--)
    {
        sleep(rand()%5+1);
        sem_wait(&full);
        time = asctime(gmtime(&timep));
        printf("\t\t\tTime: %s\t\t\tConsumer tries to enter the critical zone.\n", time);
        pthread_mutex_lock(&mutex);
        printf("\t\t\tTime: %s\t\t\tConsumer enters the critical zone success.\n", time);
        /* 读数据,移动读指针*/
        d = buffer.buf[buffer.readpos];
        gettid = buffer.tid[buffer.readpos];
        time = buffer.time[buffer.readpos];
        //usleep(CSLEEP);
        buffer.readpos++;
        if (buffer.readpos >= BUFFER)
            buffer.readpos = 0;
        printf("\t\t\tTime: %s\t\t\tConsumer buys NO. %d product success.\n", time, d);
        if (d == OVER)
            break;
        printf("\t\t\tTime: %s\t\t\tConsumer leaves the critical zone success.\n", time);
        pthread_mutex_unlock(&mutex);
        printf("\t\t\tTime: %s\t\t\tConsumer sends signal to producer success.\n", time);
        sem_post(&empty);
    }
    pthread_exit(0);
}

int main()
{
    init(&buffer);
    int rthread[18], i;
    pthread_t producer[PRODUCER]; //生产者
    pthread_t consumer[CONSUMER]; //消费者
    int sinit1 = sem_open(&empty, 0, BUFFER); //初始化同步信号量
    int sinit2 = sem_init(&full, 0, 0);
    int minit = pthread_mutex_init(&mutex, NULL); //初始化互斥信号量
    for (i = 0; i < PRODUCER; i++) //创建生产者线程
    {
        rthread[i] = pthread_create(&producer[i], NULL, Producer, NULL);
        printf("Producer %d create succcess \n", i);
        if (rthread[i])
        {
            printf("producer %d create failed \n", i);
            exit(1);
        }
    }
    for (i = 0; i < CONSUMER; i++) //创建消费者线程
    {
        rthread[i] = pthread_create(&consumer[i], NULL, Consumer, NULL);
        printf("Consumer %d create succcess \n", i);
        if (rthread[i])
        {
            printf("consumer %d create failed \n", i);
            exit(1);
        }
    }
    for (i = 0; i < PRODUCER; i++) //销毁生产者线程
    {
        pthread_join(producer[i], NULL);
        printf("Producer %d destroyed succcess \n", i);
    }
    for (i = 0; i < CONSUMER; i++) //销毁生产者线程
    {
        pthread_join(consumer[i], NULL);
        printf("Consumer %d destroyed succcess \n", i);
    }
    exit(0);
}
