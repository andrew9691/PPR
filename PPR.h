#ifndef PPR_H
#define PPR_H

#include <NTL\ZZ.h>
#include <NTL\ZZ_p.h>
#include <cmath>
#include <vector>
#include <pair.h>
#include <ostream>

bool equiv_relation(const NTL::ZZ& a, const NTL::ZZ& b, const NTL::ZZ& p)
{
	return (a - b) % p == 0;
}

NTL::ZZ_p find_PPR(const NTL::ZZ& p, const double& eps)
{
	NTL::ZZ_p::init(p);

	double f = 1 + 2 / NTL::conv<double>(p - 1);
	double log_power = std::log(NTL::conv<double>(p - 1) / 2);
	double one_eps = 1 - eps;

	double diff = DBL_MAX;
	NTL::ZZ B;
	for (size_t b = 2; b < p; b++)
	{
		double logb_lp = log_power / std::log((double)b);
		double res = f * std::pow(1 - 1 / (double)b, logb_lp);
		double new_diff = std::abs(one_eps - res);

		if (new_diff < diff)
		{
			diff = new_diff;
			B = b;
		}
		else if (new_diff > diff)
			break;
	}

	NTL::ZZ p_1 = p - 1;
	NTL::ZZ prime_div = (NTL::ZZ)2;
	std::vector<std::pair<NTL::ZZ, NTL::ZZ>> factored_p_1;

	while (prime_div < B)
	{
		NTL::ZZ deg = (NTL::ZZ)0;
		while (p_1 % prime_div == 0)
		{
			p_1 /= prime_div;
			deg++;
		}

		if (deg > 0)
			factored_p_1.emplace_back(std::make_pair(prime_div, deg));

		prime_div = NTL::NextPrime(prime_div + 1);
	}
	NTL::ZZ Q = p_1;

	NTL::ZZ_p c = (NTL::ZZ_p)1;
	for (size_t i = 0; i < factored_p_1.size(); i++)
	{
		while (true)
		{
			m:
			NTL::ZZ_p alpha = NTL::random_ZZ_p();
			if (alpha == 0)
				goto m;
			if (!equiv_relation(NTL::PowerMod(NTL::conv<NTL::ZZ>(alpha), (p - 1) / factored_p_1[i].first, p), (NTL::ZZ)1, p))
			{
				c *= NTL::conv<NTL::ZZ_p>(NTL::PowerMod(NTL::conv<NTL::ZZ>(alpha), (p - 1) / NTL::power(factored_p_1[i].first, NTL::conv<long>(factored_p_1[i].second)), p));
				break;
			}
		}
	}

	if (Q == 1)
	{
		std::cout << "probability of correctness = 1" << std::endl;
		return c;
	}

	double prob = 1 + 1 / NTL::conv<double>(Q - 1);
	double logB_Q = std::log(NTL::conv<double>(Q)) / std::log(NTL::conv<double>(B));
	prob *= std::pow(1 - 1 / NTL::conv<double>(B), logB_Q);

	while (true)
	{
		NTL::ZZ_p b = NTL::random_ZZ_p();
		NTL::ZZ_p bpowp_1Q = NTL::conv<NTL::ZZ_p>(NTL::PowerMod(NTL::conv<NTL::ZZ>(b), (p - 1) / Q, p));
		if (!equiv_relation(NTL::conv<NTL::ZZ>(bpowp_1Q), (NTL::ZZ)1, p))
		{
			std::cout << "probability of correctness = " << prob << std::endl;
			return c * bpowp_1Q;
		}
	}
}

#endif
