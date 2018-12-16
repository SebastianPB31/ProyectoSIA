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
  ui->estatus_4->clear();
}

void MainWindow::on_agregarNuevoProveedor_clicked() {
  QString rut = ui->rutProv->text(), nombre = ui->nombreProv->text(),
          apellido = ui->apellidoProv->text(), fono = ui->fonoProv->text(),
          empresa = ui->nombreEmpProv->text();

  if (rut == "" || nombre == "" || apellido == "" || fono == "" ||
      empresa == "") {
    ui->estatus_4->setText("ERROR, algún dato fue dejado en blanco");
    return;
  } else {
    ui->estatus_4->clear();
  }
  // verificacion
  QString query = "SELECT RUT_PV FROM PROVEEDOR WHERE RUT_PV = %0";
  model->setQuery(query.arg(ui->rutProv->text()));
  if (model->rowCount() != 0) {
    ui->estatus_4->setText("ERROR, Proveedor existente");
    return;
  } else
    ui->estatus_4->clear();
  // SQL query
  query = "INSERT INTO PROVEEDOR VALUES (%0, '%1', '%2', %3, '%4', '%5', 1)";
  model->setQuery(
      query.arg(rut).arg(nombre).arg(apellido).arg(fono).arg(empresa));

  ui->rutProv->setFocus();
  ui->rutProv->clear();
  ui->nombreProv->clear();
  ui->apellidoProv->clear();
  ui->fonoProv->clear();
  ui->nombreEmpProv->clear();
  ui->estatus_4->clear();
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
  ui->estatus_5->clear();

  // verif proveedores
  model->setQuery("SELECT RUT_PV FROM PROVEEDOR");
  int registros = model->rowCount();
  if (registros == 0) {
    ui->estatus_5->setText("ERROR NO HAY PROVEEDORES INGRESADOS");
    return;
  } else {
    ui->estatus_5->clear();
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


void MainWindow::on_modificarProv_clicked()
{

}

