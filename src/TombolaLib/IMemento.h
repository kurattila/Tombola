#ifndef IMEMENTO_H
#define IMEMENTO_H

class QXmlStreamReader;
class QXmlStreamWriter;

class IMemento
{
public:
    virtual void Read(QXmlStreamReader& xmlReader) = 0;
    virtual void Write(QXmlStreamWriter& xmlWriter) = 0;
};

class IMementoOriginator
{
public:
    virtual IMemento* SaveToMemento() = 0;
    virtual void RestoreFromMemento(const IMemento* memento, void* context = nullptr) = 0;
};

#endif // IMEMENTO_H
