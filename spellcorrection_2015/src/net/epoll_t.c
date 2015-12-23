#include "epoll_t.h"
#include "sysutil.h"
#include "ThreadPool.h"

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

void epoll_add_fd(epoll_t *et, int fd);
void epoll_del_fd(epoll_t *et, int fd);

void epoll_init(epoll_t *et, int listenfd, callback_t callback)
{
    et->epoll_fd_ = epoll_create(2048);
    if(et->epoll_fd_ == -1)
        ERR_EXIT("epoll_create");

    et->listenfd_ = listenfd;
    memset(&et->events_, 0, sizeof(et->events_));
    et->callback_ = callback;

    //把listenfd加入epoll中
    epoll_add_fd(et, listenfd);
}

//执行epoll_wait
void epoll_loop(epoll_t *et)
{
    int nready;
    do
    {
        nready = epoll_wait(et->epoll_fd_, et->events_, sizeof(et->events_), 5000);
    }
    while(nready == -1 && errno == EINTR);
    if(nready == -1)
        ERR_EXIT("epoll wait");
    else if(nready == 0)
        et->nready_ = 0;
    else
        et->nready_ = nready;
}

void epoll_handle_fd(epoll_t *et, ThreadPool &thp)
{
    int i;
    for(i = 0; i < et->nready_; ++i)
    {
        int fd = et->events_[i].data.fd;
        if(fd == et->listenfd_)  //listen
        {
            if(et->events_[i].events & EPOLLIN)
            {
                int peerfd = accept(et->listenfd_, NULL, NULL);
                if(peerfd == -1)
                    ERR_EXIT("accept conn");
                epoll_add_fd(et, peerfd);

                printf("%s connected.\n", get_tcp_conn_info(peerfd));
            }
        }
        else //客户fd
        {
            if(et->events_[i].events & EPOLLIN)
            {
                char recvbuf[1024] = {0};
                //size_t nread = recv_msg_with_len(fd, recvbuf, sizeof recvbuf);
                size_t nread = read(fd, recvbuf, sizeof recvbuf);
                if(nread == 0)  //close
                {
                    //从epoll中删除
                    epoll_del_fd(et, fd);
                    close(fd);
                    continue;
                }
				//去掉回车换行键
				size_t temp = nread;
				for(size_t idx = 0; idx != nread; ++idx)
				{
					if(recvbuf[idx] == '\n' || recvbuf[idx] == '\r')
					{
						recvbuf[idx] = '\0';
						--temp;
					}
				}
                //用户逻辑
                //执行回调函数
                et->callback_(recvbuf, temp, fd);

				//产生一个task,添加到线程池中
				Task task(recvbuf, fd, thp.mydict_);
				thp.addTask(task);
            }
        }
    }
}

void epoll_destroy(epoll_t *et)
{
    close(et->epoll_fd_);
}


void epoll_add_fd(epoll_t *et, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(epoll_ctl(et->epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1)
        ERR_EXIT("epoll add fd");
}

void epoll_del_fd(epoll_t *et, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    //ev.events = EPOLLIN;
    if(epoll_ctl(et->epoll_fd_, EPOLL_CTL_DEL, fd, &ev) == -1)
        ERR_EXIT("epoll add fd");
}
