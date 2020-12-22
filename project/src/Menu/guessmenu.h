#ifndef GUESSMENU_H
#define GUESSMENU_H
#include "../helper.hpp"
#include "../akinator/Akinator.hpp"

using namespace std;

namespace GameGraphics {
    DECLARE_SMART(GuessMenu, spGuessMenu)

    class GuessMenu : public oxygine::Actor {
    private:
        spSprite parent_field;

        spSprite name;
        spTextField name_txt;

        spSprite yes_button;
        spTextField yes_txt;

        spSprite no_button;
        spTextField no_txt;

        spSprite to_menu;


        void init_all(const Vector2& size, const Vector2& position) {
            Vector2 size_small_btn = Vector2(size.x / 3, size.y / 7);
            Vector2 size_big_btn = Vector2(size.x / 5 * 4, size.y / 4);

            init_name(size_big_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 7 + position.y));

            init_yes(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 15 + position.y));
            init_no(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 21 + position.y));

            init_to_menu(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 27 + position.y));
        }


        void init_name(const Vector2& btn_size, const Vector2& position) {
            name = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            name->setSize(btn_size);

            name_txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "NoName");
            name_txt->setSize(btn_size);
            name_txt->attachTo(name);
        }
        void init_yes(const Vector2& btn_size, const Vector2& position) {
            yes_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            yes_button->setSize(btn_size);

            yes_txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Yes");
            yes_txt->setSize(btn_size);
            yes_txt->attachTo(yes_button);

            add_event_yes();
        }
        void init_no(const Vector2& btn_size, const Vector2& position) {
            no_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            no_button->setSize(btn_size);

            no_txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "No");
            no_txt->setSize(btn_size);
            no_txt->attachTo(no_button);

            add_event_no();
        }
        void init_to_menu(const Vector2& btn_size, const Vector2& position) {
            to_menu = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            to_menu->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Restart");
            txt->setSize(btn_size);
            txt->attachTo(to_menu);

            add_event_to_menu();
        }


        void add_event_to_menu() {
            to_menu->addClickListener([&](Event* e) {
                ResCapture::game_state = State::Restart;
            });
        }
        void add_event_yes() {
            yes_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::StartMenu;
                ResCapture::game_started = false;
                ResCapture::guessed = true;
                ResCapture::name.clear();
            });
        }
        void add_event_no() {
            no_button->addClickListener([&](Event* e) {
                if(ResCapture::gave_up) {
                    ResCapture::game_state = State::AddCharacterName;
                    ResCapture::guessed = false;
                }
                else {
                    ResCapture::game_state = State::GameMenu;
                    ResCapture::guessed = false;
                }
            });
        }


    public:
        GuessMenu(spSprite parent_, const Vector2& size, const Vector2& position)
            : Actor(),
              parent_field(parent_)
        {
            init_all(size, position);
        }


        bool is_shown = false;

        void update() {
            name_txt->setText("Is it " + ResCapture::name + "?");
            if(ResCapture::gave_up) {
                no_txt->setText("No, submit my character");
            }
            else if(!ResCapture::gave_up) {
                no_txt->setText("No");
            }
        }

        void hide_all() {
            parent_field->removeChild(to_menu);
            parent_field->removeChild(yes_button);
            parent_field->removeChild(no_button);
            parent_field->removeChild(name);
        }

        void show_all() {
            parent_field->addChild(name);
            parent_field->addChild(yes_button);
            parent_field->addChild(no_button);

            if(ResCapture::gave_up)
                parent_field->addChild(to_menu);
        }
    };
}

#endif // GUESSMENU_H
