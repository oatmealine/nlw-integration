#include "NLWInfoPopupLayer.h"
#include "Geode/binding/GJGameLevel.hpp"
#include "Geode/loader/Log.hpp"
#include "Geode/ui/TextArea.hpp"
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/utils/web.hpp"
#include "ListManager.h"
#include "NLWRating.h"
#include "ccTypes.h"
#include <algorithm>

using namespace geode::prelude;

bool NLWInfoPopupLayer::setup(GJGameLevel* level, NLWRating* rating) {
	this->m_level = level;
	this->m_rating = rating;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto name = CCLabelBMFont::create(rating->name.c_str(), "bigFont.fnt");
	name->setPosition(ccp(90, 250));
	name->setScale(0.85);
	name->setAnchorPoint({ 0.f, 0.5f });
	m_mainLayer->addChild(name);

	auto creator = CCLabelBMFont::create(("by " + rating->creator).c_str(), "bigFont.fnt");
	creator->setPosition(ccp(90, 224));
	creator->setScale(0.85);
	creator->setAnchorPoint({ 0.f, 0.5f });
	creator->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	m_mainLayer->addChild(creator);

	auto tier = CCLabelBMFont::create((rating->type == NLWRatingType::Pending ? rating->tier : (rating->tier + " Tier")).c_str(), "bigFont.fnt");
	tier->setPosition(ccp(385, 250));
	tier->setScale(0.85);
	tier->setColor(ListManager::getTierColor(rating->tier));
	m_mainLayer->addChild(tier);

	auto skillset = CCLabelBMFont::create(rating->skillset.c_str(), "bigFont.fnt");
	skillset->setPosition(ccp(385, 224));
	skillset->setScale(0.65f);
	m_mainLayer->addChild(skillset);

	auto const layerSize = CCSize { 390.f, 120.f };
	auto const layerPos = (winSize - layerSize) / 2 - ccp(0.f, 30.f);

	auto layerBG = CCLayerColor::create({ 0, 0, 0, 75 });
	layerBG->setContentSize(layerSize);
	layerBG->setPosition(layerPos);
	m_mainLayer->addChild(layerBG);

	auto description = ScrollLayer::create(layerSize);
	description->setPosition(layerPos);
	description->setTouchEnabled(true);
	
	auto text = geode::SimpleTextArea::create(rating->description, "chatFont.fnt", 1.f, layerSize.width - 10.f);
	// help
	text->setPosition(layerSize.width / 2, layerSize.height - text->getHeight() - 3);
	text->setAnchorPoint({ 0.5f, 1.f });

	description->m_contentLayer->addChild(text);
	description->m_contentLayer->setContentSize({ layerSize.width, std::max(text->getHeight(), layerSize.height) });
	m_mainLayer->addChild(description);

	auto openBtnSpr = ButtonSprite::create("Open", "goldFont.fnt", "GJ_button_01.png", .8f);
	openBtnSpr->setScale(.8f);

	auto openBtn = CCMenuItemSpriteExtra::create(
		openBtnSpr, this, menu_selector(NLWInfoPopupLayer::onOpen)
	);
	openBtn->setPosition(.0f, -m_size.height / 2);
	m_buttonMenu->addChild(openBtn);

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
	if (ret && ret->init(420.f, 240.f, level, rating)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}