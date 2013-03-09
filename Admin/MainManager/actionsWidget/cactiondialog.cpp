#include "cactiondialog.h"
#include "ui_cactiondialog.h"

CActionDialog::CActionDialog(const QString &connectionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CActionDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление мероприятия"));
	ui->pbnTicketsManagement->setEnabled(false);
	ui->pbnTicketsManagement->setToolTip(tr("Данный функционал доступен только при редактировании"));
	ui->gbxLogo->setEnabled(false);
	ui->pbnLogoImage->setToolTip(tr("Данный функционал доступен только при редактировании"));
	mType = Add;

	ui->cbxState->addItem(Global::actionStateToText(Global::ActionInactive));
	ui->gbxState->setEnabled(false);

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	if(query.exec("SELECT DISTINCT performer FROM Actions;"))
	{
		QStringList completerData;
		while(query.next())
			completerData.append(query.value(0).toString());
		mCompleterModel.setStringList(completerData);
		mCompleter.setModel(&mCompleterModel);
		ui->lePerformer->setCompleter(&mCompleter);
	}

	if(query.exec("SELECT id, name FROM Categories;"))
		while(query.next())
			ui->cbxCategory->addItem(query.value(1).toString(), query.value(0));
	if(query.exec("SELECT id, title, address FROM Places;"))
		while(query.next())
			ui->cbxPlace->addItem(query.value(1).toString() + " (" + query.value(2).toString() + ')', query.value(0));
}

CActionDialog::CActionDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CActionDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование мероприятия"));
	ui->gbxPlace->setEnabled(false);
	ui->gbxPlace->setToolTip(tr("Данный функционал доступен только при добавлении"));
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	if(query.exec("SELECT DISTINCT performer FROM Actions;"))
	{
		QStringList completerData;
		while(query.next())
			completerData.append(query.value(0).toString());
		mCompleterModel.setStringList(completerData);
		mCompleter.setModel(&mCompleterModel);
		ui->lePerformer->setCompleter(&mCompleter);
	}

	query.prepare("SELECT Actions.title, Places.title, Actions.dateTime, Actions.state, Actions.id_category, Actions.description, Actions.performer FROM Actions, Places WHERE Actions.id = :id AND Places.id = Actions.id_place;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->leTitle->setText(query.value(0).toString());
		ui->cbxPlace->addItem(query.value(1).toString());
		QDateTime dateTime = query.value(2).toDateTime();
		ui->leDate->setText(dateTime.toString("dd.MM.yyyy"));
		ui->sbxHour->setValue(dateTime.time().hour());
		ui->sbxMinute->setValue(dateTime.time().minute());
		ui->teDescription->setPlainText(query.value(5).toString());
		ui->lePerformer->setText(query.value(6).toString());

		ui->cbxState->addItems(Global::actionStates());
		ui->cbxState->setCurrentIndex(ui->cbxState->findText(Global::actionStateToText(query.value(3).toInt())));

		if(Global::ActionInactive != query.value(3).toInt())
		{
			ui->pbnTicketsManagement->setEnabled(false);
			ui->pbnTicketsManagement->setToolTip(tr("Данный функционал доступен только для неактивных мероприятий"));
		}

		int catId = query.value(4).toInt();
		int index = 0;

		if(query.exec("SELECT id, name FROM Categories;"))
			while(query.next())
			{
				ui->cbxCategory->addItem(query.value(1).toString(), query.value(0));
				if(catId == query.value(0).toInt())
					index = ui->cbxCategory->count() - 1;
			}
		ui->cbxCategory->setCurrentIndex(index);
	}
	else
		qDebug(qPrintable(query.lastError().text()));

}

CActionDialog::~CActionDialog()
{
    delete ui;
}

void CActionDialog::on_pbnCancel_clicked()
{
	close();
}

void CActionDialog::on_pbnApply_clicked()
{
	//Проверка входных данных:
	QString error;
	if(ui->leTitle->text().isEmpty())
		error += tr("Название мероприятия не может быть пустым.\n");
	if(error.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Внимание"), error);
		return;
	}

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	if(mType == Add)
	{
		query.prepare("INSERT INTO Actions VALUES(NULL, :title, :performer, :description, :date, :state, 0, 0, 0, :id_place, :id_cat, NULL);");
		query.bindValue(":title", ui->leTitle->text());
		query.bindValue(":performer", ui->lePerformer->text());
		query.bindValue(":description", ui->teDescription->toPlainText());
		QString h = ui->sbxHour->text();
		if(h.size() == 1)
			h.prepend('0');
		QString m = ui->sbxMinute->text();
		if(m.size() == 1)
			m.prepend('0');
		query.bindValue(":date", QString("%1 %2:%3:00").arg(QDate::fromString(ui->leDate->text(), "dd.MM.yyyy").toString("yyyy-MM-dd")).arg(h).arg(m));
		query.bindValue(":state", Global::actionStateFromText(ui->cbxState->currentText()));
		query.bindValue(":id_place", ui->cbxPlace->itemData(ui->cbxPlace->currentIndex()));
		query.bindValue(":id_cat", ui->cbxCategory->itemData(ui->cbxCategory->currentIndex()));
		if(!query.exec())
			qDebug(qPrintable(query.lastError().text()));
	}
	else if(mType == Edit)
	{
		Global::ActionState state = (Global::ActionState)Global::actionStateFromText(ui->cbxState->currentText());
		if(state == Global::ActionComplited)
		{
			if(QMessageBox::Yes == QMessageBox::warning(this, tr("Внимание"), tr("При завершении мероприятия будет сформирован подробный отчет по продажам билетов, при этом само мероприятие и все его билеты будут удалены из базы данных т.е. будет невозможно экспортировать список билетов для контроля.\nВы действительно хотите продолжить?"), QMessageBox::Yes, QMessageBox::No))
			{
				CComplitedAction action;
				if(action.makeFromAction(mId, mConnectionName) && action.toQuery(&query))
				{
					if(query.exec())
					{
						query.prepare("DELETE FROM Actions WHERE id = :id");
						query.bindValue(":id", mId);
						if(!query.exec())
							qDebug(qPrintable(query.lastError().text()));
					}
					else
						qDebug(qPrintable(query.lastError().text()));
				}
				else
					qDebug("CComplitedAction error.");
			}
			else
				return;
		}
		else
		{
			query.prepare("UPDATE Actions SET title = :title, performer = :performer, description = :description, dateTime = :date, state = :state, id_category = :id_cat WHERE id = :id;");
			query.bindValue(":id", mId);
			query.bindValue(":title", ui->leTitle->text());
			query.bindValue(":performer", ui->lePerformer->text());
			query.bindValue(":description", ui->teDescription->toPlainText());
			query.bindValue(":date",
							QDateTime(
								QDate::fromString(ui->leDate->text(), "dd.MM.yyyy"),
								QTime::fromString(tr("%1:%2").arg(ui->sbxHour->value()).arg(ui->sbxMinute->value()), "h:m")
								)
							);
			query.bindValue(":state", state);
			query.bindValue(":id_cat", ui->cbxCategory->itemData(ui->cbxCategory->currentIndex()));
			if(!query.exec())
				qDebug(qPrintable(query.lastError().text()));
		}
	}

	emit dataWasUpdated();
	close();
}

void CActionDialog::on_tbnDate_clicked()
{
	CDateDialog dateDialog(this);
	if(ui->leDate->text().isEmpty() == false)
		dateDialog.setDate(QDate::fromString(ui->leDate->text(), "dd.MM.yyyy"));
	dateDialog.exec();
	if(dateDialog.result())
		ui->leDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
}

void CActionDialog::on_pbnTicketsManagement_clicked()
{
	if(mType == Edit)
	{
		CActionTicketsManagement ticketsManagement(mConnectionName, mId, this);
		ticketsManagement.exec();
	}
}

void CActionDialog::on_pbnLogoImage_clicked()
{
	CImageDialog imageDialog(mConnectionName, "Actions", "id_ticketSubstrate", tr("id = %1").arg(mId));
	imageDialog.setWindowTitle(tr("Подложка билета"));
	imageDialog.exec();
}
