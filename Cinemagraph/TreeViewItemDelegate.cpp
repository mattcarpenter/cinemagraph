#include "TreeViewItemDelegate.h"
#include <qpixmap.h>
#include <qapplication.h>
#include <qpainter.h>
#include "ProjectTreeItem.h"
#include <qstandarditemmodel.h>
#include "Layer.h"
#include "qdebug.h"

Q_DECLARE_METATYPE(ILayer*);

TreeViewItemDelegate::TreeViewItemDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

void TreeViewItemDelegate::paint(QPainter * painter,
	const QStyleOptionViewItem & option,
	const QModelIndex & index) const
{
	painter->save();

	QPixmap icon_light = QPixmap(":/Cinemagraph/Resources/eyeball_light.png");
	QPixmap icon_dark = QPixmap(":/Cinemagraph/Resources/eyeball_dark.png");

	drawBackground(painter, option, index);

	// Data
	QString text = index.model()->data(index, Qt::DisplayRole).toString();
	QVariant variant = index.model()->data(index, Qt::UserRole + 1);
	
	if (variant.canConvert<ILayer*>())
	{
		ILayer *layer = variant.value<ILayer*>();
		
		if (layer->GetType() == LayerType::VIDEO || layer->GetType() == LayerType::STILL)
		{
			// Icons
			QPixmap icons = QPixmap(16, 16);
			icons.fill(Qt::transparent);
			QPainter icons_painter(&icons);

			if (layer->GetVisible())
				icons_painter.drawPixmap(QRect(0, 0, 16, 16), icon_light);
			else
				icons_painter.drawPixmap(QRect(0, 0, 16, 16), icon_dark);

			// Options
			QStyleOptionViewItem eyeball_option = option;
			eyeball_option.decorationAlignment = Qt::AlignLeft;

			// Draw
			drawDecoration(painter, eyeball_option, option.rect, icons);

			// Text rect
			QRect text_rect = option.rect;
			text_rect.setX(text_rect.x() + 17);
			drawDisplay(painter, option, text_rect, text);
		}
		else if (layer->GetType() == LayerType::MASK)
		{
			// Icons
			QPixmap icons = QPixmap(32, 16);
			icons.fill(Qt::transparent);
			QPainter icons_painter(&icons);

			if (layer->GetVisible())
			{
				icons_painter.drawPixmap(QRect(0, 0, 16, 16), icon_light);
				icons_painter.drawPixmap(QRect(16, 0, 16, 16), icon_light);
			}
			else
			{
				icons_painter.drawPixmap(QRect(0, 0, 16, 16), icon_dark);
				icons_painter.drawPixmap(QRect(16, 0, 16, 16), icon_dark);
			}

			// Options
			QStyleOptionViewItem eyeball_option = option;
			eyeball_option.decorationAlignment = Qt::AlignLeft;

			// Draw
			drawDecoration(painter, eyeball_option, option.rect, icons);

			// Text rect
			QRect text_rect = option.rect;
			text_rect.setX(text_rect.x() + 33);
			drawDisplay(painter, option, text_rect, text);
		}
		else if (layer->GetType() == LayerType::NONE)
		{
			QRect text_rect = option.rect;
			text_rect.setX(text_rect.x() + 17);
			drawDisplay(painter, option, text_rect, text + " (empty)");
		}
	}
	else
	{
		// Not a layer
		drawDisplay(painter, option, option.rect, text);
	}

	painter->restore();
}
