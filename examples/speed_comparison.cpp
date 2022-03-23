#include "fractionalBignum.hpp"
#include <iostream>
#include <cmath>
#include <gmp.h>
#include <ctime>


int main(int argc, char const *argv[])
{
    // double r = ((double) rand() / (RAND_MAX));
    const size_t K = 8;
    u_int64_t v[K] = {1,1,1,1};
    fractionalBignum<K> f(v);

    mpf_set_default_prec(K * 64);

    mpf_t m;
    mpf_init(m);

    mpf_t p;
    mpf_init(p);
    for(int i = 1; i <= K; i++) {
        mpf_set_d(p,pow(2,-(i * 64)));
        mpf_add(m,m,p);
    }

    const double log2_10 = 3.32192809489;
    const size_t precision = floor((K * 64) / log2_10);

    std::cout << "fractionalBignum:\t";
    std::cout << f.decimal_str() << std::endl;

    std::cout << "gmp mpf_t:\t\t";
    gmp_printf("%.*Ff\n", precision, m);


    const size_t iterations = 1UL << 30;
    std::cout << "running " << iterations << " iterations" << std::endl; 

    // fractional bignum speed test
    std::cout << "running fractionalBignum" << std::endl;
    auto tstart = clock();
    fractionalBignum<K> t_fb;
    for(auto i = 0; i < iterations; i++) {
        t_fb += f;
    }
    auto fb_time = clock() - tstart;
    auto fb_time_s = ((double)fb_time) / CLOCKS_PER_SEC;
    std::cout << fb_time_s << "s" << std::endl;

    // gmp mpf_t speed test
    std::cout << "running gmp mpf_t test" << std::endl;
    tstart = clock();
    mpf_t t;
    mpf_init(t);
    for(auto i = 0; i < iterations; i++) {
        mpf_add(t,t,m);
    }
    auto mpf_time = clock() - tstart;
    auto mpf_time_s = ((double) mpf_time) / CLOCKS_PER_SEC;
    std::cout << mpf_time_s << "s" << std::endl;

    std::cout << "check results:" << std::endl;
    std::cout << "fractionalBignum:\t";
    std::cout << t_fb.decimal_str() << std::endl;

    std::cout << "gmp mpf_t:\t\t";
    gmp_printf("%.*Ff\n", precision, t);

    return 0;
}
