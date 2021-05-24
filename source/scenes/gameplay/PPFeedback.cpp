#include "PPFeedback.h"


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
    _goodjobs.push_back("Fabulous!");
    _goodjobs.push_back("Beautiful!");
    _goodjobs.push_back("Gorgeous!");
    _goodjobs.push_back("Great Job!");
    _goodjobs.push_back("Good Job!");
    _goodjobs.push_back("Cool!");
    _goodjobs.push_back("Incredible!");
    _goodjobs.push_back("Marvelous!");
}

void Feedback::add(Vec2 at, Vec2 dangerBarPoint, FeedbackType type) {
    FeedbackItem i;
    i.at = at;
    i.dangerBarPoint = dangerBarPoint;
    i.type = type;
    _items.push_back(i);
}

void Feedback::update(float timestep) {
    if (_items.size() == 0) return;
    for (auto & element : _items) {
        FeedbackType type = element.type;
        Vec2 at = element.at;
        Vec2 dangerBarPoint = element.dangerBarPoint;
        string txtt;
        ptr<Texture> txtu[3];
        switch (type) {
            case SUCCESS: {
                txtt = "correct";
                break;
            }
            case FAILURE: {
                txtt = "wrong";
                break;
            }
            default: {
                CUAssertLog(false, "Unknown feedback type.");
            }
        }
        for (uint i = 1; i <= 3; i++) {
            txtu[i - 1] =
                _assets->get<Texture>("feedback-" + txtt + to_string(i));
        }
        if (type == SUCCESS) {
            for (uint i = 0; i < 25; i++) {
                auto n = PolygonNode::allocWithTexture
                    (txtu[Random::getInstance()->getInt(2, 0)]);
                n->setAnchor(Vec2::ANCHOR_CENTER);
                n->setPosition(at);
                float scale =
                    (_screen.size.width * 0.1f) / n->getContentWidth();
                n->setScale(scale);
                addChild(n);
                double theta =
                    (2.0 * M_PI) * Random::getInstance()->getFloat(1);
                double rx = cos(theta);
                double ry = sin(theta);
                Animation::to(n, .6f, {
                    {"x",       Animation::relative(
                        rx * _screen.size.width * 0.1f)},
                    {"y",       Animation::relative(
                        ry * _screen.size.width * 0.1f)},
                    {"opacity", 0}
                }, SINE_IN_OUT, [=]() {
                    removeChild(n);
                });
            }
        } else {
            for (uint i = 0; i < 10; i++) {
                int shakeSize = _screen.size.width * 0.02f;
                auto shake = Vec2(
                    Random::getInstance()->getInt(shakeSize, -shakeSize),
                    Random::getInstance()->getInt(shakeSize, -shakeSize)
                    );
                auto n = PolygonNode::allocWithTexture
                    (txtu[Random::getInstance()->getInt(2, 0)]);
                n->setAnchor(Vec2::ANCHOR_CENTER);
                n->setPosition(at + shake);
                float scale =
                    (_screen.size.width * 0.08f) / n->getContentWidth();
                n->setScale(scale);
                addChild(n);
                Animation::to(n, .5f, {
                    {"x",       dangerBarPoint.x + shake.x},
                    {"y",       dangerBarPoint.y + shake.y},
                    {"opacity", 0},
                    {"delay",   0.02f * i}
                }, SINE_IN_OUT, [=]() {
                    removeChild(n);
                });
            }
        }
        _items.clear();
    }
}
