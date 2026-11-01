#include <Geode/Geode.hpp>
#include <Geode/modify/CCScene.hpp>
#include <algorithm>
#include <cmath>

using namespace geode::prelude;

// GD AI floating desktop-style control panel.
// One overlay is attached to each root scene: menus, level screens and gameplay.
class GDAIFloatingPanel final : public CCLayer {
public:
    static GDAIFloatingPanel* create() {
        auto ret = new GDAIFloatingPanel();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override {
        if (!CCLayer::init()) return false;
        m_screen = CCDirector::sharedDirector()->getWinSize();
        m_bubblePos = CCPoint{36.f, m_screen.height * 0.52f};
        m_windowPos = CCPoint{
            std::max(8.f, (m_screen.width - WINDOW_W) / 2.f),
            std::max(8.f, (m_screen.height - WINDOW_H) / 2.f)
        };
        buildBubble();
        buildWindow();
        m_window->setVisible(false);
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchPriority(-500);
        return true;
    }

    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent*) override {
        auto p = this->convertToNodeSpace(touch->getLocation());
        m_startPoint = p;
        m_moved = false;
        m_action = TouchAction::None;

        if (inside(p, m_bubblePos.x - 27.f, m_bubblePos.y - 27.f, 54.f, 54.f)) {
            m_action = TouchAction::Bubble;
            m_dragOffset = CCPoint{m_bubblePos.x - p.x, m_bubblePos.y - p.y};
            return true;
        }

        if (m_window->isVisible() && inside(p, m_windowPos.x, m_windowPos.y, WINDOW_W, WINDOW_H)) {
            auto localX = p.x - m_windowPos.x;
            auto localY = p.y - m_windowPos.y;
            if (localY >= WINDOW_H - TITLE_H) {
                if (localX >= WINDOW_W - 35.f) m_action = TouchAction::Close;
                else if (localX >= WINDOW_W - 70.f) m_action = TouchAction::Minimize;
                else {
                    m_action = TouchAction::WindowDrag;
                    m_dragOffset = CCPoint{m_windowPos.x - p.x, m_windowPos.y - p.y};
                }
            } else if (inside(p, m_windowPos.x + 18.f, m_windowPos.y + 25.f, 132.f, 42.f)) {
                m_action = TouchAction::IA;
            } else if (inside(p, m_windowPos.x + 180.f, m_windowPos.y + 25.f, 132.f, 42.f)) {
                m_action = TouchAction::Play;
            } else {
                m_action = TouchAction::WindowBody;
            }
            return true;
        }
        return false;
    }

    void ccTouchMoved(CCTouch* touch, CCEvent*) override {
        auto p = this->convertToNodeSpace(touch->getLocation());
        auto delta = ccpSub(p, m_startPoint);
        if (std::fabs(delta.x) + std::fabs(delta.y) > 7.f) m_moved = true;
        if (m_action == TouchAction::Bubble) {
            m_bubblePos = clampBubble(ccpAdd(p, m_dragOffset));
            m_bubble->setPosition(m_bubblePos);
            m_bubbleLabel->setPosition(m_bubblePos);
        } else if (m_action == TouchAction::WindowDrag) {
            m_windowPos = clampWindow(ccpAdd(p, m_dragOffset));
            m_window->setPosition(m_windowPos);
        }
    }

    void ccTouchEnded(CCTouch*, CCEvent*) override {
        if (!m_moved) {
            switch (m_action) {
                case TouchAction::Bubble:
                    m_window->setVisible(!m_window->isVisible());
                    break;
                case TouchAction::Minimize:
                case TouchAction::Close:
                    m_window->setVisible(false);
                    break;
                case TouchAction::IA:
                    FLAlertLayer::create(
                        "GD AI - IA",
                        "Macro Generator ready.\n\nCreation progress: 0%. The analysis and macro-generation engine is not implemented yet, so progress will only advance when real generation steps are connected.",
                        "OK"
                    )->show();
                    break;
                case TouchAction::Play:
                    FLAlertLayer::create(
                        "GD AI - PLAY",
                        "PLAY control detected!\n\nAutomatic gameplay input is not implemented yet. This button is currently an interface test.",
                        "OK"
                    )->show();
                    break;
                default:
                    break;
            }
        }
        m_action = TouchAction::None;
    }

    void ccTouchCancelled(CCTouch*, CCEvent*) override {
        m_action = TouchAction::None;
    }

private:
    static constexpr float WINDOW_W = 330.f;
    static constexpr float WINDOW_H = 250.f;
    static constexpr float TITLE_H = 30.f;

    enum class TouchAction { None, Bubble, WindowDrag, WindowBody, IA, Play, Minimize, Close };

    CCSize m_screen;
    CCPoint m_bubblePos;
    CCPoint m_windowPos;
    CCPoint m_startPoint;
    CCPoint m_dragOffset;
    CCDrawNode* m_bubble = nullptr;
    CCLabelBMFont* m_bubbleLabel = nullptr;
    CCLayerColor* m_window = nullptr;
    CCLayerColor* m_progressFill = nullptr;
    CCLabelBMFont* m_progressLabel = nullptr;
    float m_macroProgress = 0.f;
    TouchAction m_action = TouchAction::None;
    bool m_moved = false;

    static bool inside(CCPoint p, float x, float y, float w, float h) {
        return p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h;
    }

    CCPoint clampBubble(CCPoint p) const {
        p.x = std::max(27.f, std::min(m_screen.width - 27.f, p.x));
        p.y = std::max(27.f, std::min(m_screen.height - 27.f, p.y));
        return p;
    }

    CCPoint clampWindow(CCPoint p) const {
        p.x = std::max(4.f, std::min(m_screen.width - WINDOW_W - 4.f, p.x));
        p.y = std::max(4.f, std::min(m_screen.height - WINDOW_H - 4.f, p.y));
        return p;
    }

    void buildBubble() {
        m_bubble = CCDrawNode::create();
        m_bubble->drawDot({0.f, 0.f}, 25.f, ccc4f(0.04f, 0.20f, 0.52f, 1.f));
        m_bubble->drawDot({0.f, 0.f}, 21.f, ccc4f(0.12f, 0.48f, 0.88f, 1.f));
        m_bubble->setPosition(m_bubblePos);
        this->addChild(m_bubble, 10000);

        m_bubbleLabel = CCLabelBMFont::create("GD", "bigFont.fnt");
        m_bubbleLabel->setScale(0.48f);
        m_bubbleLabel->setPosition(m_bubblePos);
        this->addChild(m_bubbleLabel, 10001);
    }

    void addRect(CCNode* parent, float x, float y, float w, float h, ccColor4B color, int z = 0) {
        auto rect = CCLayerColor::create(color, w, h);
        rect->setAnchorPoint({0.f, 0.f});
        rect->setPosition({x, y});
        parent->addChild(rect, z);
    }

    void addText(CCNode* parent, const char* value, float x, float y, float scale, ccColor3B color) {
        auto label = CCLabelBMFont::create(value, "bigFont.fnt");
        label->setScale(scale);
        label->setColor(color);
        label->setPosition({x, y});
        parent->addChild(label, 20);
    }

    void setMacroProgress(float progress) {
        m_macroProgress = std::max(0.f, std::min(100.f, progress));
        if (m_progressFill) {
            m_progressFill->setContentSize(CCSizeMake(std::max(1.f, 270.f * m_macroProgress / 100.f), 9.f));
            m_progressFill->setVisible(m_macroProgress > 0.f);
        }
        if (m_progressLabel) {
            auto value = std::to_string(static_cast<int>(m_macroProgress)) + "%";
            m_progressLabel->setString(value.c_str());
        }
    }

    void buildWindow() {
        m_window = CCLayerColor::create(ccc4(35, 55, 86, 255), WINDOW_W, WINDOW_H);
        m_window->setAnchorPoint({0.f, 0.f});
        m_window->setPosition(m_windowPos);
        this->addChild(m_window, 9999);

        // Windows XP-like silver frame and blue title bar.
        addRect(m_window, 3.f, 3.f, WINDOW_W - 6.f, WINDOW_H - 6.f, ccc4(192, 192, 192, 255));
        addRect(m_window, 7.f, WINDOW_H - TITLE_H - 1.f, WINDOW_W - 14.f, TITLE_H - 3.f, ccc4(0, 74, 150, 255));
        addRect(m_window, 8.f, WINDOW_H - 8.f, WINDOW_W - 16.f, 2.f, ccc4(255, 255, 255, 255));
        addText(m_window, "GD AI MOD", 94.f, WINDOW_H - 17.f, 0.55f, ccc3(255, 255, 255));

        // XP-style minimize and close controls.
        addRect(m_window, WINDOW_W - 65.f, WINDOW_H - 25.f, 25.f, 19.f, ccc4(190, 210, 230, 255));
        addText(m_window, "_", WINDOW_W - 52.f, WINDOW_H - 16.f, 0.42f, ccc3(0, 0, 80));
        addRect(m_window, WINDOW_W - 35.f, WINDOW_H - 25.f, 25.f, 19.f, ccc4(210, 70, 65, 255));
        addText(m_window, "X", WINDOW_W - 22.f, WINDOW_H - 16.f, 0.42f, ccc3(255, 255, 255));

        addText(m_window, "NEURAL MACRO GENERATOR", WINDOW_W / 2.f, 190.f, 0.42f, ccc3(30, 50, 80));
        addText(m_window, "Macro creation progress", WINDOW_W / 2.f, 164.f, 0.34f, ccc3(55, 55, 55));

        // Progress track starts empty. It only advances when real generation work is connected.
        addRect(m_window, 28.f, 137.f, 274.f, 13.f, ccc4(95, 105, 115, 255));
        addRect(m_window, 30.f, 139.f, 270.f, 9.f, ccc4(235, 235, 235, 255));
        m_progressFill = CCLayerColor::create(ccc4(35, 175, 75, 255), 1.f, 9.f);
        m_progressFill->setAnchorPoint({0.f, 0.f});
        m_progressFill->setPosition({30.f, 139.f});
        m_window->addChild(m_progressFill, 10);
        m_progressLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        m_progressLabel->setScale(0.48f);
        m_progressLabel->setColor(ccc3(20, 65, 30));
        m_progressLabel->setPosition({WINDOW_W / 2.f, 116.f});
        m_window->addChild(m_progressLabel, 20);
        addText(m_window, "Waiting for analysis engine", WINDOW_W / 2.f, 94.f, 0.30f, ccc3(75, 75, 75));
        addText(m_window, "Analyze  >  Generate  >  Test  >  Finish", WINDOW_W / 2.f, 75.f, 0.25f, ccc3(75, 75, 75));

        // Click targets are handled by this layer's touch delegate.
        addRect(m_window, 18.f, 25.f, 132.f, 42.f, ccc4(30, 105, 190, 255));
        addRect(m_window, 20.f, 27.f, 128.f, 38.f, ccc4(220, 232, 245, 255));
        addText(m_window, "IA", 84.f, 46.f, 0.58f, ccc3(0, 45, 110));
        addRect(m_window, 180.f, 25.f, 132.f, 42.f, ccc4(30, 105, 190, 255));
        addRect(m_window, 182.f, 27.f, 128.f, 38.f, ccc4(220, 232, 245, 255));
        addText(m_window, "PLAY", 246.f, 46.f, 0.48f, ccc3(0, 45, 110));
        addText(m_window, "v0.3.0 - Neural Macro", WINDOW_W / 2.f, 10.f, 0.28f, ccc3(70, 70, 70));
    }
};

class $modify(GDAIScene, CCScene) {
    bool init() {
        if (!CCScene::init()) return false;
        auto overlay = GDAIFloatingPanel::create();
        if (overlay) this->addChild(overlay, 1000000);
        return true;
    }
};
