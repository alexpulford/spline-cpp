#include "basisview.h"

BasisView::BasisView(Spline* spline, Controller* controller, QWidget *parent) : View(parent),  s(spline), c(controller) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
}

void BasisView::updateGraph(float2 mouse, bool segment_mode) {
    int resolution = 100;
    float t, y, delta, o_t, o_y;

    vector<float> lines;
    vector<float> segment;

    bool in_view = mouse.x > 0 and mouse.x < 1 and mouse.y > 0 and mouse.y < 1;

    bool filter = !c->enabled.empty();

    for(int b = 0; b < s->basisCount(); b++) {
        for(int p = 0; p < s->spanCount(); p++) {


            //ignore filter if the mouse is in the view
            if( /*!in_view and*/ filter and !c->enabled.count(int2{b, p}))
                continue;

            //dont render if in segment mode and we are not in the segment
            bool in_span = mouse.x > s->spans[p].x and mouse.x < s->spans[p].y and mouse.y > 0 and mouse.y < 1;
            if(in_view and !in_span and segment_mode)
                continue;

            delta = s->spans[p].y - s->spans[p].x;
            //construct geometry
            //each vertex is {pos.xyz, width, color.rgb}
            for(int r = 0; r <= resolution; r++) {
                t = ((float)r/(float)resolution)*delta + s->spans[p].x;
                y = s->n[b][p](t);

                auto hue = 0.8f*(float)b/(s->basisCount());
                auto bright = (in_span or !in_view) ? 1.0f:0.6f;

                //construct line between last point and current point
                if(r != 0) {
                    segment = {o_t, o_y, 0.5, 1.0,
                               hue, 1.0, bright,
                               t, y, 0.5, 1.0,
                               hue, 1.0, bright};
                    lines.insert(end(lines), begin(segment), end(segment));
                }

                //in segment mode we render the entire function
                //uses the same r, but the domain is 0->1 instead of the span delta
                if(in_span and segment_mode) {
                    auto t1 = ((float)r/(float)resolution);
                    auto t2 = (((float)r-1)/(float)resolution);

                    auto y1 = (float)s->n[b][p](t1);
                    auto y2 = (float) s->n[b][p](t2);

                    segment = {t1, y1, 0.1, 1.0,
                               hue, 1.0, 0.6,
                               t2, y2, 0.1, 1.0,
                               hue, 1.0, 0.6};
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
            y = (float)r/resolution;

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

void BasisView::render(QMatrix4x4 view, float ratio) {
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


void BasisView::initializeGL() {
    View::initializeGL();

    makeCurrent();
    initializeGeometry();
    calculateView();
}

void BasisView::paintGL() {
    if(mouseState == Qt::MouseButton::RightButton and mouseLastState != Qt::MouseButton::RightButton) {
        for(auto i = 0; i < s->spanCount(); i++) {
            auto sp = s->spans[i];
            if(mousePos.x > sp.x and mousePos.x < sp.y) {
                for(int b = 0; b < s->basisCount(); b++) {
                     c->enabled.insert(int2{b, i});
                }
                change();
            }
        }
    }

    View::paintGL();

    updateGraph(mousePos, QApplication::mouseButtons());
    render(matrix, ratio);
}

void BasisView::customMenuRequested(QPoint pos){

    QMenu *menu=new QMenu(this);
    menu->addAction(new QAction("Mixed", this));
    menu->addAction(new QAction("Span", this));
    menu->addAction(new QAction("Basis", this));
    menu->popup(this->mapToGlobal(pos));
}

void BasisView::calculateView()
{
    matrix.setToIdentity();
    matrix.ortho(0,1, 0, 1, -1, 1);
    matrix.translate(0, 0, 0);
}

void BasisView::initializeGeometry() {
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

void BasisView::changed() {
    update();
}
