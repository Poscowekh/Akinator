#ifndef FINDCHARMENU_H
#define FINDCHARMENU_H
#include "../akinator/Akinator.hpp"
#include "../helper.hpp"

namespace GameGraphics {
    DECLARE_SMART(FindCharMenu, spFindCharMenu)

    class FindCharMenu : public oxygine::Actor {
    private:
        spSprite parent_field;

        spSprite info;
        spSprite find;
        spSprite found;
        spSprite not_found;
        spSprite to_menu;

        spSprite char_back;
        spTextField char_text;
        spSprite char_left;
        spSprite char_right;

        std::vector<string> chars = {""};
        std::vector<string>::iterator char_iter;


        void init_all(const Vector2& size, const Vector2& position) {
            Vector2 btn_size = Vector2(size.x, size.y / 25 * 3);
            Vector2 arrow_btn_size = Vector2(size.x / 4, size.y / 5);

            init_info(btn_size, Vector2(size.x / 2 + position.x, size.y / 150 * 19 + position.y));
            init_chars(btn_size, Vector2(size.x / 2 + position.x, size.y / 150 * 47 + position.y), arrow_btn_size);
            init_found(btn_size, Vector2(size.x / 2 + position.x, size.y / 2 + position.y));
            init_not_found(btn_size, Vector2(size.x / 2 + position.x, size.y / 150 * 103 + position.y));
            init_to_menu(btn_size, Vector2(size.x / 2 + position.x, size.y / 150 * 131 + position.y));
        }

        void init_info(const Vector2& size, const Vector2& pos) {
            info = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            info->setSize(size);

            spTextField txt = create_text(size, size / 2, ResCapture::text_style, "Try to find your character in the list:");
            txt->setSize(size);
            txt->attachTo(info);
        }
        void init_found(const Vector2& size, const Vector2& pos) {
            found = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            found->setSize(size);

            spTextField txt = create_text(size, size / 2, ResCapture::text_style, "I found my character");
            txt->setSize(size);
            txt->attachTo(found);

            add_event_found();
        }
        void init_not_found(const Vector2& size, const Vector2& pos) {
            not_found = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            not_found->setSize(size);

            spTextField txt = create_text(size, size / 2, ResCapture::text_style, "I didn't find my character");
            txt->setSize(size);
            txt->attachTo(not_found);

            add_event_not_found();
        }
        void init_to_menu(const Vector2& size, const Vector2& pos) {
            to_menu = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            to_menu->setSize(size);

            spTextField txt = create_text(size, size / 2, ResCapture::text_style, "Back to Menu");
            txt->setSize(size);
            txt->attachTo(to_menu);

            add_event_back();
        }
        void init_chars(const Vector2& size, const Vector2& pos, const Vector2& arrow_size) {
            char_back = create_sprite(size, pos, ResCapture::res.getResAnim("choice_button"));
            char_back->setSize(size);

            char_text = create_text(size, size / 2, ResCapture::text_style, "");
            char_text->setSize(size);
            char_text->attachTo(char_back);

            char_left = create_sprite(arrow_size, Vector2(pos.x - size.x / 2 - arrow_size.x / 3 - 5,
                                pos.y), ResCapture::res.getResAnim("arrow_button"));
            char_left->setSize(arrow_size);
            char_left->attachTo(char_back);
            char_left->setRotationDegrees(180.0);

            char_right = create_sprite(arrow_size, Vector2(pos.x + size.x / 2 + arrow_size.x / 3 + 5,
                                pos.y), ResCapture::res.getResAnim("arrow_button"));
            char_right->setSize(arrow_size);
            char_right->attachTo(char_back);

            add_event_char_left();
            add_event_char_right();
        }


        void add_event_back() {
            to_menu->addClickListener([&](Event* e) {
                ResCapture::game_state = State::StartMenu;
                ResCapture::akinator->restart();
            });
        }
        void add_event_char_left() {
            char_left->addClickListener([&](Event* e) {
                if(char_iter == chars.begin())
                    std::advance(char_iter, chars.size() - 1);
                else
                    --char_iter;
                try {
                    ResCapture::n_name = *char_iter;
                } catch(...) {
                    ResCapture::n_name = "";;
                }
            });
        }
        void add_event_char_right() {
            char_right->addClickListener([&](Event* e) {
                ++char_iter;
                if(char_iter == chars.end())
                    char_iter = chars.begin();
                try {
                    ResCapture::n_name = *char_iter;
                } catch(...) {
                    ResCapture::n_name = "";
                }
            });
        }

        void add_event_found() {
            found->addClickListener([&](Event* e) {
                if(!ResCapture::n_name.empty()) {
                    ResCapture::character_to_add = GameModel::Character(0, ResCapture::n_name, vector<GameModel::Answer>());
                    for(const auto& answer : ResCapture::akinator->usr_answers())
                        if(answer.answer_value != 0.0)
                            ResCapture::answers_and_questions.push_back(make_pair(GameModel::Question(answer.question_id, ""), answer));
                    ResCapture::game_state = State::AddCharacterMenu;
                }
            });
        }
        void add_event_not_found() {
            not_found->addClickListener([&](Event* e) {
                ResCapture::character_to_add = GameModel::Character(0, ResCapture::name, vector<GameModel::Answer>());
                for(const auto& answer : ResCapture::akinator->usr_answers())
                    if(answer.answer_value != 0.0)
                        ResCapture::answers_and_questions.push_back(make_pair(GameModel::Question(answer.question_id, ""), answer));
                ResCapture::game_state = State::AddCharacterMenu;
            });
        }


    public:
        FindCharMenu(spSprite& parent_field_, const Vector2& size, const Vector2& position)
            : Actor(),
              parent_field(parent_field_)
        {
            init_all(size, position);
        }

        bool is_shown = false;

        void update() {
            if(chars.empty())
                char_text->setText("Matches not found");
            else
                char_text->setText(*char_iter);
        }

        void hide_all() {
            parent_field->removeChild(info);
            parent_field->removeChild(found);
            parent_field->removeChild(not_found);
            parent_field->removeChild(to_menu);

            parent_field->removeChild(char_back);
            parent_field->removeChild(char_left);
            parent_field->removeChild(char_right);
        }
        void show_all() {
            vector<GameModel::Character> matches = ResCapture::akinator->find_matches(ResCapture::name);
            for(auto& character : matches)
                chars.push_back(character.name);
            char_iter = chars.begin();

            parent_field->addChild(info);
            parent_field->addChild(found);
            parent_field->addChild(not_found);
            parent_field->addChild(to_menu);

            parent_field->addChild(char_back);
            parent_field->addChild(char_left);
            parent_field->addChild(char_right);
        }
    };
}

#endif // FINDCHARMENU_H
