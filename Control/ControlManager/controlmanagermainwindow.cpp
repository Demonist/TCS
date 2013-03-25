#include "controlmanagermainwindow.h"
#include "ui_controlmanagermainwindow.h"

ControlManagerMainWindow::ControlManagerMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ControlManagerMainWindow)
{
	ui->setupUi(this);
	ui->wConnection->setConnectionType(CDataBaseConnectionWidget::ConnectionFile);
	ui->wConnection->setConnectionChoiceEnable(false);
	connect(ui->aExit, SIGNAL(activated()), this, SLOT(close()));
	connect(ui->wConnection, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));
	connect(ui->wConnection, SIGNAL(closed()), this, SLOT(close()));
	connect(ui->aImportDB, SIGNAL(activated()), this, SLOT(importDataBase()));
	connect(ui->aLogFileWrite, SIGNAL(activated()), this, SLOT(writeLogFile()));
	ui->stackedWidget->setCurrentIndex(0);
	//ui->leGetBarcode->setValidator(new QIntValidator());
}

ControlManagerMainWindow::~ControlManagerMainWindow()
{
	delete ui;
}

void ControlManagerMainWindow::on_leGetBarcode_returnPressed()
{
	QSqlDatabase db = QSqlDatabase::database(mConnectionName);
	if(db.isOpen() && db.isValid())
	{
		CTicketIdentifier *ticketIdentifier = new CTicketIdentifier(ui->leGetBarcode->text());
		QString identifier = ticketIdentifier->identifier();
		QSqlQuery query(db);
		query.prepare("SELECT COUNT(identifier), passedFlag, id FROM Tickets WHERE identifier = :identifier");
		query.bindValue(":identifier", identifier);
		if(query.exec() && query.first())
		{
			if((query.value(0).toInt() > 0) && (query.value(1).toBool() == false))
			{
				ui->wOutResult->setStyleSheet("background: green");
				QTreeWidgetItem *item = new QTreeWidgetItem;
				if(item)
				{
					item->setText(0, ui->leGetBarcode->text() + tr(" - Билет валиден"));
					item->setBackgroundColor(0, QColor(131, 255, 131));
					ui->twBarcodeControl->addTopLevelItem(item);
					QSqlQuery setTrueFlag(db);
					setTrueFlag.exec("UPDATE Tickets SET passedFlag = 'true' WHERE id = " + query.value(2).toString());
				}
			}
			else if((query.value(0).toInt() > 0) && (query.value(1).toBool() == true))
			{
				ui->wOutResult->setStyleSheet("background: yellow");
				QTreeWidgetItem *item = new QTreeWidgetItem;
				if(item)
				{
					item->setText(0, ui->leGetBarcode->text() + tr(" - Попытка повторного прохода"));
					item->setBackgroundColor(0, QColor(255, 240, 40));
					ui->twBarcodeControl->addTopLevelItem(item);
				}
			}
			else
			{
				QSqlQuery query1(db);
				query1.prepare("SELECT COUNT(identifier), id FROM ReturnedTickets WHERE identifier = :identifier;");
				query1.bindValue(":identifier", identifier);
				if(query1.exec() && query1.first())
				{

					if(query1.value(0).toInt() == 1)
					{
						ui->wOutResult->setStyleSheet("background: red");
						QTreeWidgetItem *item = new QTreeWidgetItem;
						if(item)
						{
							item->setText(0, ui->leGetBarcode->text() + tr(" - Попытка пройти по сданному билету"));
							item->setBackgroundColor(0, QColor(255, 131, 131));
							ui->twBarcodeControl->addTopLevelItem(item);
						}
					}
					else
					{
						ui->wOutResult->setStyleSheet("background: red");
						QTreeWidgetItem *item = new QTreeWidgetItem;
						if(item)
						{
							item->setText(0, ui->leGetBarcode->text() + tr(" - Попытка пройти по несуществующему билету"));
							item->setBackgroundColor(0, QColor(255, 131, 131));
							ui->twBarcodeControl->addTopLevelItem(item);
							ui->twBarcodeControl->scrollToBottom();//По всем расставить
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
		query.prepare("SELECT title, performer, dateTime FROM Actions");
		if(query.exec() && query.first())
		{
			ui->stackedWidget->slideHorizontalNext();
			ui->groupBox->setTitle(tr("База данных концерта: ") + query.value(0).toString() + tr(", Исполнитель: ") + query.value(1).toString() + tr(", Дата проведения концерта: ") + query.value(2).toString());
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
	//TODO Дописать
	QString logp = QFileDialog::getSaveFileName(this, tr("Записать лог"), QDir::currentPath(), tr("Файл лога(.txt)"));
	if(!logp.isEmpty())
	{
		QFile file(logp);
		file.open(QIODevice::Append | QIODevice::Text);
		QTextStream out(&file);

		for(int i = 0; i < ui->twBarcodeControl->topLevelItemCount(); i++)
		{
			out << ui->twBarcodeControl->topLevelItem(i)->text(0) << "\n";
		}

		file.close();
	}

}
