#ifndef AKINATOR_HPP
#define AKINATOR_HPP
#include "Algorithm.hpp"
#include <float.h>
#include <random>
#include <chrono>
#include <oxygine-framework.h>

namespace GameModel{
/// Main akinator class. Provides most of functionality
class Akinator : public oxygine::Actor {
// is set to protected for testing
public:
    enum State {
        // Additional answers needed
        AskQuestion,

        // Ask user if character with biggest rating is right guess
        Guess,

        // All answers given by user do not resemble any known character
        // so user is asked to add this character or add answers to character
        // if they were not previously known
        GiveUp,

        // Helper state to ask user for new question
        // when akinator gave up and needs to fill data_base
        AskNewQuestion,

        // Game restart initiated
        Restart,

        // All game functions must stop
        Stop
    };

protected:
    /// Database of characters and questions
    DataBase data_base;

    /// Current state of the game
    State current_state;

    /// Answers given by user
    vector<Answer> user_answers;

    /// How much bigger must character's rating be to be considered a good guess
    double guess_threshold;

    /// lHow much bigger must character's rating be to have it's rating recomputed
    double compute_threshold;

    /// Current iteration akinator is at
    long long current_iteration;

    /// Is used as temporary storage of data when creating an update for database
    Character new_character;

    /// Mersenne twister random number generator used in evaluations
    mt19937 randomizer;

    /// Is set to true if during all games at least one new character
    /// was added or some character was given additional answers
    bool server_update_created;

    /// Currently computed best_guesses
    vector<Character> best_guesses_;

    /// Current wrong answer
    vector<Character> bad_guesses;


    /// Initiates computation of ratings
    void recompute_stats() {
        if(user_answers.empty())
            return;

        compute_rating(data_base, user_answers);

        double minimum = DBL_MAX;
        double maximum = DBL_MIN;
        for(const auto& character : data_base.characters()) {
            minimum = min(minimum, character.rating);
            maximum = max(maximum, character.rating);
        };

        compute_threshold = minimum * guess_threshold + maximum * (1 - guess_threshold);

        compute_bisection_factor(data_base, compute_threshold);
    };


    /// Adds character to local storage of updates to server
    /// that should be sent at initialisation of game as whole.
    /// Simply speaking just creates a nlohmann::json file or adds data to it
    void add_to_data_base(const Character& character);


    /// Recomputes stats and returns characters with best ratings
    vector<Character>& best_guesses() {
        recompute_stats();

        best_guesses_.clear();

        for(const auto& character : data_base.characters())
            if(character.rating > compute_threshold) {
                bool flag = false;

                for(const auto& bad_guess : bad_guesses)
                    if(bad_guess == character)
                        flag = true;

                if(!flag)
                    best_guesses_.push_back(character);
            }

        return best_guesses_;
    }
    const vector<Character>& current_best() const { return best_guesses_; }

    /// Returns true if leader is better than others in best_quesses_
    bool leader_is_good() const {
        return best_guesses_.front().rating > (++best_guesses_.begin())->rating + 1.5 * guess_threshold;
    }

public:
    Akinator()
        : current_iteration(0),
          guess_threshold(0.5),
          compute_threshold(0.0),
          data_base(DataBase()),
          current_state(AskQuestion),
          user_answers(vector<Answer>()),
          new_character(Character()),
          server_update_created(false),
          best_guesses_(vector<Character>()),
          randomizer(mt19937())
    {
        randomizer.seed(chrono::system_clock::now().time_since_epoch().count());
    };

    Akinator(const Akinator&) = default;
    Akinator(Akinator&&) = default;

    /// Should be used by main program when initialising game
    Akinator(const nlohmann::json& questions, const nlohmann::json& characters)
        : Akinator()
    {
        data_base = DataBase(questions, characters);
    };


    Akinator& operator=(const Akinator&) = default;
    Akinator& operator=(Akinator&&) = default;


    /// Sets akinator flag to Stop
    void stop() { current_state = State::Stop; }

    /// Should be called each restart
    void restart() {
        user_answers.clear();
        bad_guesses.clear();
        best_guesses_.clear();
        current_state = AskQuestion;
        data_base.clear();
        guess_threshold = 0.5;
        compute_threshold = 0.0;
        recompute_stats();
    }


    /// Current state of game
    const State& state() const { return current_state; }

    /// Current game tick akinator is at
    const long long& iteration() const { return current_iteration; }

    /// True if a nlohmann::json file was created and server needs to get it
    bool data_base_updated() const { return server_update_created; }


    /// Return leading character
    Character guess() {
        recompute_stats();

        double best = DBL_MIN;
        size_t index = 0;
        long long best_index = -1;

        for(const auto& character : data_base.characters()) {
            if(character.rating > best) {
                bool flag = true;

                for(const auto& bad_guess : bad_guesses)
                    if(bad_guess.id == character.id) {
                        flag = false;
                        break;
                    }

                if(flag) {
                    best = character.rating;
                    best_index = index;
                }
            };

            ++index;
        };

        if(best_index == -1) {
            return Character(0,"",vector<Answer>());
            this->current_state = State::GiveUp;
        }

        return data_base.characters()[best_index];
    };
    /// Return name of leading character
    const string& guess_name() { return guess().name; }


    /// Retutn question from data_base using questions id
    const Question& question_by_id(const long long& id) {
        for(const auto& question : data_base.questions())
            if(question.id == id)
                return question;

        // not called ever
        return data_base.questions().front();
    };
    /// Returns on of the best bisecting questions
    Question next_question() {
        recompute_stats();

        guess_threshold *= 0.85;


        Question question = best_question(data_base);
        if(question.text.empty())
            current_state = Guess;

        return question;


        auto ranked_questions = rank_questions(data_base);

        // no more questions are possible to ask so akinator
        // should give up or try best guess
        if(ranked_questions.empty()) {
            current_state = Guess;
            return Question(0, "");
        };

        double shift = ranked_questions.back().second;
        auto chances = vector<double>();
        chances.reserve(ranked_questions.size());
        auto iter = chances.begin();

        for(const auto& question : ranked_questions)
            chances.insert(iter++, question.second - shift);

        // randomly distributes chances
        discrete_distribution<size_t> dist(chances.begin(), chances.end());

        // we take a random question this way
        return question_by_id(ranked_questions[dist(randomizer)].first);
    };


    void receive_answer(const double& value, const Question& question) {
        user_answers.push_back(Answer(question.id, value));

        sort(user_answers.begin(), user_answers.end(), [](const Answer& first, const Answer& second) {
            return first.question_id < second.question_id;
        });
    };

    void bad_guess(const Character& bad_char) {
        bad_guesses.push_back(bad_char);
    }


    /// Decides upon next state of game
    void next_state() {
        if(current_state == AskQuestion) {
            best_guesses();
            if(current_iteration > 5 && (current_best().size() == 1 || questions_remaining() == 0 ||
                                         (current_best().size() > 1 && leader_is_good())))
                current_state = Guess;
            else
                current_state = AskQuestion;
        }

        else if(current_state == Guess) {
            if(questions_remaining() == 0)
                // no more ways to guess
                current_state = GiveUp;
            else {
                // trying next question
                guess_threshold = min(3 * guess_threshold, 0.5);
                current_state = AskQuestion;
            };
        }

        else if(current_state == GiveUp) {
            best_guesses();
            if(data_base.questions().size() > user_answers.size() &&
                    current_best().size() > 0 && data_base.characters().size() > bad_guesses.size())
                current_state = Guess;
        }

        else if(current_state == AskNewQuestion ||
                current_state == Restart ||
                current_state == Stop)
            current_state = Stop;

        else
            throw std::runtime_error("undefined akinator state");

        ++current_iteration;
    }


    /// Returns matches by name from data_base characters.
    /// Is used when akinator gave up and database needs update
    vector<Character> find_matches(const string& name) {
        auto result = vector<Character>();

        for(const auto& character : data_base.characters())
            if(character.name.find(name) != string::npos)
                result.push_back(character);

        return result;
    };

    /// Is used when akinator gave up and user informed game
    /// that the character is not in its database
    void add_new_character(const string& name) {}

    /// Add info to new character
    void add_question(const string& text, const double& answer_value) {

    };

    /// Adds data to existing character
    void add_info_to_character(const string& char_name,
                               const string& question_text,
                               const double& answer_value)
    {

    };


    /// Returns count of all questions akinator may ask at this iteration
    size_t questions_remaining() const { return data_base.questions().size() - user_answers.size(); }


    auto questions() { return data_base.questions(); }
    auto characters() { return data_base.characters(); }
    const vector<Answer>& usr_answers() const { return user_answers; }
};
}


#endif // AKINATOR_HPP
