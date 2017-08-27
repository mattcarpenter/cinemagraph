#pragma once

#include <QTreeView>
#include <qstandarditemmodel.h>
#include "ProjectTreeItem.h"
#include "ILayer.h"
#include "TreeViewItemDelegate.h"

class ProjectTree : public QTreeView
{
	Q_OBJECT

public:
	ProjectTree(QWidget *parent);
	void AddVideoLayer(ILayer *layer);
	void AddStillLayer(ILayer *layer);
	~ProjectTree();

private:
	QStandardItemModel *model;
	ProjectTreeItem *root_item;
	TreeViewItemDelegate *item_delegate;
};
