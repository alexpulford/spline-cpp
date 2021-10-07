#include "curvatureview.h"

CurvatureView::CurvatureView(Spline* spline, Controller* controller, QWidget *parent) : View(parent),  s(spline), c(controller) {
}

void CurvatureView::updateGraph(float2 mouse) {
    int resolution = 100;
    float t, y, delta, o_t, o_y;

    vector<float> lines;
    vector<float> segment;

    maximum = 0;

    bool in_view = mouse.x > 0 and mouse.x < 1 and mouse.y > 0 and mouse.y < 1;

    bool filter = !c->enabled.empty();

    for(int b = 0; b < s->basisCount(); b++) {
        for(int p = 0; p < s->spanCount(); p++) {
            auto first = s->n[b][p].derivative();
            auto second = first.derivative();

            delta = s->spans[p].y - s->spans[p].x;

            //construct geometry
            //each vertex is {pos.xyz, width, color.rgb}
            for(int r = 0; r <= resolution; r++) {
                t = ((float)r/(float)resolution)*delta + s->spans[p].x;
                y = curvature(first, second, t);
                maximum = max(maximum, y);
                auto hue = 0.8f*(float)b/(s->basisCount());

                //construct line between last point and current point
                if(r != 0) {
                    segment = {o_t, o_y, 0.0, 1.0,
                               hue, 1.0, 1.0,
                               t, y, 0.0, 1.0,
                               hue, 1.0, 1.0};
                    lines.insert(end(lines), begin(segment), end(segment));
                }
                o_t = t;
                o_y = y;
            }
        }
    }

    //construct some guide lines
    //dotted lines between spans, solid lines where the spline starts and ends
    resolution = height()/4;
    for(size_t a = 0; a < s->spans.size(); a++) {
        for(int r = 0; r <= resolution; r++) {
            t = s->spans[a].x;
            y = maximum * ((float)r/resolution);

            if(r != 0) {
                //solid line
                int order = s->order;
                int size = s->spans.size();

                if ((size-order*2>0) and (a == order or a == s->spans.size()-order)) {
                    segment = {o_t, o_y, 0.0, 0.5, 0.5, 0.5, 0.5, t, y, 0.0, 0.5, 0.5, 0.5, 0.5};
                    lines.insert(end(lines), begin(segment), end(segment));
                } else if(r%4==0) { //dotted line
                    segment = {o_t, o_y, 0.0, 0.5, 0.5, 0.4, 0.4, t, y, 0.0, 0.5, 0.5, 0.4, 0.4};
                    lines.insert(end(lines), begin(segment), end(segment));
                }
            }

            o_t = t;
            o_y = y;
        }
    }

    setData(lines.data(), lines.size());
}

void CurvatureView::render(QMatrix4x4 view, float ratio) {
    static int matrixUniform;
    static int ratioUniform;
    if(!matrixUniform) {
        matrixUniform = program->uniformLocation("matrix");
        ratioUniform = program->uniformLocation("ratio");
    }
    program->bind();
    program->setUniformValue(matrixUniform, view);
    program->setUniformValue(ratioUniform, ratio);
    vao->bind();
    context()->functions()->glDrawArrays(GL_LINES, 0, count/7);
    vao->release();
    program->release();
}


void CurvatureView::initializeGL() {
    View::initializeGL();

    makeCurrent();
    initializeGeometry();

}

void CurvatureView::paintGL() {
    View::paintGL();
    updateGraph(mousePos);
    calculateView();
    render(matrix, ratio*maximum);
}

void CurvatureView::calculateView()
{
    matrix.setToIdentity();
    matrix.ortho(0, 1, 0, maximum, -1, 1);
    matrix.translate(0, 0, 0);
}

void CurvatureView::initializeGeometry() {
    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/2D_line.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/2D_line.geom");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/2D_line.frag");
    program->bindAttributeLocation("position", 0);
    program->bindAttributeLocation("color", 1);
    program->bindAttributeLocation("width", 2);
    program->link();

    vao = new QOpenGLVertexArrayObject();
    vao->create();
    vbo = new QOpenGLBuffer();
    vbo->create();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vao->bind();
    auto posAttr = program->attributeLocation("position");
    auto colorAttr = program->attributeLocation("color");
    auto widthAttr = program->attributeLocation("width");
    vbo->bind();

    program->enableAttributeArray(posAttr);
    program->enableAttributeArray(colorAttr);
    program->enableAttributeArray(widthAttr);

    program->setAttributeBuffer(posAttr, GL_FLOAT, 0, 3, 7*sizeof(float));
    program->setAttributeBuffer(colorAttr, GL_FLOAT, 4*sizeof(float), 3, 7*sizeof(float));
    program->setAttributeBuffer(widthAttr, GL_FLOAT, 3*sizeof(float), 1, 7*sizeof(float));
}

void CurvatureView::changed() {
    update();
}
