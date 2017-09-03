#pragma once

#include <QTreeView>
#include <qstandarditemmodel.h>
#include "ProjectTreeItem.h"
#include "ILayer.h"
#include "TreeViewItemDelegate.h"
#include <qevent.h>

class ProjectTree : public QTreeView
{
	Q_OBJECT

public:
	ProjectTree(QWidget *parent);
	void AddVideoLayer(ILayer *layer);
	void AddStillLayer(ILayer *layer);
	void AddMask(ILayer *layer);
	~ProjectTree();

private:
	QStandardItemModel *model;
	ProjectTreeItem *root_item;
	TreeViewItemDelegate *item_delegate;

private slots:
	void mousePressEvent(QMouseEvent *event);
};
