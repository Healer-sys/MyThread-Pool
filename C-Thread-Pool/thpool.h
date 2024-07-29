/**********************************
 * @author      Johan Hanssen Seferidis
 * License:     MIT
 *
 **********************************/

#ifndef _THPOOL_
#define _THPOOL_

#ifdef __cplusplus
extern "C" {
#endif

/* =================================== API ======================================= */


typedef struct thpool_* threadpool;


/**
 * @brief  初始化线程池
 *
 * 初始化线程池。此函数在所有线程成功初始化之前不会返回。
 *
 * @example
 *
 *    ..
 *    threadpool thpool;                     // 首先声明一个线程池
 *    thpool = thpool_init(4);               // 然后将其初始化为4个线程
 *    ..
 *
 * @param  num_threads   要在线程池中创建的线程数
 * @return threadpool    成功时返回创建的线程池,
 *                       出错时返回NULL
 */
threadpool thpool_init(int num_threads);


/**
 * @brief 将工作添加到作业队列
 *
 * 接收一个操作及其参数并将其添加到线程池的作业队列中。
 * 如果要添加具有多个参数的函数，可以通过传递指向结构体的指针来实现。
 *
 * 注意: 你必须强制转换函数和参数以避免警告。
 *
 * @example
 *
 *    void print_num(int num){
 *       printf("%d\n", num);
 *    }
 *
 *    int main() {
 *       ..
 *       int a = 10;
 *       thpool_add_work(thpool, (void*)print_num, (void*)a);
 *       ..
 *    }
 *
 * @param  threadpool    要添加工作的线程池
 * @param  function_p    指向要添加为工作的函数的指针
 * @param  arg_p         指向参数的指针
 * @return 成功返回0, 否则返回-1
 */
int thpool_add_work(threadpool, void (*function_p)(void*), void* arg_p);


/**
 * @brief 等待所有排队的作业完成
 *
 * 将等待所有作业（排队和当前正在运行的）完成。
 * 一旦队列为空且所有工作都完成，调用线程（可能是主程序）将继续。
 *
 * 在等待中使用智能轮询。轮询初始为0 - 意味着几乎没有轮询。
 * 如果在线程未完成后经过1秒，轮询间隔将指数增长直到达到max_secs秒。
 * 然后假设线程池中正在进行大量处理，将其跳回最大轮询间隔。
 *
 * @example
 *
 *    ..
 *    threadpool thpool = thpool_init(4);
 *    ..
 *    // 添加大量工作
 *    ..
 *    thpool_wait(thpool);
 *    puts("所有添加的工作已完成");
 *    ..
 *
 * @param threadpool     要等待的线程池
 * @return 无返回值
 */
void thpool_wait(threadpool);


/**
 * @brief 立即暂停所有线程
 *
 * 线程无论是空闲还是工作都会被暂停。
 * 一旦调用thpool_resume，线程将恢复到之前的状态。
 *
 * 在线程暂停期间，可以添加新工作。
 *
 * @example
 *
 *    threadpool thpool = thpool_init(4);
 *    thpool_pause(thpool);
 *    ..
 *    // 添加大量工作
 *    ..
 *    thpool_resume(thpool); // 让线程开始工作
 *
 * @param threadpool    要暂停线程的线程池
 * @return 无返回值
 */
void thpool_pause(threadpool);


/**
 * @brief 如果线程被暂停，则取消暂停
 *
 * @example
 *    ..
 *    thpool_pause(thpool);
 *    sleep(10);              // 延迟执行10秒
 *    thpool_resume(thpool);
 *    ..
 *
 * @param threadpool     要取消暂停的线程池
 * @return 无返回值
 */
void thpool_resume(threadpool);


/**
 * @brief 销毁线程池
 *
 * 此操作将等待所有当前活动线程完成，然后销毁整个线程池以释放内存。
 *
 * @example
 * int main() {
 *    线程池 thpool1 = thpool_init(2);
 *    线程池 thpool2 = thpool_init(2);
 *    ..
 *    thpool_destroy(thpool1);
 *    ..
 *    return 0;
 * }
 *
 * @param threadpool     要销毁的线程池
 * @return 无
 */
void thpool_destroy(threadpool);


/**
 * @brief 显示当前正在工作的线程数量
 *
 * 工作线程是指正在执行任务（非空闲状态）的线程。
 *
 * @example
 * int main() {
 *    threadpool thpool1 = thpool_init(2);
 *    threadpool thpool2 = thpool_init(2);
 *    ..
 *    printf("正在工作的线程数量: %d\n", thpool_num_threads_working(thpool1));
 *    ..
 *    return 0;
 * }
 *
 * @param threadpool     关注的线程池
 * @return 整数           正在工作的线程数量
 */
int thpool_num_threads_working(threadpool);


#ifdef __cplusplus
}
#endif

#endif
