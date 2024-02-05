#include "NLWInfoPopupLayer.h"
#include "Geode/binding/GJGameLevel.hpp"
#include "Geode/loader/Log.hpp"
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/utils/web.hpp"
#include "ListManager.h"
#include "NLWRating.h"
#include "PlainTextArea.hpp"
#include "ccTypes.h"
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
	name->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	name->limitLabelWidth(170.f, 0.75f, 0.1f);
	m_mainLayer->addChild(name);

	auto creator = CCLabelBMFont::create(("by " + rating->creator).c_str(), "bigFont.fnt");
	creator->setPosition(ccp(90, 204));
	creator->setScale(0.75);
	creator->setAnchorPoint({ 0.f, 0.5f });
	creator->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	creator->limitLabelWidth(195.f, 0.75f, 0.1f);
	m_mainLayer->addChild(creator);

	auto tier = CCLabelBMFont::create((rating->tier + " Tier").c_str(), "bigFont.fnt");
	tier->setPosition(ccp(385, 230));
	tier->setScale(0.75);
	tier->setColor(ListManager::getTierColor(rating->tier));
	tier->limitLabelWidth(180.f, 0.75f, 0.1f);
	m_mainLayer->addChild(tier);

	auto skillset = CCLabelBMFont::create(rating->skillset.c_str(), "bigFont.fnt");
	skillset->setPosition(ccp(385, 204));
	skillset->setScale(0.5);
	skillset->limitLabelWidth(180.f, 0.5f, 0.1f);
	m_mainLayer->addChild(skillset);

	auto const descSize = CCSize { 380.f, 70.f };
	auto const descPos = winSize / 2 - ccp(0.f, 10.f);

	auto description = PlainTextArea::create(rating->description, descSize);
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

	std::string brokenStr = rating->broken.value_or("unknown");
	auto broken = CCLabelBMFont::create(fmt::format("Broken in 2.2: {}", brokenStr).c_str(), "goldFont.fnt");
	broken->setPosition(90, 90);
	broken->setAnchorPoint({ 0.f, 0.5f });
	broken->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	broken->limitLabelWidth(150.f, 0.5f, 0.1f);
	m_mainLayer->addChild(broken);

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