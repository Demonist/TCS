#include "cclientreservationwidget.h"
#include "ui_cclientreservationwidget.h"

#define ID 0
#define FIO 1
#define BIRTHDATE 2
#define LOGIN 3
#define COUNT 4

CClientReservationWidget::CClientReservationWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CClientReservationWidget)
{
	ui->setupUi(this);
	ui->twClients->setColumnWidth(0, 0);
	ui->twClients->hideColumn(0);
}

CClientReservationWidget::~CClientReservationWidget()
{
	delete ui;
}

void CClientReservationWidget::updateData()
{
	ui->stackedWidget->setCurrentIndexAnimatedHorizontal(0);

	ui->twClients->clear();

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT Clients.id, Clients.name, Clients.birthDate, Clients.login, COUNT(Reservations.id) FROM Clients, Reservations WHERE Reservations.id_client = Clients.id;"))
	{
		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(FIO, query.value(1).toString());
				item->setText(BIRTHDATE, query.value(2).toString());
				item->setText(LOGIN, query.value(3).toString());
				item->setText(COUNT, query.value(4).toString());

				ui->twClients->addTopLevelItem(item);
			}
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));
}

void CClientReservationWidget::on_tbnClearClient_clicked()
{
	ui->leClient->clear();
}

void CClientReservationWidget::on_leClient_textChanged(const QString &arg1)
{
	for(int i = 0; i < ui->twClients->topLevelItemCount(); i++)
	{
		if(arg1.isEmpty() || ui->twClients->topLevelItem(i)->text(FIO).contains(arg1))
			ui->twClients->topLevelItem(i)->setHidden(false);
		else
			ui->twClients->topLevelItem(i)->setHidden(true);
	}
}

void CClientReservationWidget::on_pbnReservationManagement_clicked()
{
	QTreeWidgetItem *selected = ui->twClients->selectedItems().value(0);
	if(selected)
	{
		////
		ui->stackedWidget->slideHorizontalNext();
	}
}
