#pragma once

#include <QtWidgets/QDialog>
#include <unordered_map>
#include "ui_LegSupport.h"
#include <QSqlDatabase>



struct MyHashFunction
{
	mutable std::map<std::string, size_t> indexs;
	std::size_t operator()(const std::string& key) const 
	{
		std::map<std::string, size_t>::iterator iterator = indexs.find(key);
		if (iterator == indexs.end())
		{
			indexs[key] = indexs.size();
		}
		return indexs[key];
	}
};


class LegSupport : public QDialog
{
    Q_OBJECT

public:
    LegSupport(std::unordered_map<std::string, std::string, MyHashFunction>& contrast,QWidget *parent = nullptr);
    ~LegSupport();

	static bool DLG_data(bool UsesHashData, std::unordered_map<std::string, std::string, MyHashFunction>& contrast);
public slots:
	//选择支腿类型
	void whileTypeA(bool status);
	void whileTypeB(bool status);
	void whileTypeC(bool status);
	//选择支腿规格
	void choiceDN(QString &DN);
	//点击确定
	void invoke();
private:
    Ui::LegSupportClass ui;

	QSqlDatabase db_;//数据库

	QString legType_;//支腿类型

	std::unordered_map<std::string, std::string, MyHashFunction>& contrast;
};


