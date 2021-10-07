#include "splineview.h"

SplineView::SplineView(Spline* sp)
{
    s = sp;

    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/3D_line.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/3D_line.geom");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/3D_line.frag");
    program->bindAttributeLocation("position", 0);
    program->bindAttributeLocation("color", 1);
    program->link();

    vao = new QOpenGLVertexArrayObject();
    vao->create();
    vbo = new QOpenGLBuffer();
    vbo->create();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vao->bind();
    auto posAttr = program->attributeLocation("position");
    auto colorAttr = program->attributeLocation("color");
    vbo->bind();

    program->enableAttributeArray(posAttr);
    program->enableAttributeArray(colorAttr);

    program->setAttributeBuffer(posAttr, GL_FLOAT, 0, 3, 6*sizeof(float));
    program->setAttributeBuffer(colorAttr, GL_FLOAT, 3*sizeof(float), 3, 6*sizeof(float));
}

void SplineView::setData(float* data, size_t size) {
    vbo->bind();
    vbo->allocate(data, size * sizeof(float));
    count = size;
}

void SplineView::render(QOpenGLContext* ctx, QMatrix4x4 view, float ratio) {
    static int matrixUniform;
    if(!matrixUniform) {
        matrixUniform = program->uniformLocation("matrix");
    }

    program->bind();
    program->setUniformValue(matrixUniform, view);
    vao->bind();
    ctx->functions()->glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, count/6);
    vao->release();
    program->release();
}
