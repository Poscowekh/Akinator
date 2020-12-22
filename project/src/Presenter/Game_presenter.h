#ifndef GamePresenter_HPP
#define GamePresenter_HPP
#include "../View/Game_view.h"
#include "../Menu/Menu.h"
#include "../Menu/settingsmenu.hpp"
#include "../Menu/guessmenu.h"
#include "../Menu/addcharactermenu.h"
#include "../Menu/addquestiontochar.h"
#include "../Menu/findcharmenu.h"

using namespace std;

namespace GameGraphics {
    DECLARE_SMART(GamePresenter, spGamePresenter)

    /// main class that controls what is shown to user at the moment
    class GamePresenter : public oxygine::EventDispatcher {
    private:
        /// shows game
        spGameView view;

        /// static background
        spSprite background;
        /// this button pauses game and opens menu (static)
        spSprite menu_button;

        /// menu with start buttons etc
        spMenu main_menu;
        /// menu with settings buttons
        spSettingsMenu settings_menu;
        /// menu with question yes or no to define if guessed correctly
        spGuessMenu guess_menu;

        /// Add name to new char menu
        spAddCharMenu char_name_menu;
        /// Add questions and answer to new char menu
        spAddQuestionMenu char_answ_menu;
        /// Finds char in dtabase by provided name
        spFindCharMenu char_find_menu;


        short update_count = 0;
        bool init_flag = false;

        /// Provides time since game start counting
        struct Timer
        {
            public:
                time_t start_point;
                time_t end_point;
                string now;
                Timer()
                {
                    start_point = time(NULL);
                    end_point = start_point;
                    update();
                }
                void reload() { start_point = time(0); }
                void update()
                {
                    time_t t = time(0) - start_point;
                    string minutes, seconds;
                    seconds = std::to_string(t % 60);
                    minutes = std::to_string(t / 60);

                    now = minutes + ":" + seconds;
                }
                string operator()() {
                    update();
                    return now;
                }
        };
        /*class Timer {
        private:
            std::chrono::system_clock::time_point previous_time;
            size_t seconds = 0;

        public:
            Timer()
                : previous_time(std::chrono::system_clock::now())
            {}

            std::string operator()() {
                //std::chrono::seconds raw = raw_seconds();

                if((previous_time - std::chrono::system_clock::now()).count() == 1) {
                    ++seconds;
                    previous_time = std::chrono::system_clock::now();
                }

                long long seconds = seconds % 60;
                long long minutes = seconds / 60;

                std::string result = std::to_string(minutes) + ":" + std::to_string(seconds);
                return result;
            }
        };*/
        /// Counts time since game start
        Timer timer;
        spSprite timer_sprite;
        spTextField timer_txt;

        void initialize_all(const spStage& stage, const Vector2& size, const Vector2& scales) {
            create_background(stage, size, 74 * scales.y);

            create_menu_button(Vector2(200 * scales.x, 60 * scales.y),
                               Vector2(size.x - 107 * scales.x, 35 * scales.y));

            create_timer(Vector2(140 * scales.x, 60 * scales.y),
                         Vector2(77 * scales.x, 35 * scales.y));

            Vector2 menu_size = Vector2(size.x * 3 / 5, size.y - 74 * scales.y);
            Vector2 menu_pos = Vector2(size.x / 5, 74 * scales.y);

            view = new GameView(background, size, scales);
            main_menu = new Menu(background, menu_size, menu_pos);
            settings_menu = new SettingsMenu(background, menu_size, menu_pos);
            guess_menu = new GuessMenu(background, menu_size, menu_pos);

            char_name_menu = new AddCharMenu(background, menu_size, menu_pos);
            char_answ_menu = new AddQuestionMenu(background, menu_size, menu_pos);
            char_find_menu = new FindCharMenu(background, menu_size, menu_pos);

            ResCapture::akinator = new GameModel::Akinator();

            init_flag = true;
        }

        void create_background(const spStage& stage, const Vector2& size, const float& add_size) {
            background = new ColorRectSprite();
            background->setSize(size);
            background->setColor(Color(0x27B296A0));
            background->setPosition(0.0, 0.0);

            spColorRectSprite add_back = new ColorRectSprite();
            add_back->setSize(Vector2(size.x, add_size));
            add_back->setColor(Color::DimGray);
            add_back->setPosition(0.0, -1.0);

            spColorRectSprite line = new ColorRectSprite();
            line->setSize(size.x, 3);
            line->setPosition(0.0, add_size - 2);
            line->setColor(Color::Black);

            add_back->addChild(line);
            background->addChild(add_back);
            stage->addChild(background);
        }

        void create_timer_txt(const Vector2& size, const Vector2& position) {
            timer_txt = create_text(size, position, ResCapture::text_style, timer());
            timer_txt->setSize(size);
            timer_txt->attachTo(background);
            timer_txt->attachTo(timer_sprite);
        }
        void create_timer(const Vector2& size, const Vector2& position) {
            timer_sprite = create_sprite(size, position, ResCapture::res.getResAnim("choice_button"));
            timer_sprite->setSize(size);
            create_timer_txt(size, size / 2);
            timer_sprite->attachTo(background);
        }
        void update_timer() {
            timer_txt->setText(timer());;
        }

        void create_menu_button(const Vector2& size, const Vector2& position) {
            menu_button = create_sprite(size, position, ResCapture::res.getResAnim("choice_button"));
            menu_button->setSize(size);

            spTextField txt = create_text(size, size / 2, ResCapture::text_style, "Main Menu");
            txt->setSize(size);
            txt->attachTo(menu_button);

            background->addChild(menu_button);
            back_to_menu();
        }

        /// add back to menu listener
        void back_to_menu() {
            menu_button->addClickListener([&](Event* e) {
                menu_button->removeAllEventListeners();
                ResCapture::game_state = State::StartMenu;
                this->back_to_game();
            });
        }
        /// add back to game listener
        void back_to_game() {
            menu_button->addClickListener([&](Event* e) {
                menu_button->removeAllEventListeners();
                ResCapture::game_state = State::GameMenu;
                this->back_to_menu();
            });
        }

        void add_event_keyboard(const spStage& stage) {
            KeyboardHandler& kb = ResCapture::keyboard_handler;
            string& name = ResCapture::name;
            stage->addEventListener(KeyEvent::KEY_DOWN, [&kb](Event* event) {
                if(ResCapture::game_state == State::AddCharacterMenu ||
                   ResCapture::game_state == State::AddCharacterName ||
                   ResCapture::game_state == State::FindCharMenu)
                {
                    SDL_Scancode code = ((KeyEvent*)event)->data->keysym.scancode;

                    if(code == SDL_SCANCODE_LSHIFT)
                       kb.shift_toggle = true;

                    else if(code >= SDL_SCANCODE_A && code <= SDL_SCANCODE_Z) {
                        if(kb.shift_toggle) {
                            kb.last_char = char(61 + code);
                        }
                        else {
                            kb.last_char = char(93 + code);
                        }

                        kb.char_inputted = true;
                    }
                    else if(code == SDL_SCANCODE_SPACE) {
                        kb.last_char = ' ';
                        kb.char_inputted = true;
                    }
                }
            });
            stage->addEventListener(KeyEvent::KEY_UP, [&kb](Event* event) {
                if(((KeyEvent*)event)->data->keysym.scancode == SDL_SCANCODE_LSHIFT)
                    kb.shift_toggle = false;
            });
            stage->addEventListener(KeyEvent::KEY_DOWN, [&kb, &name](Event* event) {
                if(ResCapture::game_state == State::AddCharacterMenu ||
                        ResCapture::game_state == State::AddCharacterName ||
                        ResCapture::game_state == State::FindCharMenu)
                    if(((KeyEvent*)event)->data->keysym.scancode == SDL_SCANCODE_BACKSPACE)
                        if(!name.empty()) {
                            name.pop_back();
                            kb.char_destroyed = true;
                        }
            });
        }

    public:
        GamePresenter(const spStage& stage, const Vector2& display_size, const Vector2& scales)
            : oxygine::EventDispatcher(),
              timer(Timer())
        {
            initialize_all(stage, display_size, scales);
            add_event_keyboard(stage);
        }

        void update() {
            if(ResCapture::game_state == State::GameMenu ||
               ResCapture::game_state == State::Restart)
            {
                view->update();

                if(ResCapture::game_state == State::Restart)
                    timer.reload();

                if(!view->is_shown) {
                    view->show_all();

                    main_menu->hide_all();
                    settings_menu->hide_all();
                    guess_menu->hide_all();
                    char_answ_menu->hide_all();
                    char_name_menu->hide_all();
                    char_find_menu->hide_all();
                }

                update_timer();
            }

            else if(ResCapture::game_state == State::StartMenu ||
                    ResCapture::game_state == State::EndGameMenu)
            {
                main_menu->update();

                if(!main_menu->is_shown) {
                    main_menu->show_all();

                    view->hide_all();
                    settings_menu->hide_all();
                    guess_menu->hide_all();
                    char_answ_menu->hide_all();
                    char_name_menu->hide_all();
                    char_find_menu->hide_all();
                }
            }

            else if(ResCapture::game_state == State::SettingsMenu) {
                settings_menu->update();

                if(!settings_menu->is_shown) {
                    settings_menu->show_all();

                    main_menu->hide_all();
                    view->hide_all();
                    guess_menu->hide_all();
                    char_answ_menu->hide_all();
                    char_name_menu->hide_all();
                    char_find_menu->hide_all();
                }
            }

            else if(ResCapture::game_state == State::GuessMenu) {
                guess_menu->update();

                if(!guess_menu->is_shown) {
                    guess_menu->show_all();

                    main_menu->hide_all();
                    view->hide_all();
                    settings_menu->hide_all();
                    char_answ_menu->hide_all();
                    char_name_menu->hide_all();
                    char_find_menu->hide_all();
                }
            }

            /// questions!
            else if(ResCapture::game_state == State::AddCharacterMenu) {
                char_answ_menu->update();

                if(!char_answ_menu->is_shown) {
                    char_answ_menu->show_all();

                    main_menu->hide_all();
                    view->hide_all();
                    settings_menu->hide_all();
                    guess_menu->hide_all();
                    char_name_menu->hide_all();
                    char_find_menu->hide_all();
                }
            }

            else if(ResCapture::game_state == State::AddCharacterName) {
                char_name_menu->update();

                if(!char_name_menu->is_shown) {
                    char_name_menu->show_all();

                    main_menu->hide_all();
                    view->hide_all();
                    settings_menu->hide_all();
                    guess_menu->hide_all();
                    char_answ_menu->hide_all();
                    char_find_menu->hide_all();
                }
            }

            else if(ResCapture::game_state == State::FindCharMenu) {
                if(!char_find_menu->is_shown) {
                    char_find_menu->show_all();

                    main_menu->hide_all();
                    view->hide_all();
                    settings_menu->hide_all();
                    guess_menu->hide_all();
                    char_answ_menu->hide_all();
                    char_name_menu->hide_all();
                }

                char_find_menu->update();

            }
        }

        bool game_over_flag() const { return view->game_over_flag(); }
    };
}
#endif // GamePresenter_HPP
