#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include "DataBase.hpp"

namespace GameModel {
// Main algorithms for akinator are contained here

/// Returns computed rating of character by comparing answers
static double rating(const Character& character, const vector<Answer>& user_answers) {
    double rating = 0;

    for(const auto& answer : character.answers)
        for(const auto& user_answer : user_answers)
            if(answer.question_id == user_answer.question_id)
                rating += answer.answer_value * user_answer.answer_value;

    /*
    auto char_answer_iter = character.answers.begin();
    auto char_answer_end = character.answers.end();
    auto partial_answer_iter = user_answers.begin();
    auto partial_answer_end = user_answers.end();

    while(char_answer_iter != char_answer_end &&
          partial_answer_iter != partial_answer_end)
    {
        if(char_answer_iter->question_id < partial_answer_iter->question_id)
            ++char_answer_iter;
        else if(char_answer_iter->question_id > partial_answer_iter->question_id)
            ++partial_answer_iter;
        else {
            rating += char_answer_iter->answer_value * partial_answer_iter->answer_value;
            ++char_answer_iter;
            ++partial_answer_iter;
        };
    };*/

    return rating;
};


/// Similarity computation function helper.
/// Marks all questions with answers in user_answers as used
static void _mark_questions(vector<Question>& questions, const vector<Answer>& user_answers) {
    for(auto& question : questions) {
        question.used = false;

        for(const auto& answer : user_answers)
            if(question.id == answer.question_id)
                question.used = true;
    };

    /*
    auto quest_iter = questions.begin();
    auto quest_end = questions.end();
    auto answ_iter = user_answers.begin();
    auto answ_end = user_answers.end();

    while(quest_iter != quest_end && answ_iter != answ_end) {
        if(quest_iter->id < answ_iter->question_id ||
                quest_iter->id > answ_iter->question_id)
        {
            quest_iter->used = false;
            ++quest_iter;
        } else {
            quest_iter->used = true;
            ++quest_iter;
            ++answ_iter;
        };
    };*/
};

/*
 * Computes rating of every character in data_base using user answers as well as
 * marks questions as used if they are contained in user_answers
 */
/// Updates rating of characters in data_base
static void compute_rating(DataBase& data_base, const vector<Answer>& user_answers) {
    for(auto& character : data_base.characters())
        character.rating = rating(character, user_answers);

    _mark_questions(data_base.questions(), user_answers);
};


/// If character's rating >= threshold,
/// assigns positive and negative factors for questions in data_base
static void compute_bisection_factor(DataBase& data_base, const double& threshold) {
    // reset factors
    for(auto& question : data_base.questions())
        question.positive_factor = question.negative_factor = 0.0;

    for(const auto& character : data_base.characters())
        if(character.rating >= threshold)
            for(const auto& answer : character.answers) {
                if(answer.answer_value > 0)
                    data_base.questions()[answer.question_id - 1].positive_factor += answer.answer_value;
                else
                    data_base.questions()[answer.question_id - 1].negative_factor -= answer.answer_value;
            };
}

static Question best_question(DataBase& data_base) {
    double Z = -1.0;
    long long index = 0;
    long long best_index = -1;

    // evaluate Z as max possible sum of question factors
    for(const auto& question : data_base.questions()) {
        if(!question.used)
            if(Z < question.positive_factor + question.negative_factor){
                Z = question.positive_factor + question.negative_factor;
                best_index = index;
            };
            ++index;
    }

    if(best_index == -1)
        return Question(0,"");

    return data_base.questions()[best_index];
}


/*
 * Z is max possible sum of positive and negative question factors.
 * Then for every not used question
 * rating is the distance from point(Z/2, Z/2)
 * to point (this question's positive_factor, this question's negative_factor)
 * as distance in euclidean metrics multiplied by (-1)
 */
/// Returns list of pairs of question ids and ratings ordered by rating
static vector<pair<long long, double> > rank_questions(DataBase& data_base) {
    auto result = vector<pair<long long, double> >();
    double Z = 0;
    size_t count = 0;

    // evaluate Z as max possible sum of question factors
    for(const auto& question : data_base.questions())
        if(!question.used) {
            Z = max(Z, question.positive_factor + question.negative_factor);
            ++count;
        };

    result.reserve(count);
    auto iter = result.begin();
    Z /= 2;

    for(const auto& question : data_base.questions())
        if(!question.used)
            result.insert(iter++,  make_pair(question.id,
                                             hypot(Z - question.positive_factor,
                                                   Z - question.negative_factor)));

    sort(result.begin(), result.end(), [](const auto& first, const auto& second) {
        return first.second < second.second;
    });

    return result;
};
}
#endif // ALGORITHM_HPP
