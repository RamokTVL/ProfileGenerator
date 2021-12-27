#pragma once
#include "framework.h"

std::string currentISO8601TimeUTC() {
	auto now = std::chrono::system_clock::now();
	auto itt = std::chrono::system_clock::to_time_t(now);
	std::ostringstream ss;
	ss << std::put_time(gmtime(&itt), "%FT%TZ");
	return ss.str();
}

void writeShop(std::string token) {
	auto shopresponse = Fortnite::GetShop(token);
	std::ofstream shop("shop.json");
	shop << std::setw(4) << shopresponse << std::endl;
	printf("Just written the shop in shop.json");
	return;
}

void writeProfile(std::string token, std::string accountId, std::string displayName) {
	auto athenaresponse = Fortnite::GetProfileAthena(token, accountId);
	if (Util::exists(athenaresponse, "errorMessage")) {
		std::cout << "an error occured : " << athenaresponse["errorMessage"] << std::endl;
		return;
	}

	if (Util::exists(athenaresponse, "profileChanges")) {
		athenaresponse = athenaresponse["profileChanges"][0]["profile"];
	}

	auto filename = "profile_athena_" + displayName + ".json";
	std::ofstream file(filename);
	file << std::setw(4) << athenaresponse << std::endl;
	printf("Just written your profile_athena in %s", filename.c_str());
}

void writeAllItems() {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;




	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://benbot.app/api/v1/cosmetics/br?lang=en");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			return;
		}
		curl_easy_cleanup(curl);
		nlohmann::json items = nlohmann::json::parse(readBuffer);

		nlohmann::json profile = nlohmann::json::object();
		profile["_id"] = "ramok";
		profile["updated"] = currentISO8601TimeUTC();
		profile["created"] = currentISO8601TimeUTC();
		profile["wipeNumber"] = 5;
		profile["rvn"] = -1;
		profile["accountId"] = "ramok";
		profile["commandRevision"] = 0;
		profile["profileId"] = "athena";
		nlohmann::json json2 = nlohmann::json::parse("{\"sandbox_loadout\":{\"templateId\":\"CosmeticLocker:cosmeticlocker_athena\",\"attributes\":{\"locker_slots_data\":{\"slots\":{\"Pickaxe\":{\"items\":[null],\"activeVariants\":[]},\"Dance\":{\"items\":[null,null,null,null,null,null]},\"Glider\":{\"items\":[null]},\"Character\":{\"items\":[\"AthenaCharacter:cid_028_athena_commando_f\"],\"activeVariants\":[]},\"Backpack\":{\"items\":[null],\"activeVariants\":[]},\"ItemWrap\":{\"items\":[null,null,null,null,null,null,null],\"activeVariants\":[null,null,null,null,null,null,null]},\"LoadingScreen\":{\"items\":[null],\"activeVariants\":[null]},\"MusicPack\":{\"items\":[null],\"activeVariants\":[null]},\"SkyDiveContrail\":{\"items\":[null],\"activeVariants\":[null]}}},\"use_count\":0,\"banner_icon_template\":\"\",\"locker_name\":\"\",\"banner_color_template\":\"\",\"item_seen\":false,\"favorite\":false},\"quantity\":1},\"loadout_1\":{\"templateId\":\"CosmeticLocker:cosmeticlocker_athena\",\"attributes\":{\"locker_slots_data\":{\"slots\":{\"Pickaxe\":{\"items\":[null],\"activeVariants\":[]},\"Dance\":{\"items\":[null,null,null,null,null,null]},\"Glider\":{\"items\":[null]},\"Character\":{\"items\":[\"AthenaCharacter:cid_028_athena_commando_f\"],\"activeVariants\":[]},\"Backpack\":{\"items\":[null],\"activeVariants\":[]},\"ItemWrap\":{\"items\":[null,null,null,null,null,null,null],\"activeVariants\":[null,null,null,null,null,null,null]},\"LoadingScreen\":{\"items\":[null],\"activeVariants\":[null]},\"MusicPack\":{\"items\":[null],\"activeVariants\":[null]},\"SkyDiveContrail\":{\"items\":[null],\"activeVariants\":[null]}}},\"use_count\":0,\"banner_icon_template\":\"\",\"locker_name\":\"\",\"banner_color_template\":\"\",\"item_seen\":false,\"favorite\":false},\"quantity\":1}}");
		profile["items"] = json2;
		for (auto item : items) {
			auto backendType = std::string(item["backendType"]);
			auto id = std::string(item["id"]);
			std::string fullId = backendType + ":" + id;
			nlohmann::json variants = nlohmann::json::array();
			if (!item["variants"].is_null()) {
				for (auto variant : item["variants"]) {
					std::vector<std::string> types = std::vector<std::string>();
					for (auto option : variant["options"]) {
						types.push_back(std::string(option["tag"]));
					}

					nlohmann::json topush;
					topush["channel"] = variant["channel"];
					if (types.size() == 0) {
						topush["active"] = "";
					}
					else {
						topush["active"] = types[0];
					}

					topush["owned"] = types;

					variants.push_back(topush);
				}
			}

			profile["items"][fullId]["templateId"] = fullId;
			profile["items"][fullId]["attributes"]["max_level_bonus"] = 0;
			profile["items"][fullId]["attributes"]["level"] = 1;
			profile["items"][fullId]["attributes"]["item_seen"] = true;
			profile["items"][fullId]["attributes"]["xp"] = 0;
			profile["items"][fullId]["attributes"]["rnd_sel_cnt"] = 0;
			profile["items"][fullId]["attributes"]["variants"] = variants;
			profile["items"][fullId]["attributes"]["favorite"] = false;
		}

		std::ofstream file("profile_athena_allitems.json");
		file << std::setw(4) << profile << std::endl;
		file.close();
		printf("Just written leaked items in profile_athena_allitems.json");
		return;
	}
}

void writeLeaks() {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://benbot.app/api/v1/newCosmetics?lang=en");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			return;
		}
		curl_easy_cleanup(curl);
		nlohmann::json leaks = nlohmann::json::parse(readBuffer);
		printf("Writing leaks for version : %s\n", std::string(leaks["currentVersion"]).c_str());
		nlohmann::json profile = nlohmann::json::object();
		profile["_id"] = "ramok";
		profile["updated"] = currentISO8601TimeUTC();
		profile["created"] = currentISO8601TimeUTC();
		profile["wipeNumber"] = 5;
		profile["rvn"] = -1;
		profile["accountId"] = "ramok";
		profile["commandRevision"] = 0;
		profile["profileId"] = "athena";
		nlohmann::json json2 = nlohmann::json::parse("{\"sandbox_loadout\":{\"templateId\":\"CosmeticLocker:cosmeticlocker_athena\",\"attributes\":{\"locker_slots_data\":{\"slots\":{\"Pickaxe\":{\"items\":[null],\"activeVariants\":[]},\"Dance\":{\"items\":[null,null,null,null,null,null]},\"Glider\":{\"items\":[null]},\"Character\":{\"items\":[\"AthenaCharacter:cid_028_athena_commando_f\"],\"activeVariants\":[]},\"Backpack\":{\"items\":[null],\"activeVariants\":[]},\"ItemWrap\":{\"items\":[null,null,null,null,null,null,null],\"activeVariants\":[null,null,null,null,null,null,null]},\"LoadingScreen\":{\"items\":[null],\"activeVariants\":[null]},\"MusicPack\":{\"items\":[null],\"activeVariants\":[null]},\"SkyDiveContrail\":{\"items\":[null],\"activeVariants\":[null]}}},\"use_count\":0,\"banner_icon_template\":\"\",\"locker_name\":\"\",\"banner_color_template\":\"\",\"item_seen\":false,\"favorite\":false},\"quantity\":1},\"loadout_1\":{\"templateId\":\"CosmeticLocker:cosmeticlocker_athena\",\"attributes\":{\"locker_slots_data\":{\"slots\":{\"Pickaxe\":{\"items\":[null],\"activeVariants\":[]},\"Dance\":{\"items\":[null,null,null,null,null,null]},\"Glider\":{\"items\":[null]},\"Character\":{\"items\":[\"AthenaCharacter:cid_028_athena_commando_f\"],\"activeVariants\":[]},\"Backpack\":{\"items\":[null],\"activeVariants\":[]},\"ItemWrap\":{\"items\":[null,null,null,null,null,null,null],\"activeVariants\":[null,null,null,null,null,null,null]},\"LoadingScreen\":{\"items\":[null],\"activeVariants\":[null]},\"MusicPack\":{\"items\":[null],\"activeVariants\":[null]},\"SkyDiveContrail\":{\"items\":[null],\"activeVariants\":[null]}}},\"use_count\":0,\"banner_icon_template\":\"\",\"locker_name\":\"\",\"banner_color_template\":\"\",\"item_seen\":false,\"favorite\":false},\"quantity\":1}}");
		profile["items"] = json2;
		for (auto item : leaks["items"]) {
			auto backendType = std::string(item["backendType"]);
			auto id = std::string(item["id"]);
			std::string fullId = backendType + ":" + id;
			nlohmann::json variants = nlohmann::json::array();
			if (!item["variants"].is_null()) {
				for (auto variant : item["variants"]) {
					std::vector<std::string> types = std::vector<std::string>();
					for (auto option : variant["options"]) {
						types.push_back(std::string(option["tag"]));
					}

					nlohmann::json topush;
					topush["channel"] = variant["channel"];
					if (types.size() == 0) {
						topush["active"] = "";
					}
					else {
						topush["active"] = types[0];
					}

					topush["owned"] = types;

					variants.push_back(topush);
				}
			}

			profile["items"][fullId]["templateId"] = fullId;
			profile["items"][fullId]["attributes"]["max_level_bonus"] = 0;
			profile["items"][fullId]["attributes"]["level"] = 1;
			profile["items"][fullId]["attributes"]["item_seen"] = true;
			profile["items"][fullId]["attributes"]["xp"] = 0;
			profile["items"][fullId]["attributes"]["rnd_sel_cnt"] = 0;
			profile["items"][fullId]["attributes"]["variants"] = variants;
			profile["items"][fullId]["attributes"]["favorite"] = false;
		}

		std::ofstream file("profile_athena_leakedonly.json");
		file << std::setw(4) << profile << std::endl;
		file.close();
		printf("Just written leaked items in profile_athena_leakedonly.json");
		return;
 	}
}

int main() {
	printf("Welcome !\n");
	curl_global_init(CURL_GLOBAL_ALL);
	std::string username;
	auto oauthresponse = Fortnite::Login();
	if (Util::exists(oauthresponse, "errorMessage")) {
		std::cout << "Error : " << oauthresponse["errorMessage"] << std::endl;
		return 1;
	}
	username = std::string(oauthresponse["displayName"]);
#ifndef PROD
	std::cout << "Access Token : " << oauthresponse["access_token"] << std::endl;
#endif
	if (!std::filesystem::exists(DEVICE_AUTH_FILE)) {
		auto deviceauthresponse = Fortnite::DeviceAuth(oauthresponse["access_token"], oauthresponse["account_id"]);
		if (Util::exists(deviceauthresponse, "errorMessage")) {
			std::cout << "Error : " << oauthresponse["errorMessage"] << std::endl;
			return 1;
		}
		std::ofstream devicewriter(DEVICE_AUTH_FILE);
		devicewriter << std::setw(4) << deviceauthresponse << std::endl;
		std::cout << "Created deviceauth" << std::endl;
	}
	else {
		std::cout << "If you wish to recreate a new deviceauth, delete the file " << DEVICE_AUTH_FILE << " and restart." << std::endl;
	}
	printf("\n\nConnected as %s", username.c_str());
	printf("\n[1] Get the shop");
	printf("\n[2] Generate your profile athena");
	printf("\n[3] Generate a profile with every items");
	printf("\n[4] Generate a profile with only new items\n-> ");


		std::string choice;
		getline(std::cin, choice);
		switch (std::stoi(choice)) {
		case 1:
			writeShop(oauthresponse["access_token"]);
			break;
		case 2:
			writeProfile(oauthresponse["access_token"], oauthresponse["account_id"], username);
			break;
		case 3:
			writeAllItems();
			break;
		case 4:
			writeLeaks();
			break;
		default:
			std::cout << "Invalid !";
			break;
		}
	
	return 0;
}