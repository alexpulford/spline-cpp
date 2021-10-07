#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "DockManager.h"
#include "spline.h"
#include "controller.h"

#include "basisedit.h"
#include "basisview.h"
#include "knotedit.h"
#include "curvatureview.h"
#include "splineparam.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setStatus(QString new_status, int timeout=0);
signals:
    void updateOut();
public slots:
    void updateIn();
private:
    QString status;
    Ui::MainWindow *ui;

    ads::CDockManager* manager;

    Controller c;
    Spline s;
    ads::CDockWidget* basisView;
    ads::CDockWidget* basisEdit;
    ads::CDockWidget* curveView;
    ads::CDockWidget* knotEdit;
    ads::CDockWidget* splineParam;
private slots:
    void basisViewToggle(bool state);
    void basisEditToggle(bool state);
    void knotEditToggle(bool state);
    void curvatureViewToggle(bool state);
    void splineParamToggle(bool state);





};
#endif // MAINWINDOW_H
