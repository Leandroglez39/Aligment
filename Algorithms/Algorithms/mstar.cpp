#include "mstar.h"

void build_distance_matrix(vector<string>& sequences, vector<vector<unsigned char*>>& alignments, int& piv)
{
	const auto seq_len = sequences.size();
	// ReSharper disable once CppJoinDeclarationAndAssignment
	EdlibAlignResult result;
	long min_distance = 999999999;
	long sum_accumulate = 0;
	auto& piv_act = piv;
	size_t j = 0;

#pragma omp parallel for shared(min_distance, piv_act)  collapse(2) reduction (+: sum_accumulate)
	for (size_t i = 0; i < seq_len; i++)
	{
		for (j = i + 1; j < seq_len; j++)
		{
			// ReSharper disable once CppJoinDeclarationAndAssignment
			result = edlibAlign(sequences[j].c_str(), sequences[j].size(), sequences[i].c_str(), sequences[i].size(),
				edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));

			sum_accumulate += result.editDistance;

			alignments[i][j] = result.alignment;
			alignments[j][i] = result.alignment;


			edlibFreeAlignResult(result);
		}
		if (min_distance > sum_accumulate)
		{
			min_distance = sum_accumulate;
			piv_act = i;
		}
		sum_accumulate = 0;
	}
}
