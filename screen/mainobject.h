/*****************************************
 * 作者: MSC
 * 日期: 2021-01-07
 * 功能：初始化类
 * ***************************************/
#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include <QObject>

class MainWindow;

class MainObject : public QObject
{
    Q_OBJECT
public:
    explicit MainObject(QObject *parent = 0); //防止隐式转换，确保只能显式构造
    ~MainObject();

public:
    void setInit(); //初始化

private:
    MainWindow *m_mainWindow;
};

#endif // MAINOBJECT_H
