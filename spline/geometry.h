#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <iostream>

class Geometry
{
public:
    Geometry();
protected:
    QOpenGLShaderProgram *program;
    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *vbo;
    int count = 0;
    void setData(float* data, size_t size);
};

#endif // GEOMETRY_H
