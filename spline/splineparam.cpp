#include "splineparam.h"
#include "ui_splineparam.h"

SplineParam::SplineParam(Spline* spline, Controller* controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplineParam), s(spline), c(controller)
{
    ui->setupUi(this);
    changed();
}


void SplineParam::changed() {
    auto valid = s->getValidSpan();
    if(s->isValid())
        ui->validLabel->setText(QString("%1,%2").arg(valid.x).arg(valid.y));
    else
        ui->validLabel->setText(QString("None"));

    ui->orderBox->setValue(s->order);
    ui->contLabel->setText(QString("%1").arg(s->getContinuity()));
    ui->knotLabel->setText(QString("%1").arg(s->t.size()));

}

void SplineParam::orderChanged(int order) {
    if(order != s->order) {
        s->computeBasis(order);
        change();
    }
}

SplineParam::~SplineParam()
{
    delete ui;
}
