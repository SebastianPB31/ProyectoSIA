#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_conectar_clicked() {
  ui->stackedWidget->setCurrentIndex(0);
  if (model != nullptr) {
    model->destroyed();
    model = nullptr;
  }
  db.close();

  if (QSqlDatabase::connectionNames().size() == 0) {
    db = QSqlDatabase::addDatabase("QODBC3");
  }

  QString driver = ui->driver->text(), server = ui->server_name->text(),
          dataBase = ui->DDBB->text(), userID = ui->userID->text(),
          password = ui->password->text(),
          texto = "DRIVER=%0;SERVER=%1;DATABASE=%2;UID=%3;PWD=%4";

  db.setDatabaseName(
      texto.arg(driver).arg(server).arg(dataBase).arg(userID).arg(password));

  bool ok = db.open();

  if (ok) {
    ui->log->setText(QString("OK"));
    model = new QSqlQueryModel;
  } else
    ui->log->setText(QString(db.lastError().text()));
}
