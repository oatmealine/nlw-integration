#include "NLWInfoPopupLayer.h"
#include "Geode/binding/GJGameLevel.hpp"
#include "Geode/loader/Log.hpp"
#include "Geode/ui/MDTextArea.hpp"
#include "Geode/ui/TextArea.hpp"
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/utils/web.hpp"
#include "ListManager.h"
#include "NLWRating.h"
#include "ccTypes.h"
#include <algorithm>
#include <fmt/format.h>

using namespace geode::prelude;

bool NLWInfoPopupLayer::setup(GJGameLevel* level, NLWRating* rating) {
	this->m_level = level;
	this->m_rating = rating;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto name = CCLabelBMFont::create(rating->name.c_str(), "bigFont.fnt");
	name->setPosition(ccp(90, 230));
	name->setScale(0.75);
	name->setAnchorPoint({ 0.f, 0.5f });
	m_mainLayer->addChild(name);

	auto creator = CCLabelBMFont::create(("by " + rating->creator).c_str(), "bigFont.fnt");
	creator->setPosition(ccp(90, 204));
	creator->setScale(0.75);
	creator->setAnchorPoint({ 0.f, 0.5f });
	creator->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	m_mainLayer->addChild(creator);

	auto tier = CCLabelBMFont::create((rating->type == NLWRatingType::Pending ? rating->tier : (rating->tier + " Tier")).c_str(), "bigFont.fnt");
	tier->setPosition(ccp(385, 230));
	tier->setScale(0.75);
	tier->setColor(ListManager::getTierColor(rating->tier));
	m_mainLayer->addChild(tier);

	auto skillset = CCLabelBMFont::create(rating->skillset.c_str(), "bigFont.fnt");
	skillset->setPosition(ccp(385, 204));
	skillset->setScale(0.5);
	m_mainLayer->addChild(skillset);

	auto const descSize = CCSize { 380.f, 70.f };
	auto const descPos = winSize / 2 - ccp(0.f, 10.f);

	/*cocos2d::extension::CCScale9Sprite* descBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
	descBg->setContentSize(descSize);
	descBg->setPosition(descPos);
	descBg->setAnchorPoint({0.f, 0.f});
	descBg->setColor({130,64,33});
	m_mainLayer->addChild(descBg);

	auto description = ScrollLayer::create(descSize);
	description->setPosition(descPos);
	description->setTouchEnabled(true);
	
	auto text = geode::SimpleTextArea::create(rating->description, "chatFont.fnt", 1.f, descSize.width - 20.f);
	// help
	text->setPosition(descSize.width / 2, descSize.height - text->getHeight() - 10);
	text->setAnchorPoint({ 0.5f, 1.f });

	description->m_contentLayer->addChild(text);
	description->m_contentLayer->setContentSize({ descSize.width, std::max(text->getHeight(), descSize.height) });
	m_mainLayer->addChild(description);*/

	auto description = MDTextArea::create(rating->description, descSize);
	description->setPosition(descPos);
	description->setTouchEnabled(true);
	m_mainLayer->addChild(description);

	auto openBtnSpr = ButtonSprite::create("Sheet", "goldFont.fnt", "GJ_button_01.png", .8f);
	openBtnSpr->setScale(.8f);

	auto openBtn = CCMenuItemSpriteExtra::create(
		openBtnSpr, this, menu_selector(NLWInfoPopupLayer::onOpen)
	);
	openBtn->setPosition(.0f, -m_size.height / 2 + 25.f);
	m_buttonMenu->addChild(openBtn);

	auto const enjPos = ccp(name->getPositionX() + 10, name->getPositionY());

	auto enjSprite = CCSprite::createWithSpriteFrameName("NLW_button_white.png"_spr);
	enjSprite->setColor(ListManager::getEnjoymentColor(rating->enjoyment));
	enjSprite->setPosition(enjPos);
	enjSprite->setScale(0.5);
	m_mainLayer->addChild(enjSprite);

	//auto enj = CCLabelBMFont::create(std::to_string(rating->enjoyment).c_str(), "bigFont.fnt");
	auto enj = CCLabelBMFont::create(fmt::format("{:.0f}", rating->enjoyment).c_str(), "bigFont.fnt");
	enj->setPosition(enjPos);
	enj->setScale(0.415);
	m_mainLayer->addChild(enj);

	if (rating->enjoyment == -1.f) {
		enjSprite->setVisible(false);
		enj->setVisible(false);
	} else {
		name->setPosition(name->getPosition() + ccp(25, 0));
	}

	return true;
}

void NLWInfoPopupLayer::onOpen(CCObject* sender) {
	if (this->m_rating == nullptr) {
		log::error("rating is nullptr??");
		return;
	}
	auto url = ListManager::getRatingLink(*this->m_rating);
	web::openLinkInBrowser(url);
}

NLWInfoPopupLayer* NLWInfoPopupLayer::create(GJGameLevel *level, NLWRating *rating) {
	auto ret = new NLWInfoPopupLayer();
	if (ret && ret->init(420.f, 190.f, level, rating)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}