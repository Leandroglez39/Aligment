

#include <algorithm>
#include <iostream>
#include "edlib.h"

using namespace std;

void printAlignment(const char* query, const char* target,
	const unsigned char* alignment, const int alignmentLength,
	const int position, const EdlibAlignMode modeCode);

int main()
{
	const char* query = "ACCTCTG";
	const char* target = "ACTCTGAAA";

	EdlibAlignResult result = edlibAlign(query, 7, target, 9, edlibNewAlignConfig(-1, EDLIB_MODE_NW, EDLIB_TASK_PATH, NULL, 0));

	auto endLocations = result.endLocations;


	auto alignment = result.alignment;
	auto alignmentLength = result.alignmentLength;

	printAlignment(query, target, alignment, alignmentLength,
		*(endLocations), EDLIB_MODE_NW);

	std::cout << "Hello World!\n";
}



void printAlignment(const char* query, const char* target,
	const unsigned char* alignment, const int alignmentLength,
	const int position, const EdlibAlignMode modeCode) {
	int tIdx = -1;
	int qIdx = -1;
	if (modeCode == EDLIB_MODE_HW) {
		tIdx = position;
		for (int i = 0; i < alignmentLength; i++) {
			if (alignment[i] != EDLIB_EDOP_INSERT)
				tIdx--;
		}
	}
	for (int start = 0; start < alignmentLength; start += 50) {
		// target
		printf("T: ");
		int startTIdx = -1;
		for (int j = start; j < start + 50 && j < alignmentLength; j++) {
			if (alignment[j] == EDLIB_EDOP_INSERT)
				printf("-");
			else
				printf("%c", target[++tIdx]);
			if (j == start)
				startTIdx = tIdx;
		}
		printf(" (%d - %d)\n", max(startTIdx, 0), tIdx);

		// match / mismatch
		printf("   ");
		for (int j = start; j < start + 50 && j < alignmentLength; j++) {
			printf(alignment[j] == EDLIB_EDOP_MATCH ? "|" : " ");
		}
		printf("\n");

		// query
		printf("Q: ");
		int startQIdx = qIdx;
		for (int j = start; j < start + 50 && j < alignmentLength; j++) {
			if (alignment[j] == EDLIB_EDOP_DELETE)
				printf("-");
			else
				printf("%c", query[++qIdx]);
			if (j == start)
				startQIdx = qIdx;
		}
		printf(" (%d - %d)\n\n", max(startQIdx, 0), qIdx);
	}
}