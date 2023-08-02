#include "mstar.h"
#include <omp.h>
#include <iostream>

void build_distance_matrix(vector<string>& sequences, int& piv, bool complete, float dif)
{
	const auto seq_len = sequences.size();
	// ReSharper disable once CppJoinDeclarationAndAssignment
	EdlibAlignResult result;
	long min_distance = 999999999;
	auto& piv_act = piv;
	int j = 0;
	int sum_acumulate = 0;
	int max_length = 0;

	for (size_t i = 0; i < seq_len; i++)
	{
		if (max_length < sequences[i].size())
		{
			max_length = sequences[i].size();
		}
	}

	vector<long> dist(sequences.size(), 0);

	omp_set_num_threads(4);


#pragma omp parallel for shared(piv_act) private(result) 
	for (j = 1; j < seq_len; j++)
	{


		// ReSharper disable once CppJoinDeclarationAndAssignment
		result = edlibAlign(sequences[j].c_str(), sequences[j].size(), sequences[0].c_str(), sequences[0].size(),
			edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_DISTANCE, NULL, 0));

#pragma omp critical
		{
			dist[j] += result.editDistance;
		}



		edlibFreeAlignResult(result);
	}




	for (size_t i = 1; i < seq_len; i++)
	{
		for (size_t k = i + 1; k < seq_len; k++)
		{

			sum_acumulate += dist[i] + dist[k];
		}

		if (min_distance > sum_acumulate)
		{

			if (complete && sequences.size() * (static_cast<float>(max_length) / dif) > dist[i])
			{
				piv_act = i;
				return;
			}


			min_distance = sum_acumulate;
			piv_act = i;
			sum_acumulate = 0;
		}

	}
}


void pair_alignment(vector<string>& sequences, vector<pair<string, string>>& alignments, int& piv)
{

	const auto seq_len = sequences.size();
	// ReSharper disable once CppJoinDeclarationAndAssignment
	EdlibAlignResult result;
	int j = 0;


#pragma omp parallel for shared(alignments) private(result) 
	for (j = 0; j < seq_len; j++)
	{
		if (j != piv)
		{

			// ReSharper disable once CppJoinDeclarationAndAssignment
			result = edlibAlign(sequences[j].c_str(), sequences[j].size(), sequences[piv].c_str(), sequences[piv].size(),
				edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));

			alignments[j] = convert_to_vector(result.alignmentLength, result.alignment, sequences, j, piv);

			edlibFreeAlignResult(result);
		}
	}



}

void msa(vector<pair<string, string>>& pair_alignments, vector<string>& multi_align, int piv)
{
	string star;


	for (size_t i = 0; i < pair_alignments.size(); i++)
		if (i != piv)
			star = merge_star(star, pair_alignments[i].second);



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

			if (cnt == pair_alignments.size())	return;


			a = pair_alignments[cnt].second;
			b = pair_alignments[cnt].first[pos];

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
