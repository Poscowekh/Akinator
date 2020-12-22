#ifndef MENU_HPP
#define MENU_HPP
#include "../helper.hpp"

using namespace std;

namespace GameGraphics {
    DECLARE_SMART(Menu, spMenu)

    class Menu : public oxygine::Actor {
    private:
        spSprite parent_field;

        spSprite settings_button;
        spSprite exit_button;
        spSprite start_button;
        
        spSprite restart_button;
        spSprite resume_button;

        spSprite add_char_button;


        void init_all(const Vector2& size, const Vector2& position) {
            Vector2 size_small_btn = Vector2(size.x / 3 * 2, size.y / 6);
            Vector2 size_big_btn = Vector2(size.x / 3 * 2, size.y * 2 / 5);

            init_exit(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 60 * 51 + position.y));
            init_settings(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 60 * 37 + position.y));

            init_start(size_big_btn, Vector2(size.x / 2 + position.x, size.y / 15 * 4 + position.y));

            init_restart(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 20 * 3 + position.y));
            init_resume(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 60 * 23 + position.y));
        }


        void init_exit(const Vector2& btn_size, const Vector2& position) {
            exit_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            exit_button->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Exit Game");
            txt->setSize(btn_size);
            txt->attachTo(exit_button);

            add_event_exit();
        }
        void init_settings(const Vector2& btn_size, const Vector2& position) {
            settings_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            settings_button->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Settings");
            txt->setSize(btn_size);
            txt->attachTo(settings_button);

            add_event_settings();
        }

        void init_start(const Vector2& btn_size, const Vector2& position) {
            start_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            start_button->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Start Game");
            txt->setSize(btn_size);
            txt->attachTo(start_button);
            
            add_event_start_game();
        }
        
        void init_resume(const Vector2& btn_size, const Vector2& position) {
            resume_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            resume_button->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Resume Game");
            txt->setSize(btn_size);
            txt->attachTo(resume_button);

            add_event_resume_game();
        }
        void init_restart(const Vector2& btn_size, const Vector2& position) {
            restart_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            restart_button->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Restart Game");
            txt->setSize(btn_size);
            txt->attachTo(restart_button);

            add_event_restart_game();
        }
        
        void init_add_char_button(const Vector2& btn_size, const Vector2& position) {
            add_char_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            add_char_button->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Add Character");
            txt->setSize(btn_size);
            txt->attachTo(add_char_button);

            add_event_add_char();
        }


        void add_event_exit() {
            exit_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::ExitGame;

                core::requestQuit();
            });
        }
        void add_event_settings() {
            settings_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::SettingsMenu;
            });
        }
        void add_event_start_game() {
            start_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::GameMenu;
                ResCapture::game_started = true;
            });
        }
        void add_event_resume_game() {
            resume_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::GameMenu;
            });
        }
        void add_event_restart_game() {
            restart_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::Restart;
            });
        }
        void add_event_add_char() {
            add_char_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::AddCharacterMenu;
            });
        }


    public:
        Menu(spSprite parent_, const Vector2& size, const Vector2& position)
            : Actor(),
              parent_field(parent_)
        {
            init_all(size, position);
        }


        bool is_shown = false;

        void update() {

        }

        void hide_all() {
            parent_field->removeChild(settings_button);
            parent_field->removeChild(exit_button);

            parent_field->removeChild(start_button);

            parent_field->removeChild(restart_button);
            parent_field->removeChild(resume_button);
        }

        void show_all() {
            parent_field->addChild(exit_button);
            parent_field->addChild(settings_button);

            if(ResCapture::game_started) {
                parent_field->addChild(restart_button);

                if(ResCapture::game_state == State::EndGameMenu)
                    parent_field->addChild(add_char_button);
                else
                    parent_field->addChild(resume_button);
            }
            else
                parent_field->addChild(start_button);
        }
    };
}

#endif // MENU_HPP
