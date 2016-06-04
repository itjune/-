#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm> 
#include <set>

const int size = 100001;

struct suffix
{
    int index;  
    int rank[2]; 
};

void countSort(std::vector<struct suffix> &suffixes, int len, int index)
{
	int *count = new int [size];
	struct suffix *temp = new struct suffix[len];
	for (int i = 0; i < size; ++i)
	{
		count[i] = 0;
	} 
	for (int i = 0; i < len; ++i)
	{
		++count[suffixes[i].rank[index] + 1];
	}
	for(int i = 1; i < size; ++i)
	{
		count[i] += count[i-1];
	}
	for(int i = len - 1; i >= 0; --i)
	{
		temp[count[suffixes[i].rank[index] + 1] - 1] = suffixes[i];
		--count[suffixes[i].rank[index] + 1];
	}
	for(int i = 0; i < len; ++i)
	{
		suffixes[i] = temp[i];
	}
	delete [] temp;
	delete [] count;
	if (index != 0)
	{
		countSort(suffixes, len, index - 1);
	}	
	else 
		return;
} 

std::vector<int> buildSuffixArray(const std::string &txt, int len)
{
    std::vector<struct suffix> suffixes(len);
    for (int i = 0; i < len; ++i)
    {
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i] - 'a';
        suffixes[i].rank[1] = ((i + 1) < len) ? (txt[i + 1] - 'a'): -1;
    }

	countSort(suffixes, len, 1);
    std::vector<int> ind(len); 
    for (int k = 4; k < 2 * len; k = k * 2)
    {

        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < len; ++i)
        {
            if (suffixes[i].rank[0] == prev_rank &&
                    suffixes[i].rank[1] == suffixes[i-1].rank[1])
            {
                suffixes[i].rank[0] = rank;
            }
            else 
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }
        for (int i = 0; i < len; ++i)
        {
            int nextindex = suffixes[i].index + k/2;
            suffixes[i].rank[1] = (nextindex < len) ?
                                  suffixes[ind[nextindex]].rank[0]: -1;
        }

		countSort(suffixes, len, 1);
    }

    std::vector<int> suffixArr;
    for (int i = 0; i < len; ++i)
        suffixArr.push_back(suffixes[i].index);
    return suffixArr;
}
 

std::vector<int> kasai(const std::string &txt, std::vector<int> &suffixArr)
{
    int len = suffixArr.size();
    std::vector<int> lcp(len, 0);
    std::vector<int> invSuff(len, 0);
    for (int i = 0; i < len; ++i)
        invSuff[suffixArr[i]] = i;
    int k = 0;
    for (int i = 0; i < len; ++i)
    {
        if (invSuff[i] == len - 1)
        {
            k = 0;
            continue;
        }

        int j = suffixArr[invSuff[i] + 1];

        while (i + k < len && j + k < len && txt[i+k] == txt[j+k])
            ++k;
        lcp[invSuff[i]] = k; 
        if (k > 0)
            --k;
    }
    return lcp;
}

void rmq_build(std::vector<int> &lcp, int **st) 
{
    int len = lcp.size();
	for (int i = 0; i < len; ++i)
	{
		st[0][i] = lcp[i];
	}
    for (int k = 1; (1 << k) <= len; ++k)
    {
        for (int i = 0; (i + (1 << k) - 1) < len; ++i)
        { 
            st[k][i] = std::min(st[k-1][i],st[k-1][i + (1 << (k-1))]);		
        }
    }
}

int rmq(int **st, int i, int j)
{
	int k = (int) log2(j - i + 1);
	return std::min(st[k][i], st[k][j - (1 << k) + 1]); 
}
 
 
int main()
{
    std::string str;
	std::cin >> str;
	int len = str.length();
	if (len == 1)
	{
		std::cout << 0;
		return 0;
	}
    std::vector<int> suffixArr = buildSuffixArray(str, len);
    std::vector<int> lcp = kasai(str, suffixArr);
	int k = (int) log2(len);
	int **st = new int* [k+1];
	for (int i = 0; i <= k; ++i)
    {
        st[i] = new int[len];
    }
	rmq_build(lcp, st);
	std::vector<int> invSuff(len, 0);
    for (int i = 0; i < len; ++i)
	{
		invSuff[suffixArr[i]] = i;	
	}	

	std::set<int> prev;
	std::vector<int> result(len, 0);
	prev.insert(invSuff[0]);
	std::cout << result[0] << std::endl;
	std::set<int>::iterator itup, itlow, ituptmp;
	for (int i = 1; i < len; ++i)
	{
		itup = prev.upper_bound(invSuff[i]); 
		if (itup == prev.begin())
		{
			result[i] = rmq (st, invSuff[i], *itup - 1);
		}
		else 
		{
			if (itup == prev.end())
			{
				result[i] = rmq (st, *prev.rbegin(), invSuff[i] - 1);
			}
			else
			{
				ituptmp = itup;
				itlow = --itup;
				result[i] = std::max(rmq(st, *itlow, invSuff[i] - 1), rmq(st, invSuff[i], *ituptmp - 1));
			}
		}
		prev.insert(invSuff[i]);
		std::cout << result[i] << std::endl;
	}
	for (int i = 0; i <= k; ++i)
    {
        delete [] st[i];
    }
	delete [] st;
    return 0;
}