#include <stdio.h>
#include <time.h>       /* for CLOCKS_PER_SEC */
#include <assert.h>     /* for assert() */
#include "my_malloc.h"

#ifndef NOT_UNIX
#include <sys/times.h>  /* for times() */
#include <unistd.h>     /* for sbrk(), sysconf() */
static struct tms st_cpu;
#endif

//TODO: 将下面一行取消注释, 使用你的 my_malloc() 和 my_free()
#define USE_MY_MALLOC
#ifdef USE_MY_MALLOC
static void* (*malloc_func)(size_t) = my_malloc;  /* 使用 my_malloc() 获取动态内存 */
static void (*free_func)(void*) = my_free;
#else
#include <stdlib.h>
static void* (*malloc_func)(size_t) = malloc;  /* 使用系统 malloc() */
static void (*free_func)(void*) = free;
#endif

static clock_t st_time;
static void* st_brk;

/* 记录当前时钟和初始 brk */
void start_clock() {
    st_brk = sbrk(0);  /* 参数 incr 为 0 返回当前 program break */
#ifndef NOT_UNIX
    st_time = times(&st_cpu);  /* 获取程序运行时间相关数据 */
#else
    st_time = clock();
#endif
}

/* 打印 brk 增长值和运行时间 */
void end_clock(const char* msg, int i) {
#ifndef NOT_UNIX
    struct tms en_cpu;
    clock_t en_time = times(&en_cpu);
    long clockTicks = sysconf(_SC_CLK_TCK);
#else
    clock_t en_time = clock();
#endif

    printf("Loop %i %s:\nHeap total increment: %lld\n", i, msg, (long long)((char*)sbrk(0) - (char*)st_brk));
    printf("Real Time: %.3f", (double)(en_time - st_time) / (double)CLOCKS_PER_SEC);
#ifdef NOT_UNIX
    printf("\n\n");
#else
    /* real time: 实际运行时间, 真实流逝的时间
     * user time: 用户态运行时间, 可以理解为你写的代码运行的时间
     * system time: 内核态运行时间, 内核处理系统调用用的时间, 比如 sbrk()
     * 注意: real time != user time + system time,
     *       比如, 如果系统负载较大没有及时调度该程序, 二者可能相差很大 */
    printf(", User Time %.3f, System Time %.3f\n\n",
            (double)(en_cpu.tms_utime - st_cpu.tms_utime) / (double)clockTicks,
            (double)(en_cpu.tms_stime - st_cpu.tms_stime) / (double)clockTicks);
    st_cpu = en_cpu;
#endif

    st_time = en_time;
}

/* 测试 my_malloc() 和 my_free() */
void malloc_test() {
#define COUNT 10000  /* 一轮循环多少次 */
#define LOOP 5  /* 多少轮循环 */
#define my_type_t long long
    int times = LOOP;
    long long i, j, n, c;
    my_type_t *lptr;
    char *cptr;
    static union {  /* 联合类型, 每个域使用相同的内存地址 */
        char *cptr;
        my_type_t *lptr;
    } ptrs[COUNT];
    static void *big_ptr[4], *small_ptr[8];
    while (times--) {
        /* 执行 malloc */
        for (i = 0; i < COUNT; ++i) {
            ptrs[i].cptr = malloc_func(i);  /* 使用函数指针指向的函数获取动态内存 */
            assert(ptrs[i].cptr != NULL);  /* 返回 NULL 终止程序, 可能需要补全 allocate_more_chunk() */

            if ((i & 0xf) < 8)  /* 连续 8 次循环申请小内存 */
                small_ptr[i & 0xf] = malloc_func(1);
            else  /* 连续 8 次循环释放掉小内存 */
                free_func(small_ptr[(i & 0xf) - 8]);

            if ((i & 0x7) < 4)
                big_ptr[i & 0x7] = malloc_func(i * 2);
            else
                free_func(big_ptr[(i & 0x7) - 4]);

            for (j = 0, n = i / sizeof(my_type_t), lptr = ptrs[i].lptr; j < n; ++j, ++lptr)
                *lptr = (my_type_t)(i ^ -j ^ times ^ (size_t)lptr);  /* 随便填写一些数据 */
            for (j = 0, c = i % sizeof(my_type_t), cptr = ptrs[i].cptr + n * sizeof(my_type_t); j < c; ++j, ++cptr)
                *cptr = (char) (i ^ -j ^ times);  /* 填写剩余空间的数据 */
        }

        /* 检测数据一致性 */
        for (i = 0; i < COUNT; ++i) {
            for (j = 0, n = i / sizeof(my_type_t), lptr = ptrs[i].lptr; j < n; ++j, ++lptr)
                assert(*lptr == (my_type_t)(i ^ -j ^ times ^ (size_t)lptr));  /* 检测写入的值 */
            for (j = 0, c = i % sizeof(my_type_t), cptr = ptrs[i].cptr + n * sizeof(my_type_t); j < c; ++j, ++cptr)
                assert(*cptr == (char) (i ^ -j ^ times));
        }

        //TODO: 不定义宏 USE_MY_MALLOC 的情况下, 下面一行注释掉与不注释的输出有什么不同?
//        end_clock("after malloc", LOOP - times);

        //TODO: 定义了宏 USE_MY_MALLOC 的情况下, 取消注释下面一行, 会发生什么?
//        ptrs[16].cptr[16] = 123;

        /* 执行 free */
        for (i = 0; i < COUNT - 1; ++i) {  /* 最后一个没有释放 */
            free_func(ptrs[i].cptr);  /* 使用函数指针指向的函数释放动态内存 */
            ptrs[i].cptr = NULL;
        }

        //TODO: 不定义宏 USE_MY_MALLOC 的情况下, 注释掉前面的 end_clock(), 下面一行注释掉与不注释的输出有什么不同?
        free_func(ptrs[COUNT - 1].cptr);  /* 释放最后一个 */

        /* 打印总 brk 增长和这一轮运行时间 */
        end_clock("after free", LOOP - times);
    }
}

int main() {
    start_clock();
    malloc_test();
}
