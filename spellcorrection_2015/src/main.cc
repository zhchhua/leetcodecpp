#include "sysutil.h"
#include "epoll_t.h"
#include "Socket.h"
#include "InetAddress.h"
#include "ThreadPool.h"
#include "conf.h"
#include "MyDict.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>

void foo(const char *buf, size_t cnt, int peerfd)
{
	std::cout << buf << "==> size: " << cnt << std::endl;
	//write(peerfd, buf, cnt);
}

int main()
{
	//读取配置文件
	std::string filepath = "/home/luo/project/spellcorrection/conf/my.conf";
	MyConf myconf(filepath);
	bool initFlag = myconf.init();
	if(initFlag)
		myconf.show();

	std::map<std::string, std::string> &myMap = myconf.get_map();
	std::string dictpath = myMap["my_dict"];	
	std::cout << dictpath << std::endl;

	MyDict *pMydict = MyDict::createInstance(dictpath.c_str());
	//pMydict->show_index_table();
	
	std::string sip = myMap["my_ip"];
	std::string sport = myMap["my_port"];
	std::cout << sip << ":" << sport << std::endl;

	int iport = 0;
	std::stringstream ss(sport);
	ss >> iport;
	std::cout << iport << std::endl;

	//初始化线程池
	ThreadPool thp(50, 10, (*pMydict));
	thp.start();

	//创建服务器端的socket，并进行监听
	InetAddress inetAddr(sip, iport);
	Socket mySock;
	mySock.init(0, inetAddr);

	//运用epoll模型等待建立客户端连接
	int listenfd = mySock.get_sockfd();
	epoll_t et;
	epoll_init(&et, listenfd, &foo);
	while(1)
	{
		epoll_loop(&et);
		epoll_handle_fd(&et, thp);
	}

	epoll_destroy(&et);
	return 0;
}
