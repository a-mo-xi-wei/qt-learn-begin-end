#ifndef FSCAPTURE_H
#define FSCAPTURE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FSCapture;
}
QT_END_NAMESPACE

class FSCapture : public QWidget
{
    Q_OBJECT

public:
    FSCapture(QWidget *parent = nullptr);
    ~FSCapture();

private slots:
    void on_recordBtn_clicked();

private:
    Ui::FSCapture *ui;
};
#endif // FSCAPTURE_H
