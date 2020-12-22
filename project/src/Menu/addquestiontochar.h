#ifndef ADDQUESTIONTOCHAR_H
#define ADDQUESTIONTOCHAR_H
#include "../helper.hpp"

using namespace std;

namespace GameGraphics {
    DECLARE_SMART(AddQuestionMenu, spAddQuestionMenu)

    class AddQuestionMenu : public oxygine::Actor {
    private:
        spSprite parent_field;

        spSprite info;

        spSprite question;
        spTextField question_txt;

        vector<spSprite> answers;
        spSprite to_menu;


        void init_all(const Vector2& size, const Vector2& position) {
            Vector2 size_small_btn = Vector2(size.x / 2, size.y / 25 * 2);
            Vector2 size_big_btn = Vector2(size.x, size.y / 15 * 2);

            init_info(size_big_btn, Vector2(size.x / 2 + position.x, size.y / 10 + position.y));

            init_text(size_big_btn, Vector2(size.x / 2 + position.x, size.y / 15 * 4 + position.y));

            init_to_menu(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 6 * 5 + position.y));

            init_choice_buttons(size_small_btn, size, position);
        }


        void init_info(const Vector2& btn_size, const Vector2& position) {
            info = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            info->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Add new question\nand an answer to it");
            txt->setSize(btn_size);
            txt->attachTo(info);
        }
        void init_text(const Vector2& btn_size, const Vector2& position) {
            question = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            question->setSize(btn_size);

            question_txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "?");
            question_txt->setSize(btn_size);
            question_txt->attachTo(question);
        }
        void init_to_menu(const Vector2& btn_size, const Vector2& position) {
            to_menu = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            to_menu->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Save Character");
            txt->setSize(btn_size);
            txt->attachTo(to_menu);

            add_event_to_menu();
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
                    if(!ResCapture::name.empty()) {
                        ResCapture::answers_and_questions.push_back(make_pair(GameModel::Question(0, ResCapture::name + "?"), GameModel::Answer(0, 1.0)));
                        ResCapture::name.clear();
                    }
                });
            else if(i == 1)
                btn->addClickListener([&](Event*) {
                    if(!ResCapture::name.empty()) {
                        ResCapture::answers_and_questions.push_back(make_pair(GameModel::Question(0, ResCapture::name + "?"), GameModel::Answer(0, 0.5)));
                        ResCapture::name.clear();
                    }
                });
            else if(i == 2)
                btn->addClickListener([&](Event*) {
                    if(!ResCapture::name.empty()) {
                        ResCapture::answers_and_questions.push_back(make_pair(GameModel::Question(0, ResCapture::name + "?"), GameModel::Answer(0, 0.0)));
                        ResCapture::name.clear();
                    }
                });
            else if(i == 3)
                btn->addClickListener([&](Event*) {
                    if(!ResCapture::name.empty()) {
                        ResCapture::answers_and_questions.push_back(make_pair(GameModel::Question(0, ResCapture::name + "?"), GameModel::Answer(0, -0.5)));
                        ResCapture::name.clear();
                    }
                });
            else if(i == 4)
                btn->addClickListener([&](Event*) {
                    if(!ResCapture::name.empty()) {
                        ResCapture::answers_and_questions.push_back(make_pair(GameModel::Question(0, ResCapture::name + "?"), GameModel::Answer(0, -1.0)));
                        ResCapture::name.clear();
                    }
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
        void init_choice_buttons(const Vector2& button_size, const Vector2& parent_size, const Vector2& parent_pos)
        {
            for(auto i = 0; i < 5; ++i)
                answers.push_back(init_choice_button(button_size,
                            Vector2(parent_pos.x + parent_size.x / 2, parent_pos.y + parent_size.y / 5 * 2 + i * parent_size.y / 15), i));
        }


        void add_event_to_menu() {
            to_menu->addClickListener([&](Event* e) {
                ResCapture::push_new_character();
                ResCapture::game_state = State::Restart;
            });
        }
        void add_backspace_event() {
            this->addEventListener(KeyEvent::KEY_DOWN, [&](Event* event) {
                if(((KeyEvent*)event)->data->keysym.scancode == SDL_SCANCODE_BACKSPACE)
                    if(!ResCapture::name.empty())
                        ResCapture::name.pop_back();
            });
        }


    public:
        AddQuestionMenu(spSprite parent_, const Vector2& size, const Vector2& position)
            : Actor(),
              parent_field(parent_)
        {
            init_all(size, position);
        }

        bool is_shown = false;

        void update() {
            if(KeyboardHandler::char_inputted) {
                ResCapture::name.push_back(KeyboardHandler::last_char);
                KeyboardHandler::char_inputted = false;
            }
            if(KeyboardHandler::char_destroyed) {
                KeyboardHandler::char_destroyed = false;
            }

            question_txt->setText(ResCapture::name + "?");
        }

        void hide_all() {
            parent_field->removeChild(info);
            parent_field->removeChild(question);
            parent_field->removeChild(to_menu);

            for(auto& btn : answers)
                parent_field->removeChild(btn);

            this->removeAllEventListeners();
        }

        void show_all() {
            parent_field->addChild(info);
            parent_field->addChild(question);
            parent_field->addChild(to_menu);

            for(auto& btn : answers)
                parent_field->addChild(btn);

            add_backspace_event();
        }
    };
}

#endif // ADDQUESTIONTOCHAR_H
