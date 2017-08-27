#include "ProjectTreeItem.h"

ProjectTreeItem::ProjectTreeItem(QString text)
	: QStandardItem(text)
{
}

ProjectTreeItem::ProjectTreeItem(QString text, ILayer *layer)
	: QStandardItem(text)
{
	this->target = layer;
}

ProjectTreeItem::~ProjectTreeItem()
{
}
