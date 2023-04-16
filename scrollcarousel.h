/**
 * @brief Scroll Carousel
 *
 * @author Jeffrey Le
 *
 */

#ifndef SCROLLCAROUSEL_H
#define SCROLLCAROUSEL_H

#include "ingredient.h"

#include <QObject>

#include <QScroller>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QTimer>
#include <QScrollerProperties>

class ScrollCarousel : public QScrollArea
{
    Q_OBJECT
public:
    ScrollCarousel(QWidget *parent = nullptr);

    void addWidget(QWidget*);
    void applyFilter(bool (*func)(Ingredient)); // create one for recipe class too?
    void applyFilter(bool (*func)(int));

    void reset();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QBoxLayout *scrollLayout;

    QList<QWidget*> widgets;
    QMap<QWidget*, bool> widgetVisible;

    QPropertyAnimation *horizontalAnimation;
    QPropertyAnimation *verticalAnimation;
};

#endif // SCROLLCAROUSEL_H
