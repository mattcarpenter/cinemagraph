#include "ProjectTreeItem.h"
#include "qvariant.h"
#include "qdebug.h"

Q_DECLARE_METATYPE(ILayer*);

ProjectTreeItem::ProjectTreeItem(QString text)
	: QStandardItem(text)
{
}

ProjectTreeItem::ProjectTreeItem(QString text, ILayer *layer)
	: QStandardItem(text)
{
	this->target = layer;
	
	QVariant variant;
	variant.setValue(layer);
	this->setData(variant, Qt::UserRole + 1);
}

ProjectTreeItem::~ProjectTreeItem()
{
}
