#include "Geode/binding/LevelInfoLayer.hpp"
#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <string>

#include <Geode/Geode.hpp>
#include "Geode/Enums.hpp"
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/binding/GJDifficultySprite.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/menu_nodes/CCMenu.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "Geode/loader/Log.hpp"
#include "ListManager.h"
#include "NLWInfoPopupLayer.h"
#include "NLWRating.h"
#include "ccTypes.h"

using namespace geode::prelude;

class $modify(NLWInfoLayer, LevelInfoLayer) {
	struct Fields {
		NLWRating m_rating;
		CCMenu* m_menu = nullptr;
		GJDifficultySprite* m_sprite = nullptr;
		bool m_addedTier = false;
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
		if (this->m_level->m_demonDifficulty != 6) return; // extreme demob

		if (m_fields->m_menu == nullptr) return;

		auto rating = ListManager::getRating(m_level);
		if (!rating.has_value()) {
			log::info("demon rating for {} not found", m_level->m_levelName);
			return;
		}
		log::info("found demon rating for {} ({})", rating->name, rating->tier);
		m_fields->m_rating = rating.value();

		if (m_difficultySprite == nullptr) {
			log::warn("demon icon not found?? ok");
			return;
		}

		if (Mod::get()->getSettingValue<bool>("use-demon-face")) {
			// a little uglier than i'd like but it's ok
			// :normal:

			this->m_fields->m_menu->removeAllChildren(); // might result in a slow creeping memory leak..

			this->m_fields->m_menu->setPosition(m_difficultySprite->getPosition());

			auto sprite = GJDifficultySprite::create(m_level->m_demonDifficulty + 4, static_cast<GJDifficultyName>(1));
			//sprite->updateFeatureStateFromLevel(m_level);
			//sprite->updateFeatureState(m_level->m_featured);

			auto button = CCMenuItemSpriteExtra::create(
				sprite, this, menu_selector(NLWInfoLayer::openNLWInfoPane)
			);
			this->m_fields->m_menu->addChild(button);

			m_difficultySprite->setVisible(false);
		} else {
			if (this->m_fields->m_addedTier) return;
			this->m_fields->m_menu->setPosition({ m_difficultySprite->getPositionX(), m_difficultySprite->getPositionY() + 35 });

			auto label = CCLabelBMFont::create(rating->format().c_str(), "bigFont.fnt");
			label->setScale(0.5f);
			label->setColor(rating->type == NLWRatingType::Pending ? cocos2d::ccColor3B(255, 255, 255) : ListManager::getTierColor(rating->tier));
			auto tier = CCMenuItemSpriteExtra::create(
				label, this, menu_selector(NLWInfoLayer::openNLWInfoPane)
			);
			this->m_fields->m_menu->addChild(tier);
		}

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