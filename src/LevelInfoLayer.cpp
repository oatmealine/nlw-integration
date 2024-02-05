#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <string>

#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/loader/Log.hpp"
#include "ListManager.h"
#include "NLWInfoPopupLayer.h"
#include "NLWRating.h"
#include "ccTypes.h"

using namespace geode::prelude;

class $modify(NLWInfoLayer, LevelInfoLayer) {
	NLWRating m_rating;
	CCMenu* m_menu = nullptr;

	void updateDifficultyFace() {
		if (this->m_fields->m_menu != nullptr) return;

		auto rating = ListManager::getRating(m_level);
		if (!rating.has_value()) {
			log::info("demon rating for {} not found", m_level->m_levelName);
			return;
		}
		log::info("found demon rating for {} ({})", rating->name, rating->tier);
		this->m_fields->m_rating = rating.value();

		auto demon = m_difficultySprite;
		if (demon == nullptr) {
			log::warn("demon icon not found?? ok");
			return;
		}

		auto menu = CCMenu::create();
		menu->setPosition({ demon->getPositionX(), demon->getPositionY() + 35 });
		menu->setID("tier_menu"_spr);
		this->m_fields->m_menu = menu;
		this->addChild(menu);
		auto label = CCLabelBMFont::create(rating->format().c_str(), "bigFont.fnt");
		label->setScale(0.5f);
		label->setColor(rating->type == NLWRatingType::Pending ? cocos2d::ccColor3B(255, 255, 255) : ListManager::getTierColor(rating->tier));
		auto tier = CCMenuItemSpriteExtra::create(
			label, this, menu_selector(NLWInfoLayer::openNLWInfoPane)
		);
		menu->addChild(tier);
	}

	void updateLabelValues() {
		LevelInfoLayer::updateLabelValues();
		if (!ListManager::fetchedRatings || ListManager::erroredRatings) return;
		if (this->m_level->m_stars != 10) return;
		if (this->m_level->m_demonDifficulty != 6) return; // extreme demob
        
		updateDifficultyFace();
	}

	void openNLWInfoPane(CCObject *sender) {
		auto layer = NLWInfoPopupLayer::create(this->m_level, &this->m_fields->m_rating);
		layer->show();
	}
};