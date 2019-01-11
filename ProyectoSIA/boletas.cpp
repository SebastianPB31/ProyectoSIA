#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(11);
  ui->label->setText("INFORMACION DE BOLETAS");
  // desasociar modelo
  ui->tablaBoleta->setModel(nullptr);
  ui->estatus->clear();
  ui->fechaInicio->setDate(QDate::currentDate());
  ui->fechaTermino->setDate(QDate::currentDate());

  // limpiar interfaz
  ui->tablaBoleta->setModel(nullptr);
  ui->rutEmpleadoBoleta->clear();
  ui->fechaBoleta->clear();
  ui->horaboleta->clear();
  ui->totalboleta->clear();
  ui->estatus->clear();
}

void MainWindow::on_buscarNumBoleta_clicked() {
  if (ui->numBoleta->text() == "") return;
  // verificacion

  // limpiar interfaz
  ui->tablaBoleta->setModel(nullptr);
  ui->rutEmpleadoBoleta->clear();
  ui->fechaBoleta->clear();
  ui->horaboleta->clear();
  ui->totalboleta->clear();
  ui->estatus->clear();

  QString numBoleta = ui->numBoleta->text();
  QString query = "SELECT ID_VENTA FROM VENTA WHERE ID_VENTA = %0";
  model->setQuery(query.arg(numBoleta));
  if (model->rowCount() == 0) {
    ui->estatus->setText("ERROR, boleta no encontrada");
    return;
  } else
    ui->estatus->clear();
  // SQL query
  query =
      "SELECT VENTA.RUT_EP, VENTA.FECHA_VT, CAST ((SELECT CONVERT(TIME(0), "
      "VENTA.HORA_VT)) AS VARCHAR(8)) FROM VENTA WHERE VENTA.ID_VENTA = %0";
  model->setQuery(query.arg(numBoleta));
  ui->rutEmpleadoBoleta->setText(model->data(model->index(0, 0)).toString());
  ui->fechaBoleta->setText(model->data(model->index(0, 1)).toString());
  ui->horaboleta->setText(model->data(model->index(0, 2)).toString());
  query =
      "SELECT PRODUCTO.NOM_PD, PRODUCTO.COD_BR_PD, PRODUCTO.PRECIO_PD, "
      "REG_VENTA.CANT_RV, (PRODUCTO.PRECIO_PD*REG_VENTA.CANT_RV) FROM "
      "REG_VENTA,PRODUCTO WHERE REG_VENTA.COD_BR_PD=PRODUCTO.COD_BR_PD AND "
      "REG_VENTA.ID_VENTA = %0";
  model->setQuery(query.arg(numBoleta), db);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nombre"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Codigo"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Precio"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Cantidad"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Total"));

  // imprimir a pantalla
  ui->tablaBoleta->setModel(model);
  ui->tablaBoleta->show();

  // calcular total
  unsigned total = 0;
  for (int i = 0; i < model->rowCount(); i++) {
    total += model->data(model->index(i, 4)).toInt();
  }

  ui->totalboleta->setText(QString::number(total));
}

void MainWindow::on_buscarFechaBoleta_clicked() {
  // limpiar interfaz
  ui->tablaBoleta->setModel(nullptr);
  ui->rutEmpleadoBoleta->clear();
  ui->fechaBoleta->clear();
  ui->horaboleta->clear();
  ui->totalboleta->clear();
  ui->estatus->clear();

  QString fechaI = ui->fechaInicio->date().toString(Qt::ISODate);
  QString fechaT = ui->fechaTermino->date().toString(Qt::ISODate);
  QString query =
      "SELECT "
      "VENTA.ID_VENTA, VENTA.RUT_EP, VENTA.FECHA_VT, CAST ((SELECT "
      "CONVERT(TIME(0), VENTA.HORA_VT)) AS VARCHAR(8)), TOTAL.TOTAL "
      "FROM(SELECT REG_VENTA.ID_VENTA, SUM(REG_VENTA.CANT_RV * "
      "PRODUCTO.PRECIO_PD) AS 'TOTAL' FROM REG_VENTA, PRODUCTO WHERE "
      "REG_VENTA.COD_BR_PD = PRODUCTO.COD_BR_PD GROUP BY REG_VENTA.ID_VENTA) "
      "TOTAL, VENTA WHERE TOTAL.ID_VENTA = VENTA.ID_VENTA AND VENTA.FECHA_VT "
      "BETWEEN '%0' AND '%1'";

  model->setQuery(query.arg(fechaI).arg(fechaT));
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Boleta"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Rut vendedor"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Fecha"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Hora"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Total"));

  // imprimir a pantalla
  ui->tablaBoleta->setModel(model);
  ui->tablaBoleta->show();
}
