#ifndef ANSWER_HPP
#define ANSWER_HPP

namespace GameModel {
/// answer to question (is held in character)
struct Answer {
    /// id of question this answer belongs to
    long long question_id; //or ptr?

    /// value between -1 and 1 indicating some character's answer to this question
    /// -1 for No and 1 for Yes
    double answer_value;


    Answer(const long long& question_id_,
           const double& value_)
        : question_id(question_id_),
          answer_value(value_)
    {};

    Answer() = default;
    Answer(const Answer&) = default;
    Answer(Answer&&) = default;


    Answer& operator=(const Answer&) = default;
    Answer& operator=(Answer&&) = default;
};
}

#endif // ANSWER_HPP
