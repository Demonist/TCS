#include "controlmanagermainwindow.h"
#include "ui_controlmanagermainwindow.h"

#define SEAT	0
#define ROW		1
#define IDENT	2
#define INFO	3

void ControlManagerMainWindow::addItem(QTreeWidgetItem *item, const QColor &color)
{
	item->setBackgroundColor(SEAT, color);
	item->setBackgroundColor(ROW, color);
	item->setBackgroundColor(IDENT, color);
	item->setBackgroundColor(INFO, color);
	ui->twBarcodeControl->addTopLevelItem(item);
	ui->twBarcodeControl->scrollToBottom();
}

ControlManagerMainWindow::ControlManagerMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ControlManagerMainWindow)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
	ui->leGetBarcode->setValidator(new QRegExpValidator(QRegExp("\\d+")));

	ui->wConnection->setConnectionType(CDataBaseConnectionWidget::ConnectionFile);
	ui->wConnection->setConnectionChoiceEnable(false);

	connect(ui->aExit, SIGNAL(activated()), this, SLOT(close()));
	connect(ui->wConnection, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));
	connect(ui->wConnection, SIGNAL(closed()), this, SLOT(close()));
	connect(ui->aImportDB, SIGNAL(activated()), this, SLOT(importDataBase()));
	connect(ui->aLogFileWrite, SIGNAL(activated()), this, SLOT(writeLogFile()));

	mLabelColorAnimation.setTargetObject(this);
	mLabelColorAnimation.setPropertyName("labelColor");
	mLabelColorAnimation.setEasingCurve(QEasingCurve::Linear);
}

ControlManagerMainWindow::~ControlManagerMainWindow()
{
	mLabelColorAnimation.stop();
	delete ui;
}

void ControlManagerMainWindow::on_leGetBarcode_returnPressed()
{
	const static QColor treeColorCorrect(131, 255, 131);
	const static QColor treeColorWrong(255, 131, 131);
	const static QColor widgetColorCorrect(0, 200, 0);
	const static QColor widgetColorWrong(255, 50, 50);

	QSqlDatabase db = QSqlDatabase::database(mConnectionName);
	if(db.isOpen() && db.isValid())
	{
		CTicketIdentifier ticketIdentifier(ui->leGetBarcode->text());
		QString identifier = ticketIdentifier.identifier();
		QSqlQuery query(db);
		query.prepare("SELECT id, id_placeScheme FROM Tickets WHERE identifier = :identifier AND passedFlag = 'false'");
		query.bindValue(":identifier", identifier);
		if(query.exec())
		{
			if(query.first())
			{
				setLabelColorAlbescent(widgetColorCorrect);
				QTreeWidgetItem *item = new QTreeWidgetItem;
				if(item)
				{
					QSqlQuery selectScheme(db);
					selectScheme.prepare("SELECT seatNumber, row FROM PlaceSchemes WHERE id = :id");
					selectScheme.bindValue(":id", query.value(1));
					if(selectScheme.exec())
					{
						if(selectScheme.first())
						{
							item->setText(SEAT, selectScheme.value(0).toString());
							item->setText(ROW, selectScheme.value(1).toString());
						}
						else
						{
							item->setText(SEAT, tr("Фанзона"));
							item->setText(ROW, tr("Фанзона"));
						}
						item->setText(IDENT, ui->leGetBarcode->text());
						item->setText(INFO, tr("Билет валиден"));
						addItem(item, treeColorCorrect);

						QSqlQuery setTrueFlag(db);
						setTrueFlag.exec("UPDATE Tickets SET passedFlag = 'true' WHERE id = " + query.value(0).toString());
					}
				}
			}
			else
			{
				query.prepare("SELECT id, id_placeScheme, id_client FROM Tickets WHERE identifier = :identifier AND passedFlag = 'true'");
				query.bindValue(":identifier", identifier);
				if(query.exec())
				{
					if(query.first())
					{
						setLabelColorAlbescent(widgetColorWrong);
						QTreeWidgetItem *item = new QTreeWidgetItem;
						if(item)
						{
							QSqlQuery selectScheme(db);
							selectScheme.prepare("SELECT seatNumber, row FROM PlaceSchemes WHERE id = :id");
							selectScheme.bindValue(":id", query.value(1));
							if(selectScheme.exec())
							{
								if(query.isNull(2) || query.value(2).toInt() == 0)
								{
									if(selectScheme.first())
									{
										item->setText(SEAT, selectScheme.value(0).toString());
										item->setText(ROW, selectScheme.value(1).toString());
									}
									else
									{
										item->setText(SEAT, tr("Фанзона"));
										item->setText(ROW, tr("Фанзона"));
									}
									item->setText(IDENT, ui->leGetBarcode->text());
									item->setText(INFO, tr("Попытка повторного прохода"));
									addItem(item, treeColorWrong);
								}
								else
								{
									QSqlQuery clientsquery(db);
									clientsquery.prepare("SELECT login, name FROM Clients WHERE id = :id");
									clientsquery.bindValue(":id", query.value(2));
									if(clientsquery.exec() && clientsquery.first())
									{
										if(selectScheme.first())
										{
											item->setText(SEAT, selectScheme.value(0).toString());
											item->setText(ROW, selectScheme.value(1).toString());
										}
										else
										{
											item->setText(SEAT, tr("Фанзона"));
											item->setText(ROW, tr("Фанзона"));
										}
										item->setText(IDENT, ui->leGetBarcode->text());
										item->setText(INFO, tr("Попытка повторного прохода. Билет приобретен клиентом ") + clientsquery.value(1).toString() + tr(", его логин - ") + clientsquery.value(0).toString());
										addItem(item, treeColorWrong);
									}
								}
							}
						}
					}
					else
					{
						QSqlQuery query1(db);
						query1.prepare("SELECT COUNT(identifier), id, id_client FROM ReturnedTickets WHERE identifier = :identifier;");
						query1.bindValue(":identifier", identifier);
						if(query1.exec() && query1.first())
						{

							if(query1.value(0).toInt() >= 1)
							{
								setLabelColorAlbescent(widgetColorWrong);
								QTreeWidgetItem *item = new QTreeWidgetItem;
								if(item)
								{
									item->setText(SEAT, tr("Внимание"));
									item->setText(ROW, tr("Внимание"));
									if(query1.isNull(2) || query1.value(2).toInt() == 0)
									{
										item->setText(IDENT, ui->leGetBarcode->text());
										item->setText(INFO, tr("Попытка пройти по сданному билету"));
									}
									else
									{
										QSqlQuery clientsquery(db);
										clientsquery.prepare("SELECT login, name FROM Clients WHERE id = :id");
										clientsquery.bindValue(":id", query1.value(2));
										if(clientsquery.exec() && clientsquery.first())
										{
											item->setText(IDENT, ui->leGetBarcode->text());
											item->setText(INFO, tr("Попытка пройти по сданному билету. Билет приобретен клиентом ") + clientsquery.value(1).toString() + tr(", его логин - ") + clientsquery.value(0).toString());
										}
									}
									addItem(item, treeColorWrong);
								}
							}
							else
							{
								setLabelColorAlbescent(widgetColorWrong);
								QTreeWidgetItem *item = new QTreeWidgetItem;
								if(item)
								{
									item->setText(SEAT, tr("Внимание"));
									item->setText(ROW, tr("Внимание"));
									item->setText(IDENT, ui->leGetBarcode->text());
									item->setText(INFO, tr("Попытка пройти по несуществующему билету"));
									addItem(item, treeColorWrong);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			QMessageBox::critical(this, tr("Ошибка выполнения запроса."), tr("Ошибка выполнения запроса.\nБаза данных не валидна."));
		}
		ui->leGetBarcode->setText("");
	}
	else
	{
		QMessageBox::critical(this, tr("База данных не открыта!"), tr("База данных не открыта!\nОткройте базу данных"));
		ui->leGetBarcode->setText("");
	}
}

void ControlManagerMainWindow::connected(QString connectionName)
{
	mConnectionName = connectionName;
	QSqlDatabase db = QSqlDatabase::database(mConnectionName);

	if(!db.open())
	{
		QMessageBox::critical(this, tr("Ошибка в открытии базы данных!"), tr("База данных не открыта!\nОшибка в открытии базы данных!"));
	}
	else
	{
		QSqlQuery query(db);
		query.prepare("SELECT title, performer FROM Actions");
		if(query.exec() && query.first())
		{
			ui->stackedWidget->slideHorizontalNext();
			showMaximized();
			ui->aLogFileWrite->setEnabled(true);
			ui->groupBox->setTitle(tr("База данных концерта: ") + query.value(0).toString() + tr(", Исполнитель: ") + query.value(1).toString());
		}
	}
}

void ControlManagerMainWindow::importDataBase()
{
	CControlUpdateDBase cudb;
	cudb.exec();
}

void ControlManagerMainWindow::writeLogFile()
{
	QString logp = QFileDialog::getSaveFileName(this, tr("Записать лог"), QDir::currentPath(), tr("Файл лога(.txt)"));
	if(!logp.isEmpty())
	{
		QFile file(logp);
		file.open(QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);

		for(int i = 0; i < ui->twBarcodeControl->topLevelItemCount(); i++)
		{
			out << ui->twBarcodeControl->topLevelItem(i)->text(SEAT) << ui->twBarcodeControl->topLevelItem(i)->text(ROW) <<  ui->twBarcodeControl->topLevelItem(i)->text(IDENT) <<  ui->twBarcodeControl->topLevelItem(i)->text(INFO) <<"\n";
		}

		file.close();
	}
}

//public slots:

void ControlManagerMainWindow::setLabelColor(const QColor &color)
{
	ui->wOutResult->setStyleSheet(QString("QWidget{border: 1px solid qrgba(255, 255, 255, 0);\nborder-radius: 5px;\nbackground: %1;};").arg(color.name()));
}

void ControlManagerMainWindow::setLabelColorAlbescent(const QColor &color, const int duration)
{
	mLabelColorAnimation.stop();

	QColor endValue(200, 200, 200);
	int r = color.red(), g = color.green(), b = color.blue();
	if(r != g || g != b)
	{
		if(r >= g && r >= b)
		{
			r = 255;
			g = 170;
			b = 170;
		}
		else if(g >= r && g >= b)
		{
			r = 170;
			g = 255;
			b = 170;
		}
		else if(b >= r && b >= g)
		{
			r = 170;
			g = 170;
			b = 255;
		}
		endValue = QColor(r, g, b);
	}

	mLabelColorAnimation.setStartValue(color);
	mLabelColorAnimation.setEndValue(endValue);
	mLabelColorAnimation.setDuration(duration);
	mLabelColorAnimation.start();
}
