#include "knotedit.h"
#include "ui_knotedit.h"

KnotEdit::KnotEdit(Spline* spline, QWidget *parent) :
    QWidget(parent), s(spline),
    ui(new Ui::KnotEdit)
{
    ui->setupUi(this);
    populateList();

    addAction = new QAction("Add", this);
    removeAction = new QAction("Remove", this);
    uniformAction = new QAction("Make Uniform", this);
    openAction = new QAction("Make Open", this);
    closeAction = new QAction("Make Close", this);
    connect(addAction, SIGNAL(triggered()), this, SLOT(add()));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));
    connect(uniformAction, SIGNAL(triggered()), this, SLOT(uniform()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->knotView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
}

void KnotEdit::populateList() {
    int index = ui->knotView->currentRow();
    ui->knotView->clear();
    for(int i = 0; i < s->knotCount(); i++) {
        auto item = new QListWidgetItem(QString("%1").arg(s->t[i]));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->knotView->addItem(item);
    }
    ui->knotView->setCurrentRow(index);
}

void KnotEdit::knotEdit(QListWidgetItem* index) {
    int i = ui->knotView->row(index);
    float val = index->text().toFloat();
    if(i > 0)
        val = max(val, s->t[i-1]);
    if(i < s->knotCount()-1)
        val = min(val,  s->t[i+1]);
    index->setText(QString("%1").arg(val));
    s->t[i] = val;

    recompute();
}

void KnotEdit::contextMenu(QPoint point) {
    QMenu *menu=new QMenu(this);
    menu->addAction(addAction);
    menu->addAction(removeAction);
    menu->addAction(uniformAction);
    menu->addAction(openAction);
    menu->addAction(closeAction);
    menu->popup(mapToGlobal(point));
}

void KnotEdit::changed() {
    populateList();
}

void KnotEdit::remove() {
    removeAt(ui->knotView->currentRow());
}

void KnotEdit::removeAt(int index) {
    if(s->t.size() == 1)
        return;
    s->t.erase(s->t.begin()+index);
    recompute();
}
void KnotEdit::add() {
    addAt(ui->knotView->currentRow());
}
void KnotEdit::uniform() {
    s->makeUniform();
    recompute();
}
void KnotEdit::open() {
    s->makeOpen();
    recompute();
}
void KnotEdit::close() {
    s->makeClosed();
    recompute();
}
void KnotEdit::nudge(int i, int dir) {
    float val = ui->knotView->item(i)->text().toFloat();
    val += 0.01*dir;

    if(i > 0)
        val = max(val, s->t[i-1]);
    if(i < s->knotCount()-1)
        val = min(val,  s->t[i+1]);

    s->t[i] = val;
    recompute();
}
void KnotEdit::addAt(int index) {
    if(index == s->knotCount()-1)
        s->t.emplace(s->t.begin()+index+1,1.0);
    else {
        float l = s->t[index];
        float h = s->t[index+1];
        s->t.emplace(s->t.begin()+index+1, l + (h-l)/2.0);
    }
    recompute();
}

void KnotEdit::keyPressEvent(QKeyEvent* e) {
    if(e->modifiers() != Qt::NoModifier) {
        QWidget::keyPressEvent(e);
        return;
    }
    int index = ui->knotView->currentRow();
    switch(e->key()) {
        case Qt::Key_Delete:
            removeAt(index);
            break;
        case Qt::Key_Left:
            nudge(index, -1);
            break;
        case Qt::Key_Right:
            nudge(index, 1);
            break;
        default:
            QWidget::keyPressEvent(e);
            break;
    }
}

void KnotEdit::recompute() {
    s->recompute();
    populateList();
    emit change();
}

KnotEdit::~KnotEdit()
{
    delete ui;
}
