#include "creturnticketwidget.h"
#include "ui_creturnticketwidget.h"

void CReturnTicketWidget::clear()
{
	ui->leAction->clear();
	ui->leActionState->clear();
	ui->leClientName->clear();
	ui->leClientPhone->clear();
	ui->leDateTime->clear();
	ui->lePayment->clear();
	ui->lePenalty->clear();
	ui->lePlace->clear();
	ui->lePrice->clear();
	ui->lePriceGroup->clear();
	ui->leRow->clear();
	ui->leSeat->clear();
	mTicketId = 0;
	ui->pbnReturnTicket->setEnabled(false);
}

CReturnTicketWidget::CReturnTicketWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CReturnTicketWidget)
{
	ui->setupUi(this);
	mTicketId = 0;
}

CReturnTicketWidget::~CReturnTicketWidget()
{
	delete ui;
}

void CReturnTicketWidget::updateData()
{

}

void CReturnTicketWidget::on_leIdentifier_textChanged(const QString &arg1)
{
	if(arg1.isEmpty())
	{
		ui->lIdentifierState->clear();
		clear();
	}
	else if(CTicketIdentifier::isValidIdentifier(arg1))
	{
		ui->lIdentifierState->setStyleSheet("QLabel{color:green;};");
		ui->lIdentifierState->setText(tr("Нажмите 'ВВОД'"));
		mTicketId = 0;
	}
	else
	{
		ui->lIdentifierState->setStyleSheet("QLabel{color: #f66;};");
		ui->lIdentifierState->setText(tr("Неправильный идентификатор"));
		clear();
	}
}

void CReturnTicketWidget::on_leIdentifier_returnPressed()
{
	clear();

	CTicketIdentifier identifier(ui->leIdentifier->text());
	if(identifier.isValid())
	{
		QString data = identifier.data();
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("SELECT"
					  " Tickets.id,"
					  " Tickets.id_placeScheme,"
					  " Tickets.id_client,"

					  " Actions.id,"
					  " Actions.title,"
					  " Actions.state,"
					  " Actions.dateTime,"
					  " Actions.fanPrice,"
					  " Actions.fanPenalty,"

					  " Places.title,"
					  " Places.address "
					  "FROM"
					  " Tickets,"
					  " Actions,"
					  " Places "
					  "WHERE"
					  " Tickets.identifier = :identifier AND"
					  " Actions.id = Tickets.id_action AND"
					  " Places.id = Actions.id_place"
					  ";");
		query.bindValue(":identifier", data);
		if(query.exec())
		{
			if(query.first())
			{
				uint payment = 0;
				int price;
				mTicketId = query.value(0).toInt();
				int placeSchemeId = query.value(1).toInt();
				mClientId = query.value(2).toInt();
				mActionId = query.value(3).toInt();
				ui->leAction->setText(query.value(4).toString());
				Global::ActionState actionState = (Global::ActionState)query.value(5).toInt();
				ui->leActionState->setText(Global::actionStateToText(actionState));
				ui->leDateTime->setText(query.value(6).toDateTime().toString("dd.MM.yyyy hh:mm"));
				ui->lePlace->setText(query.value(9).toString() + " (" + query.value(10).toString() + ')');

				if(placeSchemeId == 0)
				{
					ui->lePriceGroup->setText(tr("Фан-зона"));
					ui->lePrice->setText(tr("%1 руб.").arg(query.value(7).toString()));
					ui->lePenalty->setText(tr("%1 руб.").arg(query.value(8).toString()));
					price = query.value(7).toInt();
					payment = query.value(7).toInt() - query.value(8).toInt();
				}
				else
				{
					query.prepare("SELECT"
								  " ActionPriceGroups.name,"
								  " ActionPriceGroups.price,"
								  " ActionPriceGroups.penalty,"

								  " PlaceSchemes.seatNumber,"
								  " PlaceSchemes.row "
								  "FROM"
								  " ActionScheme,"
								  " ActionPriceGroups,"
								  " PlaceSchemes "
								  "WHERE"
								  " ActionScheme.id_placeScheme = :placeId AND"
								  " ActionScheme.id_action = :actId AND"
								  " ActionPriceGroups.id = ActionScheme.id_priceGroup AND"
								  " PlaceSchemes.id = :placeId2"
								  ";");
					query.bindValue(":placeId", placeSchemeId);
					query.bindValue(":actId", mActionId);
					query.bindValue(":placeId2", placeSchemeId);
					if(query.exec() && query.first())
					{
						ui->lePriceGroup->setText(query.value(0).toString());
						ui->lePrice->setText(query.value(1).toString());
						ui->lePenalty->setText(query.value(2).toString());
						ui->leSeat->setText(query.value(3).toString());
						ui->leRow->setText(query.value(4).toString());
						payment = query.value(1).toInt() - query.value(2).toInt();
						price = query.value(1).toInt();
					}
					else
						qDebug(qPrintable(query.lastError().text()));
				}

				if(mClientId)
				{
					query.prepare("SELECT name, phone FROM Clients WHERE id = :id;");
					query.bindValue(":id", mClientId);
					if(query.exec() && query.first())
					{
						ui->leClientName->setText(query.value(0).toString());
						ui->leClientPhone->setText(query.value(1).toString());
					}
					else
						qDebug(qPrintable(query.lastError().text()));
				}

				switch(actionState)
				{
					case Global::ActionComplited:
					case Global::ActionInactive:
						ui->lePayment->setText(tr("Билет возврату не подлежит"));
						mTicketId = 0;
						break;
					case Global::ActionCanceled:
						ui->lePayment->setText(tr("%1 руб.").arg(price));
						break;
					default:
						ui->lePayment->setText(tr("%1 руб.").arg(payment));
						ui->pbnReturnTicket->setEnabled(true);
				}
			}
			else
			{
				ui->lIdentifierState->setStyleSheet("QLabel{color:red;};");
				ui->lIdentifierState->setText(tr("Нет такого билета"));
			}
		}
		else
			qDebug(qPrintable(query.lastError().text()));
	}
	else
	{
		ui->lIdentifierState->setStyleSheet("QLabel{color:red;};");
		ui->lIdentifierState->setText(tr("Неправильный идентификатор"));
	}
}

void CReturnTicketWidget::on_tbnClearIdentifier_clicked()
{
	ui->leIdentifier->clear();
}

void CReturnTicketWidget::on_pbnReturnTicket_clicked()
{
	if(mTicketId && QMessageBox::Yes == QMessageBox::question(this, tr("Подтвердите возврат"), tr("Вы действительно хотите возвратить данный билет?"), QMessageBox::Yes, QMessageBox::No))
	{
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("DELETE FROM Tickets WHERE id = :id;");
		query.bindValue(":id", mTicketId);
		if(query.exec())
		{
			CTicketIdentifier identifier(ui->leIdentifier->text());
			CStatisticTicketReturnedType type;
			type.marketId = CMarket::instance()->marketId();
			type.sellerId = CMarket::instance()->sellerId();
			type.actionId = mActionId;
			type.ticketId = mTicketId;
			type.barCode = identifier.identifier();
			type.ticketIdentifier = identifier.data();
			type.clientId = mClientId;
			CStatistics::instance()->write(type);

			clear();
			ui->leIdentifier->clear();
			ui->lIdentifierState->clear();
			QMessageBox::information(this, tr("Успех"), tr("Билет успешно возвращен.\nТеперь уничтожте печатную версию билета - она бесполезна."));
		}
		else
			qDebug(qPrintable(query.lastError().text()));
	}
}
