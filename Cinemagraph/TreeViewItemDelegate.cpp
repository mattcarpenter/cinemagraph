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
	// Data
	QString text = index.model()->data(index, Qt::DisplayRole).toString();
	QVariant variant = index.model()->data(index, Qt::UserRole + 1);
	
	if (variant.canConvert<ILayer*>())
	{
		
	}

	// Icons
	QPixmap icon_light = QPixmap(":/Cinemagraph/Resources/eyeball_light.png");
	QPixmap icon_dark = QPixmap(":/Cinemagraph/Resources/eyeball_dark.png");
	QPixmap icons = QPixmap(32, 16);
	icons.fill(Qt::transparent);
	QPainter icons_painter(&icons);
	icons_painter.drawPixmap(QRect(0, 0, 16, 16), icon_light);
	icons_painter.drawPixmap(QRect(16, 0, 16, 16), icon_dark);

	// Text rect
	QRect text_rect = option.rect;
	text_rect.setX(text_rect.x() + 33);
	
	// Options
	QStyleOptionViewItem eyeball_option = option;
	eyeball_option.decorationAlignment = Qt::AlignLeft;

	// Draw
	drawBackground(painter, option, index);
	drawDecoration(painter, eyeball_option, option.rect, icons);
	//drawFocus(painter, myoption, myoption.rect);
	drawDisplay(painter, option, text_rect, text);
}
