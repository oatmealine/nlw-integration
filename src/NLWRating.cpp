#include <Geode/Geode.hpp>
#include "NLWRating.h"

using namespace geode::prelude;

NLWRating::NLWRating(matjson::Value levelData) {
	this->sheetIndex = levelData["sheetIndex"].as_int();
	auto type = levelData["type"].as_string();
	if (type == "platformer") {
		this->type = NLWRatingType::Platformer;
	} else {
		this->type = NLWRatingType::Regular;
	}
	this->tier = levelData["tier"].as_string();
	this->id = levelData["id"].is_number() ? levelData["id"].as_int() : -1;
	this->name = levelData["name"].is_string() ? levelData["name"].as_string() : "?";
	this->creator = levelData["creator"].is_string() ? levelData["creator"].as_string() : "?";
	this->skillset = levelData["skillset"].is_string() ? levelData["skillset"].as_string() : "";
	this->description = levelData["description"].is_string() ? levelData["description"].as_string() : "";
	if (levelData["broken"].is_string()) {
		this->broken = levelData["broken"].as_string();
	}
}

std::string NLWRating::format() {
	return this->tier;
}