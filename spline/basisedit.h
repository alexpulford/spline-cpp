#ifndef BASISEDIT_H
#define BASISEDIT_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QKeyEvent>

#include "spline.h"
#include "controller.h"

namespace Ui {
class BasisEdit;
}

class BasisEdit : public QWidget
{
    Q_OBJECT

public:
    explicit BasisEdit(Spline* spline, Controller* controller, QWidget *parent = nullptr);
    ~BasisEdit();

private:
    Ui::BasisEdit *ui;
    Spline* s;
    Controller* c;

    void populateLists();
    void enableBasis(int basis);
    void enableSpan(int span);
    void enablePolynomial(int basis, int span);
    void keyPressEvent(QKeyEvent* e);
public slots:
    void changed();
    void changedSelection();
signals:
    void change();
};

#endif // BASISEDIT_H
