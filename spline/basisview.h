#ifndef BASISVIEW_H
#define BASISVIEW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLDebugMessage>
#include <QApplication>
#include <QMenu>

#include "geometry.h"
#include "spline.h"
#include "view.h"
#include "controller.h"

class BasisView : public View, public Geometry {
    Q_OBJECT
public:
    ~BasisView() {};
    explicit BasisView(Spline* spline, Controller* controller,  QWidget *parent = nullptr);
    void updateGraph(float2 mouse, bool segment_mode);
protected:
    void initializeGL();
    void initializeGeometry();
    void paintGL();
private:
    Spline* s;
    Controller* c;
    QMatrix4x4 matrix;
    void calculateView();
    void render(QMatrix4x4 view, float ratio);
public slots:
    void changed();
    void customMenuRequested(QPoint pos);
signals:
    void change();
};

#endif // BASISVIEW_H
