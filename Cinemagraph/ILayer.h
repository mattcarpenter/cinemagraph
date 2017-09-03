#pragma once
#include <qobject.h>

enum class LayerType {
	VIDEO,
	STILL,
	NONE,
	MASK
};

class ILayer
{

public:
	virtual ~ILayer() {}
	virtual LayerType GetType() = 0;
	virtual bool GetVisible() = 0;
	virtual void SetVisible(bool v) = 0;
	virtual void SetHighlighted(bool h) = 0;
	virtual bool GetHighlighted() = 0;
	virtual std::string GetName() = 0;
};

