
#include "LegSupport.h"

#include <QtSql>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

LegSupport::LegSupport(std::unordered_map<std::string, std::string, MyHashFunction>& contrast, QWidget *parent)
	: QDialog(parent),
	contrast(contrast)
{
	ui.setupUi(this);
	//�������ݿ�
	db_ = QSqlDatabase::addDatabase("QSQLITE");
	db_.setDatabaseName("LegSupport.db");
	db_.setUserName("root");
	db_.setPassword("1238");


	if (!db_.open()) {
		QMessageBox::critical(this, QStringLiteral("����"), QStringLiteral("���ݿ��ʧ�ܣ�"));
		qCritical() << QStringLiteral("���ݿ��ʧ�ܣ�");
		return;
	}

	//�ؼ�������ʾ��ʵ��
	connect(ui.radioButton_A, SIGNAL(toggled(bool)), this, SLOT(whileTypeA(bool)));
	connect(ui.radioButton_B, SIGNAL(toggled(bool)), this, SLOT(whileTypeB(bool)));
	connect(ui.radioButton_C, SIGNAL(toggled(bool)), this, SLOT(whileTypeC(bool)));
	ui.radioButton_C->setChecked(true);
	ui.radioButton_A->setChecked(true);

	//ѡ��֧�ȹ���Զ���ʾ
	connect(ui.comboBox_leg_dn, SIGNAL(currentTextChanged(const QString &)), this, SLOT(choiceDN(const QString &)));

	//ȷ��ȡ��
	QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(invoke()));
	QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	// ��ȡ�����ӿؼ�
	QList<QWidget*> allWidgets = findChildren<QWidget*>();
	QList<QWidget*> inputWidget;
	for (auto& widget : allWidgets)
	{
		if (QWidget* childWidget = qobject_cast<QWidget*>(widget)) 
		{
			QString objectName = childWidget->objectName();
			if (objectName.startsWith("lineEdit_")||objectName.startsWith("comboBox_")|| objectName == "groupBox_LegType")
			{
				inputWidget.push_back(childWidget);
				qDebug() << QStringLiteral("�ؼ����ƣ�") << objectName;
			}
		}
	}

	// ���ļ�
	QFile file("LegSupport_contrast.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
	{
		qDebug() << QStringLiteral("�޷����ļ�");
		return;
	}

	//��ȡ�ļ�����
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QList<QStringList> readResult;
	while (!in.atEnd()) 
	{
		QString line = in.readLine().trimmed();
		// ʹ��tab�����зָ�
		QStringList fields = line.split("\t");
		if (fields.size() >= 4) {
			fields.removeAll("");
			readResult.push_back(fields);
		}
		else {
			qDebug() << QStringLiteral("��Ч���У�") << line;
		}
	}
	file.close();

	QStringList key1s, key2s;
	for (auto & it : inputWidget)
	{
		key1s.push_back(it->objectName().section('_', 1));
	}
	for (auto & it : readResult)
	{
		key2s.push_back(it.at(3));
	}
	QStringList key1NotInKey2 = key1s.toSet().subtract(key2s.toSet()).toList();
	QStringList key2NotInKey1 = key2s.toSet().subtract(key1s.toSet()).toList();

	qDebug() << QStringLiteral("��Ŀؼ�key��") << key1NotInKey2;
	qDebug() << QStringLiteral("�ٵĿؼ�key��") << key2NotInKey1;
	
}

LegSupport::~LegSupport()
{
	if (db_.isOpen()) {
		db_.close();
	}
}

bool LegSupport::DLG_data(bool UsesHashData, std::unordered_map<std::string, std::string, MyHashFunction>& contrast)
{
	if(UsesHashData)
	{
		LegSupport *legSupportDiolag = new LegSupport(contrast);
		if (legSupportDiolag->exec() == QDialog::Accepted)
		{
			delete legSupportDiolag;
			return true;
		}

	}
	return false;
}

void LegSupport::whileTypeA(bool status)
{

	if (status == true)
	{
		legType_ == "A";
		ui.lineEdit_leg_bb->show();
		ui.lineEdit_leg_dd->show();
		ui.lineEdit_leg_lh->show();
		ui.label_leg_bb->show();
		ui.label_leg_dd->show();
		ui.label_leg_lh->show();
		QSqlQuery query(db_);
		if (!query.exec("SELECT DN FROM A_LegSupport"))
		{
			qCritical(query.lastError().text().toLocal8Bit());
			QMessageBox::critical(this, QStringLiteral("����"), query.lastError().text());
			return;
		}
		QStringList DNs;
		while (query.next())
		{
			QString str = query.value(0).toString();
			DNs.append(str);
		}
		ui.comboBox_leg_dn->clear();
		ui.comboBox_leg_dn->addItems(DNs);
	}
	else
	{
		ui.lineEdit_leg_bb->hide();
		ui.lineEdit_leg_dd->hide();
		ui.lineEdit_leg_lh->hide();
		ui.label_leg_bb->hide();
		ui.label_leg_dd->hide();
		ui.label_leg_lh->hide();
	}
}
void LegSupport::whileTypeB(bool status)
{
	legType_ == "B";
	if (status == true)
	{

	}
	else
	{

	}
}
void LegSupport::whileTypeC(bool status)
{
	legType_ == "C";
	QSqlQuery query(db_);
	if (!query.exec("SELECT DN FROM C_LegSupport"))
	{
		qCritical(query.lastError().text().toLocal8Bit());
		QMessageBox::critical(this, QStringLiteral("����"), query.lastError().text());
		return;
	}
	QStringList DNs;
	while (query.next())
	{
		QString str = query.value(0).toString();
		DNs.append(str);
	}
	ui.comboBox_leg_dn->clear();
	ui.comboBox_leg_dn->addItems(DNs);
	if (status == true)
	{
		ui.lineEdit_leg_t1->show();
		ui.lineEdit_leg_t2->show();
		ui.lineEdit_leg_w ->show();
		ui.lineEdit_leg_l2->show();
		ui.lineEdit_leg_l0->show();

		ui.label_leg_t1->show();
		ui.label_leg_t2->show();
		ui.label_leg_w->show();
		ui.label_leg_l2->show();
		ui.label_leg_l0->show();
	}
	else
	{
		ui.lineEdit_leg_t1->hide();
		ui.lineEdit_leg_t2->hide();
		ui.lineEdit_leg_w->hide();
		ui.lineEdit_leg_l2->hide();
		ui.lineEdit_leg_l0->hide();

		ui.label_leg_t1->hide();
		ui.label_leg_t2->hide();
		ui.label_leg_w->hide();
		ui.label_leg_l2->hide();
		ui.label_leg_l0->hide();
	}
}

void LegSupport::choiceDN(QString & DN)
{
	double dn = DN.toDouble();
	QSqlQuery query(db_);
	query.prepare("SELECT * FROM " + legType_ + "_LegSupport WHERE DN = :dn");
	query.bindValue(":dn", dn);
	if (query.exec()) {
		while (query.next()) {
			// ����ÿһ�е�����
			QString column1Value = query.value("֧������").toString();
			// ��ȡ�����е�ֵ���Դ�����
		}
	}
	else {
		qCritical(query.lastError().text().toLocal8Bit());
	}
}

void LegSupport::invoke()
{
	accept();
}
