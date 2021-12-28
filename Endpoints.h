#pragma once
#include "framework.h"
#define BASE_ACCOUNT_PROD std::string("https://account-public-service-prod.ol.epicgames.com")
#define BASE_FORTNITE_PROD std::string("https://fortnite-public-service-prod11.ol.epicgames.com")
#define IOSGameClient std::string("basic MzQ0NmNkNzI2OTRjNGE0NDg1ZDgxYjc3YWRiYjIxNDE6OTIwOWQ0YTVlMjVhNDU3ZmI5YjA3NDg5ZDMxM2I0MWE=")
class RequestHelper {
public :
	static std::string OAUTH() {
		return BASE_ACCOUNT_PROD + "/account/api/oauth/token";
	}

	static std::string IOSHeader() {
		return IOSGameClient;
	}

	static std::string BuildDeviceAuth(nlohmann::json deviceauth) {
		return std::string("grant_type=device_auth&account_id=" + std::string(deviceauth["accountId"]) + "&device_id=" + std::string(deviceauth["deviceId"]) + "&secret=" + std::string(deviceauth["secret"]));
	}

	static std::string DeviceAuth(std::string account_id) {
		return BASE_ACCOUNT_PROD + "/account/api/public/account/" + account_id + "/deviceAuth";
	}

	static std::string BuildAuthentificationCodeBody(std::string code) {
		return "grant_type=authorization_code&code=" + code;
	}

	static std::string getProfileAthena(std::string accountId) {
		return BASE_FORTNITE_PROD + "/fortnite/api/game/v2/profile/" + accountId + "/client/QueryProfile?profileId=athena&rvn=-1";
	}	
	
	static std::string setAffliateName(std::string accountId) {
		return BASE_FORTNITE_PROD + "/fortnite/api/game/v2/profile/" + accountId + "/client/SetAffiliateName?profileId=common_core&rvn=-1";
	}

	static std::string getCatalog() {
		return BASE_FORTNITE_PROD + "/fortnite/api/storefront/v2/catalog";
	}
 };