#pragma once

#include <vector>
#include <utility>
#include <string>

#include "Trie.h"

const int SUCCESS = 0;
const int FAILURE = 1;

#define MAX_ITEM 1000010

struct sortItem
{
	std::vector<int> * data;
	int size;

	sortItem(std::vector<int> * data)
	{
		this->data = data;
		size = data->size();
	}

	sortItem(){}
	~sortItem(){}
};

class SimSearcher
{
public:
	SimSearcher();
	~SimSearcher();

	Trie* m_tree;
	char** m_string_list;
	int* m_string_size, *searchQueue, *searchList;
	int m_q;
	int m_idx;
	int d[257][257];
	sortItem* lists;

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
	int DP(const char *query, int item, int length_query, int thres);
};

