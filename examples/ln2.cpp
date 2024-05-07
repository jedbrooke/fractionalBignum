// A program to show the calculation of the constant ln(2)
    // using the fractionalBignum library

// sample output on an i5 4590
// double: 0.402235s
// fractional bignum: 4.49475s
// gmp mpf_t: 5.33458s
// results:
// double: 0.69314718556041921
// fb:     0.69314717555994533441723212145817530153
// mpf:    0.69314717555994533441723212145817531808
// actual: 0.693147180559945309417232121458

#include "../fractionalBignum.hpp"
#include <gmp.h>
#include <iostream>
#include <ctime>

#define START_CLOCK tstart = clock();
#define STOP_CLOCK time = ((double)(clock() - tstart)) / CLOCKS_PER_SEC;




int main(int argc, char const *argv[])
{
    const size_t iterations = 1E8;

    clock_t tstart = clock();
    double time;

    START_CLOCK
    double s = 1;
    for(size_t i = 2; i < iterations; i+=2) {
        s -= 1.0 / i;
        s += 1.0 / (i+1);
    }
    STOP_CLOCK
    

    std::cout << "double: " << time << "s" << std::endl;

    
    START_CLOCK
    const size_t K = 2;
    fractionalBignum<K> s_fb(0.5);
    for(size_t i = 3; i < iterations; i+=2) {
        auto t1 = fb_div<K>(1,i);
        auto t2 = fb_div<K>(1,i+1);
        s_fb = s_fb + t1 - t2;


    }
    STOP_CLOCK

    std::cout << "fractional bignum: " << time << "s" << std::endl;


    START_CLOCK
    mpf_set_default_prec(K * 64);

    mpf_t s_mpf;
    mpf_init(s_mpf);

    const double log2_10 = 3.32192809489;
    const size_t precision = floor((K * 64) / log2_10);

    mpf_t t1_mpf;
    mpf_init(t1_mpf);
    mpf_t t2_mpf;
    mpf_init(t2_mpf);

    mpf_t one_mpf;
    mpf_init(one_mpf);
    mpf_set_ui(one_mpf, 1);


    for(size_t i = 0; i < iterations; i+=2) {
        mpf_div_ui(t1_mpf, one_mpf, i+1);
        mpf_div_ui(t2_mpf, one_mpf, i+2);

        mpf_add(s_mpf, s_mpf, t1_mpf);
        mpf_sub(s_mpf, s_mpf, t2_mpf);
    }
    STOP_CLOCK

    std::cout << "gmp mpf_t: " << time << "s" << std::endl;

    // mpq test is very slow, uncomment the section below to run
/* 
    START_CLOCK
    mpq_t s_mpq;
    mpq_init(s_mpq);

    mpq_t t1_mpq;
    mpq_init(t1_mpq);

    mpq_t t2_mpq;
    mpq_init(t2_mpq);


    for(size_t i = 0; i < iterations; i+=2) {
        mpq_set_ui(t1_mpq, 1, i+1);
        mpq_set_ui(t2_mpq, 1, i+2);
        mpq_add(s_mpq, s_mpq, t1_mpq);
        mpq_sub(s_mpq, s_mpq, t2_mpq);
    }
    STOP_CLOCK
    
    std::cout << "gmp mpq_t: " << time << "s" << std::endl; 
*/



    std::cout << "results:" << std::endl;
    std::cout.precision(17);
    std::cout << "double:\t" << s << std::endl;
    std::cout << "fbnum:\t" << s_fb.decimal_str() << std::endl;
    gmp_printf("mpf:\t%.*Ff\n", precision, s_mpf);
    // std::cout << "mpq:\t" << mpq_get_d(s_mpq) << std::endl;

    // "wikipedia" actual answer
    // https://en.wikipedia.org/wiki/Natural_logarithm_of_2
    std::cout << "actual:\t" << "0.693147180559945309417232121458" << std::endl;

    return 0;
}
