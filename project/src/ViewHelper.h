#ifndef VIEWHELPER_H
#define VIEWHELPER_H

#include "oxygine-framework.h"
using namespace oxygine;
using std::string;

class ViewHelper
{
public:
    static Resources res;
    static spSprite initSprite(Actor* parent,
                               string resAnimName,
                               Vector2 pos = Vector2(0, 0),
                               Vector2 anchor = Vector2(0, 0));
    static spSprite initSprite(spActor parent,
                               Resources* res,
                               string resAnimName,
                               Vector2 pos = Vector2(0, 0),
                               Vector2 anchor = Vector2(0, 0));

    static spTextField initTextField(Actor* parent,
                                     Resources* res,
                                     string fontName,
                                     int fontSize = 0,
                                     Color fontColor = Color::White,
                                     TextStyle::VerticalAlign vAlign = TextStyle::VALIGN_TOP,
                                     TextStyle::HorizontalAlign hAlign = TextStyle::HALIGN_LEFT,
                                     Vector2 pos = Vector2(0, 0));

    static spTextField initTextField(spActor parent,
                                     Resources* res,
                                     string fontName,
                                     int fontSize = 0,
                                     Color fontColor = Color::White,
                                     TextStyle::VerticalAlign vAlign = TextStyle::VALIGN_TOP,
                                     TextStyle::HorizontalAlign hAlign = TextStyle::HALIGN_LEFT,
                                     Vector2 pos = Vector2(0, 0));

    static string getTimeStringFromTimestamp(time_t timestamp, time_t start_point);
};

#endif
