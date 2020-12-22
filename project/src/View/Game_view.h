#ifndef GAME_VIEW_H
#define GAME_VIEW_H
#include "../helper.hpp"
#include "../akinator/Akinator.hpp"
#include <fstream>
#include <iostream>

using namespace std;
namespace GameGraphics {
    DECLARE_SMART(GameView, spGameView)

    /// shows akinator game window
    class GameView : public oxygine::Actor {
    private:
        spSprite parent_field;
        bool game_over_flag_ = false;
        Vector2 size;
        GameModel::Character guess;
        GameModel::Question question;
        nlohmann::json chars;
        nlohmann::json quests;

        //children
        GameModel::spAkinator akinator = ResCapture::akinator;
        vector<spSprite> choice_buttons;
        spSprite akinator_question;
        spTextField akinator_quest_txt;
        spSprite question_back_button;
        spTextField question_count;


        void initialize(const Vector2& scales) {
            this->attachTo(parent_field);

            init_akinator();

            init_akinator_question(Vector2(size.x / 7 * 4, size.y * 1 / 4),
                                   Vector2(size.x / 2, size.y * 3 / 13));

            init_choice_buttons(Vector2(size.x / 7 * 4, size.y / 12));

            init_quest_back_button(Vector2(size.x / 9 * 4, size.y / 12),
                                   Vector2(size.x / 2, size.y / 12 * 11));

            init_quest_count(Vector2(140 * scales.x, 60 * scales.y),
                             Vector2(size.x / 2, 35 * scales.y));
        }

        void init_akinator() {
            string q_path = "data/json/questions.json";
            string c_path = "data/json/characters.json";
            quests = from_path(q_path)["questions"];
            chars = from_path(c_path)["characters"];
            akinator = new GameModel::Akinator(quests, chars);
            ResCapture::akinator = akinator;
            question = akinator->next_question();
        }
        string init_txt(const int& i) {
            switch (i) {
            case 0:
                return string("Yes");
                break;
            case 1:
                return string("Probably Yes");
                break;
            case 2:
                return string("I don't know");
                break;
            case 3:
                return string("Probably No");
                break;
            case 4:
                return string("No");
                break;
            default:
                break;
            }

            return string("NO TEXT");
        }
        void init_events(spSprite& btn, const int& i) {
            if(i == 0)
                btn->addClickListener([&](Event*) {
                    ResCapture::value_given = true;
                    ResCapture::value = 1.0;
                });
            else if(i == 1)
                btn->addClickListener([&](Event*) {
                    ResCapture::value_given = true;
                    ResCapture::value = 0.5;
                });
            else if(i == 2)
                btn->addClickListener([&](Event*) {
                    ResCapture::value_given = true;
                    ResCapture::value = 0.0;
                });
            else if(i == 3)
                btn->addClickListener([&](Event*) {
                    ResCapture::value_given = true;
                    ResCapture::value = -0.5;
                });
            else if(i == 4)
                btn->addClickListener([&](Event*) {
                    ResCapture::value_given = true;
                    ResCapture::value = -1.0;
                });
        }
        spTextField init_choice_btn_txt(const Vector2& button_size, const int& i) {
            return create_text(button_size, button_size / 2, ResCapture::text_style, init_txt(i));
        }
        spSprite init_choice_button(const Vector2& button_size, const Vector2& position, const int& i) {
            spSprite button = create_sprite(button_size, position, ResCapture::res.getResAnim("choice_button"));
            button->setSize(button_size);

            spTextField txt = init_choice_btn_txt(button_size, i);
            txt->attachTo(button);
            txt->setSize(button_size);

            init_events(button, i);

            return button;
        }
        void init_choice_buttons(const Vector2& button_size) {
            for(auto i = 0; i < 5; ++i)
                choice_buttons.push_back(init_choice_button(button_size,
                                Vector2(size.x / 2, size.y * (5 + i) / 12), i));
        }

        void init_quest_count(const Vector2& button_size, const Vector2& position) {
            spTextField txt = create_text(button_size, position, ResCapture::text_style, "0");
            txt->setSize(button_size);
            txt->attachTo(parent_field);
        }
        void init_quest_back_button(const Vector2& button_size, const Vector2& position) {
            question_back_button = create_sprite(button_size, position, ResCapture::res.getResAnim("choice_button"));
            question_back_button->setSize(button_size);

            spTextField txt = create_text(button_size, button_size / 2, ResCapture::text_style, "Change Answer");
            txt->attachTo(question_back_button);
            txt->setSize(button_size);
        }
        void init_akinator_question(const Vector2& field_size, const Vector2& position) {
            akinator_question = create_sprite(field_size, position, ResCapture::res.getResAnim("choice_button"));
            akinator_question->setSize(field_size);

            akinator_quest_txt = create_text(field_size, field_size / 2, ResCapture::text_style, "Welcome");
            akinator_quest_txt->attachTo(akinator_question);
            akinator_quest_txt->setSize(field_size);
        }

        nlohmann::json from_path(const string& path) {
            nlohmann::json result;
            ifstream handle(path);
            if(handle.is_open())
                result = nlohmann::json::parse(handle);
            handle.close();
            return result;
        }
        void akinator_tick() {
            using namespace GameModel;

            if(akinator->state() == Akinator::State::AskQuestion) {
                if(ResCapture::value_given) {
                    question = akinator->next_question();
                    akinator->receive_answer(ResCapture::value, question);
                    ResCapture::value_given = false;
                    akinator->next_state();
                }
            }

            else if(akinator->state() == Akinator::State::Guess) {
                if(!ResCapture::guessed)
                    akinator->bad_guess(guess);
                akinator->next_state();
                guess = akinator->guess();
                ResCapture::name = guess.name;
                if(guess.name.empty())
                    ResCapture::game_state = State::AddCharacterName;
                else
                    ResCapture::game_state = State::GuessMenu;
            }

            else if(akinator->state() == Akinator::State::Restart) {
                akinator = new Akinator(quests, chars);
            }

            else if(akinator->state() == Akinator::State::GiveUp) {
                ResCapture::game_state = State::AddCharacterName;
                ResCapture::name.clear();
            }
        }
        void update_text() {
            akinator_quest_txt->setText(question.text);
            //question_count->setText(to_string(akinator->iteration() + 1));
        }


    public:
        GameView(spSprite& parent_field_, const Vector2& size_, const Vector2& scales)
            : oxygine::Actor(),
              size(size_),
              parent_field(parent_field_)
        {
            initialize(scales);
        }

        bool game_over_flag() const { return game_over_flag_; }

        void update() {
            if(ResCapture::game_state == State::Restart) {
                if(ResCapture::updated) {
                    init_akinator();
                    ResCapture::game_state = State::StartMenu;
                    return;
                }
                else {
                    akinator->restart();
                    ResCapture::game_state = State::StartMenu;
                    return;
                }
            }

            akinator_tick();
            update_text();
        }

        bool is_shown = false;

        void hide_all() {
            this->removeChild(akinator_question);
            //this->removeChild(question_count);
            //this->removeChild(question_back_button);
            for(auto& button : choice_buttons)
                this->removeChild(button);
        }
        void show_all() {
            this->addChild(akinator_question);
            //this->addChild(question_count);
            //this->addChild(question_back_button);
            for(auto& button : choice_buttons)
                this->addChild(button);
        }
    };
}
#endif // GAME_VIEW_H
