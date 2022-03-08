#include "fractionalBignum.hpp"
#include <iostream>
#include <cmath>
#include <limits>

int main(int argc, char const *argv[])
{
    // const size_t k = 4;
    // u_int64_t v1[k];
    // std::fill_n(v1, k, 0xFFFFFFFFFFFFFFFF);
    // u_int64_t v2[k] = {0};
    // v2[0] = 1UL << 63;
    // fractionalBignum<k> f1(v1);
    // fractionalBignum<k> f2(v2);
    // std::cout << f1 << std::endl;
    // std::cout << f2 << std::endl;

    // std::cout << "------" << std::endl;

    // std::cout << f1 * f2 << std::endl;

    double d1 = pow(2,-65);
    double d2 = 0.5;

    fractionalBignum<4> f1(d1);
    fractionalBignum<4> f2(d2);

    std::cout << "\t" << f1.decimal_str() << std::endl;
    std::cout << "+\t" << f2.decimal_str() << std::endl;
    std::cout << "---------------------------------------------" << std::endl;

    std::cout << "as fractionalBignum" << std::endl;
    std::cout << (f1+f2).decimal_str() << std::endl;


    std::cout << "as regular double" << std::endl;
    std::cout.precision(std::numeric_limits<double>::digits10);
    std::cout << d1+d2 << std::endl;

    



    return 0;
}
