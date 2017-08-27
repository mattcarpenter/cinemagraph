#include "ProjectTree.h"
#include <QTreeView>
#include <QStandardItemModel>

ProjectTree::ProjectTree(QWidget *parent)
	: QTreeView(parent)
{
	model = new QStandardItemModel(1, 1);
	item_delegate = new TreeViewItemDelegate(this);

	/*for (int r = 0; r < 5; r++)
	{
		QStandardItem *item = new QStandardItem(QString("Row:%0").arg(r));
		for (int i = 0; i < 3; i++)
		{
			QStandardItem *child = new QStandardItem(QString("Item %0").arg(i));
			child->setEditable(false);
			item->appendRow(child);
		}

		model->setItem(r, 0, item);
	}*/

	root_item = new ProjectTreeItem(QString("Composition"));
	model->setItem(0, root_item);

	this->setItemDelegate(item_delegate);

	this->setModel(model);
	this->setHeaderHidden(true);
}

ProjectTree::~ProjectTree()
{
}

void ProjectTree::AddVideoLayer(ILayer *layer)
{
	ProjectTreeItem *item = new ProjectTreeItem(QString("Video"), layer);
	root_item->appendRow(item);
}

void ProjectTree::AddStillLayer(ILayer *layer)
{
	ProjectTreeItem *item = new ProjectTreeItem(QString("Still"), layer);
	root_item->appendRow(item);
}