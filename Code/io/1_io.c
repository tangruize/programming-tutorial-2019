//
// Created by Ruize Tang on 2019/11/22.
//

#include <stdio.h>
#include <string.h>

#define FILENAME "file.txt"
#define LINEMAX  32

void info(const char *prompt, const char *buf) {
    char linebuf[LINEMAX];
    strncpy(linebuf, buf, LINEMAX);

    /* 输出一些信息 */
    unsigned len = strlen(buf);
    const char *newline = "";
    if (linebuf[len - 1] == '\n') {
        linebuf[len - 1] = 0;  /* 不直接将换行符打印出来 */
        newline = "\\n";  /* 而是打印\n */
    }
    fprintf(stderr, "Info: %s %d bytes \"%s%s\"\n", prompt, len, linebuf, newline);
}

void error(const char *path, const char *mode) {
    char errorprompt[256];
    snprintf(errorprompt, 256, "Error: fopen(\"%s\", \"%s\")", path, mode);  /* 格式化字符串到 errorprompt */
    perror(errorprompt);  /* perror 输出错误原因 */
}

void readfile(const char * path) {
    FILE *fp = fopen(path, "r");  /* 以只读模式打开 */
    if (!fp) {  /* 返回空指针打开失败, 可能原因: 文件不存在, 权限不足等 */
        error(path, "r");
    } else {
        char buf[LINEMAX];
        while (fgets(buf, LINEMAX, fp)) {  /* fgets 最多读取 LINEMAX-1 个字符到 buf, 并设置读取到的后一个字符为\0
                                              如果读取到了\n则\n也在 buf 中 */
            fprintf(stdout, "%s", buf);    /* printf(...) 等价于 fprintf(stdout, ...) */
            info("read", buf);    /* 打印一些信息 */
        }
        fclose(fp);  /* 用完记得关闭释放资源 */
    }
}

void writefile(const char * path, const char *buf) {
    FILE *fp = fopen(path, "w");  /* 以写入模式打开, 如果不存在会创建, 如果存在会截断 */
    if (!fp) {  /* 返回空指针打开失败, 可能原因: 权限不足, path是目录, path的父目录不存在等 */
        error(path, "w");
    } else {
        fprintf(fp, "%s", buf);  /* 与 printf 类似格式化字符串, 但输出到fp */
        fclose(fp);
        info("write", buf);
    }
}

void appendfile(const char * path, const char *buf) {
    FILE *fp = fopen(path, "a");  /* 以后附模式打开, 如果不存在会创建, 如果存在不会阶段 */
    if (!fp) {  /* 返回空指针打开失败, 可能原因: 权限不足, path是目录, path的父目录不存在等 */
        error(path, "a");
    } else {
        fprintf(fp, "%s", buf);
        fclose(fp);
        info("append", buf);
    }
}

void binaryfile(const char * path) {
    enum {APPLE, BANANA, ORANGE};
//#pragma pack(1)
    struct fruit {
        int kind;
        double price;
    } fruits[] = {
            {BANANA, 2.2},
            {ORANGE, 2.99},
            {APPLE, 2.89}
    };
//#pragma pack()
    FILE *fp = fopen(path, "wb+");
    if (fp) {
        fwrite(fruits, sizeof(struct fruit), 3, fp);

        fseek(fp, 0, SEEK_END);  /* 定位到文件尾, 如果用 "a" 模式打开文件, 文件位置指示器始终在文件尾 */
        printf("File size: %ld\n", ftell(fp));  /* 文件尾的偏移量就是文件大小,
                                                   也可以用append模式打开, 则不需要 fseek */

//        fseek(fp, 0, SEEK_SET);
        rewind(fp);  /* 定位到文件头部, 等同于 fseek(fp, 0, SEEK_SET); */
        struct fruit buf;
        while (fread(&buf, sizeof(struct fruit), 1, fp) == 1) {
            const char *kind = "";
            switch (buf.kind) {
                case APPLE: kind = "apple"; break;
                case BANANA: kind = "banana"; break;
                case ORANGE: kind = "orange"; break;
            }
            printf("The price of %s is %.2f\n", kind, buf.price);
        }
        fclose(fp);
    }
}

void removefile(const char *path) {
    if (remove(path) != 0) {  /* 删除文件 */
        perror("Error: cannot remove " FILENAME);
    } else {
        fprintf(stderr, "Info: ok\n");
    }
}

int main(int argc, const char *argv[]) {
    if (argc > 1) {  /* 设置缓冲区模式 */
        if (strcmp(argv[1], "_IOFBF") == 0)  /* 文件缓冲, 当缓冲区满了刷新, 打开文件默认的模式 */
            setvbuf(stdout, NULL, _IOFBF, BUFSIZ);
        else if (strcmp(argv[1], "_IOLBF") == 0)  /* 行缓冲, 当遇到\n时或缓冲区满了刷新, terminal中默认的模式 */
            setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
        else if (strcmp(argv[1], "_IONBF") == 0)  /* 无缓冲, 每次直接输出, stderr 默认的模式 */
            setvbuf(stdout, NULL, _IONBF, 0);
    }

    fprintf(stderr, "1 readfile:\n");
    readfile(FILENAME);

    fprintf(stderr, "\n2 writefile:\n");
    writefile(FILENAME, "hello");

    fprintf(stderr, "\n3 readfile:\n");
    readfile(FILENAME);

    fprintf(stderr, "\n4 appendfile:\n");
    appendfile(FILENAME, ", world\n");

    fprintf(stderr, "\n5 readfile:\n");
    readfile(FILENAME);

    fprintf(stderr, "\n6 binaryfile:\n");
    binaryfile(FILENAME);

    fprintf(stderr, "\n7 removefile:\n");
    removefile(FILENAME);
}
