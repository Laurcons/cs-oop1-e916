#pragma once
#include "ui_UserForm.h"
#include "FormBundle.h"
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QSpinBox>
#include <sstream>

class UserForm : public QWidget, public Observer {
	Q_OBJECT

public:
	UserForm(FormBundle bundle, QWidget* parent = Q_NULLPTR) : b(bundle) {
		ui.setupUi(this);
		this->setWindowTitle(("User: " + b.user.getName()).c_str());
		QWidget::connect(
			ui.addQuestionBtn,
			&QPushButton::clicked,
			this,
			&UserForm::onAddQuestionClick
		);
		QWidget::connect(
			ui.questionsWidget,
			&QListWidget::clicked,
			this,
			&UserForm::onQuestionClick
		);
		QWidget::connect(
			ui.addAnswerBtn,
			&QPushButton::clicked,
			this,
			&UserForm::onAddAnswerClick
		);
		QWidget::connect(
			ui.answersWidget,
			&QListWidget::clicked,
			this,
			&UserForm::onAnswerClick
		);
		QWidget::connect(
			ui.votesSpin,
			&QSpinBox::valueChanged,
			this,
			&UserForm::onVoteSpinClick
		);
		updateQuestions();
		b.globalSubject->registerObserver(this);
	}

	void notify() {
		updateQuestions();
		updateAnswers();
	}

	void updateQuestions() {
		ui.questionsWidget->clear();
		questions = b.questionSvc->get_all_sorted();
		for (auto q : questions) {
			stringstream ss;
			ss << "\"" << q.getText() << "\" "
				<< "by " << q.getUser() << " "
				<< "(" << b.questionSvc->get_answers_for(q).size() << " answers)";
			ui.questionsWidget->addItem(ss.str().c_str());
		}
	}

	void updateAnswers() {
		ui.answersWidget->clear();
		auto allQ = b.questionSvc->get_all();
		auto it = find_if(allQ.begin(), allQ.end(), [&](Question q) { return q.getId() == selectedQuestion.getId(); });
		if (it == allQ.end())
			return;
		answers = b.questionSvc->get_answers_for(selectedQuestion);
		for (auto a : answers) {
			stringstream ss;
			ss << "(" << a.getId() << ") "
				<< "\"" << a.getText() << "\" "
				<< "by " << a.getUser() << " "
				<< "with " << a.getVotes() << " votes";
			auto item = new QListWidgetItem(ss.str().c_str());
			if (a.getUser() == b.user.getName())
				item->setBackground(Qt::yellow);
			ui.answersWidget->addItem(item);
		}
	}

	void updateSpinBox() {
		ui.votesSpin->setValue(selectedAnswer.getVotes());
		ui.votesSpin->setEnabled(selectedAnswer.getUser() != b.user.getName());
	}

	void onVoteSpinClick() {
		int newval = ui.votesSpin->value();
		b.answerSvc->updateVoteCount(selectedAnswer, newval);
	}

	void onAnswerClick() {
		auto indexes = ui.answersWidget->selectionModel()->selectedIndexes();
		if (indexes.size() != 1)
			return;
		auto idx = indexes.first().row();
		selectedAnswer = answers[idx];
		updateSpinBox();
	}

	void onAddAnswerClick() {
		auto text = ui.answerLE->text().toStdString();
		if (text == "")
			return;
		b.answerSvc->add(selectedQuestion, b.user, text);
	}

	void onQuestionClick() {
		auto indexes = ui.questionsWidget->selectionModel()->selectedIndexes();
		if (indexes.size() != 1)
			return;
		auto idx = indexes.first().row();
		selectedQuestion = questions[idx];
		updateAnswers();
	}

	void onAddQuestionClick() {
		auto text = ui.questionLE->text().toStdString();
		if (text == "")
			return;
		b.questionSvc->add(text, b.user);
	}

private:
	Ui_UserForm ui;
	FormBundle b;
	vector<Question> questions;
	Question selectedQuestion{ -1, "", "" };
	vector<Answer> answers;
	Answer selectedAnswer{ -1, -1, "", "", -1};
};