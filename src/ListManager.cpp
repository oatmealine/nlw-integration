#include "ListManager.h"
#include <Geode/Geode.hpp>
#include "Geode/loader/Log.hpp"
#include "Geode/loader/Mod.hpp"
#include "NLWRating.h"
#include "ccTypes.h"
#include <Geode/utils/web.hpp>
#include <cctype>
#include <matjson.hpp>
#include <string>

using namespace geode::prelude;

const std::string NLW_API_URL = "https://nlw.oat.zone/ids?type=all";

bool ListManager::fetchedRatings;
bool ListManager::erroredRatings;
std::vector<NLWRating> ListManager::ratings;
EventListener<web::WebTask> ListManager::fetchListListener;

void ListManager::parseResponse(matjson::Value data) {
	if (!data.is_array()) {
		log::error("got unexpected data: {}", data.dump());
		ListManager::throwError("expected root element to be an array! check logs");
	}

	auto arr = data.as_array();

	for (auto& level : arr) {
		auto rating = NLWRating(level);
		ListManager::ratings.push_back(rating);
	}
}

// https://www.winehq.org/pipermail/wine-devel/2008-September/069387.html
std::string getWineVersion() {
	#ifdef GEODE_IS_WINDOWS
		static const char * (CDECL *pwine_get_version)(void);
		HMODULE hntdll = GetModuleHandle("ntdll.dll");
		if (!hntdll) return "";
		pwine_get_version = (const char *(__cdecl *)(void))GetProcAddress(hntdll, "wine_get_version");
		if (pwine_get_version) return fmt::format(" (Wine/{})", pwine_get_version());
		return "";
	#else
		return "";
	#endif
}

std::string getPlatformName() {
	return GEODE_PLATFORM_NAME + getWineVersion();
}

std::string getUserAgent() {
	return fmt::format("{}/{}; GeometryDash/{} (GeodeSDK/{}); {}",
		Mod::get()->getID(),
		Mod::get()->getVersion().toNonVString(true),
		GEODE_STR(GEODE_GD_VERSION),
		Loader::get()->getVersion().toNonVString(true),
		getPlatformName()
	);
}

void ListManager::init() {
	ListManager::fetchListListener.bind([] (web::WebTask::Event* e) {
    if (web::WebResponse* res = e->getValue()) {
			auto json = res->json();
			if (res->ok() && json.isOk()) {
				ListManager::fetchedRatings = true;
				ListManager::erroredRatings = false;
				ListManager::parseResponse(json.unwrap());
				log::info("successfully fetched {} levels", ListManager::ratings.size());
			} else {
				ListManager::throwError(fmt::format("{}: {}", res->code(), res->string().unwrapOr("No data returned")));
			}
    } else if (web::WebProgress* progress = e->getProgress()) {
			// The request is still in progress...
    } else if (e->isCancelled()) {
			log::info("request cancelled?");
    }
	});

	if (!ListManager::fetchedRatings) {
		web::WebRequest req = web::WebRequest();

		req.userAgent(getUserAgent());
		auto task = req.get(NLW_API_URL);

		ListManager::fetchListListener.setFilter(task);
	}
}

void ListManager::throwError(std::string message) {
	ListManager::fetchedRatings = true;
	ListManager::erroredRatings = true;
	std::string errorStr = "\n\n<cr>Could not fetch IDS data.</c>\nThe API could be down, or this is could be a temporary network failure. Restart your game to try again!";
	FLAlertLayer::create("Error", message + errorStr, "OK")->show();
	log::error("error fetching ratings: {}", message);
}

std::string lowercase(std::string data) {
	std::transform(data.begin(), data.end(), data.begin(),
		[](unsigned char c){ return std::tolower(c); });
	return data;
}

std::optional<NLWRating> ListManager::getRating(GJGameLevel* level) {
	auto id = level->m_levelID.value();

	for (auto rating : ListManager::ratings) {
		if (id == rating.id) {
			return rating;
		}
	}

	return std::nullopt;
}

cocos2d::ccColor3B ListManager::getTierColor(std::string tier) {
	// regular tiers
	if (tier == "Fuck")        return cocos2d::ccColor3B(0,   0,   0  );
	if (tier == "Beginner")    return cocos2d::ccColor3B(58,  134, 228);
	if (tier == "Easy")        return cocos2d::ccColor3B(0,   255, 254);
	if (tier == "Medium")      return cocos2d::ccColor3B(0,   255, 55 );
	if (tier == "Hard")        return cocos2d::ccColor3B(255, 255, 63 );
	if (tier == "Very Hard")   return cocos2d::ccColor3B(255, 153, 43 );
	if (tier == "Insane")      return cocos2d::ccColor3B(255, 3,   28 );
	if (tier == "Extreme")     return cocos2d::ccColor3B(255, 12,  251);
	if (tier == "Remorseless") return cocos2d::ccColor3B(157, 10,  250);
	if (tier == "Relentless")  return cocos2d::ccColor3B(178, 135, 232);
	if (tier == "Terrifying")  return cocos2d::ccColor3B(204, 204, 204);

	// pending
	if (tier == "Low End")            return cocos2d::ccColor3B(0,   192, 237);
	if (tier == "Low-Mid Range")      return cocos2d::ccColor3B(0,   255, 135);
	if (tier == "Mid Range")          return cocos2d::ccColor3B(255, 204, 52 );
	if (tier == "Mid-High Range")     return cocos2d::ccColor3B(255, 5,   128);
	if (tier == "High End")           return cocos2d::ccColor3B(167, 93,  242);
	if (tier == "Unknown")            return cocos2d::ccColor3B(255, 255, 255);
	if (tier == "New Rates")          return cocos2d::ccColor3B(255, 255, 255);
	if (tier == "Potential Extremes") return cocos2d::ccColor3B(235, 235, 235);

	return cocos2d::ccColor3B(255, 255, 255);
}

cocos2d::ccColor3B ListManager::getEnjoymentColor(float enjoyment) {
	if (enjoyment > 90.f) return cocos2d::ccColor3B(0,   255, 254);
	if (enjoyment > 75.f) return cocos2d::ccColor3B(179, 215, 170);
	if (enjoyment > 60.f) return cocos2d::ccColor3B(255, 229, 159);
	if (enjoyment > 40.f) return cocos2d::ccColor3B(254, 203, 160);
	if (enjoyment >  0.f) return cocos2d::ccColor3B(240, 153, 154);
	return cocos2d::ccColor3B(255, 255, 255);
}

std::string ListManager::getRatingLink(NLWRating rating) {
	int sheetID = 1309758655;
	if (rating.type == NLWRatingType::Platformer) sheetID = 506524049;

	auto rowID = std::to_string(rating.sheetIndex + 1);
	std::string range = rowID + ":" + rowID;

	return "https://docs.google.com/spreadsheets/d/15ehtAIpCR8s04qIb8zij9sTpUdGJbmAE_LDcfVA3tcU/edit#gid=" + std::to_string(sheetID) + "&range=" + range;
}

GJSearchObject* ListManager::getSearchObject(std::string tier) {
	std::stringstream download;
	bool first = true;
	for (auto rating : ListManager::ratings) {
		if (rating.tier != tier) continue;

		if (!first) {
			download << ",";
		}
		download << rating.id;
		first = false;
	}

	log::info("download str {}", download.str());
	
	download << "&gameVersion=22";
	GJSearchObject* searchObj = GJSearchObject::create(SearchType::Type19, download.str());
	return searchObj;
}