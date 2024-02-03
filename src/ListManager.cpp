#include "ListManager.h"
#include "NLWRating.h"
#include <Geode/utils/web.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

const std::string NLW_API_URL = "https://nlw.oat.zone/list?type=all";

bool ListManager::fetchedRatings;
bool ListManager::erroredRatings;
std::vector<NLWRating> ListManager::ratings;

void ListManager::parseResponse(matjson::Value data) {
	auto arr = data.as_array();

	for (auto& level : arr) {
		auto rating = NLWRating(level);
		ListManager::ratings.push_back(rating);
	}
}

void ListManager::init() {
	if (!ListManager::fetchedRatings) {
		web::AsyncWebRequest()
			.fetch(NLW_API_URL)
			.json()
			.then([](matjson::Value const& val) {
				ListManager::fetchedRatings = true;
				ListManager::erroredRatings = false;
				ListManager::parseResponse(val);
			})
			.expect([](std::string const& error) {
				ListManager::fetchedRatings = true;
				ListManager::erroredRatings = true;
				std::string errorStr = "\n\n<cr>Could not fetch NLW data.</c>\nThe API could be down, or this is could be a temporary network failure. Restart your game to try again!";
				FLAlertLayer::create("Error", error + errorStr, "OK")->show();
			});
	}
}

std::optional<NLWRating> ListManager::getRating(GJGameLevel* level) {
	auto name = level->m_levelName;
	auto creator = level->m_creatorName;

	std::vector<NLWRating> matches;

	for (auto rating : ListManager::ratings) {
		if (rating.name.find(name) != std::string::npos) {
			matches.push_back(rating);
		}
	}

	if (matches.empty()) return {};
	if (matches.size() == 1) return matches[0];

	for (auto rating : matches) {
		if (rating.creator.find(creator) != std::string::npos)
			return rating;
	}

	return matches[0];
}