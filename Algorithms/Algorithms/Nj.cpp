#include "Nj.h"

void NJ::merge(Cluster& a, Cluster& b, int dist_between)
{
	auto& cluster = a;

	cluster.size = a.size + b.size;
	cluster.sum_distances = a.sum_distances + b.sum_distances + dist_between;

	this->Tree.push(cluster);


}

void NJ::run(vector<string>& sequences, vector<vector<int>>& dist_matrix, vector<vector<string>>& profiles)
{
}

void NJ::init(size_t size)
{
	auto c1 = Cluster();

	for (size_t i = 0; i < size; i++)
	{
		c1.sum_distances = 0;
		c1.size = 1;
		c1.index_i = i;
		c1.index_j = i;

		this->Tree.push(c1);
	}
}

void NJ::update_profile(vector<vector<string>>& profiles, int id_profile, string s1, string s2,
	const unsigned char* alignment, int length_alignment)
{
}

void NJ::update_profile_multiple(vector<vector<string>>& profiles, int id_profile1, int id_profile2)
{
}

void NJ::update_dist_matrix(vector<vector<int>>& dist_matrix, int i_id, int value)
{
}

int NJ::score(vector<string> prof1, vector<string> prof2, int row, int col)
{
	return 0;
}
