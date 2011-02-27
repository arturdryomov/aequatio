#include "application.h"

#ifdef Q_WS_WIN
	#include "qt_windows.h"	
	#include <QSysInfo>
	#include <QFont>
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
	NONCLIENTMETRICS metrics;	
	ZeroMemory(&metrics, sizeof(NONCLIENTMETRICS));
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0)) {
		return QFont(); // application default font
	}
	
	int pointSize = MulDiv(-metrics.lfMessageFont.lfHeight, 72, 
		GetDeviceCaps(GetDC(GetDesktopWindow()), LOGPIXELSY));
	QFont font;	
	font.setPointSize(pointSize);
	font.setFamily(QString::fromWCharArray(metrics.lfMessageFont.lfFaceName));
	return font;	
}
