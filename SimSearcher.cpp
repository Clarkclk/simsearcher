#include "SimSearcher.h"
#include <fstream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <iostream>

#define MAX_LENGTH 256

using namespace std;

bool operator < (const sortItem& a, const sortItem& b)
{
	return a.size < b.size;	
}


int min(int a, int b)
{
	if (a > b) return b;
	return a;
}

int max(int a, int b)
{
	if (a > b) return a;
	return b;
}

SimSearcher::SimSearcher()
{
	m_tree = new Trie();
	m_string_size = new int[MAX_ITEM];
	m_string_list = new char*[MAX_ITEM];
	searchQueue = new int[MAX_ITEM];
    searchList = new int[MAX_ITEM];
   	lists = new sortItem[MAX_ITEM];

	for (int i = 0; i < 257; i++)
	{
		d[i][0] = i;
	}

	for (int j = 0; j < 257; j++)
	{
		d[0][j] = j;
	}
}

SimSearcher::~SimSearcher()
{
	delete m_tree;
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	m_q = q;
	m_idx = 0;
	ifstream fp(filename);
    char line[260];
    const int buffer_size = 260;
    while(fp.getline(line, buffer_size))
    {
        int length = strlen(line);
        m_string_list[m_idx] = new char[257];
        memcpy(m_string_list[m_idx], line, length);
        m_string_size[m_idx] = length;
       	for (int i = 0; i < length - q + 1; i++)
       	{
       		m_tree->insert(line + i, m_idx, m_q);
       	}
        m_idx++;
    }
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	int length_query = strlen(query);
	int thres = threshold;
	int lists_idx = 0;
	

	for (int i = 0; i < length_query - m_q + 1; i++)
    {
       	void* ptr = m_tree->searchStr((char*)query + i, m_q);
		if (ptr != NULL) 
		{
			sortItem item((std::vector<int>*)ptr);
			if (item.size > 0) lists[lists_idx++] = item;
		}
    }


    int searchList_idx = 0;
    memset(searchQueue, 0, sizeof(int) * MAX_ITEM);

    std::sort(lists, lists + lists_idx);

    int t = length_query - m_q + 1 - thres * m_q;

    if (t >= 1)
    {
	    for (int i = 0; i < lists_idx - t + 1; i++)
	    {
	    	for (int j = 0; j < lists[i].size; j++)
	    	{
	    		int temp = (*(lists[i].data))[j];
	    		if (searchQueue[temp] <= 0)
	    		{
	    			searchList[searchList_idx++] = temp;
	    		}
	    		searchQueue[temp]++;
	    	}
	    }


	    for (int i = 0; i < searchList_idx; i++)
	    {
	    	if (searchQueue[searchList[i]] >= t)
	    	{
	    		int dps = DP(query, searchList[i], length_query, thres);	
	    		if (dps >= 0) result.push_back(std::make_pair(searchList[i], dps));
	    		continue;
	    	}

	    	for (int j = lists_idx - t + 1; j < lists_idx; j++)
	    	{
	    		if (std::binary_search(lists[j].data->begin(), lists[j].data->end(), searchList[i]))
	    		{
	    			searchQueue[searchList[i]]++;

	    			if (searchQueue[searchList[i]] >= t)
	    			{
	    				int dps = DP(query, searchList[i], length_query, thres);
	    				if (dps >= 0) result.push_back(std::make_pair(searchList[i], dps));
	    				break;
	    			}
	    		}
	    		if (searchQueue[searchList[i]] + lists_idx - j - 1 < t) break;
	    	}
	    }

	}
	else
	{
		for (int i = 0; i < m_idx; i++)
		{
			int dps = DP(query, i, length_query, thres);
	    	if (dps >= 0) result.push_back(std::make_pair(i, dps));
		}
	}

	std::sort(result.begin(), result.end());
	return SUCCESS;
}

int SimSearcher::DP(const char* query, int item, int length_query, int thres)
{
	char* str = m_string_list[item];
	int length_str = m_string_size[item];
	if (abs(length_str - length_query) > thres) return -1;

	bool exceed = true;
	for (int i = 1; i <= length_str; i++)
	{
		exceed = true;
		int j1 = max(1, i - thres);
		int j2 = min(length_query, i + thres);
		for (int j = j1; j <= j2; j++)
		{
			int t = (str[i - 1] == query[j - 1])? 0 : 1;
			int temp1 = d[i-1][j] + 1;
			int temp2 = d[i][j-1] + 1;
			if (j + 1 - i > thres) temp1 = MAX_LENGTH;
			if (i + 1 - j > thres) temp2 = MAX_LENGTH;
			d[i][j] = min(temp1, temp2);
			d[i][j] = min(d[i][j], d[i-1][j-1] + t);
			if (d[i][j] <= thres) exceed = false;
		}
		if (d[i][0] <= thres) exceed = false;
		if (exceed) return -1;
	}
	if (d[length_str][length_query] > thres) return -1;
	return d[length_str][length_query];
}
