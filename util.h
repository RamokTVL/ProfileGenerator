#pragma once
#include "framework.h"
class Util {
public: static bool exists(nlohmann::json json, std::string key) {
	return json.find(key) != json.end();
 }
};