#ifndef CURVATUREVIEW_H
#define CURVATUREVIEW_H

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

class CurvatureView : public View, public Geometry
{
    Q_OBJECT
public:
    ~CurvatureView() {};
    explicit CurvatureView(Spline* spline, Controller* controller,  QWidget *parent = nullptr);
    void updateGraph(float2 mouse);
protected:
    void initializeGL();
    void initializeGeometry();
    void paintGL();
private:
    float maximum;
    Spline* s;
    Controller* c;
    QMatrix4x4 matrix;
    void calculateView();
    void render(QMatrix4x4 view, float ratio);
public slots:
    void changed();
signals:
    void change();
};

#endif // CURVATUREVIEW_H
