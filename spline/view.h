#ifndef VIEW_H
#define VIEW_H

#include <QOpenGLWidget>
#include <QOpenGLDebugMessage>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include "linalg.h"
using namespace linalg;
using namespace linalg::aliases;

class View : public QOpenGLWidget
{
    Q_OBJECT
public:
    View(QWidget *parent = nullptr);
    ~View() {};
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void leaveEvent(QEvent *e) override;
protected:
    float2 mousePos;
    int mouseState;
    int mouseLastState;
    int2 dimensions;
    float ratio;
private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);


};

#endif // VIEW_H
