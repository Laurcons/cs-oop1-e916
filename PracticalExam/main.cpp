#include "PracticalExam.h"
#include "service.h"
#include "UserForm.h"
#include "SearchForm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Subject globalSubject;
    UserRepository userRepo{ "users.txt" };
    QuestionRepository questionRepo{ "questions.txt" };
    AnswerRepository answerRepo{ "answers.txt" };
    UserService userSvc{ &globalSubject, &userRepo };
    QuestionService questionSvc{ &globalSubject, &questionRepo, &answerRepo };
    AnswerService answerSvc{ &globalSubject, &answerRepo };

    vector<QWidget*> forms;
    for (auto u : userSvc.get_all()) {
        auto form = new UserForm{
            FormBundle{ u, &globalSubject, &userSvc, &questionSvc, &answerSvc }
        };
        form->show();
        forms.push_back(form);
    }
    auto searchForm = new SearchForm{
        FormBundle{ User{""}, &globalSubject, &userSvc, &questionSvc, &answerSvc}
    };
    searchForm->show();
    forms.push_back(searchForm);

    auto ret = a.exec();
    for (auto qw : forms)
        delete qw;
    return ret;
}
