#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new ads::CDockManager(this);

    ads::CDockManager::setConfigFlag(ads::CDockManager::ActiveTabHasCloseButton, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::AllTabsHaveCloseButton, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasCloseButton, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasUndockButton, false);

    s = Spline();
    c = Controller();

    {
        basisEdit = new ads::CDockWidget("Basis Data");
        basisEdit->setWidget(new BasisEdit(&s, &c));
        manager->addDockWidget(ads::LeftDockWidgetArea, basisEdit);
    }

    {
        knotEdit = new ads::CDockWidget("Knot Data");
        knotEdit->setWidget(new KnotEdit(&s));
        manager->addDockWidget(ads::RightDockWidgetArea, knotEdit);
    }

    {
        basisView = new ads::CDockWidget("Basis View");
        basisView->setWidget(new BasisView(&s, &c));
        manager->addDockWidget(ads::TopDockWidgetArea, basisView);
    }

    {
        curveView = new ads::CDockWidget("Curvature View");
        curveView->setWidget(new CurvatureView(&s, &c));
    }

    {
        splineParam = new ads::CDockWidget("Spline Parameters");
        splineParam->setWidget(new SplineParam(&s, &c));
    }



    connect((BasisView*)basisView->widget(), &BasisView::change, this, &MainWindow::updateIn);
    connect((BasisEdit*)basisEdit->widget(), &BasisEdit::change, this, &MainWindow::updateIn);
    connect((KnotEdit*)knotEdit->widget(), &KnotEdit::change, this, &MainWindow::updateIn);
    connect((SplineParam*)splineParam->widget(), &SplineParam::change, this, &MainWindow::updateIn);


    connect(this, &MainWindow::updateOut, (BasisView*)basisView->widget(), &BasisView::changed);
    connect(this, &MainWindow::updateOut, (BasisEdit*)basisEdit->widget(), &BasisEdit::changed);
    connect(this, &MainWindow::updateOut, (KnotEdit*)knotEdit->widget(), &KnotEdit::changed);
    connect(this, &MainWindow::updateOut, (CurvatureView*)curveView->widget(), &CurvatureView::changed);
    connect(this, &MainWindow::updateOut, (SplineParam*)splineParam->widget(), &SplineParam::changed);

    connect(basisEdit, &ads::CDockWidget::viewToggled, ui->actionBasis_Edit, &QAction::toggle);


}

void MainWindow::basisViewToggle(bool state) {
    if(state)
        manager->addDockWidgetTab(ads::TopDockWidgetArea, basisView);
    else
        manager->removeDockWidget(basisView);
}
void MainWindow::basisEditToggle(bool state) {
    if(state)
        manager->addDockWidgetTab(ads::LeftDockWidgetArea, basisEdit);
    else
        manager->removeDockWidget(basisEdit);

}
void MainWindow::knotEditToggle(bool state) {
    if(state)
        manager->addDockWidgetTab(ads::RightDockWidgetArea, knotEdit);
    else
        manager->removeDockWidget(knotEdit);

}
void MainWindow::curvatureViewToggle(bool state) {
    if(state)
        manager->addDockWidgetTab(ads::TopDockWidgetArea, curveView);
    else
        manager->removeDockWidget(curveView);
}

void MainWindow::splineParamToggle(bool state) {
    if(state)
        manager->addDockWidgetTab(ads::TopDockWidgetArea, splineParam);
    else
        manager->removeDockWidget(splineParam);
}

void MainWindow::updateIn() {
    updateOut();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatus(QString new_status, int timeout) {
    this->statusBar()->showMessage(new_status, timeout);
}
