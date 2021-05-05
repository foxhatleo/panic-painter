#include "PPFeedback.h"

#define MAX_COMBO_TIME 3

ptr<Feedback> Feedback::alloc(const Rect &screen, const asset_t &assets) {
    auto result =
        make_shared<Feedback>();
    if (result->init())
        result->_setup(screen, assets);
    else
        return nullptr;
    return result;
}

void Feedback::_setup(const Rect &screen, const asset_t &assets) {
    _assets = assets;
    _screen = screen;
    _combo = 0;
    _timeSinceCombo = 0;
}

void Feedback::add(Vec2 at, FeedbackType type) {
    ptr<Texture> t;
    switch (type) {
        case SUCCESS: {
            t = _assets->get<Texture>("feedbackyes");
            break;
        }
        case FAILURE: {
            t = _assets->get<Texture>("feedbackno");
            break;
        }
        default: {
            CUAssertLog(false, "Unknown feedback type.");
        }
    }
    if (type == SUCCESS) {
        for (uint i = 0; i < 25; i++) {
            auto n = PolygonNode::allocWithTexture(t);
            n->setAnchor(Vec2::ANCHOR_CENTER);
            n->setPosition(at);
            float scale = (_screen.size.width * 0.06f) / n->getContentWidth();
            n->setScale(scale);
            addChild(n);
            double theta = (2.0 * M_PI) * Random::getInstance()->getFloat(1);
            double rx = cos(theta);
            double ry = sin(theta);
            Animation::to(n, .3f, {
                {"x", Animation::relative(rx * _screen.size.width * 0.1f)},
                {"y", Animation::relative(ry * _screen.size.width * 0.1f)},
                {"opacity", 0},
                {"delay", 0.02f * i}
            }, SINE_IN_OUT, [=]() {
                removeChild(n);
            });
        }
        _combo++;
        _timeSinceCombo = 0;
        if (_combo > 1) {
            auto font = _assets->get<Font>("jua");
            string comboText = "COMBO ";
            comboText.append(to_string(_combo));
            auto t = Label::alloc(comboText, font);
            t->setHorizontalAlignment(Label::HAlign::CENTER);
            t->setVerticalAlignment(Label::VAlign::BOTTOM);
            t->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
            t->setPosition(at + Vec2(0, _screen.size.width * 0.02f));
            addChild(t);
            Animation::to(t, .8f, {
                {"y", Animation::relative(_screen.size.height * .3f)},
                {"opacity", 0}
            }, STRONG_IN, [=]() {
                removeChild(t);
            });
        }
    } else {
        for (uint i = 0; i < 10; i++) {
            auto n = PolygonNode::allocWithTexture(t);
            n->setAnchor(Vec2::ANCHOR_CENTER);
            n->setPosition(at);
            float scale = (_screen.size.width * 0.04f) / n->getContentWidth();
            n->setScale(scale);
            addChild(n);
            Animation::to(n, .5f, {
                {"x", _screen.size.width / 2},
                {"y", _screen.size.height},
                {"opacity", 0},
                {"delay", 0.02f * i}
            }, SINE_IN_OUT, [=]() {
                removeChild(n);
            });
        }
    }
}

void Feedback::update(float timestep) {
    if (_combo > 0) {
        _timeSinceCombo += timestep;
        if (_timeSinceCombo > MAX_COMBO_TIME) {
            _timeSinceCombo = 0;
            _combo = 0;
        }
    }
}
