#ifndef SPLINEPARAM_H
#define SPLINEPARAM_H

#include <QWidget>
#include "spline.h"
#include "controller.h"

namespace Ui {
class SplineParam;
}

class SplineParam : public QWidget
{
    Q_OBJECT

public:
    explicit SplineParam(Spline* spline, Controller* controller, QWidget *parent = nullptr);
    ~SplineParam();
private:
    Spline* s;
    Controller* c;
    Ui::SplineParam *ui;
public slots:
    void changed();
    void orderChanged(int order);
signals:
    void change();
};

#endif // SPLINEPARAM_H
