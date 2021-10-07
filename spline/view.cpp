#include "view.h"

View::View(QWidget *parent) : QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

void View::mouseMoveEvent(QMouseEvent *e) {
    auto relative = e->pos();
    mousePos = {(float)(relative.x())/this->width(), 1.0-((float)(relative.y())/this->height())};
    update();
}

void View::mousePressEvent(QMouseEvent *e) {
    mouseState = e->buttons();
    update();
}

void View::mouseReleaseEvent(QMouseEvent *e) {
    mouseState = e->buttons();
    update();
}

void View::leaveEvent(QEvent *e) {
    mouseState = 0;
    mousePos = {-1.0, -1.0};
    update();
}

void View::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage) {
    qDebug() << debugMessage.message();
}

void View::initializeGL() {
    context()->functions()->glClearColor(0.2, 0.2, 0.2, 1.0);
    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger, &QOpenGLDebugLogger::messageLogged, this,
    &View::handleLoggedMessage);
    logger->startLogging();
    glEnable(GL_DEPTH_TEST);
}

void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto scale = QApplication::desktop()->devicePixelRatio();
    glViewport(0, 0, dimensions.x*scale, dimensions.y*scale);
    mouseLastState = mouseState;
}

void View::resizeGL(int w, int h) {
    dimensions = {w, h};
    ratio = (float)w/(float)h;
}
