#include "geometry.h"

Geometry::Geometry()
{
    /*program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/color_shader.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/color_shader.frag");
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
    program->setAttributeBuffer(colorAttr, GL_FLOAT, 3*sizeof(float), 3, 6*sizeof(float));*/
}
void Geometry::setData(float* data, size_t size) {
    vbo->bind();
    vbo->allocate(data, size * sizeof(float));
    count = size;
}

/*void geometry::render(QOpenGLContext* ctx, QMatrix4x4 view, float ratio) {
    static int matrixUniform;
    if(!matrixUniform) {
        matrixUniform = program->uniformLocation("matrix");
    }

    program->bind();
    program->setUniformValue(matrixUniform, view);
    vao->bind();
    ctx->functions()->glDrawArrays(GL_TRIANGLES, 0, count/6);
    vao->release();
    program->release();
}*/
