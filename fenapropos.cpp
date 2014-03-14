#include "fenapropos.h"
#include "ui_fenapropos.h"

FenApropos::FenApropos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FenApropos)
{
    ui->setupUi(this);
}

FenApropos::~FenApropos()
{
    delete ui;
}
