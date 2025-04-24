#ifndef PATTERNTEMPLATES
#define PATTERNTEMPLATES

#include <vector>
#include <iostream>

using namespace std;

const size_t MaxSize = 100;

template<typename T>
class Iterator;

template<typename T>
class VectorIterator;

template<typename T>
class IContainer
{
public:
    virtual ~IContainer() {}
    virtual Iterator<T>* GetIterator() const = 0;
};

template<typename T>
class Iterator
{
protected:
    Iterator() {}

public:
    virtual ~Iterator() {}
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
};

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:
    const T* Items;
    size_t Pos;
    size_t Size;
public:
    ArrayIterator(const T* items, size_t size)
        : Items(items), Size(size), Pos(0) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= Size); }
    T GetCurrent() const override { return Items[Pos]; }
};

template <typename T>
class ArrayClass : public IContainer<T>
{
private:

    T Items[MaxSize];
    size_t ArraySize;

public:

    void Add(T newObject) { Items[ArraySize++] = newObject; }
    size_t Size() const { return ArraySize; }

    T operator[](size_t index) const { return Items[index]; }

    T GetElement(size_t index) const { return Items[index]; }

    Iterator<T>* GetIterator() const override
    {
        return new ArrayIterator<T>(Items, ArraySize);
    }
    ArrayClass() : ArraySize(0) {}
};


template <typename T>
class VectorClass : public IContainer<T>
{
private:
    vector<T> Items;

public:
    T GetElement(size_t index) const { return Items[index]; }

    void Add(T newObject)
    {
        Items.push_back(newObject);
    }

    size_t Size() const { return Items.size(); }

    Iterator<T>* GetIterator() const override
    {
        return new VectorIterator<T>(this);
    }

};

template <typename T>
class VectorIterator : public Iterator<T>
{
private:
    const VectorClass<T>* VectorContainer;
    size_t Pos;

public:
    VectorIterator(const VectorClass<T>* container)
        : VectorContainer(container), Pos(0) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= VectorContainer->Size()); }
    T GetCurrent() const override { return VectorContainer->GetElement(Pos); }
};

template<typename T>
class IteratorDecorator : public Iterator<T>
{
protected:
    Iterator<T> *It;

public:
    IteratorDecorator(Iterator<T> *it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};

template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType *Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType *container)
    : Container(container)
    {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}
    virtual void First() { It = Container->begin(); }
    virtual void Next() { It++; }
    virtual bool IsDone() const { return (It == Container->end()); }
    virtual ItemType GetCurrent() const { return *It; }
};

#endif // PATTERNTEMPLATES
