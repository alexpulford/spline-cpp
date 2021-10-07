#include "basisedit.h"
#include "ui_basisedit.h"

BasisEdit::BasisEdit(Spline* spline, Controller* controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasisEdit), s(spline), c(controller)
{
    ui->setupUi(this);
    populateLists();
    ui->polyView->setSelectionMode(QAbstractItemView::MultiSelection);
}

BasisEdit::~BasisEdit()
{
    delete ui;
}


QString getFunctionLabel(Polynomial poly) {
    auto out = QList<QString>();
    int z = poly.degree();
    for (int x = 0; x <= z; x++) {
        QString term = QString("%1").arg(poly[x]);
        if(poly[x] == 0)
            continue;
        if(x > 0)
            term += "<b>x</b>";
        if(x > 1)
            term += QString("<sup>%1</sup>").arg(x);
        out.append(term);
    }
    return QStringList(out).join(" + ");
}

QLabel* newColorLabel(QString in, float hue)
{
    auto label = new QLabel(in);
    label->setStyleSheet(QString("QLabel { color: hsv(%1,255,255); }").arg(hue*360));
    return label;
}

void BasisEdit::populateLists() {
    ui->polyView->clear();
    for(int b = 0; b < s->basisCount(); b++) {
        auto bItem = new QTreeWidgetItem(ui->polyView);
        auto hue = 0.8f*(float)b/(s->basisCount());
        ui->polyView->setItemWidget(bItem, 0, newColorLabel(QString("%1").arg(b), hue));
        bItem->setExpanded(true);
        auto valid = s->getValidSpan();
        for(int p = 0; p < s->spanCount(); p++) {
            auto span = s->spans[p];
            if(s->functionExists(b, p)) {
                auto pItem = new QTreeWidgetItem(bItem);
                ui->polyView->setItemWidget(pItem, 0, newColorLabel(QString("%1").arg(p), hue));
                pItem->setText(1, QString("(%1,%2)").arg(s->spans[p].x).arg(s->spans[p].y));
                ui->polyView->setItemWidget(pItem, 2, new QLabel(getFunctionLabel(s->functionAt(b,p))));
                ui->polyView->setItemSelected(pItem, c->enabled.count({b,p}));
            }
        }
    }
}



void BasisEdit::enableBasis(int basis) {
    for(int i = 0; i < s->spanCount(); i++) {
         c->enabled.insert(int2{basis, i});
    }
}

void BasisEdit::enableSpan(int span) {
    for(int i = 0; i < s->basisCount(); i++) {
         c->enabled.insert(int2{i, span});
    }
}

void BasisEdit::enablePolynomial(int basis, int span) {
    int tmpSpan = 0;
    for(int i = 0; i < s->basisCount(); i++) {
        if(s->functionExists(basis, i)) {
            if(tmpSpan == span) {
                span = i;
                break;
            }
            tmpSpan++;
        }

    }

    c->enabled.insert(int2{basis, span});
}

void BasisEdit::changedSelection() {
    c->enabled.clear();
    for(int i = 0; i < ui->polyView->selectedItems().size(); i++) {
        auto item = ui->polyView->selectedItems()[i];

        bool isBasis = item->parent() == nullptr;
        if(isBasis) {
            int basis = ui->polyView->indexOfTopLevelItem(item);
            enableBasis(basis);
        } else {
            int basis = ui->polyView->indexOfTopLevelItem(item->parent());
            int span = item->parent()->indexOfChild(item);
            enablePolynomial(basis, span);
        }
    }
    emit change();
}

void BasisEdit::changed() {
    ui->polyView->blockSignals(true);
    populateLists();
    ui->polyView->blockSignals(false);
}

void BasisEdit::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape and e->modifiers() == Qt::NoModifier)
        ui->polyView->clearSelection();
    else
        QWidget::keyPressEvent(e);
}

