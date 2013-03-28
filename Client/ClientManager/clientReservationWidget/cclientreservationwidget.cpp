#include "cclientreservationwidget.h"
#include "ui_cclientreservationwidget.h"

#define CLIENTS_ID 0
#define CLIENTS_FIO 1
#define CLIENTS_BIRTHDATE 2
#define CLIENTS_LOGIN 3
#define CLIENTS_COUNT 4

#define RESERV_ID 0
#define RESERV_ACTION 1
#define RESERV_ROW 2
#define RESERV_SEAT 3
#define RESERV_STATE 4
#define RESERV_DATE 5

CClientReservationWidget::CClientReservationWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CClientReservationWidget)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);

	ui->twClients->setColumnWidth(0, 0);
	ui->twClients->hideColumn(0);

	ui->twReservations->setColumnWidth(0, 0);
	ui->twReservations->hideColumn(0);
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
			if(query.isNull(0) == false)
			{
				item = new QTreeWidgetItem();
				if(item)
				{
					item->setText(CLIENTS_ID, query.value(0).toString());
					item->setText(CLIENTS_FIO, query.value(1).toString());
					item->setText(CLIENTS_BIRTHDATE, query.value(2).toString());
					item->setText(CLIENTS_LOGIN, query.value(3).toString());
					item->setText(CLIENTS_COUNT, query.value(4).toString());

					ui->twClients->addTopLevelItem(item);
				}
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
		if(arg1.isEmpty() || ui->twClients->topLevelItem(i)->text(CLIENTS_FIO).contains(arg1))
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
		ui->lClient->setText(tr("Клиент: %1").arg(selected->text(CLIENTS_FIO)));

		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("SELECT"
					  " Reservations.id,"
					  " Actions.title,"
					  " Actions.state,"
					  " Actions.dateTime,"
					  " PlaceSchemes.row,"
					  " PlaceSchemes.seatNumber "
					  "FROM"
					  " Reservations,"
					  " Actions,"
					  " PlaceSchemes "
					  "WHERE"
					  " Reservations.id_client = :clientId AND "
					  " Actions.id = Reservations.id_action AND "
					  " PlaceSchemes.id = Reservations.id_placeScheme"
					  ";");
		query.bindValue(":clientId", selected->text(CLIENTS_ID).toInt());
		if(query.exec())
		{
			ui->twReservations->clear();

			QTreeWidgetItem *item;
			while(query.next())
			{
				item = new QTreeWidgetItem;
				if(item)
				{
					item->setText(RESERV_ID, query.value(0).toString());
					item->setText(RESERV_ACTION, query.value(1).toString());
					item->setText(RESERV_ROW, query.value(4).toString());
					item->setText(RESERV_SEAT, query.value(5).toString());
					item->setText(RESERV_STATE, Global::actionStateToText( query.value(2).toInt() ));
					item->setText(RESERV_DATE, query.value(3).toDateTime().toString("dd.MM.yyyy hh:mm"));

					ui->twReservations->addTopLevelItem(item);
				}
			}

			ui->stackedWidget->slideHorizontalNext();
		}
		else
			qDebug(qPrintable(query.lastError().text()));
	}
}

void CClientReservationWidget::on_pbnSelectAll_clicked()
{
	ui->twReservations->selectAll();
}

void CClientReservationWidget::on_pbnUnreservate_clicked()
{
	QList<QTreeWidgetItem*> selected = ui->twReservations->selectedItems();
	if(selected.isEmpty() == false)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите отменить бронь у выбранных мест?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Reservations WHERE id = :id;");
			for(int i = 0; i < selected.size(); i++)
			{
				query.bindValue(":id", selected[i]->text(RESERV_ID));
				query.exec();
			}
		}
	}
}

void CClientReservationWidget::on_pbnSoldReservated_clicked()
{
	QList<QTreeWidgetItem*> selected = ui->twReservations->selectedItems();
	if(selected.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Внимание"), tr("Данный функционал еще не реализован."));
	}
}

void CClientReservationWidget::on_pbnBackToClients_clicked()
{
	ui->stackedWidget->slideHorizontalPrev();
}
