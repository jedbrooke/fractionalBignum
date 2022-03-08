#include <cstdlib>
#include <string>
namespace util {
    enum bases {
        TWO=2,
        TEN=10,
        HEX=16,
        b64=64
    };

    void octoword_slli(u_int64_t v[2], int n);

    std::string base10add(const char* a, const char* b, size_t D);

}
