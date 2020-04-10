#include "PACSystemProxyConfigurator.hpp"

#include <QProcess>
#include <QStandardPaths>
#include <QStringList>
#include <QRegExp>
#ifdef Q_OS_WIN
    #include <Windows.h>
    //
    #include <WinInet.h>
#endif

#ifdef Q_OS_MACOS
QStringList macOSgetNetworkServices()
{
    QProcess p;
    p.setProgram("/usr/sbin/networksetup");
    p.setArguments(QStringList{ "-listallnetworkservices" });
    p.start();
    p.waitForStarted();
    p.waitForFinished();
    // LOG(MODULE_PROXY, p.errorString())
    QString str = p.readAllStandardOutput();
    auto lines = str.split(QRegExp("[\r\n]"));
    QStringList result;

    // Start from 1 since first line is unneeded.
    for (auto i = 1; i < lines.count(); i++)
    {
        // * means disabled.
        if (!lines[i].contains("*"))
        {
            result << (lines[i].contains(" ") ? "\"" + lines[i] + "\"" : lines[i]);
        }
    }

    // LOG(MODULE_PROXY, "Found " + QSTRN(result.size()) + " network services: " + result.join(";"))
    return result;
}
#endif
#ifdef Q_OS_WIN
bool __SetProxyOptions(LPWSTR proxy_full_addr)
{
    INTERNET_PER_CONN_OPTION_LIST list;
    bool bReturn;
    DWORD dwBufSize = sizeof(list);
    // Fill the list structure.
    list.dwSize = sizeof(list);
    // NULL == LAN, otherwise connectoid name.
    list.pszConnection = nullptr;

    // LOG(MODULE_PROXY, "Setting system proxy for PAC")
    //
    list.dwOptionCount = 2;
    list.pOptions = new INTERNET_PER_CONN_OPTION[2];

    // Ensure that the memory was allocated.
    if (nullptr == list.pOptions)
    {
        // Return FALSE if the memory wasn't allocated.
        return FALSE;
    }

    // Set flags.
    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT | PROXY_TYPE_AUTO_PROXY_URL;
    // Set proxy name.
    list.pOptions[1].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
    list.pOptions[1].Value.pszValue = proxy_full_addr;

    // Set the options on the connection.
    bReturn = InternetSetOption(nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);
    delete[] list.pOptions;
    InternetSetOption(nullptr, INTERNET_OPTION_SETTINGS_CHANGED, nullptr, 0);
    InternetSetOption(nullptr, INTERNET_OPTION_REFRESH, nullptr, 0);
    return bReturn;
}
#endif

void SetSystemProxy(const QString &address)
{
#ifdef Q_OS_WIN
    QString __a = address;

    auto proxyStrW = new WCHAR[__a.length() + 1];
    wcscpy(proxyStrW, __a.toStdWString().c_str());
    //
    //__QueryProxyOptions();

    if (!__SetProxyOptions(proxyStrW))
    {
        // LOG(MODULE_PROXY, "Failed to set proxy.")
    }

    //__QueryProxyOptions();
#elif defined(Q_OS_LINUX)
    QStringList actions;
    actions << QString("gsettings set org.gnome.system.proxy mode 'auto'");
    actions << QString("gsettings set org.gnome.system.proxy autoconfig-url '%1'").arg(address);
    bool isKDE = qEnvironmentVariable("XDG_SESSION_DESKTOP") == "KDE";
    if (isKDE)
    {
        // LOG(MODULE_PROXY, "KDE detected")
        actions << QString("kwriteconfig5  --file " + QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                           "/kioslaverc --group \"Proxy Settings\" --key ProxyType 2");

        actions << QString("kwriteconfig5  --file " + QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                           "/kioslaverc --group \"Proxy Settings\" --key \"Proxy Config Script\" " + address);
    }

    // note: do not use std::all_of / any_of / none_of,
    // because those are short-circuit and cannot guarantee atomicity.
    auto result = std::count_if(actions.cbegin(), actions.cend(),
                                [](const QString &action) { return QProcess::execute(action) == QProcess::NormalExit; }) == actions.size();

    if (!result)
    {
        // LOG(MODULE_PROXY, "There was something wrong when setting proxies.")
        // LOG(MODULE_PROXY, "It may happen if you are using KDE with no gsettings support.")
    }

#else
    for (const auto &service : macOSgetNetworkServices())
    {
        QProcess::execute("/usr/sbin/networksetup -setautoproxystate " + service + " on");
        QProcess::execute("/usr/sbin/networksetup -setautoproxyurl " + service + " " + address);
    }
#endif
}

// void ClearSystemProxy()
//{
//#ifdef Q_OS_WIN
//    INTERNET_PER_CONN_OPTION_LIST list;
//    BOOL bReturn;
//    DWORD dwBufSize = sizeof(list);
//    // Fill out list struct.
//    list.dwSize = sizeof(list);
//    // nullptr == LAN, otherwise connectoid name.
//    list.pszConnection = nullptr;
//    // Set three options.
//    list.dwOptionCount = 1;
//    list.pOptions = new INTERNET_PER_CONN_OPTION[list.dwOptionCount];

//    // Make sure the memory was allocated.
//    if (nullptr == list.pOptions)
//    {
//        // Return FALSE if the memory wasn't allocated.
//        // LOG(MODULE_PROXY, "Failed to allocat memory in DisableConnectionProxy()")
//    }

//    // Set flags.
//    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
//    list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT;
//    //
//    // Set the options on the connection.
//    bReturn = InternetSetOption(nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);
//    delete[] list.pOptions;
//    InternetSetOption(nullptr, INTERNET_OPTION_SETTINGS_CHANGED, nullptr, 0);
//    InternetSetOption(nullptr, INTERNET_OPTION_REFRESH, nullptr, 0);
//#elif defined(Q_OS_LINUX)
//    if (qEnvironmentVariable("XDG_SESSION_DESKTOP") == "KDE")
//    {
//        QProcess::execute("kwriteconfig5 --file " + QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
//                          "/kioslaverc --group \"Proxy Settings\" --key ProxyType 0");
//    }
//    QProcess::execute("gsettings set org.gnome.system.proxy mode 'none'");
//#else

//    for (auto service : macOSgetNetworkServices())
//    {
//        // LOG(MODULE_PROXY, "Clearing proxy for interface: " + service)
//        QProcess::execute("/usr/sbin/networksetup -setautoproxystate " + service + " off");
//        QProcess::execute("/usr/sbin/networksetup -setwebproxystate " + service + " off");
//        QProcess::execute("/usr/sbin/networksetup -setsecurewebproxystate " + service + " off");
//        QProcess::execute("/usr/sbin/networksetup -setsocksfirewallproxystate " + service + " off");
//    }

//#endif
//}
