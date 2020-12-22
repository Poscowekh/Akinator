#include "ViewHelper.h"
#include <ctime>

spSprite ViewHelper::initSprite(Actor* parent, std::string resAnimName, Vector2 pos, Vector2 anchor)
{
    Sprite* result = new Sprite();
    //if (!resAnimName.empty())
        //result->setResAnim(res.getResAnim(resAnimName, ep_show_warning));
    result->setPosition(pos);
    result->setAnchor(anchor);
    if (parent != nullptr)
        result->attachTo(parent);
    return result;
}

spSprite ViewHelper::initSprite(spActor parent, Resources* res, std::string resAnimName, Vector2 pos, Vector2 anchor)
{
    return initSprite(parent.get(), res, resAnimName, pos, anchor);
}

spTextField ViewHelper::initTextField(Actor *parent, Resources *res, std::string fontName, int fontSize, Color fontColor, TextStyle::VerticalAlign vAlign, TextStyle::HorizontalAlign hAlign, Vector2 pos)
{
    spTextField result = new TextField();
    result->setPosition(pos);

    TextStyle style;
    //style.font = ResourceManager::instance()->getResFont(res, fontName)->getFont(0, fontSize);
    style.color = fontColor;
    style.vAlign = vAlign;
    style.hAlign = hAlign;

    result->setStyle(style);

    if (parent)
        result->attachTo(parent);

    return result;
}

spTextField ViewHelper::initTextField(spActor parent, Resources *res, std::string fontName, int fontSize, Color fontColor, TextStyle::VerticalAlign vAlign, TextStyle::HorizontalAlign hAlign, Vector2 pos)
{
    return initTextField(parent.get(), res, fontName, fontSize, fontColor, vAlign, hAlign, pos);
}

std::string ViewHelper::getTimeStringFromTimestamp(time_t timestamp, time_t start_point)
{
    time_t t = timestamp - start_point;
    string minutes, seconds;
    seconds = std::to_string(t % 60);
    minutes = std::to_string(t / 60 % 60);

    return minutes + ":" + seconds;
}
