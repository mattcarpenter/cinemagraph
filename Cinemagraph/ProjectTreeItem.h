#pragma once

#include <QObject>
#include <qstandarditemmodel.h>
#include "ILayer.h"

class ProjectTreeItem : public QObject, public QStandardItem
{
	Q_OBJECT

public:
	ProjectTreeItem(QString text);
	ProjectTreeItem(QString text, ILayer *layer);
	void AddVideoLayer();
	void AddStillLayer();
	~ProjectTreeItem();
	ILayer* GetTarget();
private:
	ILayer *target;
};
