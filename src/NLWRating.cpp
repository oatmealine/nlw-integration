#include <Geode/Geode.hpp>
#include "NLWRating.h"

using namespace geode::prelude;

NLWRating::NLWRating(matjson::Value levelData) {
	this->sheetIndex = levelData["sheetIndex"].asInt().unwrapOrDefault();
	auto type = levelData["type"].asString().unwrapOrDefault();
	if (type == "platformer") {
		this->type = NLWRatingType::Platformer;
	} else {
		this->type = NLWRatingType::Regular;
	}
	this->tier = levelData["tier"].asString().unwrapOrDefault();
	this->id = levelData["id"].asInt().unwrapOr(-1);
	this->name = levelData["name"].asString().unwrapOr("?");
	this->creator = levelData["creator"].asString().unwrapOr("?");
	this->skillset = levelData["skillset"].asString().unwrapOrDefault();
	this->description = levelData["description"].asString().unwrapOrDefault();
	if (levelData["broken"].isString()) {
		this->broken = levelData["broken"].asString().unwrap();
	}
}

std::string NLWRating::format() {
	return this->tier;
}