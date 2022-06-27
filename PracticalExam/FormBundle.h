#pragma once
#include "service.h"

struct FormBundle {
	User user;
	Subject* globalSubject;
	UserService* userSvc;
	QuestionService* questionSvc;
	AnswerService* answerSvc;
};