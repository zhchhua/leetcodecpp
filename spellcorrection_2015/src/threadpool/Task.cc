#include "Task.h"
#include <iostream>
#include "MyDict.h"
#include <unistd.h>


std::size_t length(const std::string &str)
{
	std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		if(str[idx] &(1 << 7))
		{
			++idx;
		}
		++ilen;
	}
	return ilen;
}

int triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}


Task::Task(const std::string &expr, int sockfd, MyDict &mydict)
	: expr_(expr), 
	  sockfd_(sockfd),
	  mydict_(mydict)
{
}

Task::Task(const char *expr, int sockfd, MyDict &mydict)
	: expr_(expr),
	  sockfd_(sockfd),
	  mydict_(mydict)
{
}

void Task::execute(/*Cache &cache*/)
{
#if 0
	string str = query_cache();
	if(str! = NULL)
	{
		write(str);
		return ;
	}
#endif
	query_idx_table();
	response();

}

void Task::query_idx_table()
{
	//mydict_.show_dict();
	//mydict_.show_index_table();
	std::cout << "task is executing!" << std::endl;
	std::cout << "----" << expr_ << std::endl;
	
	std::string ch;
	auto idx_table = mydict_.get_index_table();
	for(std::size_t idx = 0; idx != expr_.size(); ++idx)
	{
		if(expr_[idx] & (1 << 7))
		{
			ch = expr_.substr(idx, 2);
			++idx;
		}
		else
		{
			ch = expr_.substr(idx, 1);
		}
		if(idx_table.count(ch))
		{
			std::cout << "idx_table count() return true" << std::endl;
			statistic(idx_table[ch]);
		}
	}
}

void Task::response()
{
	if(que_res_.empty())
	{
		std::string res = "no answer!";
		int nwrite = write(sockfd_, res.c_str(), res.size());
		std::cout << "response client" << std::endl;
	}
	else
	{
		MyResult res = que_res_.top();
		int nwrite = write(sockfd_, res.word_.c_str(), res.word_.size());
		std::cout << "response client" << std::endl;
	}
}

void Task::statistic(std::set<int> &iset)
{
	auto dict = mydict_.get_dict();
	auto iter = iset.begin();
	for(; iter != iset.end(); ++iter)
	{
		std::string rhs_word = dict[*iter].first;
		int idist = distance(rhs_word);
		if(idist < 3)
		{
			MyResult res;
			res.word_ = rhs_word;
			res.idist_ = idist;
			res.ifreq_ = dict[*iter].second;
			que_res_.push(res);
		}
	}
}

int Task::distance(const std::string &rhs)
{
	std::size_t lhs_len = length(expr_);
	std::size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(std::size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(std::size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		if(expr_[lhs_idx] & (1 << 7))
		{//处理中文
			sublhs = expr_.substr(lhs_idx, 2);
			++lhs_idx;
		}
		else
		{
			sublhs = expr_.substr(lhs_idx, 1);
		}
		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			if(rhs[rhs_idx] & (1 << 7))
			{
				subrhs = rhs.substr(rhs_idx, 2);
				++rhs_idx;
			}
			else
			{
				subrhs = rhs.substr(rhs_idx, 1);
			}
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
						editDist[dist_i][dist_j - 1] + 1,
						editDist[dist_i - 1][dist_j] + 1,
						editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}
