#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_infoEmpleado_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(1);
  ui->label->setText("INFORMACION DE EMPLEADOS");
  // desasociar modelo
  ui->tablaEmpleado->setModel(nullptr);
  ui->estatus->clear();
}

void MainWindow::on_asistenciaEmpleado_clicked() {
  model->setQuery(
      "SELECT "
      "E.RUT_EP,E.NOM_EP,E.APE_EP,E.SUELDO_EP,E.FONO_EP,E.DIRECC_EP,CAST "
      "((SELECT CONVERT(TIME(0), HORARIO.HO_EN_HO)) AS VARCHAR(8)),CAST "
      "((SELECT CONVERT(TIME(0), HORARIO.HO_SA_HO)) AS VARCHAR(8)), "
      "HORARIO.FE_TB_HO FROM "
      "EMPLEADO E,HORARIO WHERE E.RUT_EP=HORARIO.RUT_EP AND E.ESTADO = 1 ORDER "
      "BY E.RUT_EP",
      db);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("RUT"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Apellido"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Sueldo por hora"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Fono"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Direccion"));
  model->setHeaderData(6, Qt::Horizontal, QObject::tr("Hora entrada"));
  model->setHeaderData(7, Qt::Horizontal, QObject::tr("Hora salida"));
  model->setHeaderData(8, Qt::Horizontal, QObject::tr("Fecha"));

  // imprimir a pantalla
  ui->tablaEmpleado->setModel(model);
  ui->tablaEmpleado->show();
}

void MainWindow::on_asistenciaEmpleadoRut_clicked() {
  if (ui->rutEmpleadoAsistencia->text() == "") return;
  // verificacion
  QString query = "SELECT RUT_EP FROM EMPLEADO WHERE RUT_EP = %0";
  model->setQuery(query.arg(ui->rutEmpleadoAsistencia->text()));
  if (model->rowCount() == 0) {
    ui->estatus->setText("ERROR, RUT no encontrado");
    return;
  } else
    ui->estatus->clear();
  // SQL query
  query =
      "SELECT "
      "E.RUT_EP,E.NOM_EP,E.APE_EP,E.SUELDO_EP,E.FONO_EP,E.DIRECC_EP,CAST "
      "((SELECT CONVERT(TIME(0), HORARIO.HO_EN_HO)) AS VARCHAR(8)),CAST "
      "((SELECT CONVERT(TIME(0), HORARIO.HO_SA_HO)) AS VARCHAR(8)), "
      "HORARIO.FE_TB_HO FROM "
      "EMPLEADO E,HORARIO WHERE E.RUT_EP=HORARIO.RUT_EP AND E.ESTADO = 1 AND "
      "E.RUT_EP = %0 ORDER BY E.RUT_EP";

  model->setQuery(query.arg(ui->rutEmpleadoAsistencia->text()), db);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("RUT"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Apellido"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sueldo por hora"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Fono"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Direccion"));
  model->setHeaderData(6, Qt::Horizontal, QObject::tr("Hora entrada"));
  model->setHeaderData(7, Qt::Horizontal, QObject::tr("Hora salida"));
  model->setHeaderData(8, Qt::Horizontal, QObject::tr("Fecha"));

  // imprimir a pantalla
  ui->tablaEmpleado->setModel(model);
  ui->tablaEmpleado->show();
}

void MainWindow::on_listarEmpleados_clicked() {
  QString query =
      "SELECT E.RUT_EP,E.NOM_EP,E.APE_EP,E.SUELDO_EP,E.FONO_EP,E.DIRECC_EP "
      "FROM EMPLEADO E WHERE E.ESTADO=1";
  model->setQuery(query);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("RUT"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Apellido"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sueldo por hora"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Fono"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Direccion"));

  // imprimir a pantalla
  ui->tablaEmpleado->setModel(model);
  ui->tablaEmpleado->show();
}

void MainWindow::on_datoEmpleadoRut_clicked() {
  if (ui->rutEmpleadoDato->text() == "") return;
  // verificacion
  QString query = "SELECT RUT_EP FROM EMPLEADO WHERE RUT_EP = %0";
  model->setQuery(query.arg(ui->rutEmpleadoDato->text()));
  if (model->rowCount() == 0) {
    ui->estatus->setText("ERROR, RUT no encontrado");
    return;
  } else
    ui->estatus->clear();
  // SQL query
  query =
      "SELECT E.RUT_EP,E.NOM_EP,E.APE_EP,E.SUELDO_EP,E.FONO_EP,E.DIRECC_EP "
      "FROM EMPLEADO E WHERE E.ESTADO=1 AND E.RUT_EP = %0";
  model->setQuery(query.arg(ui->rutEmpleadoDato->text()));
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("RUT"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Apellido"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sueldo por hora"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Fono"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Direccion"));

  // imprimir a pantalla
  ui->tablaEmpleado->setModel(model);
  ui->tablaEmpleado->show();
}

void MainWindow::on_agregarEmpleado_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(3);
  ui->label->setText("AGREGAR EMPLEADO");
  ui->nombreEmpleado->clear();
  ui->apellidosEmpleado->clear();
  ui->sueldoEmpleado->clear();
  ui->fonoEmpleado->clear();
  ui->direccionEmpleado->clear();
  ui->estatus_2->clear();
}

void MainWindow::on_agregarEmpleado_2_clicked() {
  QString rut = ui->rutEmpleado->text(), nombre = ui->nombreEmpleado->text(),
          apellido = ui->apellidosEmpleado->text(),
          sueldo = ui->sueldoEmpleado->text(), fono = ui->fonoEmpleado->text(),
          direccion = ui->direccionEmpleado->text();

  if (rut == "" || nombre == "" || apellido == "" || sueldo == "" ||
      fono == "" || direccion == "") {
    ui->estatus_2->setText("ERROR, algún dato fue dejado en blanco");
    return;
  } else {
    ui->estatus_2->clear();
  }
  // verificacion
  QString query = "SELECT RUT_EP FROM EMPLEADO WHERE RUT_EP = %0";
  model->setQuery(query.arg(ui->rutEmpleado->text()));
  if (model->rowCount() != 0) {
    ui->estatus_2->setText("ERROR, RUT existe");
    return;
  } else
    ui->estatus_2->clear();
  // SQL query
  query = "INSERT INTO EMPLEADO VALUES (%0, '%1', '%2', %3, '%4', '%5', 1)";
  model->setQuery(
      query.arg(rut).arg(nombre).arg(apellido).arg(sueldo).arg(fono).arg(
          direccion));
}

void MainWindow::on_editarEmpleado_clicked() {
  if (model == nullptr) return;
  ui->stackedWidget->setCurrentIndex(4);
  ui->label->setText("EDITAR EMPLEADO");
  ui->listaEmpleados_3->clear();
  ui->estadoEmpleado->clear();
  ui->nombreEmpleado_2->clear();
  ui->apellidosEmpleado_2->clear();
  ui->sueldoEmpleado_2->clear();
  ui->fonoEmpleado_2->clear();
  ui->direccionEmpleado_2->clear();

  // verif empleados
  model->setQuery("SELECT RUT_EP FROM EMPLEADO");
  int registros = model->rowCount();
  if (registros == 0) {
    ui->estatus_3->setText("ERROR NO HAY EMPLEADOS INGRESADOS");
    return;
  } else {
    ui->estatus_3->clear();
  }
  // llenado de empleados
  for (int i = 0; i < registros; i++) {
    ui->listaEmpleados_3->addItem(model->data(model->index(i, 0)).toString());
  }
  ui->listaEmpleados_3->setCurrentText("");
  ui->estadoEmpleado->addItem("Activo", "1");
  ui->estadoEmpleado->addItem("Inactivo", "0");
}

void MainWindow::on_buscarEmpleado_clicked() {
  QString query =
      "SELECT E.NOM_EP,E.APE_EP,E.SUELDO_EP,E.FONO_EP,E.DIRECC_EP, E.ESTADO "
      "FROM EMPLEADO E WHERE E.RUT_EP = %0";
  model->setQuery(query.arg(ui->listaEmpleados_3->currentText()));
  ui->nombreEmpleado_2->setText(model->data(model->index(0, 0)).toString());
  ui->apellidosEmpleado_2->setText(model->data(model->index(0, 1)).toString());
  ui->sueldoEmpleado_2->setText(model->data(model->index(0, 2)).toString());
  ui->fonoEmpleado_2->setText(model->data(model->index(0, 3)).toString());
  ui->direccionEmpleado_2->setText(model->data(model->index(0, 4)).toString());
  if (model->data(model->index(0, 5)).toBool() == true) {
    ui->estadoEmpleado->setCurrentIndex(0);
  } else {
    ui->estadoEmpleado->setCurrentIndex(1);
  }
}

void MainWindow::on_modificarEmpleado_clicked() {
  // lectura datos
  QString rut = ui->listaEmpleados_3->currentText(),
          nombre = ui->nombreEmpleado_2->text(),
          apellido = ui->apellidosEmpleado_2->text(),
          sueldo = ui->sueldoEmpleado_2->text(),
          fono = ui->fonoEmpleado_2->text(),
          direccion = ui->direccionEmpleado_2->text(),
          estado = ui->estadoEmpleado->currentData().toString();

  // verificacion
  if (nombre == "" || apellido == "" || sueldo == "" || fono == "" ||
      direccion == "") {
    ui->estatus_3->setText("ERROR, algún dato fue dejado en blanco");
    return;
  } else {
    ui->estatus_3->clear();
  }
  // actualizar datos
  QString query =
      "UPDATE EMPLEADO SET NOM_EP = '%0', APE_EP = '%1',SUELDO_EP = %2, "
      "FONO_EP = '%3', DIRECC_EP = '%4', ESTADO = %5 WHERE RUT_EP = %6";
  model->setQuery(query.arg(nombre)
                      .arg(apellido)
                      .arg(sueldo)
                      .arg(fono)
                      .arg(direccion)
                      .arg(estado)
                      .arg(rut));
}
