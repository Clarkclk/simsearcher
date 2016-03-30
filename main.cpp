#include "SimSearcher.h"

using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 3, edThreshold = 3;
	double jaccardThreshold = 0.2;

	searcher.createIndex(argv[1], q);
	searcher.searchJaccard("aaa bbb eee fff", jaccardThreshold, resultJaccard);
	searcher.searchED("apnqouthtfgabqevqejwavptuysvduqgkdwyyshtxnvyebtwfeuhajaujtpnphtzlrzljhggubfafawmfsuhewbnrschbxgmpgzy", edThreshold, resultED);
	
	for (int i = 0; i < resultJaccard.size(); i++)
	{
		printf("%d %lf \n", resultJaccard[i].first, resultJaccard[i].second);
	}

	return 0;
}

