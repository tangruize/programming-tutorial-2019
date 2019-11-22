//
// Created by Ruize Tang on 2019/11/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define IS_UNIX
#ifdef IS_UNIX
#include <unistd.h>
#include <sys/times.h>
static struct tms st_cpu, en_cpu;
#endif

#define TEMPFILE "temp.txt"
#define NBYTES 10000000

static int nbytes = 0;
static size_t curbufsize;
static char *buf = NULL;

void reopen_stdout(size_t size) {
    if (freopen(TEMPFILE, "w", stdout) != stdout) {  /* 将输出流重定向到 TEMPFILE */
        perror("Error: cannot reopen stdout for write");
        exit(EXIT_FAILURE);
    }
    if (size <= 1) {
        size = 0;
        setvbuf(stdout, NULL, _IONBF, 0);  /* 无缓存 */
    } else {
        if (buf)
            free(buf);
        buf = malloc(size);
        setvbuf(stdout, buf, _IOFBF, size);  /* 全缓存 */
    }
    curbufsize = size;
}

void write_bytes() {
    int n = nbytes;
#ifdef IS_UNIX
    clock_t st_time = times(&st_cpu), en_time;  /* 获取程序运行时间相关数据: 真实时间, 用户态时间, 系统调用时间 */
    long clockTicks = sysconf(_SC_CLK_TCK);
#else
    clock_t st_time = clock();  /* 获取程序运行的真实时间 */
#endif
    while (n--)
        putchar('0');
#ifdef IS_UNIX
    en_time = times(&en_cpu);
    fprintf(stderr, "%-4ld\t%.2f\t%.2f\t%.2f\n", curbufsize, (double)(en_time - st_time) / CLOCKS_PER_SEC,
           (double)(en_cpu.tms_utime - st_cpu.tms_utime) / (double)clockTicks,
           (double)(en_cpu.tms_stime - st_cpu.tms_stime) / (double)clockTicks);
#else
    fprintf(stderr, "%-4ld\t%.2f\n", curbufsize, (double)(clock() - st_time) / CLOCKS_PER_SEC);
#endif
}

int main(int argc, const char *argv[]) {
    if (argc > 1)
        nbytes = atoi(argv[1]);  /* 从命令行获取一次循环输出的字节数 */
    if (nbytes <= 0)
        nbytes = NBYTES;
#ifdef IS_UNIX
    fprintf(stderr, "Size\tReal\tUser\tSys\n");
#else
    fprintf(stderr, "Size\tTime\n");
#endif
    for (int n = BUFSIZ; n; n /= 2) {  /* BUFSIZ 是标准库默认的缓冲区大小 */
        reopen_stdout(n);
        write_bytes();
    }

    if (buf)
        free(buf);
    fclose(stdout);
    remove(TEMPFILE);  /* 删除用于测试的文件 */
}
