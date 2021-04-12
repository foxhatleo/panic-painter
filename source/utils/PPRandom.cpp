#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"
#pragma ide diagnostic ignored "cert-msc50-cpp"

#include "PPRandom.h"

Random::Random() {
    srand((unsigned) time(nullptr));
}

int Random::getInt(int upperBound, int lowerBound) {
    CUAssertLog(upperBound >= lowerBound,
                "Lower bound cannot be lower than upper bound.");
    if (upperBound == lowerBound) return upperBound;
    return lowerBound + (rand() % (upperBound - lowerBound + 1));
}

bool Random::getBool() {
    return (bool) (rand() % 2);
}

float Random::getFloat(float upperBound, float lowerBound) {
    return lowerBound +
           static_cast<float>(rand()) /
           (RAND_MAX / (upperBound - lowerBound));
}

string Random::getStr(int len, string chars) {
    int clen = (int) (chars.length());
    CUAssertLog(clen > 0, "Characters cannot be an empty string.");
    string result;
    while (len--) result += chars[getInt(clen - 1)];
    return result;
}

#pragma clang diagnostic pop
