#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include "Geode/binding/GJGameLevel.hpp"
#include "NLWRating.h"
#include "ccTypes.h"

using namespace geode::prelude;

class ListManager {
	static std::vector<NLWRating> ratings;
	static void parseResponse(matjson::Value data);

	public:
		static bool fetchedRatings;
		static bool erroredRatings;
		static void init();
		static void throwError(std::string message);
		static std::optional<NLWRating> getRating(GJGameLevel*);
		static cocos2d::ccColor3B getTierColor(std::string tier);
		static cocos2d::ccColor3B getEnjoymentColor(float enjoyment);
		static std::string getRatingLink(NLWRating rating);
};

#endif // LISTMANAGER_H