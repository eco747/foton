#-------------------------------------------------
#
# Project created by QtCreator 2018-03-10T09:10:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = foton
TEMPLATE = app

INCLUDEPATH += xs/includes
INCLUDEPATH += xs/platforms
INCLUDEPATH += xs/sources
INCLUDEPATH += modules/base
INCLUDEPATH += litehtml/include

DEFINES += mxParse
DEFINES += mxDebug
DEFINES += mxInstrument
#DEFINES += mxProfile
DEFINES += mxReport
DEFINES += mxHostFunctionPrimitive
DEFINES += LITEHTML_UTF8

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    xs/sources/xsAll.c \
    xs/sources/xsAPI.c \
    xs/sources/xsArray.c \
    xs/sources/xsAtomics.c \
    xs/sources/xsBoolean.c \
    xs/sources/xsCode.c \
    xs/sources/xsCommon.c \
    xs/sources/xsDataView.c \
    xs/sources/xsDate.c \
    xs/sources/xsDebug.c \
    xs/sources/xsdtoa.c \
    xs/sources/xsError.c \
    xs/sources/xsFunction.c \
    xs/sources/xsGenerator.c \
    xs/sources/xsGlobal.c \
    xs/sources/xsJSON.c \
    xs/sources/xsLexical.c \
    xs/sources/xsMapSet.c \
    xs/sources/xsMarshall.c \
    xs/sources/xsMath.c \
    xs/sources/xsmc.c \
    xs/sources/xsMemory.c \
    xs/sources/xsModule.c \
    xs/sources/xsNumber.c \
    xs/sources/xsObject.c \
    xs/sources/xsPlatforms.c \
    xs/sources/xsProfile.c \
    xs/sources/xsPromise.c \
    xs/sources/xsProperty.c \
    xs/sources/xsProxy.c \
    xs/sources/xsre.c \
    xs/sources/xsRegExp.c \
    xs/sources/xsRun.c \
    xs/sources/xsScope.c \
    xs/sources/xsScript.c \
    xs/sources/xsSourceMap.c \
    xs/sources/xsString.c \
    xs/sources/xsSymbol.c \
    xs/sources/xsSyntaxical.c \
    xs/sources/xsTree.c \
    xs/sources/xsType.c \
    modules/base/console/modConsole.c \
    modules/base/timer/win/timer.c \
    modules/base/timer/modTimer.c \
    litehtml/src/background.cpp \
    litehtml/src/box.cpp \
    litehtml/src/context.cpp \
    litehtml/src/css_length.cpp \
    litehtml/src/css_selector.cpp \
    litehtml/src/document.cpp \
    litehtml/src/el_anchor.cpp \
    litehtml/src/el_base.cpp \
    litehtml/src/el_before_after.cpp \
    litehtml/src/el_body.cpp \
    litehtml/src/el_break.cpp \
    litehtml/src/el_cdata.cpp \
    litehtml/src/el_comment.cpp \
    litehtml/src/el_div.cpp \
    litehtml/src/el_font.cpp \
    litehtml/src/el_image.cpp \
    litehtml/src/el_link.cpp \
    litehtml/src/el_para.cpp \
    litehtml/src/el_script.cpp \
    litehtml/src/el_space.cpp \
    litehtml/src/el_style.cpp \
    litehtml/src/el_table.cpp \
    litehtml/src/el_td.cpp \
    litehtml/src/el_text.cpp \
    litehtml/src/el_title.cpp \
    litehtml/src/el_tr.cpp \
    litehtml/src/element.cpp \
    litehtml/src/html.cpp \
    litehtml/src/html_tag.cpp \
    litehtml/src/iterators.cpp \
    litehtml/src/media_query.cpp \
    litehtml/src/style.cpp \
    litehtml/src/stylesheet.cpp \
    litehtml/src/table.cpp \
    litehtml/src/utf8_strings.cpp \
    litehtml/src/web_color.cpp \
    litehtml/src/gumbo/attribute.c \
    litehtml/src/gumbo/char_ref.c \
    litehtml/src/gumbo/error.c \
    litehtml/src/gumbo/parser.c \
    litehtml/src/gumbo/string_buffer.c \
    litehtml/src/gumbo/string_piece.c \
    litehtml/src/gumbo/tag.c \
    litehtml/src/gumbo/tokenizer.c \
    litehtml/src/gumbo/utf8.c \
    litehtml/src/gumbo/util.c \
    litehtml/src/gumbo/vector.c \
	prebuild_js.xs.c \
    qhtmlrenderer.cpp \
    litehtml/src/atoms.cpp \
    modules/dom/xsDOM.cpp \
    qxsrunner.cpp \
    litehtml/src/xstring.cpp \
    litehtml/src/_keywords.cpp \
    litehtml/src/xstring.cpp \
    litehtml/src/xvector.cpp

HEADERS  += mainwindow.h \
    xs/includes/xs.h \
    xs/includes/xsmc.h \
    xs/platforms/win_xs.h \
    xs/platforms/xsPlatform.h \
    xs/sources/xsAll.h \
    xs/sources/xsCommon.h \
    xs/sources/xsScript.h \
    modules/base/timer/modTimer.h \
    litehtml/include/litehtml.h \
    litehtml/src/gumbo/attribute.h \
    litehtml/src/gumbo/char_ref.h \
    litehtml/src/gumbo/error.h \
    litehtml/src/gumbo/gumbo.h \
    litehtml/src/gumbo/insertion_mode.h \
    litehtml/src/gumbo/parser.h \
    litehtml/src/gumbo/string_buffer.h \
    litehtml/src/gumbo/string_piece.h \
    litehtml/src/gumbo/strings.h \
    litehtml/src/gumbo/tag_enum.h \
    litehtml/src/gumbo/tag_gperf.h \
    litehtml/src/gumbo/tag_sizes.h \
    litehtml/src/gumbo/tag_strings.h \
    litehtml/src/gumbo/token_type.h \
    litehtml/src/gumbo/tokenizer.h \
    litehtml/src/gumbo/tokenizer_states.h \
    litehtml/src/gumbo/utf8.h \
    litehtml/src/gumbo/util.h \
    litehtml/src/gumbo/vector.h \
    litehtml/src/attributes.h \
    litehtml/src/background.h \
    litehtml/src/borders.h \
    litehtml/src/box.h \
    litehtml/src/context.h \
    litehtml/src/css_length.h \
    litehtml/src/css_margins.h \
    litehtml/src/css_offsets.h \
    litehtml/src/css_position.h \
    litehtml/src/css_selector.h \
    litehtml/src/document.h \
    litehtml/src/el_anchor.h \
    litehtml/src/el_base.h \
    litehtml/src/el_before_after.h \
    litehtml/src/el_body.h \
    litehtml/src/el_break.h \
    litehtml/src/el_cdata.h \
    litehtml/src/el_comment.h \
    litehtml/src/el_div.h \
    litehtml/src/el_font.h \
    litehtml/src/el_image.h \
    litehtml/src/el_link.h \
    litehtml/src/el_para.h \
    litehtml/src/el_script.h \
    litehtml/src/el_space.h \
    litehtml/src/el_style.h \
    litehtml/src/el_table.h \
    litehtml/src/el_td.h \
    litehtml/src/el_text.h \
    litehtml/src/el_title.h \
    litehtml/src/el_tr.h \
    litehtml/src/element.h \
    litehtml/src/html.h \
    litehtml/src/html_tag.h \
    litehtml/src/iterators.h \
    litehtml/src/media_query.h \
    litehtml/src/os_types.h \
    litehtml/src/style.h \
    litehtml/src/stylesheet.h \
    litehtml/src/table.h \
    litehtml/src/types.h \
    litehtml/src/utf8_strings.h \
    litehtml/src/web_color.h \
	prebuild_js.xs.h \
    qhtmlrenderer.h \
    qxsrunner.h \
    litehtml/src/_keywords.h \
    litehtml/src/xstring.h \
    litehtml/src/xvector.h

win32:LIBS += -luser32

# ----------------------------------------------

# XS_FILES =	modules/base/timer/timer.js \
# 	modules/base/console/console.js
#
# X_SOURCES =
#
#
# {
# 	xsl_js.output = debug/prebuild_js.xs.c
# 	xsl_js.depends = FORCE
# 	xsl_js.input = X_SOURCES
# 	xsl_js.variable_out = SOURCES
# 	xsl_js.CONFIG += combine target_predeps no_link
# 	xsl_js.commands = c:/qt/projects/photonboard2/xsc/xsl -r prebuild_js ${QMAKE_FILE_NAME} -o debug
# 	QMAKE_EXTRA_COMPILERS += xsl_js
# }
#
# {
# 	xsc_js.output  = debug/${QMAKE_FILE_BASE}.xsb
# 	xsc_js.commands = c:/qt/projects/photonboard2/xsc/xsc ${QMAKE_FILE_NAME} -d -c -e -o debug
# 	xsc_js.input = XS_FILES
# 	xsc_js.variable_out = X_SOURCES
# 	xsc_js.CONFIG += target_predeps
# 	QMAKE_EXTRA_COMPILERS += xsc_js
# }
#

DISTFILES += \
    test.js \
    litehtml/include/master.css \
    modules/dom/dom.js \
    modules/dom/testfile.html
