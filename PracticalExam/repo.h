#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "domain.h"
using namespace std;

template<class T>
class Repository {
protected:
	string filename;
	vector<T> ents;

	void _load() {
		ifstream f(filename);
		if (!f.is_open())
			return;
		while (!f.eof()) {
			T entity;
			f >> entity;
			ents.push_back(entity);
		}
	}

	void _save() {
		ofstream f(filename);
		for (int i = 0; i < ents.size(); i++) {
			f << ents[i];
			if (i < ents.size() - 1)
				f << endl;
		}
	}

	virtual string _getKey(T ent) = 0;

public:

	Repository(string fn) {
		filename = fn;
		_load();
	}

	void add(T ent) {
		auto it = find_if(ents.begin(), ents.end(),
			[&](T e) { return _getKey(e) == _getKey(ent); });
		if (it != ents.end())
			throw std::exception();
		ents.push_back(ent);
		_save();
	}

	void remove(T ent) {
		auto it = find_if(ents.begin(), ents.end(),
			[&](T e) { return _getKey(e) == _getKey(ent); });
		if (it == ents.end())
			throw std::exception();
		ents.erase(it);
	}

	void update(T ent) {
		auto it = find_if(ents.begin(), ents.end(),
			[&](T e) { return _getKey(e) == _getKey(ent); });
		*it = ent;
	}

	T find(string key) {
		auto it = find_if(ents.begin(), ents.end(),
			[&](T e) { return _getKey(e) == key; });
		return *it;
	}

	vector<T> get_all() {
		return ents;
	}
};

class UserRepository : public Repository<User> {
	string _getKey(User u) {
		return u.getName();
	}
public:
	UserRepository(string fn) : Repository(fn) {}
};

class QuestionRepository : public Repository<Question> {
	string _getKey(Question q) {
		return to_string(q.getId());
	}
public:
	QuestionRepository(string fn) : Repository(fn) {}
};

class AnswerRepository : public Repository<Answer> {
	string _getKey(Answer a) {
		return to_string(a.getId());
	}
public:
	AnswerRepository(string fn) : Repository(fn) {}
};
