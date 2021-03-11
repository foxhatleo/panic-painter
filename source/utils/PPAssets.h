#ifndef PANICPAINTER_PPASSETS_H
#define PANICPAINTER_PPASSETS_H

#include "PPHeader.h"

namespace utils {
    /**
     * These are some utility functions for dealing with assets.
     * @author Dragonglass Studios
     */
    class Assets {
    public:
        /**
         * Functions for dealing with JSON.
         *
         * The problem with the CUGL JSON interface is that **they fail
         * silently**. This is not ideal, so these helper functions are designed
         * not to silence any error.
         */
        class Json {
        private:
            static void _assertArray(const json_t &json);

        public:
            /** Get a vector of integers. Json object must be int array. */
            static vec<int> asIntVec(const json_t &json);

            /** Get a vector of elements. Json object must be array. */
            static vec<json_t> asVec(const json_t &json);

            /** Get the length of an array. */
            static size_t getLength(const json_t &json);

            /** Convert JSON object to integer. If not number, fail. */
            static int asInt(const json_t &json);

            /** Convert JSON object to integer. If not number, default value. */
            static int asInt(const json_t &json, int defaultValue);

            /** Convert JSON object to float. If not number, fail. */
            static float asFloat(const json_t &json);

            /** Convert JSON object to float. If not number, default value. */
            static float asFloat(const json_t &json, float defaultValue);

            /** Get an object in a JSON dictionary. */
            static json_t getItem(const json_t &json, const char *key);

            /** Get an object in a JSON dictionary, nullptr if not exists. */
            static json_t getOptional(const json_t &json, const char *key);

            /** Get an integer in a JSON dictionary. */
            static int getInt(const json_t &json, const char *key);

            /** Get an integer in a JSON dictionary. */
            static int getInt(const json_t &json,
                              const char *key,
                              int defaultValue);

            /** Get a float in a JSON dictionary. */
            static float getFloat(const json_t &json, const char *key);

            /** Get a float in a JSON dictionary. */
            static float getFloat(const json_t &json,
                                  const char *key,
                                  float defaultValue);

            /** Pull JSON file from asset manager. */
            static json_t get(const asset_t &assets, const char *key);
        };
    };
}

#endif //PANICPAINTER_PPASSETS_H
