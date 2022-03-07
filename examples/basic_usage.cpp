#include "fractionalBignum.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    const size_t k = 18;
    u_int64_t v1[k];
    std::fill_n(v1, k, 0xFFFFFFFFFFFFFFFF);
    u_int64_t v2[k] = {0};
    v2[0] = 1UL << 63;
    fractionalBignum<k> f1(v1);
    fractionalBignum<k> f2(v2);
    std::cout << f1 << std::endl;
    std::cout << f2 << std::endl;

    std::cout << "------" << std::endl;

    std::cout << f1 * f2 << std::endl;

    return 0;
}
