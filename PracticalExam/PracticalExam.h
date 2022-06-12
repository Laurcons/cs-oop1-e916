#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PracticalExam.h"

class PracticalExam : public QMainWindow
{
    Q_OBJECT

public:
    PracticalExam(QWidget *parent = Q_NULLPTR);

private:
    Ui::PracticalExamClass ui;
};
