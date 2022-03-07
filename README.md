# fractionalBignum
bignum library for representing numbers between 0 and 1 with higher than 64 bit depth
TODO:
 - [ ] basic I/O
    - [x] printing in hex
    - [ ] printing in base 10
 - [ ] basic arithmetic
    - [x] implement addition operator
    - [ ] implement subtraction operator
    - [x] implement multiplication operator
    - [ ] implement comparision operators
 - [ ] additional arithmetic
    - [ ] implement div(int64, int64) -> fractionalBignum
    - [ ] implement karatsuba/fft based multiplication for very large bit counts
    - [ ] implement `avx2` vecorization for speedup
 - [ ] misc
    - [ ] setup proper library packaging
    - [ ] speed comparison to GNU GMP
