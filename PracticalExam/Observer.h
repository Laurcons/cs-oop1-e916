#pragma once

class Observer {
public:
	virtual void notify() = 0;
};

class Subject {
	vector<Observer*> observers;
public:
	void registerObserver(Observer* o) {
		observers.push_back(o);
	}
	void notifyAll() {
		for (auto o : observers)
			o->notify();
	}
};