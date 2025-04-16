// largely based on https://github.com/geode-sdk/geode/blob/241ddc317ac84d58ddadf3d26bb76fe159975ea8/loader/src/ui/nodes/MDTextArea.cpp

#include <Geode/Geode.hpp>
#include <Geode/binding/ProfilePage.hpp>
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/string.hpp>

#include "PlainTextArea.hpp"
#include <cocos2d.h>

using namespace geode::prelude;
using namespace cocos2d;

static constexpr float g_fontScale = 1.f;

TextRenderer::Font g_mdFont = [](int style) -> TextRenderer::Label {
    return CCLabelBMFont::create("", "chatFont.fnt");
};

class PlainContentLayer : public CCContentLayer {
protected:
    CCMenu* m_content;

public:
    static PlainContentLayer* create(CCMenu* content, float width, float height) {
        auto ret = new PlainContentLayer();
        if (ret && ret->initWithColor({0,255,0,0}, width, height)) {
            ret->m_content = content;
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void setPosition(CCPoint const& pos) override {
        // cringe CCContentLayer expect its children to
        CCLayerColor::setPosition(pos);

        // all be TableViewCells
        // so that's why based MDContentLayer expects itself
        // to have a CCMenu :-)
        if (m_content) {
            for (auto child : CCArrayExt<CCNode*>(m_content->getChildren())) {
                float y = this->getPositionY() + child->getPositionY();
                child->setVisible(
                    !((m_content->getContentSize().height < y) ||
                      (y < -child->getContentSize().height))
                );
            }
        }
    }
};

bool PlainTextArea::init(std::string const& str, CCSize const& requestedSize) {
    if (!CCLayer::init()) return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint({.5f, .5f});

    auto visible = Director::getInstance()->getVisibleSize();
    auto maxSz  = CCSize{ visible.width * .9f, visible.height * .8f };
    m_size = {
        std::min(requestedSize.width,  maxSz.width),
        std::min(requestedSize.height, maxSz.height)
    };

    m_text = str;
    this->setContentSize(m_size);

    m_renderer = TextRenderer::create();
    CC_SAFE_RETAIN(m_renderer);

    m_bgSprite = CCScale9Sprite::create("square02b_001.png", CCRect{0,0,80,80});
    m_bgSprite->setColor({0,0,0});
    m_bgSprite->setOpacity(75);
    m_bgSprite->setContentSize(m_size);
    m_bgSprite->setAnchorPoint({.5f, .5f});
    m_bgSprite->setPosition({ m_size.width/2, m_size.height/2 });
    this->addChild(m_bgSprite);

    m_scrollLayer = ScrollLayer::create({0,0, m_size.width, m_size.height}, true);
    m_scrollLayer->setTouchEnabled(true);
    this->addChild(m_scrollLayer);

    m_content = CCMenu::create();
    m_content->setZOrder(2);
    m_scrollLayer->m_contentLayer->addChild(m_content);

    this->updateLabel();
    return true;
}

PlainTextArea::~PlainTextArea() {
    CC_SAFE_RELEASE(m_renderer);
}

class BreakLine : public CCNode {
protected:
    void draw() override {
        // some nodes sometimes set the blend func to
        // something else without resetting it back
        ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ccDrawSolidRect({0,0}, this->getContentSize(), {1.f,1.f,1.f,.2f});
        CCNode::draw();
    }
public:
    static BreakLine* create(float width) {
        auto ret = new BreakLine;
        if (ret && ret->init()) {
            ret->autorelease();
            ret->setContentSize({width, 1.f});
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

void PlainTextArea::updateLabel() {
    m_renderer->begin(m_content, CCPointZero, m_size);

    m_renderer->pushFont(g_mdFont);
    m_renderer->pushScale(g_fontScale);
    m_renderer->pushVerticalAlign(TextAlignment::End);
    m_renderer->pushHorizontalAlign(TextAlignment::Begin);

    m_renderer->renderString(m_text);
    m_renderer->end();

    auto csz = m_content->getContentSize();
    if (csz.height > m_size.height) {
        m_scrollLayer->m_contentLayer->setContentSize(csz + CCSize{0.f, 12.5f});
        m_content->setPositionY(10.f);
    } else {
        m_scrollLayer->m_contentLayer->setContentSize(csz);
        m_content->setPositionY(-2.5f);
    }

    m_scrollLayer->moveToTop();
}

CCScrollLayerExt* PlainTextArea::getScrollLayer() const {
    return m_scrollLayer;
}

void PlainTextArea::setString(char const* text) {
    m_text = text;
    this->updateLabel();
}

char const* PlainTextArea::getString() {
    return m_text.c_str();
}

PlainTextArea* PlainTextArea::create(std::string const& str, CCSize const& size) {
    auto ret = new PlainTextArea;
    if (ret && ret->init(str, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}