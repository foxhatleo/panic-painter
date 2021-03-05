#include "PPAssets.h"

using namespace utils;

json_t Assets::getJsonItem(const json_t &json, const char *key) {
    json_t item = json->get(key);
    CUAssertLog(item != nullptr, "Cannot find %s.", key);
    return item;
}

json_t Assets::getJson(const asset_t &assets, const char *key) {
    json_t r = assets->get<cugl::JsonValue>(key);
    CUAssertLog(r != nullptr, "Cannot find \"%s\" in JSON assets.", key);
    return r;
}
