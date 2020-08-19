#include "Refine.h"



void refine(vector<string>& alignment, map<char, pair<int, vector<int>>>& dic)
{


	int col = 0;
	bool goon = true;
	int alignment_size = alignment[0].size();


	while (goon)
	{

		for (int i = 0; i < alignment.size(); i++)
		{

			auto val = alignment[i][col];

			const auto exist = dic.count(val) == 1;

			if (val != '-' && !exist)
			{

				dic[val] = make_pair(1, vector<int>(1, i));
			}
			else if (val != '-')
			{
				auto aux = dic[val];

				aux.first += 1;
				aux.second.push_back(i);

				dic[val] = aux;

			}


		}

		if (dic.size() > 1)
		{

			update_alignment(alignment, dic, col);
			alignment_size++;

		}

		if (col < alignment_size - 1)
		{

			col++;
		}
		else goon = false;

		dic.clear();
	}


}


void update_alignment(vector<string>& alignment, map<char, pair<int, vector<int>>>& dic, int col)
{
	int maximumn = 0;
	vector<int> positions;
	vector<int> check(alignment.size(), 0);


	for (auto x : dic)
	{

		if (x.second.first > maximumn)
		{
			maximumn = x.second.second.size();
			positions = x.second.second;

		}
	}


	for (int i = 0; i < positions.size(); i++)
	{
		alignment[positions[i]].push_back('-');
		check[positions[i]] = 1;
	}

	for (int i = 0; i < alignment.size(); i++)
	{
		if (check[i] == 0)
		{
			if (alignment[i][col] != '-')
			{
				alignment[i].insert(col, "-");
			}
			else alignment[i].push_back('-');


		}
	}

}


void erase_gaps(vector<string>& alignments)
{
	auto col = alignments[0].size() - static_cast<unsigned long long>(1);
	auto goon = true;
	int len = alignments.size();
	auto align_size = alignments[0].size() - 1;

	while (goon)
	{

		for (auto& alignment : alignments)
		{
			if (alignment[col] == '-')
			{
				len--;
			}
			else
			{
				len = alignments.size();
				break;
			}

		}

		if (len == 0)
		{
			for (auto& alignment : alignments)
				alignment.resize(align_size);
			len = alignments.size();
			align_size--;


		}
		else return;

		if (col > 0)
		{
			col--;
		}
		else goon = false;

	}

}