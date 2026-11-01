#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <algorithm>
#include <vector>

using namespace geode::prelude;

namespace gdai {
    enum class Action { Press, Release };
    struct InputEvent { unsigned int frame; Action action; };

    class Macro {
    public:
        void clear() { m_events.clear(); }
        void add(unsigned int frame, Action action) {
            m_events.push_back({frame, action});
            std::stable_sort(m_events.begin(), m_events.end(),
                [](InputEvent const& a, InputEvent const& b) { return a.frame < b.frame; });
        }
        size_t size() const { return m_events.size(); }
    private:
        std::vector<InputEvent> m_events;
    };

    static Macro s_macro;

    static void showAIStatus() {
        FLAlertLayer::create(
            "GD AI - IA",
            "Interface IA ativada!\\n\\n"
            "Esta versao adiciona os controles e prepara a estrutura de eventos por frame.\\n\\n"
            "O scanner dos objetos e o calculo automatico da rota ainda serao implementados.",
            "OK"
        )->show();
    }

    static void createMacroTest() {
        // Test fixture only; these frames are NOT a solution for the selected level.
        s_macro.clear();
        s_macro.add(30, Action::Press);
        s_macro.add(42, Action::Release);
        s_macro.add(78, Action::Press);
        s_macro.add(91, Action::Release);

        FLAlertLayer::create(
            "GD AI - PLAY",
            fmt::format("Gerador de eventos funcionando!\\n\\n{} eventos de teste criados.\\nA execucao automatica ainda nao esta ativada.", s_macro.size()),
            "OK"
        )->show();
        log::info("GD AI: generated {} sample macro events", s_macro.size());
    }
}

class $modify(GDAILevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        auto size = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition(CCPointZero);

        auto ia = CCMenuItemLabel::create(
            CCLabelBMFont::create("IA", "bigFont.fnt"),
            this, menu_selector(GDAILevelInfoLayer::onGDAI)
        );
        ia->setScale(0.65f);
        ia->setPosition({size.width - 46.f, size.height - 45.f});
        menu->addChild(ia);

        auto play = CCMenuItemLabel::create(
            CCLabelBMFont::create("PLAY", "bigFont.fnt"),
            this, menu_selector(GDAILevelInfoLayer::onGDPlay)
        );
        play->setScale(0.52f);
        play->setPosition({size.width - 49.f, size.height - 105.f});
        menu->addChild(play);

        this->addChild(menu, 1000);
        return true;
    }

    void onGDAI(CCObject*) { gdai::showAIStatus(); }
    void onGDPlay(CCObject*) { gdai::createMacroTest(); }
};
