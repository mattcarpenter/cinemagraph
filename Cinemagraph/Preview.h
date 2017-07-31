#pragma once

#include <QWidget>
#include <qlabel.h>

class Preview : public QLabel
{
	Q_OBJECT

public:
	Preview(QWidget *parent);
	~Preview();
};
