#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class TreeViewItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	TreeViewItemDelegate(QObject *parent = 0);

	void paint(QPainter * painter,
		const QStyleOptionViewItem & option,
		const QModelIndex & index) const;
};
