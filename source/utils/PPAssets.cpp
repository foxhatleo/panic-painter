#include "PPAssets.h"

using namespace utils;

json Assets::getJsonItem(const json &json, const char* key) {
    json item = json->get(key);
    CUAssertLog(item != nullptr, "Cannot find %s.", key);
    return item;
}

json Assets::getJson(const asset &assets, const char* key) {
    json r = assets->get<cugl::JsonValue>("global");
    CUAssertLog(r != nullptr, "Cannot find \"%s\" in JSON assets.", key);
    return r;
}
