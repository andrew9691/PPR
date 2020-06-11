#include <iostream>
#include "PPR.h"
#include <ctime>

using namespace std;
using namespace NTL;

int main()
{
	ZZ p = NTL::RandomLen_ZZ(200);
	cout << p << endl;
	double eps = 0.1;
	NTL::ZZ_p::init(p);

	unsigned int start = clock();
	NTL::ZZ_p res = find_PPR(p, eps);
	unsigned int end = clock();
	cout << endl << "res = " << res << endl;
	cout << "Time in sec: " << (end - start) / 1000.0 << endl;
}
