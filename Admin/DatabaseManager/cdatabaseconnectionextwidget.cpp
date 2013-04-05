#include "cdatabaseconnectionextwidget.h"
#include "ui_cdatabaseconnectionwidget.h"

#define INI_REMEMBER "General/rememberFlag"
#define INI_FILES "General/dataBaseFiles"
#define INI_CONNECTION_TYPE "General/connectionType"
#define INI_LASTFILE "General/lastFile"
#define INI_LASTSERVER "General/lastServer"
#define INI_DBNAME "dataBaseName"
#define INI_LOGIN "dataBaseLogin"
#define INI_PASSWORD "dataBasePassword"

//protected:

void CDatabaseConnectionExtWidget::processConnecting()
{
	ui->lDbConnectionStatus->setText(tr("Подключение..."));
	QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

	bool connectingToServer = ui->rbnDbServer->isChecked(); //Флаг, показывающий подключаемся ли мы к серверу или к файлу.

	QSqlDatabase db = QSqlDatabase::addDatabase(connectingToServer ? "QMYSQL" : "QSQLITE", mConnectionName);

	if(db.isValid() == false)
	{
		QMessageBox::critical(this, tr("Ошибка"), tr("Не найден драйвер базы данных."));
		return;
	}

	if(connectingToServer)
	{
		db.setHostName(ui->cbxDbHost->currentText());
		db.setDatabaseName(ui->leDbName->text());
		db.setUserName(ui->leDbLogin->text());
		db.setPassword(ui->leDbPassword->text());

		if(db.hostName().isEmpty())
		{
			ui->lDbConnectionStatus->setText(tr("Укажите хост подключения"));
			return;
		}
	}
	else    //connecting to file
	{
		db.setDatabaseName(ui->cbxDbFileName->currentText());
		if(db.databaseName().isEmpty())
		{
			ui->lDbConnectionStatus->setText(tr("Файл не выбран"));
			return;
		}
	}

	if(db.open())   //connection is successfully
	{
		mConnected = true;

		ui->lDbConnectionStatus->setText(tr("Подключение произведено"));
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

		//кеширование:
		if(mCache)
		{
			if(connectingToServer)
			{
				mCache->setValue(INI_CONNECTION_TYPE, "server");
				mCache->setValue(INI_LASTSERVER, db.hostName());

				if(ui->chbxDbRemember->isChecked()) //Галочка 'Запомнить'
				{
					mCache->beginGroup(db.hostName());
					mCache->setValue(INI_DBNAME, db.databaseName());
					mCache->setValue(INI_LOGIN, db.userName());
					mCache->setValue(INI_PASSWORD, Global::crypt(db.password()));
					mCache->endGroup();
				}
			}
			else    //connecting to file
			{
				mCache->setValue(INI_CONNECTION_TYPE, "file");
				mCache->setValue(INI_LASTFILE, ui->cbxDbFileName->currentText());

				QStringList files = mCache->value(INI_FILES).toStringList();
				if(files.contains(ui->cbxDbFileName->currentText()) == false)
				{
					files.append(ui->cbxDbFileName->currentText());
					mCache->setValue(INI_FILES, files);
				}
			}

			mCache->sync();
		}

		if(!connectingToServer)
			emit connectedToFile(ui->cbxDbFileName->currentText());
		else if(db.databaseName().isEmpty())
			emit connectedToHost();
		else
			emit connectedToDatabase(db.databaseName());
	}
	else
	{
		ui->lDbConnectionStatus->setText(tr("Ошибка подключения"));
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		qDebug("CDataBaseConnectionWidget::on_pbnDbConnect_clicked: error: %d", db.lastError().number());
		QString errorText = tr("Не удалось подключиться к базе данных.\n");
		switch(db.lastError().number())
		{
			case 1045:
				errorText += tr("Ошибка аутентификации. Проверьте правильность логина и пароля.\n");
				break;
			case 1044:
				errorText += tr("Неверно указано имя базы данных.\n");
				break;
			case 2005:
				errorText += tr("Не удалось найти указанный хост.\n");
				break;
			case -1:
				errorText += tr("Указанный файл не является базой данных.");
				break;
		}
		if(db.lastError().number() >= 0)
			errorText += '\n' + db.lastError().text();

		QMessageBox::warning(this, tr("Ошибка подключения"), errorText);
		ui->lDbConnectionStatus->clear();
	}
}

QString CDatabaseConnectionExtWidget::selectDatabaseFile()
{
	QString ret = QFileDialog::getSaveFileName(this, tr("Укажите файл для создания базы данных"), "", tr("Файл базы данных (*.db);;Все файлы (*.*)"));
	if(ret.isEmpty() == false)
	{
		QString suffix = QFileInfo(ret).completeSuffix();
		if(suffix != "db")
		{
			ret.chop(suffix.size());
			if(ret.endsWith('.') == false)
				ret.append(".db");
			else
				ret.append("db");
		}
	}
	return ret;
}

//public:

CDatabaseConnectionExtWidget::CDatabaseConnectionExtWidget(QWidget *parent) :
	CDataBaseConnectionWidget(parent)
{
}
