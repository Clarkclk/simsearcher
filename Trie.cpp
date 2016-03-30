#include "Trie.h"
#include <cstdio>

TrieNode::TrieNode()
{
	for (int i = 0; i < 128; i++)
	{
		m_child_nodes[i] = 0;
	}
}

TrieNode::~TrieNode()
{

}

Trie::Trie()
{
	m_pool = new TrieNode[2000000];
	m_idx = 1;
}

Trie::~Trie()
{

}

void Trie::insert(char* str, int idx, int length)
{
	int node = 0;
	int it = 0;
	while (it < length)
	{
		int index = str[it];
		if (m_pool[node].m_child_nodes[index] == 0) m_pool[node].m_child_nodes[index] = m_idx++;
		node = m_pool[node].m_child_nodes[index];
		it++;
	}
	if (!m_pool[node].m_list.empty() && m_pool[node].m_list[m_pool[node].m_list.size() - 1] == idx) return;
	m_pool[node].m_list.push_back(idx);
}

std::vector<int> * Trie::searchStr(char* str, int length)
{
	int node = 0;
	int it = 0;
	while (it < length)
	{
		int index = str[it];
		if (m_pool[node].m_child_nodes[index] == 0) return NULL;
		node = m_pool[node].m_child_nodes[index];
		it++;
	}
	return &(m_pool[node].m_list);
}