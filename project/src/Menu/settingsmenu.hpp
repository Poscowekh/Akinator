#ifndef SETTINGSMENU_HPP
#define SETTINGSMENU_HPP
#include "../helper.hpp"
#include <vector>

namespace GameGraphics {
    DECLARE_SMART(SettingsMenu, spSettingsMenu)

    /// Menu for changing settings for game
    class SettingsMenu : public oxygine::Actor {
    private:
        spSprite parent_field;

        spSprite back_button;

        spSprite resol_back;
        spTextField resol_text;
        spSprite resol_left;
        spSprite resol_right;

        spSprite type_back;
        spTextField type_text;
        spSprite type_left;
        spSprite type_right;

        std::vector<Point> resolutions;
        std::vector<Point>::iterator resol_iter;


        void init_all(const Vector2& size, const Vector2& position) {
            Vector2 btn_size = Vector2(size.x / 3 * 2, size.y / 5);
            Vector2 arrow_btn_size = Vector2(size.x / 7, size.y / 6);

            init_resolutions();
            init_back_button(btn_size, Vector2(size.x / 2 + position.x, size.y / 5 + position.y));
            init_change_resol(btn_size, Vector2(size.x / 2 + position.x, size.y / 2 + position.y), arrow_btn_size);
            init_change_type(btn_size, Vector2(size.x / 2 + position.x, size.y / 5 * 4 + position.y), arrow_btn_size);
        }

        void init_resolutions() {
            resolutions.push_back(Point(960,640));
            resolutions.push_back(Point(1280,1024));
            resolutions.push_back(Point(1440,900));
            resolutions.push_back(Point(1600,900));
            resolutions.push_back(Point(1920,1080));
            resol_iter = resolutions.begin();
        }

        void init_back_button(const Vector2& size, const Vector2& pos) {
            back_button = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            back_button->setSize(size);

            spTextField txt = create_text(size, size / 2, ResCapture::text_style, "Save and Exit");
            txt->setSize(size);
            txt->attachTo(back_button);

            add_event_back();
        }
        void init_change_resol(const Vector2& size, const Vector2& pos, const Vector2& arrow_size) {
            resol_back = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            resol_back->setSize(size);

            resol_text = create_text(size, size / 2, ResCapture::text_style, "Resolution Here");
            resol_text->setSize(size);
            resol_text->attachTo(resol_back);

            resol_left = create_sprite(arrow_size, Vector2(pos.x - size.x / 2 - arrow_size.x / 2 - 5,
                                pos.y), ResCapture::res.getResAnim("arrow_button"));
            resol_left->setSize(arrow_size);
            resol_left->attachTo(resol_back);
            resol_left->setRotationDegrees(180.0);

            resol_right = create_sprite(arrow_size, Vector2(pos.x + size.x / 2 + arrow_size.x / 2 + 5,
                                pos.y), ResCapture::res.getResAnim("arrow_button"));
            resol_right->setSize(arrow_size);
            resol_right->attachTo(resol_back);

            add_event_resol_left();
            add_event_resol_right();
        }
        void init_change_type(const Vector2& size, const Vector2& pos, const Vector2& arrow_size) {
            type_back = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            type_back->setSize(size);

            type_text = create_text(size, size / 2, ResCapture::text_style, "Character Types Here");
            type_text->setSize(size);
            type_text->attachTo(type_back);

            type_left = create_sprite(arrow_size, Vector2(pos.x - size.x / 2 - arrow_size.x / 2 - 5,
                                pos.y), ResCapture::res.getResAnim("arrow_button"));
            type_left->setSize(arrow_size);
            type_left->attachTo(type_back);
            type_left->setRotationDegrees(180.0);

            type_right = create_sprite(arrow_size, Vector2(pos.x + size.x / 2 + arrow_size.x / 2 + 5,
                                pos.y), ResCapture::res.getResAnim("arrow_button"));
            type_right->setSize(arrow_size);
            type_right->attachTo(type_back);

            add_event_resol_left();
            add_event_resol_right();
        }


        void add_event_back() {
            back_button->addClickListener([&](Event* e) {
                ResCapture::game_state = State::StartMenu;
            });
        }
        void add_event_resol_left() {
            resol_left->addClickListener([&](Event* e) {
                if(resol_iter == resolutions.begin())
                    std::advance(resol_iter, resolutions.size() - 1);
                else
                    --resol_iter;
                getStage()->setSize(*resol_iter);
            });
        }
        void add_event_resol_right() {
            resol_right->addClickListener([&](Event* e) {
                ++resol_iter;
                if(resol_iter == resolutions.end())
                    resol_iter = resolutions.begin();
                getStage()->setSize(resol_iter->y, resol_iter->x);
            });
        }
        void add_event_type_left() {
            type_left->addClickListener([&](Event* e) {

            });
        }
        void add_event_type_right() {
            type_right->addClickListener([&](Event* e) {

            });
        }


        // no implement
        bool restart_needed() {
            return false;
        }


    public:
        SettingsMenu(spSprite& parent_field_, const Vector2& size, const Vector2& position)
            : Actor(),
              parent_field(parent_field_),
              resolutions(std::vector<Point>())
        {
            init_all(size, position);
        }

        bool is_shown = false;

        void update() {
            resol_text->setText(to_string(resol_iter->y) + "x" + to_string(resol_iter->x));
        }

        void hide_all() {
            parent_field->removeChild(back_button);

            parent_field->removeChild(type_back);
            parent_field->removeChild(type_left);
            parent_field->removeChild(type_right);

            parent_field->removeChild(resol_back);
            parent_field->removeChild(resol_left);
            parent_field->removeChild(resol_right);
        }
        void show_all() {
            parent_field->addChild(back_button);

            parent_field->addChild(type_back);
            parent_field->addChild(type_left);
            parent_field->addChild(type_right);

            parent_field->addChild(resol_back);
            parent_field->addChild(resol_left);
            parent_field->addChild(resol_right);
        }
    };
}

#endif // SETTINGSMENU_HPP
