#ifndef NLWRATING_H
#define NLWRATING_H

#include <matjson.hpp>
#include <optional>

class NLWRating {
	public:
		explicit NLWRating(matjson::Value levelData);

		NLWRating() = default;

		int sheetIndex;
		std::string tier;
		int id;
		std::string name;
		std::string creator;
		std::string skillset;
		std::string description;
		std::optional<std::string> broken;

		std::string format();
};


#endif //NLWRATING_H