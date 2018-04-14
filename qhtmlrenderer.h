#ifndef QHTMLRENDERER_H
#define QHTMLRENDERER_H

#include <memory>
#include <QWidget>

struct html_renderer_private;

class QHtmlRenderer
	: public QWidget
{
	Q_OBJECT

private:
	html_renderer_private*	m_private;

	int		m_top;
	int		m_left;
	int		m_max_top;
	int		m_max_left;

public:
	explicit QHtmlRenderer(QWidget *parent = 0);

	void	loadFile(const QString& fname);
	void	loadText( const QString& text );

protected:
	void	scroll_to(int x, int y);
	void	preparePage();

protected:
	void	paintEvent(QPaintEvent *event);
	void	resizeEvent( QResizeEvent* event );
	void	mouseMoveEvent(QMouseEvent* event );
	//void	wheelEvent(QWheelEvent*event);
};

#endif // QHTMLRENDERER_H
