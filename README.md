# MUL2213
MUL2213 algorithm using avx vectors 









INPUT: x = [x0, . . . , x3], y = [y0, . . . , y3] ∈ [−2^57, 2^57 − 1] × [0, 2^56 − 1]
OUTPUT: z ∈ [−2^57, 2^57 − 1] × [0, 2^56 − 1]^3










I limited the data type to 32 bit instead however I might implement a new version that takes 64 bit numbers






















where z ≡ x · y (mod t4 − 24)














https://eprint.iacr.org/2014/852.pdf

