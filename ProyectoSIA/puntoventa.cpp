#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_realizarVenta_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(9);
  ui->label->setText("REALIZAR VENTA");
  ui->detalleBoleta->clear();
  listaVenta.clear();
  ui->totalBoleta->display(0);
  ui->detalleBoleta->append(encabezado);
  ui->codigoPistola->setEnabled(false);
  ui->listaEmpleados->clear();

  model->setQuery("SELECT RUT_EP FROM EMPLEADO");
  int registros = model->rowCount();
  if (registros == 0) {
    ui->label_28->setText("ERROR NO HAY VENDEDORES INGRESADOS");
    return;
  }

  // llenado de empleados
  for (int i = 0; i < registros; i++) {
    ui->listaEmpleados->addItem(model->data(model->index(i, 0)).toString());
  }

  ui->label_28->clear();
  ui->codigoPistola->setEnabled(true);
}

void MainWindow::on_codigoPistola_returnPressed() {
  QString codigo = ui->codigoPistola->text();
  QString cantidad = ui->cantidad->text();
  if (codigo == "" || cantidad == "") return;
  int cantidadI = cantidad.toInt();
  // validación
  QString query = "SELECT * FROM PRODUCTO WHERE COD_BR_PD = %0 AND ESTADO = 1";
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

  // generar impresion
  QPrinter impresora;
  QPrintDialog *ventanaImpresion = new QPrintDialog(&impresora);
  ventanaImpresion->setWindowTitle("Imprimir boleta");
  if (ventanaImpresion->exec() == QDialog::Accepted) {
    QPainter painter;
    painter.begin(&impresora);
    QString texto = "Botillería Santa Isabel\n" +
                    QDate::currentDate().toString(Qt::ISODate) + "\n\n" +
                    ui->detalleBoleta->toPlainText();
    painter.setFont(QFont("hack",8));
    painter.drawText(100, 100, 500, 500, Qt::AlignLeft | Qt::AlignTop, texto);
    painter.end();
  }
  // limpiar
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
