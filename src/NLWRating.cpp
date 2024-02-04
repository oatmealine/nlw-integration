#include "NLWRating.h"

using namespace geode::prelude;

NLWRating::NLWRating(matjson::Value levelData) {
	this->sheetIndex = levelData["sheetIndex"].as_int();
	auto type = levelData["type"].as_string();
	if (type == "platformer") {
		this->type = NLWRatingType::Platformer;
	} else if (type == "pending") {
		this->type = NLWRatingType::Pending;
	} else {
		this->type = NLWRatingType::Regular;
	}
	this->tier = levelData["tier"].as_string();
	this->name = levelData["name"].is_string() ? levelData["name"].as_string() : "?";
	this->creator = levelData["creator"].is_string() ? levelData["creator"].as_string() : "?";
	this->skillset = levelData["skillset"].is_string() ? levelData["skillset"].as_string() : "";
	try {
		this->enjoyment = levelData["enjoyment"].is_string() ? std::stoi(levelData["enjoyment"].as_string()) : -1.0f;
	} catch (const std::invalid_argument& ia) {
	  this->enjoyment = -1.0f;
  }
	this->description = levelData["description"].is_string() ? levelData["description"].as_string() : "";
}

std::string NLWRating::format() {
	if (this->type == NLWRatingType::Regular || this->type == NLWRatingType::Platformer) return this->tier;
	if (this->type == NLWRatingType::Pending) return "Pending";
	return "";
}