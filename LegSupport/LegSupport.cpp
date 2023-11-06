
#include "LegSupport.h"

#include <QFile>
#include <QDir>
#include <QDebug>
LegSupport::LegSupport(std::unordered_map<std::string, std::string, MyHashFunction>& contrast, QWidget *parent)
	: QDialog(parent),
	contrast(contrast)
{
    ui.setupUi(this);
	connect(ui.radioButton_A, SIGNAL(toggled(bool)), this, SLOT(whileTypeA(bool)));
	connect(ui.radioButton_B, SIGNAL(toggled(bool)), this, SLOT(whileTypeB(bool)));
	connect(ui.radioButton_C, SIGNAL(toggled(bool)), this, SLOT(whileTypeC(bool)));
	ui.radioButton_C->setChecked(true);
	ui.radioButton_A->setChecked(true);
	//确认取消
	QObject::connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(invoke()));
	QObject::connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	// 获取所有子控件
	QList<QWidget*> allWidgets = findChildren<QWidget*>();
	QList<QWidget*> inputWidget;
	for (auto& widget : allWidgets)
	{
		if (QWidget* childWidget = qobject_cast<QWidget*>(widget)) 
		{
			QString objectName = childWidget->objectName();
			if (objectName.startsWith("lineEdit_")||objectName.startsWith("comboBox")|| objectName == "groupBox_LegType")
			{
				inputWidget.push_back(childWidget);
				qDebug() << QStringLiteral("控件名称：") << objectName;
			}
		}
	}

	// 打开文件
	QFile file("LegSupport_contrast.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
	{
		qDebug() << QStringLiteral("无法打开文件");
		return;
	}

	//读取文件内容
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QList<QStringList> readResult;
	while (!in.atEnd()) 
	{
		QString line = in.readLine().trimmed();
		// 使用tab符进行分割
		QStringList fields = line.split("\t");
		if (fields.size() >= 4) {
			fields.removeAll("");
			readResult.push_back(fields);
		}
		else {
			qDebug() << QStringLiteral("无效的行：") << line;
		}
	}
	file.close();
}

LegSupport::~LegSupport()
{}

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
		ui.lineEdit_leg_bb->show();
		ui.lineEdit_leg_dd->show();
		ui.lineEdit_leg_lh->show();
		ui.label_leg_bb->show();
		ui.label_leg_dd->show();
		ui.label_leg_lh->show();
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
	if (status == true)
	{

	}
	else
	{

	}
}
void LegSupport::whileTypeC(bool status)
{
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

void LegSupport::invoke()
{
	accept();
}
