#include "MyDict.h"
#include <iostream>
#include <fstream>
#include <sstream>

MyDict *MyDict::pInstance = NULL;

MyDict* MyDict::createInstance(const char *dictpath)
{
	if(NULL == pInstance)
	{
		pInstance = new MyDict(dictpath);
	}
	return pInstance;
}

std::vector<std::pair<std::string, int> > &MyDict::get_dict()
{
	return dict_;
}

std::map<std::string, std::set<int> > &MyDict::get_index_table()
{
	return index_table_;
}

void MyDict::show_dict()
{
	auto iter = dict_.begin();
	for(; iter != dict_.end(); ++iter)
	{
		std::cout << iter->first << "-->"
				  << iter->second << std::endl;
	}
}

void MyDict::show_index_table()
{
	auto iter = index_table_.begin();
	for(; iter != index_table_.end(); ++iter)
	{
		std::cout << iter->first << "-->";
		auto sit = iter->second.begin();
		for(; sit != iter->second.end(); ++sit)
		{
			std::cout << *sit << " ";
		}
		std::cout << std::endl;
	}
}

//private:
MyDict::MyDict(const char *dictpath)
{
	std::ifstream in(dictpath);	
	if(!in)
	{
		std::cout << __DATE__ << " " << __TIME__
				  << __FILE__ << " " << __LINE__
	              << ": dict read error" << std::endl;
		exit(-1);
	}
	
	std::string line;
	while(getline(in, line))
	{
		std::stringstream ss(line);
		std::string key;
		int value;
		ss >> key >> value;
		dict_.push_back(make_pair(key, value));
	}
	in.close();

	//创建索引表
	for(size_t idx = 0; idx != dict_.size(); ++idx)
	{
		record_to_index(idx);
	}
}

void MyDict::record_to_index(int idx)
{
	std::string key;
	std::string word = dict_[idx].first;
	for(std::size_t iidx = 0; iidx != word.size(); ++iidx)
	{
		if(word[iidx] & (1 << 7))
		{//存储GBK编码的中文字符
			key = word.substr(iidx, 2);
			++iidx;
		}
		else
		{
			key = word.substr(iidx, 1);
		}
		index_table_[key].insert(idx);
	}
}
