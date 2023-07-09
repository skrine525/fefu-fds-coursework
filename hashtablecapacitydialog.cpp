#include "hashtablecapacitydialog.h"
#include "ui_hashtablecapacitydialog.h"

HashTableCapacityDialog::HashTableCapacityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HashTableCapacityDialog)
{
    ui->setupUi(this);

    result = false;
}

HashTableCapacityDialog::~HashTableCapacityDialog()
{
    delete ui;
}

void HashTableCapacityDialog::setMainWindow(MainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}

void HashTableCapacityDialog::on_pushButtonCancel_clicked()
{
    close();
}


void HashTableCapacityDialog::on_pushButtonApply_clicked()
{
    mainWindow->resetViewAndData(ui->spinBoxHashTableCapacity1->value(), ui->spinBoxHashTableCapacity2->value());
    result = true;
    close();
}

bool HashTableCapacityDialog::getResult()
{
    return result;
}

