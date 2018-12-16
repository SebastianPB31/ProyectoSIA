#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_realizarCompra_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(10);
  ui->label->setText("REALIZAR COMPRA");
  ui->detalleBoleta_2->clear();
  listaVenta.clear();
  ui->listaProveedores->clear();
  ui->listaEmpleados_2->clear();
  ui->totalBoleta_2->display(0);
  ui->detalleBoleta_2->append(encabezado);
  ui->fechaVenc->setDate(QDate::currentDate());
  ui->fechaPago->setDate(QDate::currentDate());
  ui->codigoPistola_2->setEnabled(false);
  ui->agregarCaja->setEnabled(false);

  // verif empleados
  model->setQuery("SELECT RUT_EP FROM EMPLEADO");
  int registros = model->rowCount();
  if (registros == 0) {
    ui->label_48->setText("ERROR NO HAY EMPLEADOS INGRESADOS");
    return;
  }

  // llenado de empleados
  ui->listaEmpleados_2->clear();
  for (int i = 0; i < registros; i++) {
    ui->listaEmpleados_2->addItem(model->data(model->index(i, 0)).toString());
  }

  // verif proveedor
  model->setQuery("SELECT RUT_PV FROM PROVEEDOR");
  int registrosProv = model->rowCount();
  if (registrosProv == 0) {
    ui->label_48->setText("ERROR NO HAY PROVEEDORES INGRESADOS");
    return;
  }

  // llenado de proveedores
  for (int i = 0; i < registrosProv; i++) {
    ui->listaProveedores->addItem(model->data(model->index(i, 0)).toString());
  }

  ui->listaProveedores->clearEditText();
  ui->label_48->clear();
  ui->codigoPistola_2->setEnabled(true);
  ui->listaProveedores->setFocus();
}


void MainWindow::on_codigoPistola_2_returnPressed() {
  QString codigo = ui->codigoPistola_2->text();
  QString query = "SELECT * FROM PRODUCTO WHERE COD_BR_PD = %0";
  model->setQuery(query.arg(codigo), db);
  QString verif = model->data(model->index(0, 0)).toString();
  if (verif == codigo) {
    QString nombre = model->data(model->index(0, 2)).toString();
    ui->label_48->setText(nombre);
    ui->agregarCaja->setEnabled(true);
    ui->precioCaja->setFocus();
  }

  else {
    ui->label_48->setText("PRODUCTO NO EXISTE");
    ui->agregarCaja->setEnabled(false);
  }
}

void MainWindow::on_agregarCaja_clicked() {
  QString codigo = ui->codigoPistola_2->text();
  QString cantidad = ui->cantidad_2->text();
  QString precio = ui->precioCaja->text();
  QString rutProv = ui->listaProveedores->currentText();
  QDate fechaVencimiento = ui->fechaVenc->date();
  if (codigo == "" || cantidad == "" || precio == "") return;
  int cantidadI = cantidad.toInt();
  // validación
  QString query = "SELECT * FROM PRODUCTO WHERE COD_BR_PD = %0";
  model->setQuery(query.arg(codigo), db);

  QString verif = model->data(model->index(0, 0)).toString();

  if (verif == codigo) {
    // producto existe
    QString nombre = model->data(model->index(0, 2)).toString();
    int precio = ui->precioCaja->text().toInt();
    QString linea = nombre + QString(" ").repeated(30 - nombre.size()) +
                    ui->precioCaja->text();
    linea = linea +
            QString(" ").repeated(encabezado.size() - linea.size() -
                                  cantidad.size()) +
            cantidad;
    ui->detalleBoleta_2->append(linea);

    listaCompra.append(std::tuple<QString, int, int, QDate, QString>(
        codigo, precio, cantidadI, fechaVencimiento, linea));

    ui->totalBoleta_2->display(ui->totalBoleta_2->intValue() + precio);
    ui->codigoPistola_2->clear();
    ui->cantidad_2->clear();
    ui->precioCaja->clear();
    ui->fechaVenc->setDate(QDate::currentDate());

    ui->codigoPistola_2->setFocus();
  } else
    ui->label_48->setText("PRODUCTO NO EXISTE");
}

void MainWindow::on_emitirBoleta_2_clicked() {
  QString rutProv = ui->listaProveedores->currentText();
  QString fechaPago = ui->fechaPago->date().toString(Qt::ISODate);
  int deuda = ui->totalBoleta_2->intValue();

  QString query =
      "INSERT INTO PROVISION VALUES(%0,(SELECT CONVERT(VARCHAR(8), "
      "GETDATE(), "
      "112) AS [YYYYMMDD]), (SELECT CONVERT(TIME, GETDATE())), '%1', %2 )";
  model->setQuery(
      query.arg(rutProv).arg(fechaPago).arg(QString::number(deuda)));

  // obtener id provision
  query = "SELECT MAX(ID_PV) from PROVISION";
  model->setQuery(query);
  QString idProvision = model->data(model->index(0, 0)).toString();

  for (std::tuple<QString, int, int, QDate, QString> elemento : listaCompra) {
    // registrar cajas
    query = "INSERT INTO REG_PROVISION VALUES(%0, %1, %2, '%3')";
    model->setQuery(query.arg(idProvision)
                        .arg(std::get<0>(elemento))
                        .arg(std::get<2>(elemento))
                        .arg(std::get<3>(elemento).toString(Qt::ISODate)));
    // actualizar stock
    // verificar existencia en stock
    query = "SELECT COD_BR_PD FROM STOCK WHERE COD_BR_PD = %0";
    model->setQuery(query.arg(std::get<0>(elemento)));
    if (model->rowCount() == 0) {
      query = "INSERT INTO STOCK VALUES(%0, %1)";
      model->setQuery(
          query.arg(std::get<0>(elemento)).arg(std::get<2>(elemento)));
    } else {
      query =
          "UPDATE STOCK SET CANTIDAD_RP = CANTIDAD_RP + %0 WHERE "
          "COD_BR_PD = %1";
      model->setQuery(
          query.arg(std::get<2>(elemento)).arg(std::get<0>(elemento)));
    }
  }

  ui->codigoPistola_2->clear();
  ui->cantidad_2->clear();
  ui->precioCaja->clear();
  ui->fechaVenc->setDate(QDate::currentDate());
  ui->detalleBoleta_2->clear();
  ui->totalBoleta_2->display(0);
  listaCompra.clear();
}
