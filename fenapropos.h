#ifndef FENAPROPOS_H
#define FENAPROPOS_H

#include <QDialog>

namespace Ui {
    class FenApropos;
}

class FenApropos : public QDialog
{
    Q_OBJECT

public:
    explicit FenApropos(QWidget *parent = 0);
    ~FenApropos();

private:
    Ui::FenApropos *ui;
};

#endif // FENAPROPOS_H
