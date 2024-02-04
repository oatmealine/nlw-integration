#ifndef NLWRATING_H
#define NLWRATING_H

#include <matjson.hpp>

enum NLWRatingType {
	Regular,
	Platformer,
	Pending,
};

class NLWRating {
	public:
		explicit NLWRating(matjson::Value levelData);

		NLWRating() = default;

		int sheetIndex;
		NLWRatingType type;
		std::string tier;
		std::string name;
		std::string creator;
		std::string skillset;
		float enjoyment;
		std::string description;

		std::string format();
};


#endif //NLWRATING_H