#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_listaProducto_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(2);
  ui->label->setText("LISTA DE PRODUCTOS");

  // SQL query
  model->setQuery(
      "SELECT "
      "PRODUCTO.NOM_PD,PRODUCTO.COD_BR_PD,PRODUCTO.PRECIO_PD,CLASIFICACION.NOM_"
      "CF,PRODUCTO.CANT_CRIT_PD,ISNULL (STOCK.CANTIDAD_RP,0) FROM PRODUCTO "
      "LEFT JOIN CLASIFICACION "
      "ON PRODUCTO.ID_CF=CLASIFICACION.ID_CF LEFT JOIN STOCK ON "
      "PRODUCTO.COD_BR_PD=STOCK.COD_BR_PD WHERE PRODUCTO.ESTADO=1",
      db);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nombre producto"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("codigo"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Precio unitario"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tipo producto"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Stock critico"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Stock actual"));

  // imprimir a pantalla
  ui->tablaProducto->setModel(model);
  ui->tablaProducto->show();
}

void MainWindow::on_agregarProducto_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(7);
  ui->label->setText("AGREGAR PRODUCTO");
  ui->tipoProd->clear();
  ui->codigoProd->clear();
  ui->nombreProd->clear();
  ui->precioProd->clear();
  ui->cantidadMinProd->clear();

  model->setQuery("SElECT * FROM CLASIFICACION");
  int registros = model->rowCount();
  // llenado de tipos de productos
  for (int i = 0; i < registros; i++) {
    ui->tipoProd->addItem(model->data(model->index(i, 1)).toString(),
                          model->data(model->index(i, 0)).toString());
  }
}

void MainWindow::on_agregarNuevoProducto_clicked() {
  QString query = "INSERT INTO PRODUCTO VALUES (%0, %1, '%2', %3, %4)";
  model->setQuery(query.arg(ui->codigoProd->text())
                      .arg(ui->tipoProd->currentData().toString())
                      .arg(ui->nombreProd->text())
                      .arg(ui->precioProd->text())
                      .arg(ui->cantidadMinProd->text()),
                  db);

  ui->codigoProd->setFocus();
  ui->codigoProd->clear();
  ui->nombreProd->clear();
  ui->precioProd->clear();
  ui->cantidadMinProd->clear();
}

void MainWindow::on_editarProducto_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(8);
  ui->label->setText("EDITAR PRODUCTO");
  // entrada -> SQL query
}
