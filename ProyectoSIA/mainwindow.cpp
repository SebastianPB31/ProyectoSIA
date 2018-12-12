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

void MainWindow::on_conectar_clicked() {
  ui->stackedWidget->setCurrentIndex(0);
  db.close();
  db.removeDatabase("QODBC3");
  if (model != nullptr) {
    model->destroyed();
  }
  QString driver = ui->driver->text(), server = ui->server_name->text(),
          dataBase = ui->DDBB->text(), userID = ui->userID->text(),
          password = ui->password->text(),
          texto = "DRIVER=%0;SERVER=%1;DATABASE=%2;UID=%3;PWD=%4";

  db = QSqlDatabase::addDatabase("QODBC3");
  db.setDatabaseName(
      texto.arg(driver).arg(server).arg(dataBase).arg(userID).arg(password));
  model = new QSqlQueryModel;
  bool ok = db.open();

  if (ok)
    ui->log->setText(QString("OK"));
  else
    ui->log->setText(QString(db.lastError().text()));
}

void MainWindow::on_infoEmpleado_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(1);
  ui->label->setText("INFORMACION DE EMPLEADOS");

  // SQL query
  model->setQuery("SELECT * FROM EMPLEADO", db);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("RUT"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sueldo"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Apellido"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Telefono"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Direccion"));

  // imprimir a pantalla
  ui->tablaEmpleado->setModel(model);
  ui->tablaEmpleado->show();
}

void MainWindow::on_listaProducto_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(2);
  ui->label->setText("LISTA DE PRODUCTOS");

  // SQL query
  model->setQuery(
      "SELECT COD_BR_PD,NOM_PD,NOM_CF,PRECIO_PD,CANT_CRIT_PD FROM "
      "PRODUCTO,CLASIFICACION WHERE PRODUCTO.ID_CF=CLASIFICACION.ID_CF",
      db);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Codigo"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre producto"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Tipo producto"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Precio unitario"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Stock critico"));

  // imprimir a pantalla
  ui->tablaProducto->setModel(model);
  ui->tablaProducto->show();
}

void MainWindow::on_agregarEmpleado_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(3);
  ui->label->setText("AGREGAR EMPLEADO");
  // entrada -> SQL query
}

void MainWindow::on_editarEmpleado_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(4);
  ui->label->setText("EDITAR EMPLEADO");
  // entrada -> SQL query
}

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

void MainWindow::on_agregarProducto_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(7);
  ui->label->setText("AGREGAR PRODUCTO");
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

void MainWindow::on_realizarVenta_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(9);
  ui->label->setText("REALIZAR VENTA");
  ui->detalleBoleta->clear();
  listaVenta.clear();
  ui->totalBoleta->display(0);
  ui->detalleBoleta->append(encabezado);

  model->setQuery("SELECT RUT_EP FROM EMPLEADO");
  int registros = model->rowCount();
  if (registros == 0) {
    ui->label_28->setText("ERROR NO HAY VENDEDORES INGRESADOS");
    ui->codigoPistola->setDisabled(true);
    return;
  }

  // llenado de empleados
  for (int i = 0; i < registros; i++) {
    ui->listaEmpleados->addItem(model->data(model->index(i, 0)).toString());
  }

  ui->label_28->clear();
  ui->codigoPistola->setDisabled(false);
}

void MainWindow::on_codigoPistola_returnPressed() {
  QString codigo = ui->codigoPistola->text();
  QString cantidad = ui->cantidad->text();
  if (codigo == "" || cantidad == "") return;
  int cantidadI = cantidad.toInt();
  // validación
  QString query = "SELECT * FROM PRODUCTO WHERE COD_BR_PD = %0";
  model->setQuery(query.arg(codigo), db);

  QString verif = model->data(model->index(0, 0)).toString();

  if (verif == codigo) {
    // producto existe
    QString nombre = model->data(model->index(0, 2)).toString();
    int precio = model->data(model->index(0, 3)).toInt();
    ui->label_28->setText(nombre);
    QString linea = nombre + QString(" ").repeated(30 - nombre.size()) +
                    QString::number(precio);
    linea = linea +
            QString(" ").repeated(encabezado.size() - linea.size() -
                                  cantidad.size()) +
            cantidad;
    ui->detalleBoleta->append(linea);

    listaVenta.append(std::tuple<QString, int, int, QString>(codigo, precio,
                                                             cantidadI, linea));

    ui->totalBoleta->display(ui->totalBoleta->intValue() + precio * cantidadI);
    ui->cantidad->setText("1");
    ui->codigoPistola->clear();
    ui->codigoPistola->setFocus();
  } else
    ui->label_28->setText("PRODUCTO NO EXISTE");
}

void MainWindow::on_emitirBoleta_clicked() {
  // actualizar ventas
  QString query =
      "INSERT INTO VENTA VALUES (%0, (SELECT CONVERT(VARCHAR(8), GETDATE(), "
      "112) AS [YYYYMMDD]), (SELECT CONVERT(TIME, GETDATE())))";
  model->setQuery(query.arg(ui->listaEmpleados->currentText()));

  // obtener id boleta generado
  query = "SELECT MAX(ID_VENTA) from VENTA";
  model->setQuery(query);
  QString idBoleta = model->data(model->index(0, 0)).toString();

  for (std::tuple<QString, int, int, QString> elemento : listaVenta) {
    // actualizar stock
    query =
        "UPDATE STOCK SET CANTIDAD_RP = CANTIDAD_RP - %0 WHERE "
        "COD_BR_PD = %1";
    model->setQuery(
        query.arg(std::get<2>(elemento)).arg(std::get<0>(elemento)));

    // actualizar detalle venta
    query = "INSERT INTO REG_VENTA VALUES (%0 , %1, %2)";
    model->setQuery(query.arg(idBoleta)
                        .arg(std::get<0>(elemento))
                        .arg(std::get<2>(elemento)));
  }

  ui->detalleBoleta->clear();
  ui->totalBoleta->display(0);
  listaVenta.clear();
}

void MainWindow::on_codigoBorrar_returnPressed() {
  // borrar elemento
  if (ui->codigoBorrar->text() == "") return;
  QString codigo = ui->codigoBorrar->text();
  ui->detalleBoleta->clear();
  ui->detalleBoleta->append(encabezado);
  // volver a calular precio
  int precioTotal = 0;
  QVector<std::tuple<QString, int, int, QString>> nuevaListaVenta;

  for (std::tuple<QString, int, int, QString> elemento : listaVenta) {
    if (std::get<0>(elemento) != codigo) {
      nuevaListaVenta.append(elemento);
      precioTotal += std::get<1>(elemento) * std::get<2>(elemento);
      ui->detalleBoleta->append(std::get<3>(elemento));
    }
  }
  ui->codigoBorrar->clear();
  ui->totalBoleta->display(precioTotal);
  listaVenta = nuevaListaVenta;
}

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

  // verif vendedor
  model->setQuery("SELECT RUT_EP FROM EMPLEADO");
  int registros = model->rowCount();
  if (registros == 0) {
    ui->label_48->setText("ERROR NO HAY VENDEDORES INGRESADOS");
    ui->codigoPistola_2->setEnabled(false);
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
    ui->codigoPistola_2->setEnabled(false);
    return;
  }

  // llenado de proveedores
  ui->listaProveedores->clear();
  for (int i = 0; i < registrosProv; i++) {
    ui->listaProveedores->addItem(model->data(model->index(i, 0)).toString());
  }

  ui->listaProveedores->clearEditText();
  ui->label_48->clear();
  ui->codigoPistola_2->setEnabled(true);
  ui->listaProveedores->setFocus();
}

void MainWindow::on_salir_clicked() { QApplication::quit(); }

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
