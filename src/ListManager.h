#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include "Geode/binding/GJGameLevel.hpp"
#include "NLWRating.h"

using namespace geode::prelude;

class ListManager {
	static std::vector<NLWRating> ratings;
	static void parseResponse(matjson::Value data);

	public:
		static bool fetchedRatings;
		static bool erroredRatings;
		static void init();
		static std::optional<NLWRating> getRating(GJGameLevel*);
};

#endif // LISTMANAGER_H