#ifndef NLWRATING_H
#define NLWRATING_H

#include <matjson.hpp>
#include <optional>

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
		int id;
		std::string name;
		std::string creator;
		std::string skillset;
		float enjoyment;
		std::string description;
		std::optional<std::string> broken;

		std::string format();
};


#endif //NLWRATING_H