CREATE DATABASE SANTA_ISABEL;	/* CREACI�N DE LA BASE DE DATOS DE LA BOTILLERIA SANTA ISABEL */
GO
USE SANTA_ISABEL;

/* CREAR TABLAS SIN CLAVES FORANEAS */

CREATE TABLE EMPLEADO			/* CREACI�N DE LA TABLA EMPLEADO */
(RUT_EP		INT PRIMARY KEY,
NOM_EP		VARCHAR(30),
APE_EP		VARCHAR(30),
SUELDO_EP	INT,
FONO_EP		VARCHAR(30),
DIRECC_EP	VARCHAR(30),
ESTADO BIT NOT NULL);

CREATE TABLE CLASIFICACION		/* CREACI�N DE LA TABLA CLASIFICACI�N */
(ID_CF		INT PRIMARY KEY,
NOM_CF		VARCHAR(30));

CREATE TABLE PROVEEDOR			/* CREACI�N DE LA TABLA PROVEEDOR */
(RUT_PV		INT PRIMARY KEY,
NOM_PV		VARCHAR(30),
APE_PV		VARCHAR(30),
FONO_PV		VARCHAR(15),
EMPRESA_PV	VARCHAR(30),
ESTADO BIT NOT NULL);

/*CREAR TABLAS CON UNA CLAVE FORANEA*/

CREATE TABLE HORARIO			/* CREACI�N DE LA TABLA HORARIO */
(ID_HO		INT PRIMARY KEY,
RUT_EP		INT FOREIGN KEY REFERENCES EMPLEADO(RUT_EP),
HO_EN_HO	TIME,
HO_SA_HO	TIME,
FE_TB_HO	DATE);

CREATE TABLE VENTA				/* CREACI�N DE LA TABLA VENTA */
(ID_VENTA	INT IDENTITY(1,1) PRIMARY KEY,
RUT_EP		INT FOREIGN KEY REFERENCES EMPLEADO(RUT_EP),
FECHA_VT	DATE,
HORA_VT		TIME);

CREATE TABLE PRODUCTO			/* CREACI�N DE LA TABLA PRODUCTO */
(COD_BR_PD	BIGINT PRIMARY KEY,
ID_CF		INT FOREIGN KEY REFERENCES CLASIFICACION(ID_CF),
NOM_PD		VARCHAR(30),
PRECIO_PD	INT,
CANT_CRIT_PD INT,
ESTADO BIT NOT NULL);

CREATE TABLE PROVISION			/* CREACI�N DE LA TABLA PROVISI�N */
(ID_PV		INT IDENTITY(1,1) PRIMARY KEY,
RUT_PV		INT FOREIGN KEY REFERENCES PROVEEDOR(RUT_PV),
FECHA_PI	DATE,
HORA_PI		TIME,
FECHA_MAX_PI DATE,
DEUDA_PI	INT);

/*CREAR TABLAS CON DOS CLAVES FORANEAS*/

CREATE TABLE REG_VENTA			/* CREACI�N DE LA TABLA	REGISTRO VENTA */
(ID_VENTA	INT FOREIGN KEY REFERENCES VENTA(ID_VENTA),
COD_BR_PD	BIGINT FOREIGN KEY REFERENCES PRODUCTO(COD_BR_PD),
CANT_RV		INT NOT NULL);

CREATE TABLE REG_PROVISION		/* CREACI�N DE LA TABLA REGISTRO PROVISI�N */
(ID_PV		INT FOREIGN KEY REFERENCES PROVISION(ID_PV),
COD_BR_PD	BIGINT FOREIGN KEY REFERENCES PRODUCTO(COD_BR_PD),
CANTIDAD_RP	INT,
FECHA_VENCI_RP DATE);

CREATE TABLE STOCK
(COD_BR_PD BIGINT FOREIGN KEY REFERENCES PRODUCTO(COD_BR_PD),
CANTIDAD_RP	INT
);
