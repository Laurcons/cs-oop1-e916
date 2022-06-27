#pragma once
#include "repo.h"
#include "Observer.h"

class UserService {
	UserRepository* userRepo;
public:
	UserService(Subject* gs, UserRepository* ur) : userRepo(ur) {}

	vector<User> get_all() {
		return userRepo->get_all();
	}
};

class QuestionService {
	QuestionRepository* questionRepo;
	AnswerRepository* answerRepo;
	Subject* globalSubject;
public:
	QuestionService(Subject* gs, QuestionRepository* ur, AnswerRepository* ar) : globalSubject(gs), questionRepo(ur), answerRepo(ar) {}

	vector<Question> get_all() {
		return questionRepo->get_all();
	}

	vector<Question> get_all_sorted() {
		auto all = questionRepo->get_all();
		sort(all.begin(), all.end(), [&](Question q1, Question q2) {
			return get_answers_for(q1).size() > get_answers_for(q2).size();
			});
		return all;
	}

	vector<Answer> get_answers_for(Question q) {
		auto all = answerRepo->get_all();
		vector<Answer> out;
		copy_if(all.begin(), all.end(), back_inserter(out), [&](Answer a) { return a.getQuestionId() == q.getId(); });
		return out;
	}

	void add(string text, User user) {
		int id = questionRepo->get_all().size() + 1;
		Question q{ id, text, user.getName() };
		questionRepo->add(q);
		globalSubject->notifyAll();
	}
};

class AnswerService {
	AnswerRepository* answerRepo;
	Subject* globalSubject;
public:
	AnswerService(Subject* gs, AnswerRepository* ur) :globalSubject(gs), answerRepo(ur) {}

	vector<Answer> get_all() {
		return answerRepo->get_all();
	}

	void add(Question q, User user, string text) {
		int id = answerRepo->get_all().size() + 1;
		answerRepo->add(Answer{
			id,
			q.getId(),
			text,
			user.getName(),
			0
		});
		globalSubject->notifyAll();
	}

	void updateVoteCount(Answer a, int newVotes) {
		a.setVotes(newVotes);
		answerRepo->update(a);
		globalSubject->notifyAll();
	}
};

