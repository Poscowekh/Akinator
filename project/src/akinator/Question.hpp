#ifndef QUESTION_HPP
#define QUESTION_HPP
#include <string>
#include <utility>

using namespace std;
namespace GameModel {
struct Question {
    /// Sum of all positive answers to this question in database
    double positive_factor;

    /// Sum of all negative answer to this question in database *(-1)
    double negative_factor;

    /// Changes to true if question was used in game
    bool used;

    /// Unique question id
    long long id;

    /// Actual question text
    string text;


    Question(const long long& id_, const string& text_)
        : id(id_),
          text(text_),
          used(false),
          positive_factor(0.0),
          negative_factor(0.0)
    {};
    Question(const long long& id_, string&& text_)
        : id(id_),
          text(forward<string&&>(text_)),
          used(false),
          positive_factor(0.0),
          negative_factor(0.0)
    {};

    Question() = default;
    Question(const Question&) = default;
    Question(Question&&) = default;


    Question& operator=(const Question&) = default;
    Question& operator=(Question&&) = default;
};
}

#endif // QUESTION_HPP
