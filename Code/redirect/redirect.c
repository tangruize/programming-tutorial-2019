//
// Created by Ruize Tang on 2019/10/27.
//

#include <fcntl.h>  /* 获得 open() 和 close() 的声明 */
#include <unistd.h>  /* 获得 dup2() 的声明 */
#include <sys/stat.h>  /* 获得 S_IRUSR 和 S_IWUSR 的声明 */

/* 函数说明：每次做 OJ 都要调好几百遍，把输入一次又一次地复制粘贴太麻烦了，不如从文件读取吧！
 * 参数说明：argv 是一个数组，每个元素是一个字符串
 *          argc 表示 argv 有多少个字符串
 * 命令行使用说明：./main [input] [output]
 *          ./main 本程序，argv[0]
 *          input 可选，输入文件，argv[1]。不指定输入文件输入就是从原来的黑框框读取输入
 *          output 可选，输出文件，argv[2]。不指定就是打印到原来的黑框框。必须要有输入文件才能有输出文件。 */
void init(int argc, char *argv[]) {
    if (argc > 1) {  /* 如果命令行参数个数大于1
                      * 比如 "./main input" 中的 "./main" 是第0个 argv[0]，"input" 是第1个 argv[1] */

        int fd = open(argv[1], O_RDONLY);  /* 以只读方式打开输入文件 argv[1] 并获得文件描述符 fd */

        if (fd != -1) {  /* 判断是否打开成功，绝大多数 Linux 系统调用返回 -1 为失败 */
            dup2(fd, STDIN_FILENO);  /* 将 fd 复制到 STDIN_FILENO （将输入流重定向为文件描述符 fd）
                                      * 原来的输入流文件描述符 STDIN_FILENO 先被关闭
                                      * 新的文件描述符 fd 被复制 (duplicate) 到 STDIN_FILENO */
        }

        close(fd);  /* 由于 fd 被复制了，所以用不上了，关掉释放资源 */

        if (argc > 2) {  /* 如果命令行参数个数大于2，比如 "./main input output" */

            /* 以写入 (O_WRONLY) 方式打开输出文件 argv[2]
             * 如果 argv[2] 不存在将被创建 (O_CREAT)，创建的权限为用户可读 (S_IRUSR) 写 (S_IWUSR)
             * 如果 argv[2] 存在将被重写 (O_TRUNC) */
            fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

            if (fd != -1)
                dup2(fd, STDOUT_FILENO);  /* 将 fd 复制到 STDOUT_FILENO （将输出流重定向为文件描述符 fd） */
            close(fd);
        }
    }
}
