#include "ext.h"
#include "device.h"
#include "guard.h"
#include "worker.h"
#include "helpers.h"

using namespace v8;
using namespace std;
using namespace node;

NAN_METHOD(GetDeviceCount)
{
    Guard();
    NanScope();
#ifdef CPU
    NanReturnValue(NanNew<Number>(1));
#else
    NanReturnValue(NanNew<Number>(af::getDeviceCount()));
#endif
}

NAN_METHOD(GetDevice)
{
    Guard();
    NanScope();

#ifdef CPU
    NanReturnValue(NanNew<Number>(0));
#else
    NanReturnValue(NanNew<Number>(af::getDevice()));
#endif
}

NAN_METHOD(SetDevice)
{
    Guard();
    NanScope();

#ifndef CPU
    af::setDevice(args[0]->Uint32Value());
#endif

    NanReturnUndefined();
}

NAN_METHOD(GetDeviceInfo)
{
    Guard();
    NanScope();

    bool IsDoubleAvailable = af::isDoubleAvailable(af::getDevice());
    auto info = NanNew<Object>();

#ifdef CPU
    info->Set(NanNew<String>("name"), NanNew<String>("CPU"));
    info->Set(NanNew<String>("platform"), NanNew<String>("CPU"));
    info->Set(NanNew<String>("toolkit"), NanNew<String>("CPU"));
    info->Set(NanNew<String>("compute"), NanNew<String>("CPU"));
    info->Set(NanNew<String>("isDoubleAvailable"), NanNew<Boolean>(IsDoubleAvailable));
#else
    char name[256], platform[256], toolkit[256], compute[256];
    af::deviceprop(name, platform, toolkit, compute);
    info->Set(NanNew<String>("name"), NanNew<String>(name));
    info->Set(NanNew<String>("platform"), NanNew<String>(platform));
    info->Set(NanNew<String>("toolkit"), NanNew<String>(toolkit));
    info->Set(NanNew<String>("compute"), NanNew<String>(compute));
    info->Set(NanNew<String>("isDoubleAvailable"), NanNew<Boolean>(IsDoubleAvailable));
#endif

    NanReturnValue(info);
}

NAN_METHOD(IsDoubleAvailable)
{
    Guard();
    NanScope();

    NanReturnValue(NanNew<Number>(af::isDoubleAvailable(args[0]->Uint32Value())));
}

NAN_METHOD(Sync)
{
    NanScope();

    int device = -1;
    NanCallback *callback = nullptr;

    if (args.Length() > 0)
    {
        int idx = 0;
        if (args[idx]->IsNumber())
        {
            device = args[idx++]->Int32Value();
#ifdef CPU
            if (device > 1 || device < -1)
            {
                return NanThrowRangeError("Device is out of range.");
            }
#else
            if (device >= af::getDeviceCount() || device < -1)
            {
                return NanThrowRangeError("Device is out of range.");
            }
#endif
        }
        if (idx < args.Length() && args[idx]->IsFunction())
        {
            callback = new NanCallback(args[idx].As<Function>());
        }
    }

    auto exec = [=]()
    {
        Guard();
        af::sync(device);
    };

    NanAsyncQueueWorker(new Worker<void>(callback, exec));
    NanReturnUndefined();
}

pair<af::dtype, unsigned> getAllocPars(unsigned elements, unsigned udtype)
{
    auto dtypeInfo = ConvDtype(udtype);
    unsigned sizeOf = dtypeInfo.second;
    af::dtype dtype = dtypeInfo.first;
    unsigned size = sizeOf * elements;
    return move(make_pair(dtype, size));
}

NAN_METHOD(Alloc)
{
    NanScope();

    try
    {
        unsigned elements = args[0]->Uint32Value();
        unsigned udtype = args[1]->Uint32Value();
        auto allocPars = getAllocPars(elements, udtype);

        Guard();
        char* ptr = (char*)af::alloc(elements, allocPars.first);
        auto gcCallback = [](char* data, void* hint)
        {
            af::free(data);
        };

        NanReturnValue(NanNewBufferHandle(ptr, 0, gcCallback, nullptr)); // void*
    }
    catch (exception& ex)
    {
        NanThrowError(ex.what());
    }
}

NAN_METHOD(Pinned)
{
    NanScope();

    try
    {
        unsigned elements = args[0]->Uint32Value();
        unsigned udtype = args[1]->Uint32Value();
        auto allocPars = getAllocPars(elements, udtype);

        Guard();
        char* ptr = (char*)af::pinned(elements, allocPars.first);
        auto gcCallback = [](char* data, void* hint)
        {
            af::freePinned(data);
        };

        NanReturnValue(NanNewBufferHandle(ptr, allocPars.second, gcCallback, nullptr));
    }
    catch (exception& ex)
    {
        NanThrowError(ex.what());
    }
}

void InitDevice(v8::Handle<v8::Object> exports)
{
    exports->Set(NanNew<String>("getDeviceCount"), NanNew<FunctionTemplate>(GetDeviceCount)->GetFunction());
    exports->Set(NanNew<String>("getDevice"), NanNew<FunctionTemplate>(GetDevice)->GetFunction());
    exports->Set(NanNew<String>("setDevice"), NanNew<FunctionTemplate>(SetDevice)->GetFunction());
    exports->Set(NanNew<String>("getDeviceInfo"), NanNew<FunctionTemplate>(GetDeviceInfo)->GetFunction());
    exports->Set(NanNew<String>("isDoubleAvailable"), NanNew<FunctionTemplate>(IsDoubleAvailable)->GetFunction());
    exports->Set(NanNew<String>("sync"), NanNew<FunctionTemplate>(Sync)->GetFunction());
    exports->Set(NanNew<String>("alloc"), NanNew<FunctionTemplate>(Alloc)->GetFunction());
    exports->Set(NanNew<String>("pinned"), NanNew<FunctionTemplate>(Pinned)->GetFunction());
}