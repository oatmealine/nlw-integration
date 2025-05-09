#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <string>

#include <Geode/Geode.hpp>
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/loader/Log.hpp"
#include "ListManager.h"
#include "NLWInfoPopupLayer.h"
#include "NLWRating.h"

using namespace geode::prelude;

class $modify(NLWInfoLayer, LevelInfoLayer) {
	struct Fields {
		NLWRating        m_rating;
		CCMenu*          m_menu = nullptr;
		GJDifficultySprite* m_sprite = nullptr;
		bool             m_addedTier = false;
	};

	bool init(GJGameLevel* p0, bool p1) {
		LevelInfoLayer::init(p0, p1);

		auto menu = CCMenu::create();
		menu->setID("tier_menu"_spr);

		this->addChild(menu);
		m_fields->m_menu = menu;

		updateDifficultyFace();

		return true;
	}

	void updateDifficultyFace() {
		if (!ListManager::fetchedRatings || ListManager::erroredRatings) return;
		if (this->m_level->m_stars != 10) return;
		if (this->m_level->m_demonDifficulty != 5) return; // INSANE demob

		if (m_fields->m_menu == nullptr) return;

		auto ratingOpt = ListManager::getRating(m_level);
		if (!ratingOpt.has_value()) {
			log::info("demon rating for {} not found", m_level->m_levelName);
			return;
		}
		m_fields->m_rating = ratingOpt.value();

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
		label->setColor(ListManager::getTierColor(rating->tier));
		auto tier = CCMenuItemSpriteExtra::create(
			label, this, menu_selector(NLWInfoLayer::openNLWInfoPane)
		);
		menu->addChild(tier);

		this->m_fields->m_addedTier = true;
	}

	void updateLabelValues() {
		LevelInfoLayer::updateLabelValues();
		updateDifficultyFace();
	}

	void openNLWInfoPane(CCObject *sender) {
		auto layer = NLWInfoPopupLayer::create(this->m_level, &m_fields->m_rating);
		layer->show();
	}
};