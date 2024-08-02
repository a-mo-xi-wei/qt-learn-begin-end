#include <QCoreApplication>
#include<iostream>
#include<QFile>
#include<QTextStream>
void test() {    //基本类型
    //  C 风格
    quint8 age = 25;
    qsizetype index = 0;
    qDebug("age is %d", age);
    qInfo("index is %lld", index);
    qWarning("this ids a warring...");
    qCritical("this is a critical...");
    if (0)qFatal("致命错误\n");   //直接中断

    //  C++ 风格
    qDebug() << "debug";
    qInfo() << "info";
    qWarning() << "warning";
    qCritical() << "critical";
    if (0)qFatal() << "致命错误";

}

void test1() {   //设置日志输出格式
    qSetMessagePattern("%{time yyyy-mm-dd hh:mm:ss} %{type} %{file} [%{function}(%{line})] %{message}");
    qDebug() << "debug 注意参数!";//注意release模式下无法输出文件路径
}
void MessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    //std::cout<<msg.toStdString()<<std::endl;
    QString str = qFormatLogMessage(type, context, msg);
    //std::cout<<str.toStdString()<<std::endl;
    QFile file("log.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << str << "\n";
    }
}
class Person
{
public:
    Person(QString na, quint8 ag) :name(na), age(ag) {}
    QString name;
    quint8 age;
};
QDebug operator<<(QDebug debug, const Person& p) {
    debug << "Person : " << p.name << " " << p.age;
    return debug;
}
void test2() {
    qInstallMessageHandler(MessageHandler);
    qDebug() << "Hello Qt";
    qInfo() << "Hello Info";
    Person p("Tom", 30);
    qDebug() << p;
    qInstallMessageHandler(nullptr);
    qDebug() << "debug output";
    qInfo() << "info output";
    qInfo() << p;
    qCritical() << "错误输出";
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    //test();

    //test1();
    qSetMessagePattern("%{time yyyy-mm-dd hh:mm:ss} %{type} %{file} [%{function}(%{line})] %{message}");
    test2();
    return a.exec();
}
