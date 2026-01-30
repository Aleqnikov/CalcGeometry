#ifndef NAVIGATIONVIEW_H
#define NAVIGATIONVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QTimer>

class NavigationView : public QGraphicsView {
    Q_OBJECT
private:
    QTimer *moveTimer_;
    bool keyW_ = false, keyA_ = false, keyS_ = false, keyD_ = false;
    static constexpr int MOVE_SPEED = 30;

public:
    explicit NavigationView(QGraphicsScene *scene, QWidget *parent = nullptr)
        : QGraphicsView(scene, parent)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setMouseTracking(true);

        setRenderHint(QPainter::Antialiasing, false);
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
        setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
        setCacheMode(QGraphicsView::CacheBackground);

        scene->setBackgroundBrush(QBrush(QColor("#1a1a1a")));
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);

        moveTimer_ = new QTimer(this);
        moveTimer_->setInterval(16);
        connect(moveTimer_, &QTimer::timeout, this, &NavigationView::performMovement);
    }

signals:
    void returnToMenu();
    void mouseMovedTo(QPointF scenePos);
    void spacePressed();

protected:
    void wheelEvent(QWheelEvent *event) override {
        const double scaleFactor = 1.15;
        qreal currentScale = transform().m11();

        if (event->angleDelta().y() > 0) {
            if (currentScale < 100.0) scale(scaleFactor, scaleFactor);
        } else {
            if (currentScale > 0.01) scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
        updateRenderHints();
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::RightButton) {
            setDragMode(QGraphicsView::ScrollHandDrag);
            QGraphicsView::mousePressEvent(event);
            setDragMode(QGraphicsView::RubberBandDrag);
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (dragMode() == QGraphicsView::RubberBandDrag) {
            QRect rubberRect = rubberBandRect();
            if (!rubberRect.isEmpty())
                fitInView(mapToScene(rubberRect).boundingRect(), Qt::KeepAspectRatio);
        }
        QGraphicsView::mouseReleaseEvent(event);
        updateRenderHints();
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        emit mouseMovedTo(mapToScene(event->pos()));
        QGraphicsView::mouseMoveEvent(event);
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Escape && !event->isAutoRepeat()) {
            emit returnToMenu();
            event->accept();
            return;
        }

        if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
            emit spacePressed();
            event->accept();
            return;
        }

        if (!event->isAutoRepeat()) {
            if (event->key() == Qt::Key_W) { keyW_ = true; startMovementTimer(); event->accept(); return; }
            if (event->key() == Qt::Key_A) { keyA_ = true; startMovementTimer(); event->accept(); return; }
            if (event->key() == Qt::Key_S) { keyS_ = true; startMovementTimer(); event->accept(); return; }
            if (event->key() == Qt::Key_D) { keyD_ = true; startMovementTimer(); event->accept(); return; }
        }

        QGraphicsView::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        if (!event->isAutoRepeat()) {
            if (event->key() == Qt::Key_W) { keyW_ = false; stopMovementTimerIfNeeded(); event->accept(); return; }
            if (event->key() == Qt::Key_A) { keyA_ = false; stopMovementTimerIfNeeded(); event->accept(); return; }
            if (event->key() == Qt::Key_S) { keyS_ = false; stopMovementTimerIfNeeded(); event->accept(); return; }
            if (event->key() == Qt::Key_D) { keyD_ = false; stopMovementTimerIfNeeded(); event->accept(); return; }
        }
        QGraphicsView::keyReleaseEvent(event);
    }

private:
    void startMovementTimer() {
        if (!moveTimer_->isActive()) moveTimer_->start();
    }

    void stopMovementTimerIfNeeded() {
        if (!keyW_ && !keyA_ && !keyS_ && !keyD_) moveTimer_->stop();
    }

    void performMovement() {
        qreal dx = 0, dy = 0;
        if (keyW_) dy += MOVE_SPEED;
        if (keyS_) dy -= MOVE_SPEED;
        if (keyA_) dx -= MOVE_SPEED;
        if (keyD_) dx += MOVE_SPEED;

        if (dx != 0 || dy != 0) {
            qreal currentScale = transform().m11();
            qreal scaledDx = dx / currentScale;
            qreal scaledDy = dy / currentScale;

            QPointF center = mapToScene(viewport()->rect().center());
            centerOn(center.x() + scaledDx, center.y() + scaledDy);

            QPoint mousePos = mapFromGlobal(QCursor::pos());
            if (rect().contains(mousePos)) {
                emit mouseMovedTo(mapToScene(mousePos));
            }
        }
    }

    void updateRenderHints() {
        qreal scale = transform().m11();
        setRenderHint(QPainter::Antialiasing, scale < 5.0);
    }
};

#endif
