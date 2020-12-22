#ifndef HELPER_HPP
#define HELPER_HPP
#include "Menu/keyboardhandler.h"
#include <chrono>
#include "akinator/Akinator.hpp"
#include "nlohmann/json.hpp"
#include <fstream>

/// Game states
enum State {
    /// main menu is on screen
    StartMenu,

    /// akinator game on screen
    GameMenu,

    /// akinator gave returned a guess
    GuessMenu,

    /// game was ended: menu is back on screen
    EndGameMenu,

    /// game was ended: player wanted to add new character to base
    AddCharacterMenu,

    /// find names like characters
    FindCharMenu,

    /// game was ended: we need new char's name
    AddCharacterName,

    /// some menu was called: user changes settings
    SettingsMenu,

    /// exit button clicked
    ExitGame,

    /// restart button clicked
    Restart
};

using namespace oxygine;

namespace GameModel {
    DECLARE_SMART(Akinator, spAkinator)
}

///helper-class for static values visible to the whole program
struct ResCapture {
    static Resources res;
    static State game_state;
    static TextStyle text_style;
    static KeyboardHandler keyboard_handler;
    static bool game_started;
    static bool value_given;
    static bool gave_up;
    static bool guessed;
    inline static bool updated = false;
    static double value;
    static std::string name;
    inline static std::string n_name = "";
    static GameModel::spAkinator akinator;
    static GameModel::Character character_to_add;
    static vector<pair<GameModel::Question, GameModel::Answer>> answers_and_questions;

    static nlohmann::json from_path(const string& path) {
        nlohmann::json result;
        ifstream handle(path);
        if(handle.is_open())
            result = nlohmann::json::parse(handle);
        handle.close();
        return result;
    }

    static void push_new_character() {
        using json = nlohmann::json;

        const json answer_pattern = R"({"id": 0,"asnwer": 0.0})"_json;

        json new_char = R"({"id": 0,"name": "","answers": []})"_json;

        const json question_pattern = R"({"id": 0,"text": ""})"_json;

        string q_path = "data/json/questions.json";
        string c_path = "data/json/characters.json";
        json quests = ResCapture::from_path(q_path);
        json chars = ResCapture::from_path(c_path);

        size_t biggest_id_q = 0;
        for(const auto& quest : quests["questions"])
            if(quest["id"] > biggest_id_q)
                biggest_id_q = quest["id"];

        size_t biggest_id_c = 0;
        for(const auto& char_ : chars["characters"])
            if(char_["id"] > biggest_id_c)
                biggest_id_c = char_["id"];

        vector<pair<size_t, string>> new_questions;
        for(auto& answ_and_quest : ResCapture::answers_and_questions) {
            if(answ_and_quest.first.text.empty())
                break;
            bool new_question_flag = true;
            for(const auto& question : quests["questions"])
                if(question["text"] == answ_and_quest.first.text) {
                    new_question_flag = false;
                    break;
                }

                if(new_question_flag) {
                    new_questions.push_back(make_pair(++biggest_id_q, answ_and_quest.first.text));
                    answ_and_quest.first.id = answ_and_quest.second.question_id = biggest_id_q;
                }
            }

        for(const auto& q : new_questions) {
            json new_q = question_pattern;
            new_q["text"] = q.second;
            new_q["id"] = q.first;
            (quests["questions"]).push_back(new_q);
        }

        new_questions.clear();
        new_char["name"] = character_to_add.name;
        for(auto& character : chars["characters"])
            /// found this character in base
            if(character["name"] == new_char["name"]) {
                for(const auto& answ_and_quest : ResCapture::answers_and_questions) {
                    bool new_answer_flag = true;
                    for(const auto& answ : character["answers"])
                        if(answ_and_quest.first.id == answ["id"]) {
                            new_answer_flag = false;
                            break;
                        }

                    if(new_answer_flag) {
                        json answer = answer_pattern;
                        answer["answer"] = answ_and_quest.second.answer_value;
                        answer["id"] = answ_and_quest.second.question_id;
                        (character["answers"]).push_back(answer);
                    }
                }

                ofstream qout(q_path);
                qout << quests;
                qout.close();

                ofstream chout(c_path);
                chout << chars;
                chout.close();

                ResCapture::updated = true;

                return;
            }

        /// characer is completely new
        for(const auto& answ : ResCapture::answers_and_questions) {
            json answer = answer_pattern;
            answer["id"] = answ.second.question_id;
            answer["answer"] = answ.second.answer_value;
            (new_char["answers"]).push_back(answer);
        }

        (chars["characters"]).push_back(new_char);

        ofstream qout(q_path);
        qout << quests;
        qout.close();

        ofstream chout(c_path);
        chout << chars;
        chout.close();

        ResCapture::updated = true;
    }
};

static Sprite* create_sprite(const Vector2& size, const Vector2& position, ResAnim* res_anim = 0) {
    Sprite* result = new Sprite();

    result->setSize(size);
    result->setPosition(position);
    result->setAnchor(0.5, 0.5);

    if(res_anim != 0)
        result->setResAnim(res_anim);

    return result;
}
static Sprite* create_sprite(const Vector2& size, const Vector2& position, ResAnim* resAnim, spSprite& parent) {
    Sprite* result = create_sprite(size, position, resAnim);
    result->attachTo(parent);
    return result;
}


static TextStyle create_style(ResFont* font, const int& font_size, const Color& color = Color::Black,
                       TextStyle::VerticalAlign valign = TextStyle::VALIGN_MIDDLE,
                       TextStyle::HorizontalAlign halign = TextStyle::HALIGN_MIDDLE)
{
    TextStyle result = TextStyle();

    if(font != 0)
        result.font = font;
    result.fontSize = font_size;
    result.vAlign = valign;
    result.hAlign = halign;
    result.color = color;
    result.alignMiddle();

    return result;
}


static TextField* create_text(const Vector2& size, const Vector2& position,
                              const TextStyle& style, const std::string& text)
{
    TextField* result = new TextField();

    result->setSize(size);
    result->setPosition(position);
    result->setStyle(style);
    result->setText(text);
    result->setAnchor(0.5, 0.5);

    return result;
}
static TextField* create_text(const Vector2& size, const Vector2& position, const TextStyle& style,
                              const std::string& text, spSprite& parent)
{

    TextField* result = create_text(size, position, style, text);
    result->attachTo(parent);
    return result;
}


static std::string time_to_string(const std::chrono::system_clock::time_point& from) {
    using std::chrono::system_clock;
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::chrono::minutes;

    auto duration = system_clock::now() - from;
    auto mins = duration_cast<minutes>(duration).count();
    auto secs = duration_cast<seconds>(duration).count();

    std::string result = std::to_string(mins) + ":" + std::to_string(secs);
    return result;
}

#endif // HELPER_HPP
