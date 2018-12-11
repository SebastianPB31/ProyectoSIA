#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private slots:
  void on_infoEmpleado_clicked();

  void on_listaProducto_clicked();

  void on_agregarEmpleado_clicked();

  void on_editarEmpleado_clicked();

  void on_agregarProveedor_clicked();

  void on_editarProveedor_clicked();

  void on_agregarProducto_clicked();

  void on_editarProducto_clicked();

  void on_realizarVenta_clicked();

  void on_realizarCompra_clicked();

  void on_codigoPistola_returnPressed();

  void on_cantidad_returnPressed();

  void on_salir_clicked();

  void on_emitirBoleta_clicked();

  void on_codigoBorrar_returnPressed();

  void on_conectar_clicked();

  void on_agregarNuevoProducto_clicked();

 private:
  Ui::MainWindow *ui;
  QSqlDatabase db;
  QSqlQueryModel *model;
  QString encabezado;
  QVector<std::tuple<int, int, int, QString>> ListaCompra;
};

#endif  // MAINWINDOW_H
