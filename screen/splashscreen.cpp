/*****************************************
 * 作者: MSC
 * 日期: 2025-06-1
 * 功能：创建闪屏
 * ***************************************/
#include "splashscreen.h"
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QGuiApplication>
#include <QDebug>

SplashScreen *SplashScreen::m_instance = nullptr;

// 构造函数
SplashScreen::SplashScreen(const QPixmap &pixmap)
    : QSplashScreen(pixmap)
    , m_percent(0)
    , m_mainWidget(nullptr)
    , m_propertyAnimation(nullptr)
{

}

SplashScreen::~SplashScreen()
{

}

// 单例模式
SplashScreen *SplashScreen::getInstance()
{
    if(nullptr == m_instance)
    {
        m_instance = new SplashScreen(QPixmap(":/res/res/image/other/splash.png"));
    }
    return m_instance;
}


// 绘制事件
void SplashScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    const int OFFSET_VALUE = 70;
    const int SLIDER_HEIGHT = 10;
    const int BORDER_X_RADIUS = 8;
    const int BORDER_Y_RADIUS = 4;

    // 创建 QPainter 对象
    QPainter painter(this);
    // 启用抗锯齿渲染，使图形边缘更平滑
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font(QString("微软雅黑"));

    // 创建画笔(QPen)并设置为白色
    QPen painterPen;
    painterPen.setColor(Qt::white);
    painter.setPen(painterPen);

    // 创建画刷(QBrush)并设置为灰色
    QBrush brush(QColor(100, 100, 100, 255));
    painter.setBrush(brush);

    // 若需要显示setStart传入的图徽，开启以下注释
    /*if (!m_pixLogo.isNull()) {
            painter.drawPixmap(20, 20, m_pixLogo.scaled(50, 50, Qt::KeepAspectRatio));
        }*/

    // 绘制一个圆角矩形
    painter.drawRoundedRect(
                    OFFSET_VALUE,                           // 左上角 x 位置
                    this->height() - OFFSET_VALUE,          // 左上角 y 位置
                    (this->width() - OFFSET_VALUE * 2),     // 宽度
                    BORDER_X_RADIUS,                        // 水平圆角半径
                    BORDER_Y_RADIUS,                        // 垂直圆角半径
                    SLIDER_HEIGHT                           // 高度
                );
    // 绘制消息文本
    {
        font.setPixelSize(16);      // 设置字体大小为 16px
        font.setBold(true);         // 加粗
        painter.setFont(font);      // 应用字体
        painterPen.setColor(Qt::yellow);    // 设置画笔颜色为黄色
        painter.setPen(painterPen);         // 应用画笔

        painter.drawText(0,                                 // 文本起始 X 坐标（左对齐）
                         this->height()- OFFSET_VALUE * 2,  // 文本起始 Y 坐标（靠近顶部）
                         this->width(),                     // 文本宽度（占满窗口）
                         OFFSET_VALUE,                      // 文本高度（行高）
                         Qt::AlignCenter,                   // 居中对齐
                         m_message);                         // 要绘制的文本
    }

    // 绘制进度条
    {
        painterPen.setColor(Qt::white);  // 设置画笔颜色为白色（进度条边框）
        painter.setPen(painterPen);      // 应用画笔
        if (m_percent != 0)              // 如果进度不为 0
        {
            QBrush brush(QColor(150, 230, 100, 255));  // 绿色填充
            painter.setBrush(brush);     // 应用画刷
            painter.drawRoundedRect(
                OFFSET_VALUE,
                this->height() - OFFSET_VALUE,
                (this->rect().width() - OFFSET_VALUE * 2) * m_percent / 100,  // 动态宽度
                BORDER_X_RADIUS,
                BORDER_Y_RADIUS,
                SLIDER_HEIGHT
            );
        }
    }

    // 绘制进度百分比文本
    {
        font.setPixelSize(14);           // 设置字体大小为 14px
        font.setBold(false);             // 不加粗
        painter.setFont(font);           // 应用字体
        painterPen.setColor(Qt::white);  // 设置画笔颜色为白色
        painter.setPen(painterPen);      // 应用画笔
        QString drawText = QString::number(m_percent, 'f', 0) + "%";  // 格式化百分比（如 "50%"）
        painter.drawText(
            0,                           // 文本起始 X 坐标（左对齐）
            this->height() - OFFSET_VALUE,  // 文本起始 Y 坐标（靠近底部）
            this->width(),               // 文本宽度（占满窗口）
            OFFSET_VALUE,                // 文本高度（行高）
            Qt::AlignCenter,             // 居中对齐
            drawText                     // 要绘制的文本
        );
        painter.end();                   // 结束绘制（通常不需要显式调用）
    }

    // 绘制圆角窗体
    {
        QPainterPath painterPathPath;    // 创建路径对象
        QRectF rect = QRectF(0, 0, this->width(), this->height());  // 窗口矩形
        painterPathPath.addRoundRect(rect, BORDER_X_RADIUS, BORDER_Y_RADIUS);  // 添加圆角矩形路径
        QPolygon polygon = painterPathPath.toFillPolygon().toPolygon();  // 转换为多边形
        QRegion region(polygon);         // 创建区域对象
        this->setMask(region);           // 应用遮罩（使窗口呈现圆角）
    }
}

// 设置进度
void SplashScreen::setStagePercent(const int &percent, const QString &message)
{
    // 检查启动画面是否隐藏，如果是则显示
    if(this->isHidden())
    {
        this->show();
    }

    // 检查启动画面是否是活动窗口，如果不是则激活并提升
    if(!this->isActiveWindow())
    {
        this->activateWindow();
        this->raise();
    }
    m_message = message;

    // 逐步增加进度百分比并刷新界面
    while (m_percent < percent)
    {
        m_percent = m_percent + 0.01;
        qApp->processEvents();
        this->repaint();
    }
}

// 开始
void SplashScreen::setStart(QWidget *widget, const QString &title, const QString &logoFile)
{
    // 检查 widget 是否为空指针
    if (nullptr != widget)
    {
        m_mainWidget = widget;

        // 加载徽标图像
        m_pixLogo = QPixmap(logoFile);
        // 设置标题文本
        m_textLogo = title;
        // 将主窗口部件的初始不透明度设置为 0.0
        m_mainWidget->setWindowOpacity(0.0);
        // 检查属性动画是否已经存在
        if (nullptr == m_propertyAnimation)
        {
            // 创建 QPropertyAnimation 对象
            m_propertyAnimation = new QPropertyAnimation(m_mainWidget, "windowOpacity");
            // 设置动画持续时间
            m_propertyAnimation->setDuration(ANIMATION_DURATION);
            // 设置动画的起始值
            m_propertyAnimation->setStartValue(0.0);
            // 设置动画的结束值
            m_propertyAnimation->setEndValue(1.0);
        }
    }
}

// 结束
void SplashScreen::setFinish()
{
    // 关闭启动画面窗口
    this->close();

    // 检查并激活主窗口部件
    if (nullptr != m_mainWidget)
    {
        m_mainWidget->activateWindow();
        m_mainWidget->raise();
    }

    // 检查并启动属性动画
    if (nullptr != m_propertyAnimation)
    {
        m_propertyAnimation->start();
    }
}
