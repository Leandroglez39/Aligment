#include "mstar.h"
#include <omp.h>
#include <iostream>

void build_distance_matrix(vector<string>& sequences, vector<vector<pair<string, string>>>& alignments, int& piv)
{
	const auto seq_len = sequences.size();
	// ReSharper disable once CppJoinDeclarationAndAssignment
	EdlibAlignResult result;
	long min_distance = 999999999;
	auto& piv_act = piv;
	int j = 0;
	int i = 0;


	vector<long> dist(sequences.size(), 0);

	omp_set_num_threads(4);

	for (i = 0; i < seq_len; i++)
	{
#pragma omp parallel for shared(alignments, piv_act, i) private(result) 
		for (j = i + 1; j < seq_len; j++)
		{
			// ReSharper disable once CppJoinDeclarationAndAssignment
			result = edlibAlign(sequences[j].c_str(), sequences[j].size(), sequences[i].c_str(), sequences[i].size(),
				edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));

#pragma omp critical
			{
				dist[i] += result.editDistance;
				dist[j] += result.editDistance;
			}

			alignments[i][j] = convert_to_vector(result.alignmentLength, result.alignment, sequences, j, i);
			alignments[j][i] = make_pair(alignments[i][j].second, alignments[i][j].first);


			edlibFreeAlignResult(result);
		}

		if (sequences.size() * 220 > dist[i])
		{
			piv_act = i;
			return;
		}
		//alignments[i].clear();

	}

	for (size_t i = 0; i < seq_len; i++)
		if (min_distance > dist[i])
		{
			min_distance = dist[i];
			piv_act = i;
		}


}


void msa(vector<vector<pair<string, string>>>& pair_alignments, vector<string>& multi_align, int piv)
{
	string star;


	for (size_t i = 0; i < pair_alignments[piv].size(); i++)
		if (i != piv)
			star = merge_star(star, pair_alignments[piv][i].second);

	

	multi_align[piv] = star;

	string substr;

	int len = star.size();
	int cnt = 0;
	string a;
	char b;

	for (int i = 0; i < multi_align.size(); i++) {
		if (i == piv)continue;
		int pos = 0;
		substr = "";
		for (int j = 0; j < len; j++)
		{
			if (piv == cnt) cnt++;

			if (cnt == pair_alignments[piv].size())	return;


			a = pair_alignments[piv][cnt].second;
			b = pair_alignments[piv][cnt].first[pos];
			
			if (a[pos] == star[j])
			{
				substr += b;
				pos++;
			}
			else
			{
				substr += '-';
			}
		}
		multi_align[i] = substr;
		cnt++;
	}

}

string merge_star(string str1, string str2)
{


	string comstr;


	int len1 = str1.size();
	int len2 = str2.size();
	int pivx = 0;
	int pivy = 0;
	while (len1 != pivx || len2 != pivy) {
		if (len1 == pivx) {
			comstr += str2[pivy];
			pivy++;
		}
		else if (len2 == pivy) {
			comstr += str1[pivx];
			pivx++;
		}
		else {
			if (str1[pivx] == '-' && str2[pivy] == '-')
			{
				comstr += str1[pivx];
				pivx++;
				pivy++;
			}
			else if (str1[pivx] == '-') {
				comstr += str1[pivx];
				pivx++;
			}
			else if (str2[pivy] == '-') {
				comstr += str2[pivy];
				pivy++;
			}
			else {
				comstr += str1[pivx];
				pivx++;
				pivy++;
			}
		}
	}

	return comstr;

}

pair<string, string> convert_to_vector(const int length, const unsigned char* alignment, vector<string>& sequences, int index_q, int index_t)
{

	pair<string, string> s;


	string query;
	string target;
	int s1_index = 0;
	int s2_index = 0;



	for (auto i = 0; i < length; i++)
	{
		switch (alignment[i])
		{
		case 3:
		{
			target.push_back(sequences[index_t][s2_index]);
			query.push_back('-');
			query.push_back(sequences[index_q][s1_index]);
			target.push_back('-');
			s2_index++;
			s1_index++;
			break;
		}
		case 1:
		{
			query.push_back(sequences[index_q][s1_index]);
			target.push_back('-');
			s1_index++;
			break;
		}
		case 2:
		{
			target.push_back(sequences[index_t][s2_index]);
			query.push_back('-');
			s2_index++;
			break;
		}
		default:
		{
			query.push_back(sequences[index_q][s1_index]);
			target.push_back(sequences[index_t][s2_index]);
			s1_index++;
			s2_index++;
		}
		}
	}


	s.first = query;
	s.second = target;


	return  s;
}

void parse_alignment(vector<string>& multi_align, int piv, vector<string>& sequences, vector<string>& alig_result)
{

	for (int i = 0; i < multi_align.size(); i++)
	{

		if (i == piv)
		{
			int j = 0;
			int x = 0;

			while (x < multi_align[i].size())
			{
				switch (multi_align[i][x])
				{

				case '1':
				{
					alig_result[i].push_back('-');
					x++;
					break;
				}
				default:
				{

					alig_result[i].push_back(sequences[i][j]);
					j++;
					x++;
					break;;
				}

				}

			}

		}
		else
		{
			int j = 0;
			int x = 0;

			while (x < multi_align[i].size())
			{
				switch (multi_align[i][x])
				{

				case '2':
				{
					alig_result[i].push_back('-');
					x++;
					break;
				}
				default:
				{

					alig_result[i].push_back(sequences[i][j]);
					j++;
					x++;
					break;;
				}

				}
			}

		}


	}

}
