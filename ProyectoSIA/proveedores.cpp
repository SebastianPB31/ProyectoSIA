#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_agregarProveedor_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(5);
  ui->label->setText("AGREGAR PROVEEDOR");
  ui->rutProv->setFocus();
  ui->rutProv->clear();
  ui->nombreProv->clear();
  ui->apellidoProv->clear();
  ui->fonoProv->clear();
  ui->nombreEmpProv->clear();
  ui->estatus->clear();
}

void MainWindow::on_agregarNuevoProveedor_clicked() {
  QString rut = ui->rutProv->text(), nombre = ui->nombreProv->text(),
          apellido = ui->apellidoProv->text(), fono = ui->fonoProv->text(),
          empresa = ui->nombreEmpProv->text();

  if (rut == "" || nombre == "" || apellido == "" || fono == "" ||
      empresa == "") {
    ui->estatus->setText("ERROR, algún dato fue dejado en blanco");
    return;
  } else {
    ui->estatus->clear();
  }
  // verificacion
  QString query = "SELECT RUT_PV FROM PROVEEDOR WHERE RUT_PV = %0";
  model->setQuery(query.arg(ui->rutProv->text()));
  if (model->rowCount() != 0) {
    ui->estatus->setText("ERROR, Proveedor existente");
    return;
  } else
    ui->estatus->clear();
  // SQL query
  query = "INSERT INTO PROVEEDOR VALUES (%0, '%1', '%2', %3, '%4', '%5', 1)";
  model->setQuery(
      query.arg(rut).arg(nombre).arg(apellido).arg(fono).arg(empresa));

  // verificar cambios
  if (model->lastError().isValid())
    ui->estatus->setText("ERROR, proveedor existe");
  else
    ui->estatus->setText("Proveedor ingresado");
}

void MainWindow::on_editarProveedor_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(6);
  ui->label->setText("EDITAR PROVEEDOR");
  ui->listaProv->clear();
  ui->listaProv->setFocus();
  ui->nombreProv_2->clear();
  ui->apellidoProv_2->clear();
  ui->fonoProv_2->clear();
  ui->nombreEmpProv_2->clear();
  ui->listaProv->clear();
  ui->estadoProveedor->clear();
  ui->estatus->clear();

  // verif proveedores
  model->setQuery("SELECT RUT_PV FROM PROVEEDOR");
  int registros = model->rowCount();
  if (registros == 0) {
    ui->estatus->setText("ERROR NO HAY PROVEEDORES INGRESADOS");
    return;
  } else {
    ui->estatus->clear();
  }
  // llenado de proveedores
  for (int i = 0; i < registros; i++) {
    ui->listaProv->addItem(model->data(model->index(i, 0)).toString());
  }
  ui->listaProv->setCurrentText("");
  ui->estadoProveedor->addItem("Activo", "1");
  ui->estadoProveedor->addItem("Inactivo", "0");
}

void MainWindow::on_buscarProv_clicked() {
  QString query =
      "SELECT NOM_PV, APE_PV, FONO_PV, EMPRESA_PV, ESTADO "
      "FROM PROVEEDOR WHERE RUT_PV = %0";
  model->setQuery(query.arg(ui->listaProv->currentText()));
  ui->nombreProv_2->setText(model->data(model->index(0, 0)).toString());
  ui->apellidoProv_2->setText(model->data(model->index(0, 1)).toString());
  ui->fonoProv_2->setText(model->data(model->index(0, 2)).toString());
  ui->nombreEmpProv_2->setText(model->data(model->index(0, 3)).toString());
  if (model->data(model->index(0, 4)).toBool() == true) {
    ui->estadoEmpleado->setCurrentIndex(0);
  } else {
    ui->estadoEmpleado->setCurrentIndex(1);
  }
}

void MainWindow::on_modificarProv_clicked() {
  // lectura datos
  QString rut = ui->listaProv->currentText(), nombre = ui->nombreProv_2->text(),
          apellido = ui->apellidoProv_2->text(),
          empresa = ui->nombreEmpProv_2->text(),
          fono = ui->nombreEmpProv_2->text(),
          estado = ui->estadoProveedor->currentData().toString();

  // verificacion
  if (rut == "" || nombre == "" || apellido == "" || empresa == "" ||
      fono == "") {
    ui->estatus->setText("ERROR, algún dato fue dejado en blanco");
    return;
  } else {
    ui->estatus->clear();
  }
  // actualizar datos
  QString query =
      "UPDATE PROVEEDOR SET NOM_PV = '%0', APE_PV = '%1', FONO_PV = '%2', "
      "EMPRESA_PV = '%3', ESTADO = %5 WHERE RUT_PV = %6";
  model->setQuery(
      query.arg(nombre).arg(apellido).arg(fono).arg(empresa).arg(estado).arg(
          rut));
  // verificar cambios
  query = "SELECT RUT_PV FROM PROVEEDOR WHERE RUT_PV = %0";
  model->setQuery(query.arg(rut));
  if (model->rowCount() == 0)
    ui->estatus->setText("ERROR, proveedor no existe");
  else
    ui->estatus->setText("Proveedor modificado");
}
