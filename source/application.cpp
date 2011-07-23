#include "application.h"

#include <QSysInfo>
#include <QFont>

#ifdef Q_WS_WIN
	#include <qt_windows.h>
#endif

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
#ifdef Q_WS_WIN
	// adjust application font to Segoe UI on Vista and older
	if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA)	 {
		setFont(systemFont());
	}
#endif	
}

QFont Application::systemFont()
{
#ifdef Q_WS_WIN
	NONCLIENTMETRICS metrics;	
	ZeroMemory(&metrics, sizeof(NONCLIENTMETRICS));
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0)) {
		// application default font
		return QFont();
	}
	
	int pointSize = MulDiv(-metrics.lfMessageFont.lfHeight, 72, 
		GetDeviceCaps(GetDC(GetDesktopWindow()), LOGPIXELSY));
#endif	
	QFont font;
#ifdef Q_WS_WIN		
	font.setPointSize(pointSize);
	font.setFamily(QString::fromWCharArray(metrics.lfMessageFont.lfFaceName));
#endif	
	return font;	
}
