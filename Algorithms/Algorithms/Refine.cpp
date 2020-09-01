#include "Refine.h"



void refine(vector<string>& alignment, map<char, pair<int, vector<int>>>& dic)
{


	long long col = 0;
	bool goon = true;
	long long alignment_size = alignment[0].size();


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
	long long len = alignments.size();


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
				alignment.erase(col,1);
			len = alignments.size();



		}


		if (col > 0)
		{
			col--;
		}
		else goon = false;

	}

}

void block_shift_refinement(vector<string>& alignments)
{
	const auto col = alignments[0].size() - 1;
	vector<char> col_assign(col + 1, '*');

	for (auto& alignment : alignments)
	{
		for (auto j = 0; j <= col; j++)
			if (col_assign[j] == '*' || col_assign[j] == '-')
			{
				col_assign[j] = alignment[j];
			}
	}

	for (auto i = 0; i < alignments.size(); i++)
	{
		for (auto j = 0; j <= col; j++)
		{
			if (alignments[i][j] == '-')
			{

				for (auto k = j + 1; k <= col; k++)
				{
					if (alignments[i][k] != '-')
					{
						int pos;

						if (col_to_swap(col_assign, j, k - 1, alignments[i][k], pos))
						{
							swap(alignments[i][k], alignments[i][pos]);
							j = pos + 1;
						}
						else
						{
							j = k;
							break;
						}
					}
				}
			}
		}
	}


}


bool col_to_swap(vector<char>& col_asign, int init, int end, char val, int& pos)
{

	for (auto i = init; i <= end; i++)
		if (col_asign[i] == val)
		{
			pos = i;
			return true;
		}

	return false;
}
