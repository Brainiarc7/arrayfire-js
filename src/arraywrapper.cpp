#include "ext.h"
#include "arraywrapper.h"
#include "helpers.h"
#include "guard.h"
#include "worker.h"
#include "errors.h"

using namespace v8;
using namespace std;
using namespace node;

Persistent<Function> ArrayWrapper::constructor;

ArrayWrapper::ArrayWrapper(af::array* array) :
    array(array)
{
    assert(array);
}

ArrayWrapper::~ArrayWrapper()
{
    delete array;
}

void ArrayWrapper::Init(v8::Local<v8::Object> exports)
{
    auto tmpl = NanNew<FunctionTemplate>(New);
    tmpl->SetClassName(NanNew("AFArray"));

    int noOfMethods = 21;
    tmpl->InstanceTemplate()->SetInternalFieldCount(noOfMethods);
    NanSetPrototypeTemplate(tmpl, NanNew("elements"), NanNew<FunctionTemplate>(Elements), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("host"), NanNew<FunctionTemplate>(Host), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("copyToHost"), NanNew<FunctionTemplate>(Host), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("type"), NanNew<FunctionTemplate>(Type), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("dims"), NanNew<FunctionTemplate>(Dims), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("numdims"), NanNew<FunctionTemplate>(NumDims), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("numDims"), NanNew<FunctionTemplate>(NumDims), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bytes"), NanNew<FunctionTemplate>(Bytes), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("copy"), NanNew<FunctionTemplate>(Copy), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isempty"), NanNew<FunctionTemplate>(IsEmpty), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isEmpty"), NanNew<FunctionTemplate>(IsEmpty), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isscalar"), NanNew<FunctionTemplate>(IsScalar), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isScalar"), NanNew<FunctionTemplate>(IsScalar), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isvector"), NanNew<FunctionTemplate>(IsVector), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isVector"), NanNew<FunctionTemplate>(IsVector), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isrow"), NanNew<FunctionTemplate>(IsRow), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isRow"), NanNew<FunctionTemplate>(IsRow), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("iscolumn"), NanNew<FunctionTemplate>(IsColumn), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isColumn"), NanNew<FunctionTemplate>(IsColumn), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("iscomplex"), NanNew<FunctionTemplate>(IsComplex), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isComplex"), NanNew<FunctionTemplate>(IsComplex), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isreal"), NanNew<FunctionTemplate>(IsReal), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isReal"), NanNew<FunctionTemplate>(IsReal), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isdouble"), NanNew<FunctionTemplate>(IsDouble), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isDouble"), NanNew<FunctionTemplate>(IsDouble), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("issingle"), NanNew<FunctionTemplate>(IsSingle), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isSingle"), NanNew<FunctionTemplate>(IsSingle), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isrealfloating"), NanNew<FunctionTemplate>(IsRealFloating), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isRealFloating"), NanNew<FunctionTemplate>(IsRealFloating), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isfloating"), NanNew<FunctionTemplate>(IsFloating), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isFloating"), NanNew<FunctionTemplate>(IsFloating), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isinteger"), NanNew<FunctionTemplate>(IsInteger), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isInteger"), NanNew<FunctionTemplate>(IsInteger), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isbool"), NanNew<FunctionTemplate>(IsBool), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("isBool"), NanNew<FunctionTemplate>(IsBool), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("eval"), NanNew<FunctionTemplate>(Eval), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("at"), NanNew<FunctionTemplate>(At), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("row"), NanNew<FunctionTemplate>(Row), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("col"), NanNew<FunctionTemplate>(Col), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("slice"), NanNew<FunctionTemplate>(Slice), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rows"), NanNew<FunctionTemplate>(Rows), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("cols"), NanNew<FunctionTemplate>(Cols), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("slices"), NanNew<FunctionTemplate>(Slices), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("as"), NanNew<FunctionTemplate>(As), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("assign"), NanNew<FunctionTemplate>(Assign), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("set"), NanNew<FunctionTemplate>(Assign), v8::ReadOnly);

    NanSetPrototypeTemplate(tmpl, NanNew("add"), NanNew<FunctionTemplate>(Add), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("addAssign"), NanNew<FunctionTemplate>(AddAssign), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("sub"), NanNew<FunctionTemplate>(Sub), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("subAssign"), NanNew<FunctionTemplate>(SubAssign), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("mul"), NanNew<FunctionTemplate>(Mul), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("mulAssign"), NanNew<FunctionTemplate>(MulAssign), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("div"), NanNew<FunctionTemplate>(Div), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("divAssign"), NanNew<FunctionTemplate>(DivAssign), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitshiftl"), NanNew<FunctionTemplate>(BitShiftL), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitShiftL"), NanNew<FunctionTemplate>(BitShiftL), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitshiftr"), NanNew<FunctionTemplate>(BitShiftR), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitShiftR"), NanNew<FunctionTemplate>(BitShiftR), v8::ReadOnly);

    NanSetPrototypeTemplate(tmpl, NanNew("lt"), NanNew<FunctionTemplate>(Lt), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("gt"), NanNew<FunctionTemplate>(Gt), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("le"), NanNew<FunctionTemplate>(Le), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("ge"), NanNew<FunctionTemplate>(Ge), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("eq"), NanNew<FunctionTemplate>(Eq), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("neq"), NanNew<FunctionTemplate>(Neq), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("and"), NanNew<FunctionTemplate>(And), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("or"), NanNew<FunctionTemplate>(Or), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("neg"), NanNew<FunctionTemplate>(Neg), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("not"), NanNew<FunctionTemplate>(Not), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitAnd"), NanNew<FunctionTemplate>(BitAnd), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitOr"), NanNew<FunctionTemplate>(BitOr), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitXor"), NanNew<FunctionTemplate>(BitXor), v8::ReadOnly);

    auto f = tmpl->GetFunction();
    f->Set(NanNew("create"), NanNew<FunctionTemplate>(Create)->GetFunction());
    NanAssignPersistent<Function>(constructor, f);
    exports->Set(NanNew("AFArray"), f);
}

v8::Local<Object> ArrayWrapper::New(af::array* array)
{
    assert(array);
    Local<Value> args[] = { WrapPointer(array) };
    auto c = NanNew(constructor);
    auto inst = c->NewInstance(1, args);
    assert(ObjectWrap::Unwrap<ArrayWrapper>(inst)->array == array);
    return inst;
}

void ArrayWrapper::NewAsync(const v8::FunctionCallbackInfo<v8::Value>& args, const std::function<af::array*()>& arrayFactory)
{
    if (args.Length() >= 1 && args[args.Length() - 1]->IsFunction())
    {
        auto callback = new NanCallback(args[args.Length() - 1].As<Function>());
        auto worker = new Worker<af::array*>(callback, arrayFactory, [](Worker<af::array*>* w, af::array* a){ return ArrayWrapper::New(a); });
        NanAsyncQueueWorker(worker);
        NanReturnUndefined();
    }
    else
    {
        NAN_THROW("Last argument have to be a callback!");
    }
}

af::array* ArrayWrapper::GetArray(v8::Local<v8::Value> value)
{
    auto array = TryGetArray(value);
    if (array) return array;
    FIRE_THROW("Argument is not an AFArray instance.");
}

af::array* ArrayWrapper::GetArrayAt(const v8::FunctionCallbackInfo<v8::Value>& args, int index)
{
    auto array = TryGetArrayAt(args, index);
    if (array) return array;
    stringstream ss;
    ss << "Argument at position " << to_string(index) << ". is not an AFArray instance.";
    FIRE_THROW(ss.str().c_str());
}

af::array* ArrayWrapper::TryGetArray(v8::Local<v8::Value> value)
{
    try
    {
        if (value->IsObject())
        {
            auto wrapper = ObjectWrap::Unwrap<ArrayWrapper>(value.As<Object>());
            if (wrapper) return wrapper->array;
        }
    }
    catch (...)
    {
    }
    return nullptr;
}

af::array* ArrayWrapper::TryGetArrayAt(const v8::FunctionCallbackInfo<v8::Value>& args, int index)
{
    if (index < args.Length())
    {
        return GetArray(args[index]);
    }
    return nullptr;
}

void ArrayWrapper::New(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    NanScope();

    try
    {
        ArrayWrapper* instance = nullptr;
        int buffIdx = -1;
        try
        {
            if (args.Length() == 0)
            {
                instance = new ArrayWrapper(new af::array());
            }
            else if (args.Length() == 1)
            {
                if (Buffer::HasInstance(args[0]))
                {
                    instance = new ArrayWrapper(reinterpret_cast<af::array*>(Buffer::Data(args[0])));
                }
            }
            else
            {
                for (int i = 0; i < args.Length(); i++)
                {
                    if (Buffer::HasInstance(args[i]))
                    {
                        buffIdx = i;
                        break;
                    }
                }

                if (buffIdx == -1)
                {
                    // Creating new
                    auto dimAndType = ParseDimAndTypeArgs(args);
                    instance = new ArrayWrapper(new af::array(dimAndType.first, dimAndType.second));
                }
            }
        }
        catch (...)
        {
            delete instance;
            throw;
        }

        if (!instance)
        {
            return NAN_THROW("Invalid arguments.");
        }

        instance->Wrap(args.Holder());

        //Set instance props: NanObjectWrapHandle(obj)->Set(NanNew(symbol), value);

        NanReturnValue(args.Holder());
    }
    FIRE_CATCH
}

template<typename T>
af::array* ArrayWrapper::CreateArray(void* ptr, af_source src, const af::dim4& dim4)
{
    Guard();
    return new af::array(dim4, (T*)ptr, src);
}

NAN_METHOD(ArrayWrapper::Create)
{
    NanScope();

    try
    {
        int buffIdx = -1;
        function<af::array*()> factory;

        for (int i = 0; i < args.Length(); i++)
        {
            if (Buffer::HasInstance(args[i]))
            {
                buffIdx = i;
                break;
            }
        }

        if (buffIdx == -1)
        {
            return NAN_THROW("Buffer argument expected.");
        }
        else if (buffIdx + 1 < args.Length())
        {
            // Copy / wrap ptr
            // args: dim0..dimn, dtype, ptr[, source]
            af_source src = afHost;
            if (buffIdx + 1 < args.Length() && args[buffIdx + 1]->IsNumber())
            {
                src = (af_source)(args[buffIdx + 2]->Int32Value());
            }
            auto buffObj = args[buffIdx]->ToObject();
            char* ptr = Buffer::Data(buffObj);
            auto dimAndType = ParseDimAndTypeArgs(args, buffIdx);
            switch (dimAndType.second)
            {
                case f32:
                    factory = [=]() { return CreateArray<float>(ptr, src, dimAndType.first); };
                    break;
                case f64:
                    factory = [=]() { return CreateArray<double>(ptr, src, dimAndType.first); };
                    break;
                case s32:
                    factory = [=]() { return CreateArray<int>(ptr, src, dimAndType.first); };
                    break;
                case u32:
                    factory = [=]() { return CreateArray<unsigned>(ptr, src, dimAndType.first); };
                    break;
                case u8:
                    factory = [=]() { return CreateArray<unsigned char>(ptr, src, dimAndType.first ); };
                    break;
                case c32:
                    factory = [=]() { return CreateArray<af::cfloat>(ptr, src, dimAndType.first); };
                    break;
                case c64:
                    factory = [=]() { return CreateArray<af::cdouble>(ptr, src, dimAndType.first); };
                    break;
                case b8:
                    factory = [=]() { return CreateArray<char>(ptr, src, dimAndType.first); };
                    break;
                case s64:
                    factory = [=]() { return CreateArray<int64_t>(ptr, src, dimAndType.first); };
                    break;
                case u64:
                    factory = [=]() { return CreateArray<uint64_t>(ptr, src, dimAndType.first); };
                    break;
            }
        }

        if (!factory)
        {
            return NAN_THROW_INVALID_ARGS();
        }

        NanCallback *callback = nullptr;
        if (args[args.Length() - 1]->IsFunction())
        {
            callback = new NanCallback(args[args.Length() - 1].As<Function>());
        }

        if (!callback)
        {
            return NAN_THROW_CB_EXPECTED();
        }

        auto conv = [](Worker<af::array*>* w, af::array* a)
        {
            return New(a);
        };
        auto worker = new Worker<af::array*>(callback, move(factory), move(conv));
        worker->SaveToPersistent("data", args[buffIdx]->ToObject());

        NanAsyncQueueWorker(worker);
        NanReturnUndefined();
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Elements)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(NanNew<Number>(GetArray(args.This())->elements()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Host)
{
    NanScope();

    try
    {
        ARGS_LEN(1)

        char* buffData;
        auto pArray = GetArray(args.This());

        if (Buffer::HasInstance(args[0]))
        {
            buffData = Buffer::Data(args[0]);

            if (Buffer::Length(args[0]) < pArray->bytes())
            {
                return NAN_THROW("Buffer is too small to hold values.");
            }

            NanCallback *callback = nullptr;
            if (args.Length() > 1 && args[1]->IsFunction())
            {
                callback = new NanCallback(args[1].As<Function>());
            }
            if (!callback)
            {
                return NAN_THROW_CB_EXPECTED();
            }

            af::array array(*pArray);
            auto exec = [=]()
            {
                Guard();
                array.host(buffData);
            };
            auto worker = new Worker<void>(callback, move(exec));
            worker->SaveToPersistent("data", args[0]->ToObject());

            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        else
        {
            NanCallback *callback = nullptr;
            if (args[0]->IsFunction())
            {
                callback = new NanCallback(args[0].As<Function>());
            }
            if (!callback)
            {
                return NAN_THROW_CB_EXPECTED();
            }

            size_t size = pArray->elements() * GetDTypeInfo(pArray->type()).second;
            buffData = new char[size];
            try
            {
                af::array array(*pArray);
                auto exec = [=]()
                {
                    Guard();
                    array.host(buffData);
                    return buffData;
                };
                auto conv = [=](Worker<char*>* w, char* data)
                {
                    return NanNewBufferHandle(data, size, [](char* data, void* hint) { delete[] data; }, nullptr);
                };
                auto worker = new Worker<char*>(callback, move(exec), move(conv));
                NanAsyncQueueWorker(worker);
                NanReturnUndefined();
            }
            catch (...)
            {
                delete[] buffData;
                throw;
            }
        }
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Type)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(GetArray(args.This())->type());
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Dims)
{
    NanScope();

    try
    {
        Guard();

        auto pArray = GetArray(args.This());
        if (!args.Length())
        {
            auto dims = pArray->dims();
            auto jsDims = NanNew<Object>();
            jsDims->Set(NanNew("elements"), NanNew<Number>(dims.elements()));
            jsDims->Set(NanNew("ndims"), NanNew<Number>(dims.ndims()));
            jsDims->Set(NanNew("dim0"), NanNew<Number>(dims[0]));
            jsDims->Set(NanNew("dim1"), NanNew<Number>(dims[1]));
            jsDims->Set(NanNew("dim2"), NanNew<Number>(dims[2]));
            jsDims->Set(NanNew("dim3"), NanNew<Number>(dims[3]));
            auto pDims = NanNew<Array>(4);
            pDims->Set(0, NanNew<Number>(dims[0]));
            pDims->Set(1, NanNew<Number>(dims[1]));
            pDims->Set(2, NanNew<Number>(dims[2]));
            pDims->Set(3, NanNew<Number>(dims[3]));
            jsDims->Set(NanNew("dims"), pDims);

            NanReturnValue(jsDims);
        }
        else
        {
            NanReturnValue(NanNew<Number>(pArray->dims(args[0]->Uint32Value())));
        }
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::NumDims)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->numdims()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Bytes)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(NanNew<Number>((unsigned)GetArray(args.This())->bytes()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Copy)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(new af::array(move(GetArray(args.This())->copy()))));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsEmpty)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isempty()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsScalar)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isscalar()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsVector)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isvector()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsRow)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isrow()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsColumn)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->iscolumn()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsComplex)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->iscomplex()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsReal)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isreal()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsDouble)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isdouble()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsSingle)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->issingle()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsRealFloating)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isrealfloating()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsFloating)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isfloating()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsInteger)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->isinteger()));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsBool)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->type() == b8));
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Eval)
{
    NanScope();

    try
    {
        Guard();
        GetArray(args.This())->eval();
        NanReturnUndefined();
    }
    FIRE_CATCH
}

NAN_METHOD(ArrayWrapper::At)
{
    // Aka "indexing"
    NanScope();

    try
    {
        Guard();

        ARGS_LEN(1)

        if (args.Length() < 2)
        {
            NanReturnValue(New(new af::array(GetArray(args.This())->operator()(ToIndex(args[0])))));
        }
        else if (args.Length() < 3)
        {
            NanReturnValue(New(new af::array(GetArray(args.This())->operator()(ToIndex(args[0]), ToIndex(args[1])))));
        }
        else if (args.Length() < 4)
        {
            NanReturnValue(New(new af::array(GetArray(args.This())->operator()(ToIndex(args[0]), ToIndex(args[1]), ToIndex(args[2])))));
        }
        else
        {
            NanReturnValue(New(new af::array(GetArray(args.This())->operator()(ToIndex(args[0]), ToIndex(args[1]), ToIndex(args[2]), ToIndex(args[2])))));
        }
    }
    FIRE_CATCH
}

#define AFARRAY_IMPL_IDX1(F, f)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    try\
    {\
        ARGS_LEN(1)\
        NanReturnValue(New(new af::array(GetArray(args.This())->f(args[0]->Int32Value()))));\
    }\
    FIRE_CATCH\
}

AFARRAY_IMPL_IDX1(Row, row)
AFARRAY_IMPL_IDX1(Col, col)
AFARRAY_IMPL_IDX1(Slice, slice)

#define AFARRAY_IMPL_IDX2(F, f)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    try\
    {\
        ARGS_LEN(2);\
        NanReturnValue(New(new af::array(GetArray(args.This())->f(args[0]->Int32Value(), args[1]->Int32Value()))));\
    }\
    FIRE_CATCH\
}

AFARRAY_IMPL_IDX2(Rows, rows)
AFARRAY_IMPL_IDX2(Cols, cols)
AFARRAY_IMPL_IDX2(Slices, slices)

NAN_METHOD(ArrayWrapper::As)
{
    NanScope();
    try
    {
        ARGS_LEN(1)
        af::dtype type = GetDTypeInfo(args[0]->Uint32Value()).first;
        NanReturnValue(New(new af::array(GetArray(args.This())->as(type))));
    }
    FIRE_CATCH
}

#define AFARRAY_IMPL_ASSIGN(F, Op)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    \
    try\
    {\
        auto& array = *GetArray(args.This());\
        ARGS_LEN(1)\
        auto value = args[0];\
        auto pOtherArray = TryGetArray(value);\
        if (pOtherArray)\
        {\
            auto& otherArray = *pOtherArray;\
            Guard();\
            array Op otherArray;\
        }\
        else if (value->IsNumber())\
        {\
            double v = value->NumberValue();\
            if (floor(v) == v)\
            {\
                Guard();\
                array Op value->Int32Value();\
            }\
            else if (af::isDoubleAvailable(af::getDevice()))\
            {\
                Guard();\
                array Op v;\
            }\
            else\
            {\
                Guard();\
                array Op (float)v;\
            }\
        }\
        else if (value->IsObject())\
        {\
            if (af::isDoubleAvailable(af::getDevice()))\
            {\
                auto v = ToDComplex(value);\
                Guard();\
                array Op v;\
            }\
            else\
            {\
                auto v = ToFComplex(value);\
                Guard();\
                array Op v;\
            }\
        }\
        else if (value->IsString())\
        {\
            String::Utf8Value str(value);\
            intl v = strtoll(*str, nullptr, 10);\
            Guard();\
            array Op v;\
        }\
        else\
        {\
            return NAN_THROW_INVALID_ARGS();\
        }\
        \
        NanReturnValue(args.This());\
    }\
    FIRE_CATCH\
}

AFARRAY_IMPL_ASSIGN(Assign, =)
AFARRAY_IMPL_ASSIGN(AddAssign, +=)
AFARRAY_IMPL_ASSIGN(SubAssign, -=)
AFARRAY_IMPL_ASSIGN(MulAssign, *=)
AFARRAY_IMPL_ASSIGN(DivAssign, /=)

#define AFARRAY_IMPL_BINOP(F, Op)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    \
    try\
    {\
        auto& array = *GetArray(args.This());\
        ARGS_LEN(1)\
        auto value = args[0];\
        auto pOtherArray = TryGetArray(value);\
        af::array* result = nullptr;\
        if (pOtherArray)\
        {\
            auto& otherArray = *pOtherArray;\
            Guard();\
            result = new af::array(array Op otherArray);\
        }\
        else if (value->IsNumber())\
        {\
            double v = value->NumberValue();\
            if (floor(v) == v)\
            {\
                Guard();\
                result = new af::array(array Op value->Int32Value());\
            }\
            else if (af::isDoubleAvailable(af::getDevice()))\
            {\
                Guard();\
                result = new af::array(array Op v);\
            }\
            else\
            {\
                Guard();\
                result = new af::array(array Op (float)v);\
            }\
        }\
        else if (value->IsObject())\
        {\
            if (af::isDoubleAvailable(af::getDevice()))\
            {\
                auto v = ToDComplex(value);\
                Guard();\
                result = new af::array(array Op v);\
            }\
            else\
            {\
                auto v = ToFComplex(value);\
                Guard();\
                result = new af::array(array Op v);\
            }\
        }\
        else if (value->IsString())\
        {\
            String::Utf8Value str(value);\
            intl v = strtoll(*str, nullptr, 10);\
            Guard();\
            result = new af::array(array Op v);\
        }\
        else\
        {\
            return NAN_THROW_INVALID_ARGS();\
        }\
        \
        NanReturnValue(New(result));\
    }\
    FIRE_CATCH\
}

AFARRAY_IMPL_BINOP(Add, +)
AFARRAY_IMPL_BINOP(Sub, -)
AFARRAY_IMPL_BINOP(Mul, *)
AFARRAY_IMPL_BINOP(Div, /)
AFARRAY_IMPL_BINOP(BitShiftL, <<)
AFARRAY_IMPL_BINOP(BitShiftR, >>)

AFARRAY_IMPL_BINOP(Lt, <)
AFARRAY_IMPL_BINOP(Gt, >)
AFARRAY_IMPL_BINOP(Le, <=)
AFARRAY_IMPL_BINOP(Ge, >=)
AFARRAY_IMPL_BINOP(Eq, ==)
AFARRAY_IMPL_BINOP(Neq, !=)
AFARRAY_IMPL_BINOP(And, &&)
AFARRAY_IMPL_BINOP(Or, ||)
AFARRAY_IMPL_BINOP(BitAnd, &)
AFARRAY_IMPL_BINOP(BitOr, |)
AFARRAY_IMPL_BINOP(BitXor, ^)

#define AFARRAY_IMPL_UNOP(F, Op)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    \
    try\
    {\
        auto& array = *GetArray(args.This());\
        NanReturnValue(New(new af::array(array.operator Op())));\
    }\
    FIRE_CATCH\
}

AFARRAY_IMPL_UNOP(Neg, -)
AFARRAY_IMPL_UNOP(Not, !)
