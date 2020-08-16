#include "mstar.h"

#include <iostream>

void build_distance_matrix(vector<string>& sequences, vector<vector<pair<unsigned char*, int>>>& alignments, int& piv)
{
	const auto seq_len = sequences.size();
	// ReSharper disable once CppJoinDeclarationAndAssignment
	EdlibAlignResult result;
	long min_distance = 999999999;
	auto& piv_act = piv;
	size_t j = 0;

	vector<long> dist(sequences.size(), 0);

	//#pragma omp parallel for shared(min_distance, piv_act)  collapse(2) reduction (+: sum_accumulate)
	for (size_t i = 0; i < seq_len; i++)
	{
		for (j = i + 1; j < seq_len; j++)
		{
			// ReSharper disable once CppJoinDeclarationAndAssignment
			result = edlibAlign(sequences[j].c_str(), sequences[j].size(), sequences[i].c_str(), sequences[i].size(),
				edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));


			dist[i] += result.editDistance;
			dist[j] += result.editDistance;

			alignments[i][j] = make_pair(result.alignment, result.editDistance);
			alignments[j][i] = make_pair(result.alignment, result.editDistance);


			edlibFreeAlignResult(result);
		}
	}

	for (size_t i = 0; i < seq_len; i++)
		if (min_distance > dist[i])
		{
			min_distance = dist[i];
			piv_act = i;
		}


}


void msa(vector<vector<pair<unsigned char*, int>>>& pair_alignments, vector<string>& multi_align, int piv)
{
	auto star = merge_star(pair_alignments, piv);

	multi_align[0] = star;
	string substr;

	int len = star.size();
	int cnt = 0;
	for (int i = 0; i < multi_align.size(); i++) {
		if (i == piv)continue;
		int pos = 0;
		substr = "";
		for (int j = 0; j < len; j++)
		{

			auto a = static_cast<char>(pair_alignments[piv][cnt].first[pos]) - '0';

			if (piv != cnt && a == star[j]) {
				substr += to_string(static_cast<int>(pair_alignments[piv][cnt].first[pos]));
				pos++;
			}
			else {
				substr += '1';
			}
		}
		multi_align[i] = substr;
		cnt++;
	}

}

string merge_star(vector<vector<pair<unsigned char*, int>>>& pair_alignments, int piv)
{


	string comstr;
	string str1;
	unsigned char* str2;
	for (size_t i = 0; i < pair_alignments[piv].size(); i++)
	{
		str1 = comstr;

		str2 = pair_alignments[piv][i].first;

		comstr.clear();

		int len1 = str1.size();
		int len2 = pair_alignments[piv][i].second;
		int pivx = 0;
		int pivy = 0;
		while (len1 != pivx || len2 != pivy) {
			if (len1 == pivx) {
				comstr += to_string(static_cast<int>(str2[pivy]));
				pivy++;
			}
			else if (len2 == pivy) {
				comstr += str1[pivx];
				pivx++;
			}
			else {
				if (str1[pivx] == '1' && str2[pivy] == 1) {
					comstr += str1[pivx];
					pivx++;
					pivy++;
				}
				else if (str1[pivx] == '1') {
					comstr += str1[pivx];
					pivx++;
				}
				else if (str2[pivy] == 1) {
					comstr += to_string(static_cast<int>(str2[pivy]));
					pivy++;
				}
				else {
					comstr += str1[pivx];
					pivx++;
					pivy++;
				}
			}
		}
	}
	return comstr;

}
