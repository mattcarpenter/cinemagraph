#include "ProjectTreeItem.h"
#include "qvariant.h"
#include "qdebug.h"
#include <qmenu.h>

Q_DECLARE_METATYPE(ILayer*);

ProjectTreeItem::ProjectTreeItem(QString text)
	: QStandardItem(text)
{
	this->setEditable(false);
}

ProjectTreeItem::ProjectTreeItem(QString text, ILayer *layer)
	: QStandardItem(text)
{
	this->target = layer;
	this->setEditable(false);
	QVariant variant;
	variant.setValue(layer);
	this->setData(variant, Qt::UserRole + 1);
}

ProjectTreeItem::~ProjectTreeItem()
{
}

ILayer* ProjectTreeItem::GetTarget()
{
	return target;
}
