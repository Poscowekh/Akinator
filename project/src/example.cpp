#include "oxygine-framework.h"
#include <functional>
#include "Presenter/Game_presenter.h"

using namespace oxygine;
//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources ResCapture::res;
State ResCapture::game_state = State::StartMenu;
TextStyle ResCapture::text_style = TextStyle();

bool ResCapture::game_started = false;
bool ResCapture::value_given = false;
bool ResCapture::gave_up = false;
bool ResCapture::guessed = false;

double ResCapture::value = 0.0;
string ResCapture::name = "";

GameModel::spAkinator ResCapture::akinator = new GameModel::Akinator();
KeyboardHandler ResCapture::keyboard_handler = KeyboardHandler();
GameModel::Character ResCapture::character_to_add = GameModel::Character();
vector<pair<GameModel::Question, GameModel::Answer>> ResCapture::answers_and_questions;


GameGraphics::spGamePresenter presenter;
//GameGraphics::spMenu menu;
unsigned short frame = 0;

void example_preinit() {}

Vector2 get_scales(const Vector2& display_size) {
    float x = 960 / display_size.x;
    float y = 640 / display_size.y;

    /*if(x - y > 0.5)
        x = y;
    else if(y - x > 0.5)
        y = x;

    if(x < 0.5)
        x = 0.5;
    if(x > 1.5)
        x = 1.5;*/

    if(y < 0.5)
        y = 0.5;
    if(y > 1.5)
        y = 0.5;

    return Vector2(x, y);
}

//called from main.cpp
void example_init() {
    ResCapture::game_state = State::StartMenu;
    Point tmp = core::getDisplaySize();



    Vector2 display_size = Vector2(tmp.x, tmp.y);
    Vector2 scales = get_scales(display_size);

    ResCapture::res.loadXML("data/res.xml");
    ResCapture::text_style = create_style(ResCapture::res.getResFont("main_font"), 24 / max(scales.x, scales.y));

    presenter = new GameGraphics::GamePresenter(getStage(), display_size, scales);
}

//called each frame from main.cpp
void example_update() {
    if(frame % 30 == 0) {
        presenter->update();

        frame = 1;
    }

    ++frame;
}

//called each frame from main.cpp
void example_destroy()
{
    //st->~GameState();
    //free previously loaded resources
}
