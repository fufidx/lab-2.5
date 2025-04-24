#include <iostream>
#include  "patterntemplates.h"
#include <cstdlib>
#include <ctime>
#include <list>

using namespace std;

enum class PitanieEnum : int
{
  blok_pitaniya,
  powerbank,
  prikurivatel,

  None
};

class PitanieStrategy
{
public:
  virtual ~PitanieStrategy() {}
  virtual void Pit() = 0;
};

class blok_pitaniyaStrategy : public PitanieStrategy
{
  void Pit() { cout << "zaryad ot bloka pitaniya"; }
};

class powerbankStrategy : public PitanieStrategy
{
  void Pit() { cout << "zaryad ot powerbanka"; }
};

class prikurivatelStrategy : public PitanieStrategy
{
  void Pit() { cout << "zaryad ot prikurivatela"; }
};

PitanieStrategy *CreatePitanieStrategy(PitanieEnum tipPitanie)
{
  switch(tipPitanie)
  {
    case PitanieEnum::blok_pitaniya: return new blok_pitaniyaStrategy;
    case PitanieEnum::powerbank: return new powerbankStrategy;
    case PitanieEnum::prikurivatel: return new powerbankStrategy;
    default: return nullptr;
  }
}


class Connector
{


protected:

    bool reversee;
    int pinCount;
    int maxPower;

public:

    Connector(int pins, int power, bool reversible);
    virtual ~Connector();

    int GetpinCount() const { return pinCount; }
    int GetmaxPower() const { return maxPower; }
    bool Getreversee() const { return reversee; }

    virtual void Connect() = 0;
    virtual void TransferData() = 0;
    virtual void ChargeDevice() = 0;
    void SetPitanie(PitanieEnumStrategy *tipPitanie) { PitanieEnum = tipPitanie; }
};

Connector::Connector(int pins, int power, bool reversible) : pinCount(pins), maxPower(power), reversee(reversible)
{
    cout << "Connector created." << endl;
}
Connector::~Connector()
{
    cout << "Connector destroyed." << endl;
}

class USBC : public Connector
{
public:
    USBC() : Connector(24, 240, 1){}

    void Connect()
    {
        cout << "USB-C connected." << endl;
    }
    void TransferData()
    {
        cout << "USB-C transferring data at high speed." << endl;
    }
    void ChargeDevice()
    {
        cout << "USB-C charging at " << maxPower << "V." << endl;
    }

};

class Lightning : public Connector
{
public:
    Lightning() : Connector(8, 20, 1){}

    void Connect()
    {
        cout << "Lightning connected." << endl;
    }
    void TransferData()
    {
        cout << "Lightning transferring data." << endl;
    }
    void ChargeDevice()
    {
        cout << "Lightning charging at " << maxPower << "V." << endl;
    }

};

class microUSB : public Connector
{
public:
    microUSB() : Connector(5, 9, 0){}

    void Connect()
    {
        cout << "microUSB connected." << endl;
    }
    void TransferData()
    {
        cout << "microUSB transferring data at lower speed." << endl;
    }
    void ChargeDevice()
    {
        cout << "microUSB charging at " << maxPower << "V." << endl;
    }

};

enum class ConnectorType : int
{
    USBC = 1,
    Lightning = 2,
    microUSB = 3,
    Undefined = 0
};

Connector *CreateConnector(ConnectorType type)
{
    Connector *newConnector = nullptr;

    if(type == ConnectorType::USBC)
    {
        newConnector = new USBC;
    }
    else if(type == ConnectorType::Lightning)
    {
        newConnector = new Lightning;
    }
    else if(type == ConnectorType::microUSB)
    {
        newConnector = new microUSB;
    }

    return newConnector;
}

void ConnectAll(Iterator<Connector*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Connector *currentConnector = it->GetCurrent();
        currentConnector->Connect();
    }
}

class PowerFilterDecorator : public IteratorDecorator<Connector*>
{
private:
    int MinPower;

public:
    PowerFilterDecorator(Iterator<Connector*> *it, int minPower)
        : IteratorDecorator<Connector*>(it), MinPower(minPower) {}

    void First()
    {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetmaxPower() < MinPower)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        }
        while (!It->IsDone() && It->GetCurrent()->GetmaxPower() < MinPower);
    }
};

class ReversibilityFilterDecorator : public IteratorDecorator<Connector*>
{
private:
    bool TargetReversibility;

public:
    ReversibilityFilterDecorator(Iterator<Connector*> *it, bool reversibility)
        : IteratorDecorator<Connector*>(it), TargetReversibility(reversibility) {}

    void First()
    {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->Getreversee() != TargetReversibility)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        }
        while (!It->IsDone() && It->GetCurrent()->Getreversee() != TargetReversibility);
    }
};

class PinCountFilterDecorator : public IteratorDecorator<Connector*>
{
private:
    int MinPins;

public:
    PinCountFilterDecorator(Iterator<Connector*> *it, int minPins)
        : IteratorDecorator<Connector*>(it), MinPins(minPins) {}

    void First()
    {
        It->First();
        while (!It->IsDone() && It->GetCurrent()->GetpinCount() < MinPins)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        }
        while (!It->IsDone() && It->GetCurrent()->GetpinCount() < MinPins);
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    wcout << L"Какой разъём создать";
    cout<< "(1 - USB-C, 2 - Lightning , 3 - microUSB)?" << endl;
    ConnectorType type = ConnectorType::Undefined;
    int ii;
    cin >> ii;
    type = static_cast<ConnectorType>(ii);

    Connector *newConnector = CreateConnector(type);
    newConnector->Connect();
    delete newConnector;

    size_t N = 0;
    wcout << L"Введите количество разъёмов: ";
    cin >> N;

    cout << endl;

    ArrayClass<Connector*> ArrayConnector;
    for(size_t i=0; i<N; i++)
    {
        int Connector_num = rand()%3+1;
        ConnectorType connector_type = static_cast<ConnectorType>(Connector_num);
        Connector *newConnector = CreateConnector(connector_type);
        ArrayConnector.Add(newConnector);
    }

    Iterator<Connector*> *it = ArrayConnector.GetIterator();
    ConnectAll(it);
    delete it ;


    VectorClass<Connector*> VectorConnector;
    for(size_t i=0; i<N; i++)
    {
        int Connector_num = rand()%3+1;
        ConnectorType connector_type = static_cast<ConnectorType>(Connector_num);
        Connector *newConnector = CreateConnector(connector_type);
        VectorConnector.Add(newConnector);
    }

    Iterator<Connector*> *it2 = VectorConnector.GetIterator();
    ConnectAll(it2);
    delete it2 ;

    wcout << endl << L"Подключение всех (обычный цикл):" << endl;
    for(size_t i = 0; i < ArrayConnector.Size(); ++i)
    {
        Connector *c = ArrayConnector[i];
        c->Connect();
    }

    wcout << endl << L"Подключение всех через итератор:" << endl;
    Iterator<Connector*> *allIt = ArrayConnector.GetIterator();
    ConnectAll(allIt);
    delete allIt;

    wcout << endl << L"Подключение всех мощных (мощность >= 20):" << endl;
    Iterator<Connector*> *powerIt = new PowerFilterDecorator(ArrayConnector.GetIterator(), 20);
    ConnectAll(powerIt);
    delete powerIt;

    wcout << endl << L"Подключение только симметричных:" << endl;
    Iterator<Connector*> *revIt = new ReversibilityFilterDecorator(ArrayConnector.GetIterator(), true);
    ConnectAll(revIt);
    delete revIt;

    wcout << endl << L"Подключение только с количеством пинов >= 8:" << endl;
    Iterator<Connector*> *pinIt = new PinCountFilterDecorator(ArrayConnector.GetIterator(), 8);
    ConnectAll(pinIt);
    delete pinIt;

    wcout << endl << L"Подключение симметричных мощных разъёмов с >=8 пинами:" << endl;
    Iterator<Connector*> *combinedIt = new PinCountFilterDecorator(
                                       new ReversibilityFilterDecorator(
                                       new PowerFilterDecorator(ArrayConnector.GetIterator(), 20),true),8);
    ConnectAll(combinedIt);
    delete combinedIt;



    list<Connector*> connectorList;
    for(size_t i = 0; i < N; i++)
    {
        int connector_num = rand() % 3 + 1;
        ConnectorType connector_type = static_cast<ConnectorType>(connector_num);
        Connector *newConnector = CreateConnector(connector_type);
        connectorList.push_back(newConnector);
    }

    wcout << L"Размер списка разъёмов: " << connectorList.size() << endl;

    wcout << endl << L"Подключение из списка через адаптер + декораторы:" << endl;
    Iterator<Connector*> *adaptedIt = new ConstIteratorAdapter<list<Connector*>, Connector*>(&connectorList);
    Iterator<Connector*> *adaptedFiltered = new PinCountFilterDecorator(
                                            new ReversibilityFilterDecorator(
                                            new PowerFilterDecorator(adaptedIt, 20),true), 8);
    ConnectAll(adaptedFiltered);
    delete adaptedFiltered;
    return 0;
}
