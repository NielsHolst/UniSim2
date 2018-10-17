#ifndef BASE_ALTOVA_XML_H
#define BASE_ALTOVA_XML_H
#include <QObject>
#include <QSet>
#include <QStringList>
#include <QTextStream>

namespace base {


class AltovaXml : public QObject {
public:
    AltovaXml(QObject *parent = nullptr);
    void run(QString inputFilePath,
             QString outputFilePath);
private:
    // Data
    QString _xsltFileName;
    QStringList _searchPath;
    // Methods
    QString xsltPath();
    QString altovaPath();
};

}

#endif
