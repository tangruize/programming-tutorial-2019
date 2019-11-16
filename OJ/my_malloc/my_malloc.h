/* 声明了我们自己实现的动态内存分配相关函数:
   my_malloc(): 类似于 malloc() 函数, 申请动态内存
   my_free(): 类似于 free() 函数, 释放动态内存 */

#ifndef MALLOC_MY_MALLOC_H
#define MALLOC_MY_MALLOC_H

#include <stddef.h>

#define NOT_UNIX  /* 非 Linux 环境使用 malloc 来获取内存 */

#ifdef NOT_UNIX
void* sbrk(size_t increment);
#endif

/* struct my_malloc_chunk 定义了一个双向链表, 用于管理可分配的 chunk

 * 示意图

   free_list +-----> +--------+--------+--------+--------+
                    | MAGIC  | 32     | NULL   | NEXT   |
            +-----> +--------+--------+--------+---+----+
            |                                      |
            |    +---------------------------------+
            |    |
            +-----------------------------+
                 |                        |
                 +> +--------+--------+---+----+--------+--------+--------+
                    | MAGIC  | 48     | PRE    | NEXT   |        |        |
            +-----> +--------+--------+--------+---+----+--------+--------+
            |                                      |
            |    +---------------------------------+
            |    |
            +-----------------------------+
                 |                        |
                 +> +--------+--------+---+----+--------+
                    | MAGIC  | 32     | PRE    | NULL   |
                    +--------+--------+--------+--------+

                    unused   size     previous next

 * 结构体每个域的解释:
   unused: 主要用来对齐到两倍字长, 填写了一个 MAGIC 可用来检测是否发生了不正确的操作改变了这个值
   size: 当前 chunk 的大小, 从 unused 的起始地址算起
   previous: 指向前一个 chunk 的指针, 如果没有前一个则为 NULL,
             当该 chunk 被 malloc() 分配到时, chunk 从 free_list 中移除, malloc() 返回的指针与 previous 首地址相同
   next: 指向后一个 chunk 的指针, 如果没有后一个则为 NULL

 * 对齐:
   要求对齐到两倍机器字长的地址边界 (32位系统为 4 * 2 = 8, 64位系统为 8 * 2 = 16)

 * 大小分配要求:
   由于一个 chunk 结构体大小为 sizeof(struct my_malloc_chunk), 所以使用 malloc() 时最小需要该大小空间,
   如果 用户申请的空间 + sizeof(unused) + sizeof(size) > sizeof(struct my_malloc_chunk),
   则会申请不等式左边 **向上对齐** 到地址边界的大小, 以64位系统举例: {31, 32, 33} = {32, 32, 48}

 */
struct my_malloc_chunk {
    size_t unused;
    size_t size;
    struct my_malloc_chunk *previous;
    struct my_malloc_chunk *next;
};

/* 不用 typedef 必须写 struct, 比如 sizeof(struct my_malloc_chunk),
   typedef 可以简化这个步骤: sizeof(my_malloc_chunk) */
typedef struct my_malloc_chunk my_malloc_chunk;

/* 与标准库函数 malloc() 功能类似的函数 */
void *my_malloc(size_t size);

/* 与标准库函数 free() 功能类似的函数 */
void my_free(void *ptr);

/* TODO advanced:
    实现另外两个与 malloc 相关的函数.
    malloc, calloc, realloc, free 集齐后, 可以将名称中的 my_ 去掉,
    通过静态链接或动态链接的方式可替换掉标准库的这4个函数.
    如果不集齐就替换, 程序如果同时使用了 malloc 和 realloc,
    前者是你的, 后者是标准库的, 会导致程序崩溃 */
void *my_calloc(size_t nelem, size_t elsize);
void *my_realloc(void *ptr, size_t size);

#endif //MALLOC_MY_MALLOC_H
