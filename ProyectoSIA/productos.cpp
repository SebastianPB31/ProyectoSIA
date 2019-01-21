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
      "CF,PRODUCTO.CANT_CRIT_PD,ISNULL (STOCK.CANTIDAD_RP,0), ISNULL((SELECT "
      "CONVERT(VARCHAR, REG_PROVISION.FECHA_VENCI_RP,105)),'SIN FECHA') FROM "
      "PRODUCTO LEFT JOIN CLASIFICACION ON PRODUCTO.ID_CF=CLASIFICACION.ID_CF "
      "LEFT JOIN STOCK ON PRODUCTO.COD_BR_PD=STOCK.COD_BR_PD LEFT JOIN "
      "REG_PROVISION ON PRODUCTO.COD_BR_PD = REG_PROVISION.COD_BR_PD WHERE "
      "PRODUCTO.ESTADO=1",
      db);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nombre producto"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("codigo"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Precio unitario"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tipo producto"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Stock critico"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Stock actual"));
  model->setHeaderData(6, Qt::Horizontal,
                       QObject::tr("Fecha vencimiento ultimo lote"));

  // imprimir a pantalla
  ui->tablaProducto->setModel(model);
  ui->tablaProducto->show();
}

void MainWindow::on_tablaProducto_doubleClicked(const QModelIndex &index) {
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText(model->data(index).toString());
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
  ui->estatus->clear();

  model->setQuery("SElECT * FROM CLASIFICACION");
  int registros = model->rowCount();
  // llenado de tipos de productos
  for (int i = 0; i < registros; i++) {
    ui->tipoProd->addItem(model->data(model->index(i, 1)).toString(),
                          model->data(model->index(i, 0)).toString());
  }
}

void MainWindow::on_agregarNuevoProducto_clicked() {
  QString codigo = ui->codigoProd->text(),
          clasificacion = ui->tipoProd->currentData().toString(),
          nombre = ui->nombreProd->text(), precio = ui->precioProd->text(),
          cantMin = ui->cantidadMinProd->text();

  if (codigo == "" || nombre == "" || clasificacion == "" || nombre == "" ||
      cantMin == "") {
    ui->estatus->setText("ERROR, algún dato fue dejado en blanco");
    return;
  } else {
    ui->estatus->clear();
  }

  // SQL query
  QString query = "INSERT INTO PRODUCTO VALUES (%0, %1, '%2', %3, %4, 1)";
  model->setQuery(
      query.arg(codigo).arg(clasificacion).arg(nombre).arg(precio).arg(cantMin),
      db);

  // verificar cambios
  if (model->lastError().isValid())
    ui->estatus->setText("ERROR, producto exsite");
  else
    ui->estatus->setText("Producto agregado");
}

void MainWindow::on_editarProducto_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(8);
  ui->label->setText("EDITAR PRODUCTO");
  ui->tipoProd_2->clear();
  ui->nombreProd_2->clear();
  ui->precioProd_2->clear();
  ui->cantidadMinProd_2->clear();
  ui->estatus->clear();
  ui->codigoProd_2->clear();
  ui->codigoProd_2->setFocus();

  model->setQuery("SElECT * FROM CLASIFICACION");
  int registros = model->rowCount();
  // llenado de tipos de productos
  for (int i = 0; i < registros; i++) {
    ui->tipoProd_2->addItem(model->data(model->index(i, 1)).toString(),
                            model->data(model->index(i, 0)).toString());
  }

  ui->estadoProducto->addItem("Activo", "1");
  ui->estadoProducto->addItem("Inactivo", "0");
}

void MainWindow::on_codigoProd_2_returnPressed() {
  QString codigo = ui->codigoProd_2->text();
  QString query =
      "SELECT P.COD_BR_PD, P.NOM_PD, C.NOM_CF, P.PRECIO_PD, P.CANT_CRIT_PD, "
      "P.ESTADO FROM PRODUCTO P, CLASIFICACION C WHERE P.ID_CF=C.ID_CF AND "
      "P.COD_BR_PD = %0";
  model->setQuery(query.arg(codigo));
  ui->nombreProd_2->setText(model->data(model->index(0, 1)).toString());
  ui->tipoProd_2->setCurrentText(model->data(model->index(0, 2)).toString());
  ui->precioProd_2->setText(model->data(model->index(0, 3)).toString());
  ui->cantidadMinProd_2->setText(model->data(model->index(0, 4)).toString());
  if (model->data(model->index(0, 5)).toBool() == true) {
    ui->estadoProducto->setCurrentIndex(0);
  } else {
    ui->estadoProducto->setCurrentIndex(1);
  }
  ui->tipoProd_2->setCurrentText("");
}

void MainWindow::on_agregarNuevoProducto_2_clicked() {
  // lectura datos
  QString codigo = ui->codigoProd_2->text(),
          clasificacion = ui->tipoProd_2->currentData().toString(),
          nombre = ui->nombreProd_2->text(), precio = ui->precioProd_2->text(),
          cantidad = ui->cantidadMinProd_2->text(),
          estado = ui->estadoProducto->currentData().toString();

  // verificacion
  if (codigo == "" || clasificacion == "" || nombre == "" || precio == "" ||
      cantidad == "") {
    ui->estatus->setText("ERROR, algún dato fue dejado en blanco");
    return;
  } else {
    ui->estatus->clear();
  }
  // actualizar datos
  QString query =
      "UPDATE PRODUCTO SET ID_CF = %0, NOM_PD = '%1', PRECIO_PD = %2, "
      "CANT_CRIT_PD = %3, "
      "ESTADO = %4 WHERE COD_BR_PD = %5";
  model->setQuery(query.arg(clasificacion)
                      .arg(nombre)
                      .arg(precio)
                      .arg(cantidad)
                      .arg(estado)
                      .arg(codigo));
  // verificar cambios
  query = "SELECT COD_BR_PD FROM PRODUCTO WHERE COD_BR_PD = %0";
  model->setQuery(query.arg(codigo));
  if (model->rowCount() == 0)
    ui->estatus->setText("ERROR, producto no existe");
  else
    ui->estatus->setText("Producto modificado");
}
