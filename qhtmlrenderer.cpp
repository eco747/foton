#include "qhtmlrenderer.h"

#include "QFont"
#include "QFontMetrics"
#include "QPainter"
#include "QFile"
#include "QFileInfo"
#include "QDebug"

#include "QMouseEvent"
#include "QWheelEvent"

#include <qxsrunner.h>

//#define		__debugger	__asm int 3
#define		__debugger

typedef std::map<QByteArray, QImage*>	images_map;

class	container :
	public litehtml::document_container
{
public:
	QWidget*	m_owner;
	QString		m_base_url;
	QImage		m_broken_img;
	images_map	m_images;

	litehtml::position::vector	m_clips;

	container( QWidget* parent ) {
		m_owner = parent;
	}

	void setDefImage( const QImage& img ) {
		m_broken_img = img;
	}

	virtual litehtml::uint_ptr	create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* pfm) {

		std::string		fn( faceName );

		// slip coma separated font names
		int pos = fn.find( ',' );
		if( pos!=std::string::npos ) {
			fn = fn.substr( 0, pos );
		}

		// trim '' & "" separators
		fn.erase(0, fn.find_first_not_of('\''));       //prefixing spaces
		fn.erase(fn.find_last_not_of('\'')+1);         //surfixing spaces
		fn.erase(0, fn.find_first_not_of('\"'));       //prefixing spaces
		fn.erase(fn.find_last_not_of('\"')+1);         //surfixing spaces

		//	create it
		QFont*	fnt = new QFont( fn.c_str(), size, weight, italic );
		QFontMetrics	fm( *fnt );

		pfm->ascent		= fm.ascent();
		pfm->descent	= fm.descent();
		pfm->height		= fm.height();
		pfm->x_height	= fm.xHeight();

		return (uintptr_t)fnt;
	}

	virtual void				delete_font(litehtml::uint_ptr hFont) {
		delete (QFont*)hFont;
	}

	virtual int		text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont) {
		QFont*			fnt = (QFont*)hFont;
		QFontMetrics	fm( *fnt );
		return fm.width( text );
	}

	virtual void	draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) {


		QFont*			fnt = (QFont*) hFont;
		QPainter*		pnt	= (QPainter*)hdc;
		QFontMetrics	mtx( *fnt );

		//cairo_save(cr);
		//apply_clip(cr);

		int x = pos.left();
		int y = pos.bottom() - mtx.descent();

		//set_color(cr, color);
		//fnt->show_text(cr, x, y, text);
		//cairo_restore(cr);

		pnt->setFont( *fnt );
		pnt->setPen( cvtColor(color) );
		pnt->drawText( x, y, text );
	}

	virtual int	pt_to_px(int pt) {
		__debugger;
		return 0;
	}

	virtual int	get_default_font_size() const {
		return 8;
	}

	virtual const litehtml::tchar_t* get_default_font_name() const {
		return "arial";
	}

	virtual void	draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
	{
		QPainter*	pnt = (QPainter*)hdc;
		apply_clip( pnt );

		int draw_x		= marker.pos.x;
		int draw_y		= marker.pos.y;
		int draw_width	= marker.pos.width;
		int draw_height	= marker.pos.height;

		switch(marker.marker_type)
		{
		case litehtml::list_style_type_circle:
			{
				pnt->setPen( QPen( cvtColor(marker.color) ) );
				pnt->setBrush( Qt::NoBrush );
				pnt->drawEllipse( draw_x, draw_y, draw_width, draw_height );
				break;
			}

		case litehtml::list_style_type_disc:
			{
				pnt->setBrush( QBrush( cvtColor(marker.color) ) );
				pnt->drawEllipse( draw_x, draw_y, draw_width, draw_height );
				break;
			}

		case litehtml::list_style_type_square:
			{
				pnt->fillRect( draw_x, draw_y, draw_width, draw_height, cvtColor(marker.color) );
			}
			break;

		//list_style_type_armenian,
		//list_style_type_cjk_ideographic,
		//list_style_type_decimal,
		//list_style_type_decimal_leading_zero,
		//list_style_type_georgian,
		//list_style_type_hebrew,
		//list_style_type_hiragana,
		//list_style_type_hiragana_iroha,
		//list_style_type_katakana,
		//list_style_type_katakana_iroha,
		//list_style_type_lower_alpha,
		//list_style_type_lower_greek,
		//list_style_type_lower_latin,
		//list_style_type_lower_roman,
		//list_style_type_upper_alpha,
		//list_style_type_upper_latin,
		//list_style_type_upper_roman,
		}
	}

	QImage* getImage( const litehtml::tchar_t* src,  const litehtml::tchar_t* baseurl )
	{
		QString fname = make_url( src, baseurl );
		images_map::iterator img = m_images.find(fname.toUtf8());
		if(img != m_images.end()) {
			return img->second;
		}

		return &m_broken_img;
	}

	virtual void	load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready) {

		QString fname = make_url( src, baseurl );

		if( m_images.find(fname.toUtf8())==m_images.end() ) {
			QImage*	img = new QImage( );
			if( img->load( fname ) ) {
				m_images[fname.toUtf8()] = img;
			}
			else {
				qDebug() << "cannot load image: " << fname;
			}
		}
	}

	virtual void	get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz)
	{
		QImage*	img = getImage( src, baseurl );
		if( img ) {
			sz.width = img->width();
			sz.height = img->height();
		}
	}


	virtual void	draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) {

		QPainter* pnt = (QPainter*)hdc;

		//cairo_save(cr);
		apply_clip( pnt );

		//rounded_rectangle(cr, bg.border_box, bg.border_radius);
		//cairo_clip(cr);

		pnt->fillRect(bg.clip_box.x, bg.clip_box.y, bg.clip_box.width, bg.clip_box.height, QBrush(cvtColor(bg.color)) );

		//cairo_rectangle(cr, bg.clip_box.x, bg.clip_box.y, bg.clip_box.width, bg.clip_box.height);
		//cairo_clip(cr);

		/*if(bg.color.alpha)
		{
			set_color(cr, bg.color);
			cairo_paint(cr);
		}*/

		if( !bg.image.empty() ) {

			QImage*	img = getImage( bg.image.c_str(), bg.baseurl.c_str() );
			if( img )
			{
				switch(bg.repeat)
				{
				case litehtml::background_repeat_no_repeat:
					//draw_txdib(cr, bgbmp.get(), bg.position_x, bg.position_y, bgbmp->getWidth(), bgbmp->getHeight());
					pnt->drawImage( bg.position_x, bg.position_y, *img, 0, 0, img->width(), img->height() );
					break;

				case litehtml::background_repeat_repeat_x:
					//cairo_set_source(cr, pattern);
					//cairo_rectangle(cr, bg.clip_box.left(), bg.position_y, bg.clip_box.width, bgbmp->getHeight());
					//cairo_fill(cr);
					break;

				case litehtml::background_repeat_repeat_y:
					//cairo_set_source(cr, pattern);
					//cairo_rectangle(cr, bg.position_x, bg.clip_box.top(), bgbmp->getWidth(), bg.clip_box.height);
					//cairo_fill(cr);
					break;

				case litehtml::background_repeat_repeat:
					//cairo_set_source(cr, pattern);
					//cairo_rectangle(cr, bg.clip_box.left(), bg.clip_box.top(), bg.clip_box.width, bg.clip_box.height);
					//cairo_fill(cr);
					break;
				}
			}
		}
	}

	virtual void	draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) {

		QPainter* pnt = (QPainter*)hdc;
		apply_clip( pnt );

		int		l = draw_pos.left();
		int		r = draw_pos.right();
		int		t = draw_pos.top();
		int		b = draw_pos.bottom();

		int		it = (borders.top.width && borders.top.style>litehtml::border_style_hidden) ? 1 : 0;
		int		ib = (borders.bottom.width && borders.bottom.style>litehtml::border_style_hidden) ? 1 : 0;
		int		il = (borders.left.width && borders.left.style>litehtml::border_style_hidden) ? 1 : 0;
		int		ir = (borders.right.width && borders.right.style>litehtml::border_style_hidden) ? 1 : 0;

		//	todo: adjust pen style
		//	todo: optimize with path

		// draw left border
		if( il )
		{
			QPen	pen( QBrush(cvtColor(borders.left.color)), 1 );
			pnt->setPen( pen );

			if( borders.left.width==1 ) {
				pnt->drawLine( l, t, l, b-1 );
			}
			else {
				for(int x=0, L=l,T=t,B=b-1; x<borders.left.width; x++, L++,T+=it,B-=ib ) {
					pnt->drawLine( L, T, L, B );
				}
			}
		}

		// draw right border
		if( ir )
		{
			QPen	pen( QBrush(cvtColor(borders.right.color)), 1 );
			pnt->setPen( pen );

			if( borders.right.width==1 ) {
				pnt->drawLine( r-1, t, r-1, b-1 );
			}
			else {
				for(int x=0, R=r-1,T=t,B=b-1; x < borders.right.width; x++, R--,T+=it,B-=ib ) {
					pnt->drawLine( R, T, R, B );
				}
			}
		}

		// draw top border
		if( it )
		{
			QPen	pen( QBrush(cvtColor(borders.top.color)), 1 );
			pnt->setPen( pen );

			if( borders.top.width==1 ) {
				pnt->drawLine( l, t, r-1, t );
			}
			else {
				for(int x=0,T=t,L=l,R=r-1; x < borders.top.width; x++,T++,L+=il,R-=ir ) {
					pnt->drawLine( L, T, R, T );
				}
			}
		}

		// draw bottom border
		if( ib )
		{
			QPen	pen( QBrush(cvtColor(borders.bottom.color)), 1 );
			pnt->setPen( pen );

			if( borders.bottom.width==1 ) {
				pnt->drawLine( l, b-1, r, b-1 );
			}
			else {
				for(int x=0, B=b-1,L=l,R=r-1; x < borders.bottom.width; x++,B--,L+=il,R-=ir ) {
					pnt->drawLine( L, B, R, B );
				}
			}
		}
	}

	virtual	void	set_caption(const litehtml::tchar_t* caption) {
//		__debugger;
	}

	virtual	void	set_base_url(const litehtml::tchar_t* base_url) {
		m_base_url = base_url;
	}

	virtual void	link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el) {
		__debugger;
	}

	virtual void	on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) {
		__debugger;
	}

	virtual	void	set_cursor(const litehtml::tchar_t* cursor) {

		QCursor	c( Qt::ArrowCursor );

		if( stricmp(cursor,"pointer")==0 ) {
			c.setShape( Qt::PointingHandCursor );
		}

		m_owner->setCursor( c );
	}

	virtual	void	transform_text(litehtml::tstring& text, litehtml::text_transform tt) {
		__debugger;
	}

	virtual void	import_css(litehtml::xstring& text, const litehtml::xstring& url, litehtml::xstring& baseurl) {

		QString fname = make_url( url.c_str(), baseurl.c_str() );

		QFile		file( fname );
		if( !file.open( QFile::ReadOnly ) ) {
			qDebug() << "cannot open css file: " << fname;
			return;
		}

		QString	css = file.readAll();
		text = css.toUtf8();
	}

	virtual void	set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y)
	{
		litehtml::position clip_pos = pos;
		QRect	rc = m_owner->rect( );

		if(!valid_x)
		{
			clip_pos.x		= rc.left();
			clip_pos.width	= rc.width();
		}

		if(!valid_y)
		{
			clip_pos.y		= rc.top();
			clip_pos.height	= rc.height();
		}

		m_clips.push_back( clip_pos );
	}

	virtual void	del_clip() {
		if(!m_clips.empty())
		{
			m_clips.pop_back();
			//if(!m_clips.empty())
			//{
			//	litehtml::position clip_pos = m_clips.back();
			//}
		}
	}

	void	apply_clip( QPainter* pnt )
	{
		if(!m_clips.empty())
		{
			litehtml::position clip_pos = m_clips.back();
			pnt->setClipRect( clip_pos.left(), clip_pos.top(), clip_pos.width, clip_pos.height );
		}
	}

	virtual void	get_client_rect(litehtml::position& client) const
	{
		QRect	rc = m_owner->rect();
		client.x = rc.left();
		client.y = rc.top();
		client.width = rc.width();
		client.height = rc.height();
	}

	virtual litehtml::element::ptr	create_element(const litehtml::tchar_t *tag_name, const litehtml::attr_map &attributes, const std::shared_ptr<litehtml::document> &doc )
	{
		return 0;
	}

	virtual void	get_media_features(litehtml::media_features& media) const
	{
		// todo desktop
		media.type = litehtml::media_type_all;
		media.width = 4096;
		media.height = 2048;
		media.device_width = 4096;
		media.device_height = 2048;
		media.color = 8;
		media.color_index = 0;
		media.resolution = 96;
	}

	virtual void	get_language(litehtml::tstring& language, litehtml::tstring & culture) const
	{
		__debugger;
	}


	QColor cvtColor( const litehtml::web_color& color )
	{
		return	QColor(color.red(),color.green(),color.blue(),color.alpha());
	}

	QString make_url( const char* url, const char* basepath, QUrl* pUrl=NULL )
	{
		QUrl	u = QUrl::fromUserInput( url, (basepath && basepath[0]) ? basepath : m_base_url );
		if( pUrl )
			*pUrl = u;
		return u.toLocalFile();
	}
};

/**
 * @brief The html_renderer_private struct
 */

struct	html_renderer_private
{
	litehtml::document::ptr	m_document;
	litehtml::context		m_context;
	container				m_container;
	QXsRunner*				m_xs;

	html_renderer_private( QWidget* owner ) :
		m_container( owner ),
		m_document( NULL ),
		m_xs( NULL )
	{
		QFile	master( "../../resources/html5reset-1.6.1.css" );
		if( master.open(QFile::ReadOnly) ) {
			QString css = master.readAll();
			m_context.load_master_stylesheet( css.toUtf8() );
		}

		m_container.setDefImage( QImage("../../resources/broken_image.gif") );
	}

	void	setPage( litehtml::document::ptr doc ) {
		m_document = doc;
		m_xs = new QXsRunner( doc.get() );
		m_xs->start( );
	}
};


/**
 * @brief QHtmlRenderer::QHtmlRenderer
 * @param parent
 */

QHtmlRenderer::QHtmlRenderer(QWidget *parent)
	: QWidget(parent)
{
	m_private = new html_renderer_private( this );
	m_top = 0;
	m_left = 0;
	m_max_top = 0;
	m_max_left = 0;
	setMouseTracking( true );
}


void	QHtmlRenderer::loadFile( const QString& fname )
{
	QFile		file( fname );
	if( !file.open( QFile::ReadOnly) ) {
		qDebug() << "cannot open file: " << fname;
		return;
	}

	QFileInfo	fi( file );
	m_private->m_container.set_base_url( fi.absolutePath().toUtf8() );

	loadText( file.readAll() );
}

void	QHtmlRenderer::loadText( const QString& text )
{
	m_private->setPage( litehtml::document::createFromUTF8( text.toUtf8(), &m_private->m_container, &m_private->m_context ) );
	preparePage( );
	repaint();
}

void QHtmlRenderer::preparePage( ) {

	QRect	rc = rect( );

	m_private->m_document->render( rc.width() );
	m_private->m_document->render( rc.width() );

	m_max_top = m_private->m_document->height() - rc.height();
	if(m_max_top < 0) m_max_top = 0;

	m_max_left = m_private->m_document->width() - rc.width();
	if(m_max_left < 0) m_max_left = 0;

	resize( m_private->m_document->width(), m_private->m_document->height() );
}

void QHtmlRenderer::paintEvent(QPaintEvent *event) {

	if( m_private && m_private->m_document ) {
		QPainter pnt( this );
		m_private->m_document->draw( (uintptr_t)&pnt, -m_left, -m_top, 0 );
	}
}

void QHtmlRenderer::resizeEvent(QResizeEvent* event)
{
	if( m_private && m_private->m_document ) {
		m_private->m_document->media_changed();
		preparePage();
	}
}

void QHtmlRenderer::mouseMoveEvent(QMouseEvent* event)
{
	//qDebug() << "mouse_move( " << event->x() << ", " << event->y() << ")";

	if( m_private && m_private->m_document ) {

		int x = event->x();
		int y = event->y();

		litehtml::position::vector redraw_boxes;
		if(m_private->m_document->on_mouse_over( x + m_left, y + m_top, x, y, redraw_boxes) )
		{
			QRegion	rgn;

			for(litehtml::position::vector::iterator box = redraw_boxes.begin(); box != redraw_boxes.end(); box++)
			{
				box->x -= m_left;
				box->y -= m_top;
				QRect	rcRedraw( box->x, box->y, box->width, box->height );
				rgn += rcRedraw;
			}

			repaint( rgn );
		}
	}
}

/*void QHtmlRenderer::wheelEvent(QWheelEvent* event)
{
	if( m_private && m_private->m_page ) {

		int newTop  = m_top - event->delta();

		if(newTop < 0) {
			newTop = 0;
		}

		if(newTop > m_max_top) {
			newTop = m_max_top;
		}

		if(newTop != m_top) {
			scroll_to(m_left, newTop);
		}
	}
}*/

void QHtmlRenderer::scroll_to( int x, int y )
{
	scroll( x-m_left, y-m_top );
	m_left = x;
	m_top = y;
}

