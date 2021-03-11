#include "PPAssets.h"

json_t Assets::Json::get(const asset_t &assets, const char *key) {
    json_t r = assets->get<JsonValue>(key);
    CUAssertLog(r != nullptr, "Cannot find \"%s\" in JSON assets.", key);
    return r;
}

json_t Assets::Json::getItem(const json_t &json, const char *key) {
    CUAssertLog(json != nullptr, "JSON is null.");
    json_t item = json->get(key);
    CUAssertLog(item != nullptr, "Cannot find %s.", key);
    return item;
}

json_t Assets::Json::getOptional(const json_t &json, const char *key) {
    return json == nullptr ? nullptr : json->get(key);
}

int Assets::Json::asInt(const json_t &json) {
    CUAssertLog(json != nullptr && json->isNumber(), "Not a number.");
    return json->asInt();
}

int Assets::Json::asInt(const json_t &json, const int defaultValue) {
    if (json == nullptr) return defaultValue;
    return json->asInt(defaultValue);
}

float Assets::Json::asFloat(const json_t &json) {
    CUAssertLog(json != nullptr && json->isNumber(), "Not a number.");
    return json->asFloat();
}

float Assets::Json::asFloat(const json_t &json, float defaultValue) {
    if (json == nullptr) return defaultValue;
    return json->asFloat(defaultValue);
}

int Assets::Json::getInt(const json_t &json, const char *key) {
    return asInt(getItem(json, key));
}

int Assets::Json::getInt(const json_t &json,
                         const char *key,
                         int defaultValue) {
    return asInt(getOptional(json, key), defaultValue);
}

float Assets::Json::getFloat(const json_t &json, const char *key) {
    return asFloat(getItem(json, key));
}

float Assets::Json::getFloat(const json_t &json,
                           const char *key,
                           float defaultValue) {
    return asFloat(getOptional(json, key), defaultValue);
}

void Assets::Json::_assertArray(const json_t &json) {
    CUAssertLog(json != nullptr && json->isArray(), "Not a valid array.");
}

size_t Assets::Json::getLength(const json_t &json) {
    _assertArray(json);
    return json->size();
}

vec<json_t> Assets::Json::asVec(const json_t &json) {
    _assertArray(json);
    vec<json_t> r;
    for (size_t i = 0, j = json->size(); i < j; i++) {
        r.push_back(json->get(i));
    }
    return r;
}

vec<int> Assets::Json::asIntVec(const json_t &json) {
    vec<json_t> v = asVec(json);
    vec<int> r;
    for (auto &i : v) {
        r.push_back(asInt(i));
    }
    return r;
}
