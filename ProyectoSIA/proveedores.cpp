#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_agregarProveedor_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(5);
  ui->label->setText("AGREGAR PROVEEDOR");
  // entrada -> SQL query
}

void MainWindow::on_agregarNuevoProveedor_clicked() {
  QString query = "INSERT INTO PROVEEDOR VALUES(%0,'%1','%2',%3,'%4')";
  model->setQuery(query.arg(ui->rutProv->text())
                      .arg(ui->nombreProv->text())
                      .arg(ui->apellidoProv->text())
                      .arg(ui->fonoProv->text())
                      .arg(ui->nombreEmpProv->text()),
                  db);

  ui->rutProv->setFocus();
  ui->rutProv->clear();
  ui->nombreProv->clear();
  ui->apellidoProv->clear();
  ui->fonoProv->clear();
  ui->nombreEmpProv->clear();
}

void MainWindow::on_editarProveedor_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(6);
  ui->label->setText("EDITAR PROVEEDOR");
  // entrada -> SQL query
}
