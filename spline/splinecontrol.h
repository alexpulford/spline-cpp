#ifndef SPLINECONTROL_H
#define SPLINECONTROL_H

#include <QWidget>

class SplineControl : public QObject
{
    Q_OBJECT
public:
    explicit SplineControl();
public slots:
    virtual void basisChanged(int basis);
    virtual void spanChanged(int span);
signals:
    void basisSelected(int basis);
    void spanSelected(int span);
};

#endif // SPLINECONTROL_H
