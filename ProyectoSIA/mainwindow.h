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

  void on_salir_clicked();

  void on_emitirBoleta_clicked();

  void on_codigoBorrar_returnPressed();

  void on_conectar_clicked();

  void on_agregarNuevoProducto_clicked();

  void on_agregarNuevoProveedor_clicked();

  void on_codigoPistola_2_returnPressed();

  void on_agregarCaja_clicked();

  void on_emitirBoleta_2_clicked();

  void on_asistenciaEmpleado_clicked();

  void on_asistenciaEmpleadoRut_clicked();

  void on_listarEmpleados_clicked();

  void on_datoEmpleadoRut_clicked();

  void on_agregarEmpleado_2_clicked();

  void on_buscarEmpleado_clicked();

  void on_modificarEmpleado_clicked();

  void on_buscarProv_clicked();

  void on_modificarProv_clicked();

  void on_codigoProd_2_returnPressed();

  void on_agregarNuevoProducto_2_clicked();

  void on_pushButton_clicked();

  void on_buscarNumBoleta_clicked();

  void on_buscarFechaBoleta_clicked();

private:
  Ui::MainWindow *ui;
  QSqlDatabase db;
  QSqlQueryModel *model;
  QString encabezado;
  // codigo-precio-cantidad-lineaBoleta
  QVector<std::tuple<QString, int, int, QString>> listaVenta;
  // codigo-precio-cantidad-lineaBoleta
  QVector<std::tuple<QString, int, int, QDate, QString>> listaCompra;
};

#endif  // MAINWINDOW_H
