#pragma once
#include "framework.h"
#include "Endpoints.h"
#define DEVICE_AUTH_FILE "device.json"
#define PROD
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

class Fortnite {

public: static nlohmann::json GetProfileAthena(std::string token, std::string accountId) {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	struct curl_slist* headers = NULL;
	curl = curl_easy_init();

	if (curl) {
		headers = curl_slist_append(headers, std::string("Authorization: bearer " + token).c_str());
		headers = curl_slist_append(headers, std::string("Content-Type: application/json").c_str());
	//	auto url = endpoints.anyMCP(std::string(info["account_id"]), "ClaimLoginReward", "campaign");
		auto url = RequestHelper::getProfileAthena(accountId);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		nlohmann::json j = nlohmann::json::object();
		auto jsonned = j.dump();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonned.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		return nlohmann::json::parse(readBuffer);
		curl_easy_cleanup(curl);
	}
}

public: static nlohmann::json GetShop(std::string token) {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	struct curl_slist* headers = NULL;
	if (curl) {
		auto header = std::string("Authorization: bearer " + token);
		headers = curl_slist_append(headers, header.c_str());

		curl_easy_setopt(curl, CURLOPT_URL, RequestHelper::getCatalog().c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "http request failed: %s\n",
				curl_easy_strerror(res));

		curl_easy_cleanup(curl);
#ifndef PROD
		std::cout << readBuffer << std::endl;
#endif
		return nlohmann::json::parse(readBuffer);
	}
}

public: static nlohmann::json Login() {
	if (std::filesystem::exists(DEVICE_AUTH_FILE)) {
		std::ifstream file(DEVICE_AUTH_FILE);
		nlohmann::json deviceauth;
		file >> deviceauth;

		CURL* curl;
		CURLcode res;
		std::string readBuffer;

		curl = curl_easy_init();
		struct curl_slist* headers = NULL;
		if (curl) {
#ifndef PROD
			std::cout << "curl !" << std::endl;
#endif
			auto header = std::string("Authorization: " + RequestHelper::IOSHeader());
#ifndef PROD
			std::cout << header << std::endl;
#endif
			headers = curl_slist_append(headers, header.c_str());
			headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
#ifndef PROD
			std::cout << "finished headers" << std::endl;
#endif
#ifndef PROD
			std::cout << deviceauth << std::endl;
#endif
			std::string body = RequestHelper::BuildDeviceAuth(deviceauth);
#ifndef PROD
			std::cout << body << std::endl;
#endif
			curl_easy_setopt(curl, CURLOPT_URL, RequestHelper::OAUTH().c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1L);

			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				fprintf(stderr, "http request failed: %s\n",
					curl_easy_strerror(res));

			curl_easy_cleanup(curl);
#ifndef PROD
			std::cout << readBuffer << std::endl;
#endif
			return nlohmann::json::parse(readBuffer);
		}

	}
	else {
		printf("You can generate an authentification code here : https://www.epicgames.com/id/api/redirect?clientId=3446cd72694c4a4485d81b77adbb2141&responseType=code \n");
		printf("Authentification Code :");
		std::string str;
		while (true) {
			getline(std::cin, str);
			if (str.size() != 32) {
				printf("Authentification code should be 32 chars long.\n");
				printf("Authentification Code :");
			}
			else {
				break;
			}
			Sleep(1000 / 60);
		}
		CURL* curl;
		CURLcode res;
		std::string readBuffer;
		curl = curl_easy_init();
		struct curl_slist* headers = NULL;
		if (curl) {
			auto header = std::string("Authorization: " + RequestHelper::IOSHeader());
			auto body = RequestHelper::BuildAuthentificationCodeBody(str);
			headers = curl_slist_append(headers, header.c_str());
			headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

			curl_easy_setopt(curl, CURLOPT_URL, RequestHelper::OAUTH().c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1L);

			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				fprintf(stderr, "http request failed: %s\n",
					curl_easy_strerror(res));

			curl_easy_cleanup(curl);
		}
		return nlohmann::json::parse(readBuffer);
	}
}

public: static nlohmann::json DeviceAuth(std::string token, std::string account_id) {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	curl = curl_easy_init();
	struct curl_slist* headers = NULL;
	if (curl) {
		auto header = std::string("Authorization: Bearer " + token);

		headers = curl_slist_append(headers, header.c_str());
		headers = curl_slist_append(headers, std::string("Content-Type: application/json").c_str());


		curl_easy_setopt(curl, CURLOPT_URL, RequestHelper::DeviceAuth(account_id).c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, nlohmann::json::object().dump().c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "http request failed: %s\n",
				curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}
#ifndef PROD
	std::cout << readBuffer << std::endl;
#endif
	return nlohmann::json::parse(readBuffer);
}

};