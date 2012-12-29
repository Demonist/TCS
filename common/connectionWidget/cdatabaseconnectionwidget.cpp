#include "cdatabaseconnectionwidget.h"
#include "ui_cdatabaseconnectionwidget.h"

#define INI_REMEMBER "General/rememberFlag"
#define INI_FILES "General/dataBaseFiles"
#define INI_CONNECTION_TYPE "General/connectionType"
#define INI_LASTFILE "General/lastFile"
#define INI_LASTSERVER "General/lastServer"
#define INI_DBNAME "dataBaseName"
#define INI_LOGIN "dataBaseLogin"
#define INI_PASSWORD "dataBasePassword"


//private:



//public:

CDataBaseConnectionWidget::CDataBaseConnectionWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CDataBaseConnectionWidget)
{
	ui->setupUi(this);
	mConnected = false;
	mConnectionName = "common";
	mConnectionType = ConnectionServer;

	mCache = new QSettings("settings/dataBaseConnection.ini", QSettings::IniFormat, this);
	if(mCache)
	{
		if("file" == mCache->value(INI_CONNECTION_TYPE).toString())
			ui->rbnDbFile->setChecked(true);
		ui->chbxDbRemember->setChecked(mCache->value(INI_REMEMBER).toBool());

		QList<QString> files = mCache->value(INI_FILES).toStringList();
		ui->cbxDbFileName->addItems(files);
		ui->cbxDbFileName->setCurrentIndex(ui->cbxDbFileName->findText(mCache->value(INI_LASTFILE).toString()));

		QList<QString> groups = mCache->childGroups();
		groups.removeOne("General");
		ui->cbxDbHost->addItems(groups);
		ui->cbxDbHost->setCurrentIndex(ui->cbxDbHost->findText(mCache->value(INI_LASTSERVER).toString()));
	}
}

CDataBaseConnectionWidget::~CDataBaseConnectionWidget()
{
	delete ui;

	if(mCache)
	{
		mCache->setValue(INI_REMEMBER, ui->chbxDbRemember->isChecked());

		delete mCache;
		mCache = 0;
	}
}

bool CDataBaseConnectionWidget::isConnected() const
{
	if(mConnected)
	{
		QSqlDatabase db = QSqlDatabase::database(mConnectionName);
		if(db.isOpen())
			return true;
		else
			mConnected = false;
	}
	return false;
}

bool CDataBaseConnectionWidget::isCloseButtonVisible() const
{
	return ui->pbnDbExit->isVisible();
}

bool CDataBaseConnectionWidget::isConnectionChoiceEnable() const
{
	return ui->gbxConnectionType->isVisible();
}

//private slots:

void CDataBaseConnectionWidget::on_pbnDbExit_clicked()
{
	emit closed();
}

void CDataBaseConnectionWidget::on_pbnDbConnect_clicked()
{
	ui->lDbConnectionStatus->setText(tr("Подключение..."));
	QApplication::processEvents();

	bool connectingToServer = ui->rbnDbServer->isChecked(); //Флаг, показывающий подключаемся ли мы к серверу или к файлу.
	QSqlDatabase db = QSqlDatabase::addDatabase(connectingToServer ? "QMYSQL" : "QSQLITE", mConnectionName);

	if(connectingToServer)
	{
		db.setHostName(ui->cbxDbHost->currentText());
		db.setDatabaseName(ui->leDbName->text());
		db.setUserName(ui->leDbLogin->text());
		db.setPassword(ui->leDbPassword->text());
	}
	else    //connecting to file
	{
		db.setDatabaseName(ui->cbxDbFileName->currentText());
	}

	if(db.open() && db.tables().count())   //connection is successfully
	{
		mConnected = true;

		ui->lDbConnectionStatus->setText(tr("Подключение произведено"));
		QApplication::processEvents();

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
					mCache->setValue(INI_PASSWORD, qCompress( db.password().toUtf8() ));
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

		emit connectedToDatabase(mConnectionName);
	}
	else
	{
		ui->lDbConnectionStatus->setText(tr("Ошибка подключения"));
		QApplication::processEvents();
		qDebug("%d", db.lastError().number());
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

void CDataBaseConnectionWidget::on_rbnDbServer_toggled(bool checked)
{
	if(checked)
	{
		ui->stackedWidget->setCurrentIndex(0);
		mConnectionType = ConnectionServer;
	}
}

void CDataBaseConnectionWidget::on_rbnDbFile_toggled(bool checked)
{
	if(checked)
	{
		ui->stackedWidget->setCurrentIndex(1);
		mConnectionType = ConnectionFile;
	}
}

void CDataBaseConnectionWidget::on_tbnDbFile_clicked()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите файл базы данных"), "", tr("Файл базы данных (*.db);;Все файлы (*.*)"));
	if(fileName.isEmpty() == false)
	{
		int index = ui->cbxDbFileName->findText(fileName);
		if(index == -1)
			ui->cbxDbFileName->addItem(fileName);
		index = ui->cbxDbFileName->findText(fileName);
		ui->cbxDbFileName->setCurrentIndex(index);
	}
}

void CDataBaseConnectionWidget::on_cbxDbHost_currentIndexChanged(const QString &arg1)
{
	if(mCache)
	{
		if(mCache->childGroups().contains(arg1))
		{
			ui->leDbName->setText(mCache->value(arg1 + '/' + INI_DBNAME).toString());
			ui->leDbLogin->setText(mCache->value(arg1 + '/' + INI_LOGIN).toString());
			ui->leDbPassword->setText(qUncompress( mCache->value(arg1 + '/' + INI_PASSWORD).toByteArray() ));
		}
	}
}

//public slots:

/**
Задает имя подключения к базе данных.

@attention Не работает, если уже произведено подключение к базе данных.
*/
void CDataBaseConnectionWidget::setConnectionName(const QString &connectionName)
{
	if(isConnected() == false)
	{
		mConnectionName = connectionName;
	}
}

void CDataBaseConnectionWidget::setCloseButtonVisible(bool show)
{
	ui->pbnDbExit->setVisible(show);
}

/**
Задает возможность выбора расположения базы данных (сервер или файл).

@attention Не работает если уже произведено подключение к базе данных.
*/
void CDataBaseConnectionWidget::setConnectionChoiceEnable(bool enable)
{
	if(isConnected() == false)
	{
		ui->gbxConnectionType->setVisible(enable);
	}
}

/**
Задает тип расположения базы данных (сервер или файл).

@attention Не работает если уже произведено подключение к базе данных.
*/
void CDataBaseConnectionWidget::setConnectionType(const ConnectionType &connectionType)
{
	if(isConnected() == false)
	{
		if(connectionType == ConnectionServer)
			ui->rbnDbServer->setChecked(true);
		else
			ui->rbnDbFile->setChecked(true);
	}
}
