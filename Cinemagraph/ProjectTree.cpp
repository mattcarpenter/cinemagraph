#include "ProjectTree.h"
#include <QTreeView>
#include <QStandardItemModel>
#include "ILayer.h"
#include <qdebug.h>
#include <qstring.h>
#include <qmenu.h>

Q_DECLARE_METATYPE(ILayer*);

ProjectTree::ProjectTree(QWidget *parent)
	: QTreeView(parent)
{
	model = new QStandardItemModel(1, 1);
	item_delegate = new TreeViewItemDelegate(this);
	root_item = new ProjectTreeItem(QString("Composition"));
	root_item->setEditable(false);
	model->setItem(0, root_item);
	this->setItemDelegate(item_delegate);
	this->setModel(model);
	this->setHeaderHidden(true);
	this->expandAll();

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ContextMenu(QPoint)));
}

ProjectTree::~ProjectTree()
{
}

void ProjectTree::AddMask(ILayer *layer)
{
	ProjectTreeItem *mask_item = new ProjectTreeItem(QString::fromStdString(layer->GetName()), layer);

	for (int i = 0; i < root_item->rowCount(); i++)
	{
		ProjectTreeItem* c = (ProjectTreeItem*)root_item->child(i);
		if (c->GetTarget()->GetType() == LayerType::VIDEO)
		{
			c->appendRow(mask_item);
		}
	}
	this->expandAll();
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

void ProjectTree::mousePressEvent(QMouseEvent *event)
{
	QModelIndex index = indexAt(event->pos());

	// Return if click did not land on an item
	if (!index.isValid())
	{
		return QTreeView::mousePressEvent(event);
	}

	// Attempt to load data for the clicked item
	QVariant variant = index.model()->data(index, Qt::UserRole + 1);
	if (variant.canConvert<ILayer*>())
	{
		ILayer *layer = variant.value<ILayer*>();

		if (layer->GetType() == LayerType::VIDEO || layer->GetType() == LayerType::STILL)
		{
			QRect vrect = visualRect(index);
			int clicked_x = event->pos().x();

			// Determine if user clicked visibility icon
			if (clicked_x >= vrect.x() && clicked_x < (vrect.x() + 16))
			{
				// Toggle
				layer->SetVisible(!layer->GetVisible());
			}
		}
		else if (layer->GetType() == LayerType::MASK)
		{
			QRect vrect = visualRect(index);
			int clicked_x = event->pos().x();

			// Determine if user clicked visibility icon
			if (clicked_x >= vrect.x() && clicked_x < (vrect.x() + 16))
			{
				// Toggle
				layer->SetVisible(!layer->GetVisible());
			}

			// Determine if user clicked mask visibility icon
			if (clicked_x >= (vrect.x() + 16) && clicked_x < (vrect.x() + 32))
			{
				// Toggle
				layer->SetHighlighted(!layer->GetHighlighted());
			}
		}
	}

	this->update();
	QTreeView::mousePressEvent(event);
}

void ProjectTree::ContextMenu(QPoint point)
{
	QMenu *menu = new QMenu;
	QModelIndex index = this->currentIndex();
	QVariant variant = index.model()->data(index, Qt::UserRole + 1);
	if (variant.canConvert<ILayer*>())
	{
		ILayer *layer = variant.value<ILayer*>();
		if (layer->GetType() == LayerType::MASK)
		{
			menu->addAction(QString("Delete"), this, SLOT(Delete()));
			menu->exec(QCursor::pos());
		}
	}
}

/**
 * Deletes a mask layer from the video
 */
void ProjectTree::Delete()
{
	// TODO - Fix. This kind of smells because we're filtering down to the video layer's
	//        children so that index.row() makes sense. Ideally this method would be generic
	//        enough to emit a delete event for any applicable layer selected in the QTreeView.

	QModelIndex index = this->currentIndex();
	QVariant variant = index.model()->data(index, Qt::UserRole + 1);
	if (variant.canConvert<ILayer*>())
	{
		ILayer *layer = variant.value<ILayer*>();
		for (int i = 0; i < root_item->rowCount(); i++)
		{
			ProjectTreeItem* c = (ProjectTreeItem*)root_item->child(i);
			if (c->GetTarget()->GetType() == LayerType::VIDEO)
			{
				c->removeRow(index.row());
				emit DeleteLayer(layer);
			}
		}
	}
}
