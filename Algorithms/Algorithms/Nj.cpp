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

	char* query;
	char* target;

	if (this->Tree.size() >= 2)
	{
		auto& c1 = this->Tree.front();
		this->Tree.pop();
		auto& c2 = this->Tree.front();
		this->Tree.pop();


		string s1 = sequences[c1.index_i];
		string s2 = sequences[c2.index_i];


		if (c1.size == 1 && c2.size == 1)
		{
			query = static_cast<char*>(malloc(sizeof(char) * (s1.size() + 1)));
			target = static_cast<char*>(malloc(sizeof(char) * (s1.size() + 1)));

			copy(s1.begin(), s1.end(), query);
			query[s1.size()] = '\0';
			copy(s2.begin(), s2.end(), target);
			target[s2.size()] = '\0';

			EdlibAlignResult result = edlibAlign(query, s1.size(), target, s2.size(), edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, nullptr, 0));

			merge(c1, c2, dist_matrix[c1.index_i][c2.index_i]);

			update_dist_matrix(dist_matrix, c1.index_i, dist_matrix[c1.index_i][c2.index_i]);

			update_profile(profiles, c1.index_i, s1, s2, result.alignment, result.alignmentLength);


		}
		else
		{

			merge(c1, c2, dist_matrix[c1.index_i][c2.index_i]);

			update_dist_matrix(dist_matrix, c1.index_i, dist_matrix[c1.index_i][c2.index_i]);

			update_profile_multiple(profiles, c1.index_i, c2.index_i);
		}
	}
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
	vector<string> profile;

	string query;
	string target;
	int s1_index = 0;
	int s2_index = 0;



	for (auto i = 0; i < length_alignment; i++)
	{
		switch (alignment[i])
		{
		case 1:
		{
			query.push_back(s1[s1_index]);
			target.push_back('-');
			s1_index++;
		}
		case 2:
		{
			target.push_back(s2[s1_index]);
			query.push_back('-');
			s2_index++;
		}
		default:
		{
			query.push_back(s1[s1_index]);
			target.push_back(s2[s2_index]);
			s1_index++;
			s2_index++;
		}
		}
	}

	profile.push_back(target);
	profile.push_back(query);

	profiles[id_profile] = profile;

}

void NJ::update_profile_multiple(vector<vector<string>>& profiles, int id_profile1, int id_profile2)
{

	vector<string> profile;




}

void NJ::update_dist_matrix(vector<vector<int>>& dist_matrix, int i_id, int value)
{
	for (int i = 0; i < dist_matrix[i_id].size(); i++)
	{
		if (i != i_id)
		{
			dist_matrix[i][i_id] = dist_matrix[i][i_id] + value;
			dist_matrix[i_id][i] = dist_matrix[i_id][i] + value;
		}
	}
}

int NJ::score(vector<string> prof1, vector<string> prof2, int row, int col)
{
	return 0;
}

vector<string> NJ::align_alignments(vector<string> profil1, vector<string> profile2)
{

	vector<string> result();


	
	
}
