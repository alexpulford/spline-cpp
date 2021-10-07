#ifndef SPLINEVIEW_H
#define SPLINEVIEW_H

#include "geometry.h"
#include "spline.h"

typedef struct {
    float radius;
    float inclination;
    float azimuth;
} SphericalCoord;

const float deg2rad = 3.141f/180.0f;

class SplineView : public Geometry
{
public:
    SplineView(Spline* s);
    void render(QOpenGLContext* ctx, QMatrix4x4 view, float ratio);
protected:
    void setData(float* data, size_t size);
private:
    Spline* s;
};

#endif // SPLINEVIEW_H
