#pragma once
#include <string>
#include <istream>
#include <ostream>
using namespace std;

class User {
	string name;
public:
	string getName() { return name; }

	User() {}
	User(string n) : name(n) {}

	friend istream& operator>>(istream& is, User& u) {
		getline(is, u.name, '\n');
		return is;
	}
	friend ostream& operator<<(ostream& os, User& u) {
		os << u.name;
		return os;
	}
};

class Question {
	int id;
	string text;
	string user;
public:
	int getId() { return id; }
	string getText() { return text; }
	string getUser() { return user; }

	Question() {}
	Question(int id, string text, string user) : id(id), text(text), user(user) {}

	int similarityCount(string term) {
		auto pos = text.find(term);
		if (pos == string::npos)
			return 0;
		int osize = 0;
		int tp = 0;
		while (tp < term.size() && text[pos++] == term[tp++]) {
			osize++;
		}
		return osize;
	}

	friend istream& operator>>(istream& is, Question& q) {
		string temp;
		getline(is, temp, ',');
		q.id = stoi(temp);
		getline(is, q.text, ',');
		getline(is, q.user);
		return is;
	}
	friend ostream& operator<<(ostream& os, Question& q) {
		os
			<< q.id << ','
			<< q.text << ','
			<< q.user;
		return os;
	}
};

class Answer {
	int id;
	int questionId;
	string user;
	string text;
	int votes;
public:
	int getId() { return id; }
	int getQuestionId() { return questionId; }
	string getText() { return text; }
	string getUser() { return user; }
	int getVotes() { return votes; }
	Answer& setVotes(int v) { votes = v; return *this; }

	Answer() {}
	Answer(
		int id,
		int qId,
		string text,
		string user,
		int votes
	) :
		id(id),
		questionId(qId),
		text(text),
		user(user),
		votes(votes)
	{}

	friend istream& operator>>(istream& is, Answer& a) {
		string temp;
		getline(is, temp, ',');
		a.id = stoi(temp);
		getline(is, temp, ',');
		a.questionId = stoi(temp);
		getline(is, a.user, ',');
		getline(is, a.text, ',');
		getline(is, temp);
		a.votes = stoi(temp);
		return is;
	}
	friend ostream& operator<<(ostream& os, Answer& a) {
		os
			<< a.id << ','
			<< a.questionId << ','
			<< a.user << ','
			<< a.text << ','
			<< a.votes;
		return os;
	}
};
