#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "Answer.hpp"
#include <string>
#include <vector>
#include <utility>

using namespace std;
namespace GameModel {
struct Character {
    /// Name
    string name;

    /* Answers that players had given to this entity.
     * Note that this list almost surely is incomplete;
     * that is, not all questions in the database have answers here.
     *
     * Invariant: the Answer::question pointers are in increasing order.
     */

    /// Existing answers of this character to some questions sorted by question ids
    vector<Answer> answers;

    /// Computed rating of this character to existing answers given by user
    double rating;

    /// This character's unique id
    long long id;


    Character(const long long& id_, const string& name_, const vector<Answer>& answers_)
        : id(id_),
          name(name_),
          answers(answers_),
          rating(0.0)
    {};
    Character(const long long& id_, string&& name_, vector<Answer>&& answers_)
        : id(id_),
          name(forward<string&&>(name_)),
          answers(forward<vector<Answer>&&>(answers_)),
          rating(0.0)
    {};

    Character() = default;
    Character(const Character&) = default;
    Character(Character&&) = default;


    Character& operator=(const Character&) = default;
    Character& operator=(Character&&) = default;


    bool operator==(const Character& other) const {
        return id == other.id;
    }
};
}

#endif // CHARACTER_HPP
