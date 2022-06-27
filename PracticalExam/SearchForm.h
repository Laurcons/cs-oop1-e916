#pragma once
#include "ui_SearchForm.h"
#include "FormBundle.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeView>
#include <QList>
#include <sstream>

class SearchForm : public QWidget, public Observer {
	Q_OBJECT

public:
	SearchForm(FormBundle bundle, QWidget* parent = Q_NULLPTR) : b(bundle) {
		ui.setupUi(this);
		QWidget::connect(
			ui.lineEdit,
			&QLineEdit::textChanged,
			this,
			&SearchForm::onLineEditChanged
		);
		updateList();
		b.globalSubject->registerObserver(this);
	}

	void onLineEditChanged() {
		updateList();
	}

	void updateList() {
		ui.listWidget->clear();
		questions = b.questionSvc->get_all();
		auto searchTerm = ui.lineEdit->text().toStdString();
		sort(
			questions.begin(), questions.end(),
			[&](Question q1, Question q2) { return q1.similarityCount(searchTerm) > q2.similarityCount(searchTerm); }
		);
		auto it = questions.begin();
		auto q = *it;
		if (q.similarityCount(searchTerm) == 0)
			return;
		stringstream ss;
		ss << "\"" << q.getText() << "\" "
			<< "by " << q.getUser() << " "
			<< "(" << b.questionSvc->get_answers_for(q).size() << " answers)";
		ui.listWidget->addItem(ss.str().c_str());
		auto answers = b.questionSvc->get_answers_for(q);
		sort(
			answers.begin(), answers.end(),
			[&](Answer a1, Answer a2) { return a1.getVotes() > a2.getVotes(); }
		);
		for (int i = 0; i < 3 && i < answers.size(); i++) {
			auto a = answers[i];
			stringstream ss;
			ss << "\t\"" << a.getText() << "\" "
				<< "by " << a.getUser() << " "
				<< "with " << a.getVotes() << " votes";
			ui.listWidget->addItem(ss.str().c_str());
		}
	}

	void notify() {
		updateList();
	}

private:
	Ui_SearchForm ui;
	FormBundle b;
	vector<Question> questions;
};