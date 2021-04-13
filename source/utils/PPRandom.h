#ifndef PANICPAINTER_PPRANDOM_H
#define PANICPAINTER_PPRANDOM_H

#include "PPHeader.h"

namespace utils {
    /**
     * Random is a nifty utility that helps you get random numbers, strings, etc.
     * @author Dragonglass Studios
     */
    class Random {
    private:
        static inline Random *_instance;

        Random();

    public:
        /**
         * Get a random integer within a specific range (inclusive of both
         * ends).
         * @param upperBound The upper bound.
         * @param lowerBound The lower bound. By default, 0.
         * @return The random integer generated.
         */
        int getInt(int upperBound, int lowerBound = 0);

        /**
         * Get a random boolean.
         * @return The random boolean generated.
         */
        bool getBool();

        /**
         * Get a random float within a specific range (inclusive of both ends).
         * @param upperBound The upper bound.
         * @param lowerBound The lower bound. By default, 0.
         * @return The random float generated.
         */
        float getFloat(float upperBound, float lowerBound = 0.0f);

        /**
         * Get a random string with a specific length and predefined characters.
         * @param len The length.
         * @param chars The characters to build the string from. If a character
         * is repeated, the chance of it being used increases.
         * @return The random string generated.
         */
        string getStr(int len, string chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");

        static Random *getInstance() {
            if (!_instance) _instance = new Random();
            return _instance;
        }
    };
}

#endif //PANICPAINTER_PPRANDOM_H
