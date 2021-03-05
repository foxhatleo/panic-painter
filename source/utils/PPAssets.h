#ifndef PANICPAINTER_PPASSETS_H
#define PANICPAINTER_PPASSETS_H

#include <cugl/cugl.h>
#include "PPTypeDefs.h"

namespace utils {
    /**
     * These are some utility functions for dealing with assets.
     * @author Dragonglass Studios
     */
    class Assets {
    public:
        /**
         * Load an object in a JSON object. Does not fail silently.
         * @param json JSON object.
         * @param key Key name.
         * @return The JSON object.
         */
        static json getJsonItem(const json &json, const char* key);

        /**
         * Pull JSON file from asset manager.
         * @param assets Asset manager.
         * @param key Key name.
         * @return The JSON object.
         */
        static json getJson(const asset &assets, const char* key);
    };
}

#endif //PANICPAINTER_PPASSETS_H
