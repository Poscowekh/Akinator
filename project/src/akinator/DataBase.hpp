#ifndef DATABASE_HPP
#define DATABASE_HPP
#include "Question.hpp"
#include "Character.hpp"
#include <nlohmann/json.hpp>

using namespace std;
namespace GameModel {
/// DataBase converted from json file
class DataBase {
private:
    vector<Question> questions_;
    vector<Character> characters_;

    /// Assumes json is already checked
    void init_questions(const nlohmann::json& questions) {
        questions_.reserve(questions.size());
        auto iter = questions_.begin();

        for(const auto& question : questions)
            questions_.insert(iter++, Question(question["id"], question["text"]));

        sort(questions_.begin(), questions_.end(), [](const Question& first, const Question& second) {
            if(second.id > first.id)
                return true;
            return false;
        });
    };

    /// Assimes json is already checked
    vector<Answer> init_answers(const nlohmann::json& answers) {
        auto result = vector<Answer>();
        result.reserve(answers.size());
        auto iter = result.begin();

        for(const auto& answer : answers)
            result.insert(iter++, Answer(answer["id"], answer["answer"]));

        sort(result.begin(), result.end(), [](const Answer& first, const Answer& second){
            if(second.question_id > first.question_id)
                return true;
            return false;
        });

        return result;
    };

    /// Assumes json is already checked
    void init_characters(const nlohmann::json& characters) {
        characters_.reserve(characters.size());
        auto iter = characters_.begin();

        for(const auto& character : characters) {
            auto answers = init_answers(character["answers"]);
            characters_.insert(iter++, Character(character["id"], character["name"], answers));
        };

        sort(characters_.begin(), characters_.end(), [](const Character& first, const Character& second) {
            if(second.id > first.id)
                return true;
            return false;
        });
    };

public:
    DataBase() = default;
    DataBase(const DataBase&) = delete;
    DataBase(DataBase&&) = default;

    DataBase(const nlohmann::json& questions, const nlohmann::json& characters)
        : DataBase()
    {
        init_questions(questions);
        init_characters(characters);
    };

    DataBase & operator=(const DataBase&) = delete;
    DataBase & operator=(DataBase&&) = default;

    void clear() {
        for(auto& character : characters_)
            character.rating = 0.0;

        for(auto& question : questions_) {
            question.negative_factor = question.negative_factor = 0.0;
            question.used = false;
        }
    }

    vector<Question>& questions() { return questions_; }
    const vector<Question>& questions() const { return questions_; }

    vector<Character>& characters() { return characters_; }
    const vector<Character>& characters() const { return characters_; }
};
}

#endif // DATABASE_HPP
