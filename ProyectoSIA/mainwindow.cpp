#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  model = nullptr;
  encabezado = QString("PRODUCTO");
  encabezado +=
      QString(" ").repeated(30 - encabezado.size()) + QString("PRECIO");
  encabezado +=
      QString(" ").repeated(40 - encabezado.size()) + QString("CANTIDAD");
}

MainWindow::~MainWindow() {
  if (model) model->destroyed();
  db.close();
  delete ui;
}

void MainWindow::on_salir_clicked() { QApplication::quit(); }


