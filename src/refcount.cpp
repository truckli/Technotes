class RCObject
{
    public:
    RCObject():refCount(0),shareable(true){}
    RCObject(constRCObject&):refCount(0),shareable(true){}
    　　RCObject& Operator=(constRCObject& rhs){return *this;}
    　　virtual ~RCObject()=0;
    　　void AddReference(){++refCount;}
    　　void RemoveReference(){if (--refCount == 0) delete this;}

    　　void markUnshareable(){shareable = false;}
    　　bool isShareable() const{return shareable;}
    　　bool isShared() const {return refCount > 1;}
    　PRivate:
    　　int refCount;
    　　bool shareable;
};
RCObject::~RCObject(){}

template <classT>
class RCPtr
{
    　public:
    　　RCPtr(T* realPtr = 0):pointee(realPtr){ init();}
    　　RCPtr(const RCPtr& rhs):pointee(rhs.pointee){init();}
    　　~RCPtr(){ if (pointee) pointee->RemoveReference();}
    　　RCPtr& operator = (const RCPtr& rhs)
    　　{
        　　　if (pointee!=rhs.pointee)
        　　　{
            　　　　if (pointee) 
            　　　　　pointee->RemoveReference();
            　　　　　pointee = rhs.pointee;
            　　　　　init();
        　　　}
        　　　return *this;
    　　}
    　　T* operator->() const { return pointee;}
    　　T& operator*() const { return *pointee;}
    　private:
    　　T* pointee;
    　　void init()
    　　{
        　　　if (pointee == 0) 
        　　　　return;
        　　　if (pointee->isShareable() == false) 
        　　　　pointee = new T(*pointee);
        　　　pointee->AddReference();
    　　}
};

class String
{
    　public:
    　　String(const char* value = ""): value(new StringValue(value)){}
    　　const char& operator[](intnIndex) const
    　　{
        　　　return value->data[nIndex]; 
    　　}
    　　char& operator[](intnIndex)
    　　{
        　　　if (value->isShared()) 
        　　　　value = new StringValue(value->data);
        　　　　value->markUnshareable();
        　　　　return value->data[nIndex];
    　　}
    　protected:
    　private:
    　　struct StringValue:public RCObject
    　　{
        　　　char* data;
        　　　String Value(const char* initValue)
        　　　{
            　　　　init(initValue); 
        　　　}
        　　　String Value(const StringValue& rhs)
        　　　{
            　　　　init(rhs.data);
        　　　}
        　　　void init(const char * initValue)
        　　　{
            　　　　data = new char[strlen(initValue) + 1];
            　　　　strcpy(data,initValue);
        　　　}
        　　　~String Value()
        　　　{
            　　　　delete [] data;
        　　　}
    　　};
    　　RCPtr<StringValue> value;
};



