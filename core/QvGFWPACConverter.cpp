/* ORIGINAL LICENSE: Do What The F*ck You Want To Public License
 * AUTHOR: LBYPatrick
 *
 * MODIFIED BY Leroy.H.Y @lhy0403 re-licenced under GPLv3
 */

#include <QString>
#include <string>

using namespace std;

// Private function
string getRawDomain(string originLine)
{
    size_t startPosition = 0;
    size_t endPosition = originLine.size();
    string returnBuffer;
    bool skipRule1 = originLine.find("[") != string::npos; // [Auto xxxx...
    bool skipRule2 = originLine.find("!") != string::npos; // Comments
    bool skipRule3 = originLine.find("@") != string::npos; // Non-proxy Lines
    bool skipRule4 = originLine.find("*") != string::npos;
    bool passRule1 = originLine.find("|") != string::npos; // Proxy Lines
    bool passRule2 = originLine.find(".") != string::npos; // Link-Contained Lines

    if (originLine[endPosition] == '\n')
    {
        endPosition -= 1;
    }

    if (originLine.find("http://") != string::npos)
    {
        startPosition += 8;
    }
    else if (originLine.find("https://") != string::npos)
    {
        startPosition += 9;
    }

    // Skip unrelated lines
    if (skipRule1 || skipRule2 || skipRule3 || skipRule4)
    {
        return "";
    }
    else if (passRule2)
    {
        if (passRule1)
        {
            startPosition += originLine.find_last_of("|") + 1;
        }

        if (originLine[startPosition] == '\n')
            startPosition += 1;

        for (size_t i = startPosition; i < endPosition; ++i)
        {
            returnBuffer += originLine[i];
        }
    }

    return returnBuffer;
}

QString ConvertGFWToPAC(const QByteArray &rawContent, const QString &customProxyString)
{
    auto rawFileContent = QByteArray::fromBase64(rawContent).toStdString();
    string readBuffer = ""; // cleanup
    string writeBuffer;
    string domainListCache = "";

    for (size_t i = 0; i < rawFileContent.size(); ++i)
    {
        readBuffer += rawFileContent[i];

        if (rawFileContent[i + 1] == '\n')
        {
            writeBuffer = getRawDomain(readBuffer);

            if (writeBuffer != "")
            {
                domainListCache += writeBuffer + "\n";
            }

            readBuffer = "";
            i += 1;
        }
    }

    size_t rotatorTwo = 0;
    string readDomainBuffer = "";
    bool isFirstLine = true;
    string outputContent = "";
    // Header
    outputContent += "var domains = {\n";

    // Read and process output content line by line
    while (rotatorTwo < domainListCache.size())
    {
        while (true)
        {
            // Get Domain
            readDomainBuffer += domainListCache[rotatorTwo];

            if (domainListCache[rotatorTwo + 1] == '\n')
            {
                rotatorTwo += 2;
                break;
            }

            rotatorTwo++;
        }

        // Format
        if (!isFirstLine)
            outputContent += ",\n";
        else
            isFirstLine = false;

        outputContent += "\t\"";
        outputContent += readDomainBuffer;
        outputContent += "\" : 1";
        readDomainBuffer = "";
    }

    // End Message
    outputContent += "\r\n"
                     "};"
                     "\r\n"
                     ""
                     "\r\n"
                     "   var proxy = \"" +
                     customProxyString.toStdString() + ";\";" +
                     "\r\n"
                     "   var direct = 'DIRECT;';"
                     "\r\n"
                     "    function FindProxyForURL(url, host) {"
                     "\r\n"
                     "        var suffix;"
                     "\r\n"
                     "        var pos = host.lastIndexOf('.');"
                     "\r\n"
                     "        pos = host.lastIndexOf('.', pos - 1);"
                     "\r\n"
                     "        //"
                     "\r\n"
                     "        while (1) {"
                     "\r\n"
                     "            if (domains[host] != undefined) {"
                     "\r\n"
                     "                return proxy;"
                     "\r\n"
                     "            }"
                     "\r\n"
                     "            else if (pos <= 0) {"
                     "\r\n"
                     "                return domains['.' + host] != undefined ? proxy : direct;"
                     "\r\n"
                     "            }"
                     "\r\n"
                     "            suffix = host.substring(pos);"
                     "\r\n"
                     "            if (domains[suffix] != undefined) {"
                     "\r\n"
                     "                return proxy;"
                     "\r\n"
                     "            }"
                     "\r\n"
                     "            pos = host.lastIndexOf('.', pos - 1);"
                     "\r\n"
                     "        }"
                     "\r\n"
                     "    }";
    //
    return QString::fromStdString(outputContent);
}
