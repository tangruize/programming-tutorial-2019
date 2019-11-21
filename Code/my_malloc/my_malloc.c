#include <assert.h>
#include "my_malloc.h"  /* 获得相关结构体的声明 */

#ifdef NOT_UNIX  /* 非 Linux 环境使用 malloc 来获取内存 */
#include <stdlib.h>
void* sbrk(size_t increment) {  /* 模拟 sbrk() */
#define MALLOC_SIZE 256 * 1024 * 1024  /* 用 malloc 申请一块大内存作为 my_malloc() 能分配的最大空间 */
    static unsigned char* program_break = NULL;
    static unsigned char* limit = NULL;
    if (program_break == NULL) {
        program_break = (unsigned char*)malloc(MALLOC_SIZE);  /* 只分配一次 */
        limit = program_break + MALLOC_SIZE;
    }
    if (increment == 0)
        return program_break;
    if (program_break + increment > limit)  /* 超过限制返回 -1 表示失败 */
        return (void*)-1;
    void* old_brk = (void*)program_break;
    program_break += increment;
    return old_brk;  /* 返回调整前的 brk */
}
#else
#include <unistd.h>  /* 获得 sbrk() 的声明 */
#endif

#define MM_SIZE_SZ (sizeof(size_t))              /* 当前系统字长, 一般在32位系统中为4, 64位系统中为8 */
#define MM_CHUNK_SIZE (sizeof(my_malloc_chunk))  /* 一个chunk的大小, 是 malloc() 分配的最小空间 */
#define MM_ADDITIONAL_SIZE (2 * MM_SIZE_SZ)      /* 头部的 unused 和 size 占用的额外空间 */
#define MM_ALIGNMENT (2 * MM_SIZE_SZ)            /* 两倍字长对齐要求 */
#define MM_ALIGN_MASK (MM_ALIGNMENT - 1)         /* 对齐掩码, 举例:
                                                    设 EXP 为 2 的整数次方, MASK = EXP - 1, 则 ~MASK = -EXP
                                                    将 size 向下对齐: (size & ~MASK) 或 (size & -EXP)
                                                    将 size 向上对齐: (size + MASK) & ~MASK */

#define MM_MIN_SIZE (size_t)((MM_CHUNK_SIZE + MM_ALIGN_MASK) & ~MM_ALIGN_MASK)  /* malloc() 分配的最小空间向上对齐 */
#define MM_MIN_SPLIT (MM_MIN_SIZE * 4)  /* 如果 chunk 拆分后大小超过 MM_MIN_SPLIT 则拆, 避免过多内存碎片 */
#define MM_TOP_PAD (128 * 1024)  /* 当 free_list 没有可用空间时, 向系统申请额外的空间, 以减少 sbrk() 的调用,
                                    申请空间需向上对齐到 MM_TOP_PAD 地址边界, 你可以修改这个值 */

#define MAGIC 0xfeedbeaf  /* 一个 magic 数字, 写到 unused 中, 正常的程序不应该改变它 */

#define PTR_OFFSET(ptr, offset) (void *)((char *)ptr + offset)  /* 将 ptr 偏移 offset 字节 */

my_malloc_chunk *free_list = NULL;  /* 指向第一个 chunk 的指针, 如果没有可用的 chunk 则为 NULL */

/* 获得 malloc() 真实申请空间的大小:
   如果 size 小于 MM_MIN_SIZE, 则返回 MM_MIN_SIZE,
   否则返回 size + MM_ADDITIONAL_SIZE 向上对齐到两倍字长边界的值

   static: 文件作用域, 其他 .c 文件将看不到这个函数,
           防止其他文件使用了相同的名字导致重定义错误
   inline: 内联函数, 建议编译器将这个函数的机器代码直接编译到调用函数中,
           通常用于简短的经常调用的函数以优化执行速度
 */
static inline size_t request_size(size_t size) {
    if (size + MM_ADDITIONAL_SIZE <= MM_MIN_SIZE)
        return MM_MIN_SIZE;
    else {
        //TODO: 对齐到两倍字长地址边界
        return size + MM_ADDITIONAL_SIZE;
    }
}

/* 当 free_list 找不到合适大小的 chunk 时, 向系统申请额外空间的大小
   返回 size 向上对齐到 MM_TOP_PAD 的值 */
static inline size_t pad_size(size_t size) {
    //TODO: 返回 size 向上对齐到 MM_TOP_PAD 的值 (但似乎直接返回 size 性能更好)
    return size;
}

/* 在 free_list 中找到合适大小(大于等于需要大小)的 chunk, 有多种查找策略:
   first-fit: 返回第一个满足大小要求的chunk, 缺点是内存容易碎片化, 从而导致较高的内存使用和较多的难以分配的内存
   best-fit: 找到最小满足需要大小的chunk, 缺点是某次循环可能花费较长时间 */
static my_malloc_chunk *find_fit_chunk(size_t size) {
    my_malloc_chunk *p = free_list;
    my_malloc_chunk *current_best = NULL;
    while (p) {
        //TODO: 实现 best-fit, 运行并对比
        if (p->size >= size) {
            return p;  /* first-fit */
        }
        p = p->next;
    }
    return current_best;
}

/* 向系统申请额外的空间, 原理:
   程序可以增加 heap 的大小来动态分配内存, heap 的当前限制被称为 program break
   sbrk() 系统调用向系统申请额外的 heap 空间, 它调整了 program break 的位置 */
static my_malloc_chunk *allocate_more_chunk(size_t size) {
    size = pad_size(size);  /* 额外申请空间的实际大小 */

    //TODO: 调用 sbrk(), 补全下面一行代码
    my_malloc_chunk *chunk_ptr = (my_malloc_chunk *)-1; /* 调整当前 program break (增加 size),
                                                           返回上一个 program break */
    if (chunk_ptr == (my_malloc_chunk *)-1)  /* 返回 (void *)-1 表示申请空间失败了, 极少出现 */
        return NULL;
    chunk_ptr->unused = MAGIC;  /* 写入 MAGIC */
    chunk_ptr->size = size;  /* 记录当前 chunk 的大小 */
    return chunk_ptr;
}

/* 检查 p 和 p->next 是否可合并, 若发生了合并, 返回 p, 否则返回 p->next. 返回值可用于下一次合并 */
static my_malloc_chunk *merge_two_chunk(my_malloc_chunk *p) {
    my_malloc_chunk *next = p->next;
    if (PTR_OFFSET(p, p->size) == next) {
        p->size += next->size;
        p->next = next->next;
        if (next->next)
            next->next->previous = p;
        return p;
    }
    return next;
}

/* ptr 指向的 chunk 未被使用 (向系统新申请的或被释放的), 加入 free_list, 加入策略:
  1. 直接把 ptr 放在最前面, 即令 free_list 指向 ptr, 缺点是不能合并相邻的 chunk
  2. 按 ptr 地址大小排序, 即每次加入后大于前一个 chunk 地址且小于后一个,
     发现前一个 chunk 刚好连接 ptr, 则合并这两个 chunk 以减少内存碎片 */
static void add_to_free(my_malloc_chunk *ptr) {
    if (!free_list) {  /* 还没有可用的 chunk, 直接将 free_list 指向 ptr */
        ptr->previous = ptr->next = NULL;
        free_list = ptr;
    } else {  /* 使用方案1, 简单地将 ptr 放在 free_list 表头 */
        //TODO: 简单地删掉下面的代码, 运行并对比
        //TODO: 使用方案2, 根据地址大小将 ptr 加入到 free_list 合适位置, 合并连续的空间, 运行并对比
        ptr->previous = NULL;
        ptr->next = free_list;
        free_list->previous = ptr;
        free_list = ptr;
        if (free_list->next)
            merge_two_chunk(free_list);
    }
}

/* 当 ptr 指向的 chunk 大小全部被分配出去, 则将该 chunk 从 free_list 中删除 */
static void delete_from_free(my_malloc_chunk *ptr) {
    //TODO: 实现链表删除操作, 从 free_list 中删除 ptr 并维护 free_list 指针
    free_list = NULL;
}

/* 返回 malloc() 真实返回的指针,
   参数 chunk_ptr 为已经满足大小要求的 chunk,
   如果 chunk_ptr 大小比需要的 size 大 MM_MIN_SPLIT, 则将该 chunk 拆分,
   否则将整块 chunk 都分配出去, 即从 free_list 中删除该 chunk,
   返回分配的 chunk 的 previous 域的首地址 */
static void *get_pointer(my_malloc_chunk *chunk_ptr, size_t size) {
    //TODO: 删除下面一行并取消下一行注释, 实现拆分 chunk 的代码
    if (0)
//    if (size + MM_MIN_SPLIT <= chunk_ptr->size)  /* 需要的 size 较小, 拆分 chunk  */
    {
        /* 在这里实现 chunk 拆分并返回一个合适的指针 */
    }
    else {  /* 需要的 size 较大, 分配整个 chunk */
        delete_from_free(chunk_ptr);  /* 一整块都分配出去了, 从 free_list 中移除 */
        return PTR_OFFSET(chunk_ptr, MM_ADDITIONAL_SIZE);  /* 返回 chunk_ptr 偏移 MM_ADDITIONAL_SIZE 字节的指针 */
    }
}

/* 与标准库函数 malloc() 功能类似的函数 */
/*TODO advanced: 实现 fastbin 链表, 对小的内存(小于一个特定值)直接使用 fastbin 以减少碎片.
                 由于 fastbin 不会被拆分或在链表中间插入删除, 可以用单向链表实现 */
/*TODO advanced: 添加线程安全特性, 允许多个线程同时调用 my_malloc() 和 my_free(),
                 可以简单地对 my_malloc() 和 my_free() 的进入和退出加锁 */
/*TODO advanced: 对大的内存(大于一个特定值)直接使用 mmap() 得到一块内存, 而不通过 free_list 管理,
                 该内存使用 munmap() 释放 */
void *my_malloc(size_t size) {
    size = request_size(size);  /* 得到真实需要分配的空间大小 */
    my_malloc_chunk *chunk_ptr = find_fit_chunk(size);  /* 找到合适大小的 chunk */
    if (!chunk_ptr) {  /* 没找到 */
        chunk_ptr = allocate_more_chunk(size);  /* 向系统要 */
        if (!chunk_ptr)  /* 系统也没有了 */
            return NULL;  /* 算了 */
        if (chunk_ptr->size == size)  /* 没有分配多余的空间, 直接返回而不加入 free_list */
            return PTR_OFFSET(chunk_ptr, MM_ADDITIONAL_SIZE);
        add_to_free(chunk_ptr);  /* 新申请的加入到 free_list */
    }
    return get_pointer(chunk_ptr, size);  /* 根据需要拆分或直接分配 chunk, 得到返回值 */
}

/* 与标准库函数 free() 功能类似的函数,
   my_free() 实际上并没有向系统归还内存, 而是将内存回收利用,
   程序结束时所有内存都将归还给系统 */
/*TODO advanced: 当有一块较大的 chunk (大于一个特定值) 可释放时, 向系统归还该空间 */
void my_free(void *ptr) {
    if (ptr) { /* 如果 ptr 不为 NULL */
        my_malloc_chunk *chunk_ptr = (my_malloc_chunk *) PTR_OFFSET(ptr, -(int)MM_ADDITIONAL_SIZE);
        assert(chunk_ptr->unused == MAGIC);  /* 检查 unused 是否被改变, 如果被改变将终止程序 */
        add_to_free(chunk_ptr);  /* 加入 free_list */
    }
}
