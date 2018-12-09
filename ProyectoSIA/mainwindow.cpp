#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = nullptr;
    encabezado= QString("PRODUCTO");
    encabezado += QString(" ").repeated(30 - encabezado.size()) + QString("PRECIO");
    encabezado += QString(" ").repeated(40 - encabezado.size()) + QString("CANTIDAD");

}

MainWindow::~MainWindow()
{
    if (model) model->destroyed();
    db.close();
    delete ui;
}

void MainWindow::on_conectar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    db.close();
    db.removeDatabase("QODBC3");
    if (model != nullptr){
        model->destroyed();
    }
    QString driver = ui->driver->text(),
            server = ui->server_name->text(),
            dataBase = ui->DDBB->text(),
            userID = ui->userID->text(),
            password = ui->password->text(),
            texto = "DRIVER=%0;SERVER=%1;DATABASE=%2;UID=%3;PWD=%4";

    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName(texto.arg(driver).arg(server).arg(dataBase).arg(userID).arg(password));
    model = new QSqlQueryModel;
    bool ok = db.open();

    if (ok) ui->log->setText(QString("OK"));
    else ui->log->setText(QString(db.lastError().text()));

}

void MainWindow::on_infoEmpleado_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(1);
    ui->label->setText("INFORMACION DE EMPLEADOS");
    // SQL query
    // imprimir a pantalla

    model->setQuery("SELECT * FROM EMPLEADO", db);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("RUT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sueldo"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Apellido"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Telefono"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Direccion"));
    ui->tablaEmpleado->setModel(model);
    ui->tablaEmpleado->show();

}


void MainWindow::on_listaProducto_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(2);
    ui->label->setText("LISTA DE PRODUCTOS");
    // SQL query
    // imprimir a pantalla
    model->setQuery("SELECT * FROM PRODUCTO", db);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Codigo"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Codigo caja"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nombre producto"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Precio unitario"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Stock"));

    ui->tablaProducto->setModel(model);
    ui->tablaProducto->show();
}

void MainWindow::on_agregarEmpleado_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(3);
    ui->label->setText("AGREGAR EMPLEADO");
    // entrada -> SQL query

}

void MainWindow::on_editarEmpleado_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(4);
    ui->label->setText("EDITAR EMPLEADO");
    // entrada -> SQL query
}

void MainWindow::on_agregarProveedor_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(5);
    ui->label->setText("AGREGAR PROVEEDOR");
    // entrada -> SQL query
}

void MainWindow::on_editarProveedor_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(6);
    ui->label->setText("EDITAR PROVEEDOR");
    // entrada -> SQL query
}

void MainWindow::on_agregarProducto_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(7);
    ui->label->setText("AGREGAR PRODUCTO");
    // entrada -> SQL query
}

void MainWindow::on_editarProducto_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(8);
    ui->label->setText("EDITAR PRODUCTO");
    // entrada -> SQL query
}

void MainWindow::on_realizarVenta_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(9);
    ui->label->setText("REALIZAR VENTA");
    // entrada -> SQL query
    ui->detalleBoleta->clear();
    ListaCompra.clear();
    ui->totalBoleta->display(0);
    ui->detalleBoleta->append(encabezado);
}

void MainWindow::on_realizarCompra_clicked()
{
    if (model == nullptr) return;
    ui->stackedWidget->setCurrentIndex(10);
    ui->label->setText("REALIZAR COMPRA");
    // entrada -> SQL query
}

void MainWindow::on_salir_clicked()
{
    QApplication::quit();
}

void MainWindow::on_codigoPistola_returnPressed()
{
    QString codigo = ui->codigoPistola->text();
    QString cantidad = ui->cantidad->text();
    if (codigo == "" || cantidad == "") return;
    int cantidadI = cantidad.toInt();
    // validación
    QString query = "SELECT * FROM PRODUCTO WHERE COD_BR_PD = %0";
    model->setQuery(query.arg(codigo) , db);

    QString verif = model->data(model->index(0,0)).toString();

    if (verif == codigo){
        // producto existe
        QString nombre = model->data(model->index(0, 2)).toString();
        int precio= model->data(model->index(0, 3)).toInt();
        ui->label_28->setText(nombre);
        QString linea = nombre + QString(" ").repeated(30 - nombre.size()) + QString::number(precio);
        linea = linea + QString(" ").repeated(encabezado.size() - linea.size() - cantidad.size()) + cantidad;
        ui->detalleBoleta->append(linea);

        ListaCompra.append(std::tuple<int,int,int,QString>(codigo.toInt(),cantidadI, precio,linea));

        ui->totalBoleta->display(ui->totalBoleta->intValue() + precio*cantidadI);
        ui->cantidad->setText(QString("1"));
        ui->codigoPistola->clear();
        ui->codigoPistola->setFocus();
    }
    else ui->label_28->setText("PRODUCTO NO EXISTE");

}

void MainWindow::on_cantidad_returnPressed()
{
    ui->codigoPistola->setFocus();
}


void MainWindow::on_emitirBoleta_clicked()
{
    // actualizar base
    // generar PDF
    // Imprimir

    for (std::tuple<int,int,int,QString> elemento: ListaCompra){
        QString queryCantidad = "SELECT CANTIDAD_RP FROM REG_PROVISION WHERE COD_BR_PD = %0";
        QString query = "UPDATE REG_PROVISION SET CANTIDAD_RP = %0 WHERE COD_BR_PD = %1";
        model->setQuery(queryCantidad.arg(std::get<0>(elemento)),db);
        int cantidad = model->data(model->index(0,0)).toInt();
        model->setQuery(query.arg(cantidad - std::get<1>(elemento)).arg(std::get<0>(elemento)));
    }

    ui->detalleBoleta->clear();
    ui->totalBoleta->display(0);
    ListaCompra.clear();
}

void MainWindow::on_codigoBorrar_returnPressed()
{
    //borrar elemento
    if (ui->codigoBorrar->text() == "") return;
    int codigo = ui->codigoBorrar->text().toInt();
    ui->detalleBoleta->clear();
    ui->detalleBoleta->append(encabezado);
    // volver a calular precio
    int precioTotal = 0;
    QVector<std::tuple<int,int,int,QString>> nuevaListaCompra;
    for (std::tuple<int,int,int,QString> elemento: ListaCompra){
        if (std::get<0>(elemento) != codigo){
            nuevaListaCompra.append(elemento);
            precioTotal+=std::get<1>(elemento)*std::get<2>(elemento);
            ui->detalleBoleta->append(std::get<3>(elemento));
        }
    }
    ui->codigoBorrar->clear();
    ui->totalBoleta->display(precioTotal);
    ListaCompra=nuevaListaCompra;
}



