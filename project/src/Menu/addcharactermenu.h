#ifndef ADDCHARACTERMENU_H
#define ADDCHARACTERMENU_H
#include "../helper.hpp"
#include "keyboardhandler.h"

using namespace std;

namespace GameGraphics {
    DECLARE_SMART(AddCharMenu, spAddCharMenu)

    class AddCharMenu : public oxygine::Actor {
    private:
        spSprite parent_field;

        spSprite info;

        spSprite name;
        spTextField name_txt;

        spSprite continue_button;
        spSprite to_menu;


        void init_all(const Vector2& size, const Vector2& position) {
            Vector2 size_small_btn = Vector2(size.x / 2, size.y / 8);
            Vector2 size_big_btn = Vector2(size.x / 5 * 4, size.y / 7 * 2);

            init_info(size_big_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 7 + position.y));

            init_name(size_big_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 15 + position.y));
            init_continue(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 23 + position.y));

            init_to_menu(size_small_btn, Vector2(size.x / 2 + position.x, size.y / 32 * 27 + position.y));

            ResCapture::name.clear();
        }


        void init_info(const Vector2& btn_size, const Vector2& position) {
            info = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            info->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "I couldn't guess your character\nEnter your characters's name:");
            txt->setSize(btn_size);
            txt->attachTo(info);
        }
        void init_name(const Vector2& btn_size, const Vector2& position) {
            name = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            name->setSize(btn_size);

            name_txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "");
            name_txt->setSize(btn_size);
            name_txt->attachTo(name);
        }
        void init_continue(const Vector2& btn_size, const Vector2& position) {
            continue_button = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            continue_button->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Continue");
            txt->setSize(btn_size);
            txt->attachTo(continue_button);

            add_event_continue();
        }
        void init_to_menu(const Vector2& btn_size, const Vector2& position) {
            to_menu = create_sprite(btn_size, position, ResCapture::res.getResAnim("choice_button"));
            to_menu->setSize(btn_size);

            spTextField txt = create_text(btn_size, btn_size / 2, ResCapture::text_style, "Back to menu");
            txt->setSize(btn_size);
            txt->attachTo(to_menu);

            add_event_to_menu();
        }


        void add_event_to_menu() {
            to_menu->addClickListener([&](Event* e) {
                ResCapture::game_state = State::Restart;
            });
        }
        void add_event_continue() {
            continue_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::FindCharMenu;
            });
        }

    public:
        AddCharMenu(spSprite parent_, const Vector2& size, const Vector2& position)
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
                name_txt->setText(ResCapture::name);
            }
            if(KeyboardHandler::char_destroyed) {
                KeyboardHandler::char_destroyed = false;
                name_txt->setText(ResCapture::name);
            }
        }

        void hide_all() {
            parent_field->removeChild(info);
            parent_field->removeChild(name);
            parent_field->removeChild(continue_button);
            parent_field->removeChild(to_menu);
        }

        void show_all() {
            parent_field->addChild(info);
            parent_field->addChild(name);
            parent_field->addChild(continue_button);
            parent_field->addChild(to_menu);
        }
    };
}

#endif // ADDCHARACTERMENU_H
