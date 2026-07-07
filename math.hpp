#ifndef MATH_HPP
#define MATH_HPP

#include <QPoint>

static QPoint linear_interp(QPoint a, QPoint b, float t) {
  return QPoint(
	        a.x() + std::rint(t * (float(b.x()) - a.x())),
		a.y() + std::rint(t * (float(b.y()) - a.y()))
		);
}

#endif
