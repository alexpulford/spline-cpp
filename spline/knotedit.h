#ifndef KNOTEDIT_H
#define KNOTEDIT_H

#include <QWidget>

#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QKeyEvent>

#include "spline.h"

namespace Ui {
class KnotEdit;
}

class KnotEdit : public QWidget
{
    Q_OBJECT
public:
    explicit KnotEdit(Spline* spline, QWidget *parent = nullptr);
    ~KnotEdit();
    Spline* s;
    void populateList();
    void keyPressEvent(QKeyEvent* e);
    void removeAt(int index);
    void addAt(int index);
    void nudge(int index, int dir);
public slots:
    void changed();
    void knotEdit(QListWidgetItem* index);
    void contextMenu(QPoint point);
    void remove();
    void add();
    void uniform();
    void open();
    void close();
private:
    QAction* addAction;
    QAction* removeAction;
    QAction* uniformAction;
    QAction* openAction;
    QAction* closeAction;
    void recompute();
signals:
    void change();
private:
    Ui::KnotEdit *ui;
};

#endif // KNOTEDIT_H
