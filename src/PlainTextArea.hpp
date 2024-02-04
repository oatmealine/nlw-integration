// largely based on https://github.com/geode-sdk/geode/blob/241ddc317ac84d58ddadf3d26bb76fe159975ea8/loader/include/Geode/ui/MDTextArea.hpp

#pragma once

#include <Geode/binding/FLAlertLayerProtocol.hpp>

using namespace geode::prelude;

struct MDParser;
class CCScrollLayerExt;

class PlainTextArea :
		public cocos2d::CCLayer,
		public cocos2d::CCLabelProtocol,
		public FLAlertLayerProtocol {
protected:
	std::string m_text;
	cocos2d::CCSize m_size;
	cocos2d::extension::CCScale9Sprite* m_bgSprite = nullptr;
	cocos2d::CCMenu* m_content = nullptr;
	CCScrollLayerExt* m_scrollLayer = nullptr;
	TextRenderer* m_renderer = nullptr;

	bool init(std::string const& str, cocos2d::CCSize const& size);

	virtual ~PlainTextArea();

public:
	static PlainTextArea* create(std::string const& str, cocos2d::CCSize const& size);

	void updateLabel();

	void setString(char const* text) override;
	char const* getString() override;

	CCScrollLayerExt* getScrollLayer() const;
};