/*
Copyright (c) 2014-2015, ArrayFire
Copyright (c) 2015 Gábor Mező aka unbornchikken (gabor.mezo@outlook.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the ArrayFire nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ext.h"
#include "arraywrapper.h"
#include "helpers.h"
#include "guard.h"
#include "worker.h"
#include "errors.h"
#include "symbols.h"

using namespace v8;
using namespace std;
using namespace node;

Persistent<Function> ArrayWrapper::constructor;

ArrayWrapper::ArrayWrapper(af::array* array) :
    _array(array)
{
    assert(array);
}

ArrayWrapper::~ArrayWrapper()
{
    delete _array;
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
    NanSetPrototypeTemplate(tmpl, NanNew("scalar"), NanNew<FunctionTemplate>(Scalar), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("value"), NanNew<FunctionTemplate>(Scalar), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("write"), NanNew<FunctionTemplate>(Write), v8::ReadOnly);
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
    NanSetPrototypeTemplate(tmpl, NanNew("bitAnd"), NanNew<FunctionTemplate>(BitAnd), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitOr"), NanNew<FunctionTemplate>(BitOr), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("bitXor"), NanNew<FunctionTemplate>(BitXor), v8::ReadOnly);

    NanSetPrototypeTemplate(tmpl, NanNew("rhsAdd"), NanNew<FunctionTemplate>(RhsAdd), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsSub"), NanNew<FunctionTemplate>(RhsSub), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsMul"), NanNew<FunctionTemplate>(RhsMul), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsDiv"), NanNew<FunctionTemplate>(RhsDiv), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsBitshiftl"), NanNew<FunctionTemplate>(RhsBitShiftL), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsBitShiftL"), NanNew<FunctionTemplate>(RhsBitShiftL), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsBitshiftr"), NanNew<FunctionTemplate>(RhsBitShiftR), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsBitShiftR"), NanNew<FunctionTemplate>(RhsBitShiftR), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsLt"), NanNew<FunctionTemplate>(RhsLt), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsGt"), NanNew<FunctionTemplate>(RhsGt), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsLe"), NanNew<FunctionTemplate>(RhsLe), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsGe"), NanNew<FunctionTemplate>(RhsGe), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsEq"), NanNew<FunctionTemplate>(RhsEq), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsNeq"), NanNew<FunctionTemplate>(RhsNeq), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsAnd"), NanNew<FunctionTemplate>(RhsAnd), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsOr"), NanNew<FunctionTemplate>(RhsOr), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsBitAnd"), NanNew<FunctionTemplate>(RhsBitAnd), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsBitOr"), NanNew<FunctionTemplate>(RhsBitOr), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("rhsBitXor"), NanNew<FunctionTemplate>(RhsBitXor), v8::ReadOnly);

    NanSetPrototypeTemplate(tmpl, NanNew("neg"), NanNew<FunctionTemplate>(Neg), v8::ReadOnly);
    NanSetPrototypeTemplate(tmpl, NanNew("not"), NanNew<FunctionTemplate>(Not), v8::ReadOnly);

    auto f = tmpl->GetFunction();
    f->Set(NanNew("create"), NanNew<FunctionTemplate>(Create)->GetFunction());
    NanAssignPersistent<Function>(constructor, f);
    exports->Set(NanNew("AFArray"), f);
}

v8::Local<Object> ArrayWrapper::New(const af::array& array)
{
    return New(new af::array(array));
}

v8::Local<v8::Object> ArrayWrapper::New(af::array* array)
{
    assert(array);
    Local<Value> args[] = { WrapPointer(array) };
    auto c = NanNew(constructor);
    auto inst = c->NewInstance(1, args);
    assert(ObjectWrap::Unwrap<ArrayWrapper>(inst)->_array == array);
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
    ARRAYFIRE_THROW("Argument is not an AFArray instance.");
}

af::array* ArrayWrapper::TryGetArray(v8::Local<v8::Value> value)
{
    try
    {
        if (value->IsObject())
        {
            auto obj = value.As<Object>();
            if (obj->GetConstructorName()->Equals(NanNew(Symbols::AFArrayClass)))
            {
                auto wrapper = ObjectWrap::Unwrap<ArrayWrapper>(value.As<Object>());
                return wrapper->_array;
            }
        }
    }
    catch (...)
    {
    }
    return nullptr;
}

af::array* ArrayWrapper::GetArray(v8::Local<v8::Object> value)
{
    auto array = TryGetArray(value);
    if (array) return array;
    ARRAYFIRE_THROW("Argument is not an AFArray instance.");
}

af::array* ArrayWrapper::TryGetArray(v8::Local<v8::Object> value)
{
    try
    {
        auto wrapper = ObjectWrap::Unwrap<ArrayWrapper>(value.As<Object>());
        if (wrapper) return wrapper->_array;
    }
    catch (...)
    {
    }
    return nullptr;
}

af::array* ArrayWrapper::GetArrayAt(const v8::FunctionCallbackInfo<v8::Value>& args, int index)
{
    auto array = TryGetArrayAt(args, index);
    if (array) return array;
    stringstream ss;
    ss << "Argument at position " << to_string(index) << ". is not an AFArray instance.";
    ARRAYFIRE_THROW(ss.str().c_str());
}

af::array* ArrayWrapper::TryGetArrayAt(const v8::FunctionCallbackInfo<v8::Value>& args, int index)
{
    if (index < args.Length())
    {
        return GetArray(args[index]);
    }
    return nullptr;
}

void ArrayWrapper::New(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    NanScope();

    try
    {
        ArrayWrapper* instance = nullptr;
        try
        {
            if (args.Length() == 0)
            {
                Guard();
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
                Guard();
                auto dimAndType = ParseDimAndTypeArgs(args);
                instance = new ArrayWrapper(new af::array(dimAndType.first, dimAndType.second));
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
        NanReturnValue(args.Holder());
    }
    ARRAYFIRE_CATCH
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
                default:
                    assert(false);
            }
        }

        if (!factory)
        {
            return NAN_THROW_INVALID_ARGS();
        }

        auto conv = [](Worker<af::array*>* w, af::array* a)
        {
            return New(a);
        };
        auto worker = new Worker<af::array*>(GetCallback(args), move(factory), move(conv));
        worker->SaveToPersistent("data", args[buffIdx]->ToObject());

        NanAsyncQueueWorker(worker);
        NanReturnUndefined();
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Elements)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(NanNew<Number>(GetArray(args.This())->elements()));
    }
    ARRAYFIRE_CATCH
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

            af::array array(*pArray);
            auto exec = [=]()
            {
                Guard();
                array.host(buffData);
            };
            auto worker = new Worker<void>(GetCallback(args), move(exec));
            worker->SaveToPersistent("data", args[0]->ToObject());

            NanAsyncQueueWorker(worker);
            NanReturnUndefined();
        }
        else
        {
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
                auto worker = new Worker<char*>(GetCallback(args), move(exec), move(conv));
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
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Scalar)
{
    NanScope();

    try
    {
        ARGS_LEN(1)

        auto pArray = GetArray(args.This());
        af::array array(*pArray);
        switch (array.type())
        {
            case f32:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<float>();
                    };
                    auto worker = new Worker<float>(GetCallback(args), move(exec));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case f64:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<double>();
                    };
                    auto worker = new Worker<double>(GetCallback(args), move(exec));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case s32:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<int>();
                    };
                    auto worker = new Worker<int>(GetCallback(args), move(exec));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case u32:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<unsigned>();
                    };
                    auto worker = new Worker<unsigned>(GetCallback(args), move(exec));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case u8:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<unsigned char>();
                    };
                    auto worker = new Worker<unsigned char>(GetCallback(args), move(exec));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case b8:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<char>();
                    };
                    auto worker = new Worker<char>(GetCallback(args), move(exec));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case c32:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<af::cfloat>();
                    };
                    auto conv = [=](Worker<af::cfloat>* w, af::cfloat data)
                    {
                        return ToV8Complex(data);
                    };
                    auto worker = new Worker<af::cfloat>(GetCallback(args), move(exec), move(conv));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case c64:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<af::cdouble>();
                    };
                    auto conv = [=](Worker<af::cdouble>* w, af::cdouble data)
                    {
                        return ToV8Complex(data);
                    };
                    auto worker = new Worker<af::cdouble>(GetCallback(args), move(exec), move(conv));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case s64:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<long long>();
                    };
                    auto conv = [=](Worker<long long>* w, long long data)
                    {
                        return NanNew(to_string(data).c_str());
                    };
                    auto worker = new Worker<long long>(GetCallback(args), move(exec), move(conv));
                    NanAsyncQueueWorker(worker);
                }
                break;
            case u64:
                {
                    auto exec = [=]()
                    {
                        Guard();
                        return array.scalar<unsigned long long>();
                    };
                    auto conv = [=](Worker<unsigned long long>* w, unsigned long long data)
                    {
                        return NanNew(to_string(data).c_str());
                    };
                    auto worker = new Worker<unsigned long long>(GetCallback(args), move(exec), move(conv));
                    NanAsyncQueueWorker(worker);
                }
                break;
            default:
                assert(false);
        }
        NanReturnUndefined();
    }
    ARRAYFIRE_CATCH
}


NAN_METHOD(ArrayWrapper::Write)
{
    NanScope();

    try
    {
        ARGS_LEN(3)

        char* buffData;
        auto pArray = GetArray(args.This());

        if (Buffer::HasInstance(args[0]))
        {
            buffData = Buffer::Data(args[0]);
        }
        else
        {
            return NAN_THROW("First argument is no a Buffer.");
        }

        unsigned bytes = args[1]->Uint32Value();
        af_source src = afHost;
        if (args.Length() > 3)
        {
            src = (af_source)(args[2]->Int32Value());
        }

        af::array array(*pArray);
        auto exec = [=]()
        {
            Guard();
            af_write_array(array.get(), buffData, bytes, src);
        };
        auto worker = new Worker<void>(GetCallback(args), move(exec));
        worker->SaveToPersistent("data", args[0]->ToObject());

        NanAsyncQueueWorker(worker);
        NanReturnUndefined();
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Type)
{
    NanScope();

    try
    {
        NanReturnValue(GetArray(args.This())->type());
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Dims)
{
    NanScope();

    try
    {
        auto pArray = GetArray(args.This());
        if (!args.Length())
        {
            auto dims = pArray->dims();
            auto jsDims = NanNew<Object>();
            jsDims->Set(NanNew(Symbols::Elements), NanNew<Number>(dims.elements()));
            jsDims->Set(NanNew(Symbols::Ndims), NanNew<Number>(dims.ndims()));
            jsDims->Set(NanNew(Symbols::NDims), NanNew<Number>(dims.ndims()));
            auto pDims = NanNew<Array>(4);
            pDims->Set(0, NanNew<Number>(dims[0]));
            pDims->Set(1, NanNew<Number>(dims[1]));
            pDims->Set(2, NanNew<Number>(dims[2]));
            pDims->Set(3, NanNew<Number>(dims[3]));
            jsDims->Set(NanNew(Symbols::Values), pDims);

            NanReturnValue(jsDims);
        }
        else
        {
            NanReturnValue(NanNew<Number>(pArray->dims(args[0]->Uint32Value())));
        }
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::NumDims)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew<Number>(GetArray(args.This())->numdims()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Bytes)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew<Number>((unsigned)GetArray(args.This())->bytes()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::Copy)
{
    NanScope();

    try
    {
        Guard();
        NanReturnValue(New(GetArray(args.This())->copy()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsEmpty)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isempty()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsScalar)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isscalar()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsVector)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isvector()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsRow)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isrow()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsColumn)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->iscolumn()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsComplex)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->iscomplex()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsReal)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isreal()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsDouble)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isdouble()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsSingle)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->issingle()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsRealFloating)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isrealfloating()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsFloating)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isfloating()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsInteger)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->isinteger()));
    }
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::IsBool)
{
    NanScope();

    try
    {
        NanReturnValue(NanNew(GetArray(args.This())->type() == b8));
    }
    ARRAYFIRE_CATCH
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
    ARRAYFIRE_CATCH
}

NAN_METHOD(ArrayWrapper::At)
{
    // Aka "indexing"
    NanScope();

    try
    {
        ARGS_LEN(1)

        Guard();

        if (args.Length() == 1)
        {
            auto ri = ToRegionIndex(args[0]);
            switch (get<0>(ri))
            {
                case Region::Row:
                    NanReturnValue(New(GetArray(args.This())->row(get<1>(ri))));
                    break;
                case Region::Rows:
                    NanReturnValue(New(GetArray(args.This())->rows(get<1>(ri), get<2>(ri))));
                    break;
                case Region::Col:
                    NanReturnValue(New(GetArray(args.This())->col(get<1>(ri))));
                    break;
                case Region::Cols:
                    NanReturnValue(New(GetArray(args.This())->cols(get<1>(ri), get<2>(ri))));
                    break;
                case Region::Slice:
                    NanReturnValue(New(GetArray(args.This())->slice(get<1>(ri))));
                    break;
                case Region::Slices:
                    NanReturnValue(New(GetArray(args.This())->slices(get<1>(ri), get<2>(ri))));
                    break;
                default:
                    NanReturnValue(New(GetArray(args.This())->operator()(ToIndex(args[0]))));
                    break;
            }
        }
        else if (args.Length() == 2)
        {
            NanReturnValue(New(GetArray(args.This())->operator()(ToIndex(args[0]), ToIndex(args[1]))));
        }
        else if (args.Length() == 3)
        {
            NanReturnValue(New(GetArray(args.This())->operator()(ToIndex(args[0]), ToIndex(args[1]), ToIndex(args[2]))));
        }
        else
        {
            NanReturnValue(New(GetArray(args.This())->operator()(ToIndex(args[0]), ToIndex(args[1]), ToIndex(args[2]), ToIndex(args[3]))));
        }
    }
    ARRAYFIRE_CATCH
}

#define AFARRAY_IMPL_IDX1(F, f)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    try\
    {\
        ARGS_LEN(1)\
        Guard();\
        auto pArray = GetArray(args.This());\
        NanReturnValue(New(pArray->f(args[0]->Int32Value())));\
    }\
    ARRAYFIRE_CATCH\
}

AFARRAY_IMPL_IDX1(Row, row)
AFARRAY_IMPL_IDX1(Col, col)
AFARRAY_IMPL_IDX1(Slice, slice)
#undef AFARRAY_IMPL_IDX1

#define AFARRAY_IMPL_IDX2(F, f)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    try\
    {\
        ARGS_LEN(2);\
        Guard();\
        auto pArray = GetArray(args.This());\
        NanReturnValue(New(pArray->f(args[0]->Int32Value(), args[1]->Int32Value())));\
    }\
    ARRAYFIRE_CATCH\
}

AFARRAY_IMPL_IDX2(Rows, rows)
AFARRAY_IMPL_IDX2(Cols, cols)
AFARRAY_IMPL_IDX2(Slices, slices)
#undef AFARRAY_IMPL_IDX2

NAN_METHOD(ArrayWrapper::As)
{
    NanScope();
    try
    {
        ARGS_LEN(1);
        af::dtype type = GetDTypeInfo(args[0]->Uint32Value()).first;
        Guard();
        NanReturnValue(New(GetArray(args.This())->as(type)));
    }
    ARRAYFIRE_CATCH
}

#define AFARRAY_IMPL_ASSIGN(F, Op)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    \
    try\
    {\
        auto pArray = GetArray(args.This());\
        auto& array = *pArray;\
        bool isDouble = NeedsDouble(array);\
        ARGS_LEN(1)\
        if (args.Length() == 1)\
        {\
            auto value = args[0];\
            auto pOtherArray = TryGetArray(value);\
            Guard();\
            if (pOtherArray)\
            {\
                auto& otherArray = *pOtherArray;\
                array Op otherArray;\
            }\
            else if (value->IsNumber())\
            {\
                double v = value->NumberValue();\
                if (floor(v) == v)\
                {\
                    array Op value->Int32Value();\
                }\
                else if (isDouble)\
                {\
                    array Op v;\
                }\
                else\
                {\
                    array Op (float)v;\
                }\
            }\
            else if (value->IsObject())\
            {\
                if (isDouble)\
                {\
                    auto v = ToDComplex(value);\
                    array Op v;\
                }\
                else\
                {\
                    auto v = ToFComplex(value);\
                    array Op v;\
                }\
            }\
            else if (value->IsString())\
            {\
                String::Utf8Value str(value);\
                long long v = strtoll(*str, nullptr, 10);\
                array Op v;\
            }\
            else\
            {\
                return NAN_THROW_INVALID_ARGS();\
            }\
        }\
        else if (args.Length() == 2)\
        {\
            auto regIdx = ToRegionIndex(args[0]);\
            auto reg = get<0>(regIdx);\
            auto value = args[1];\
            auto pOtherArray = TryGetArray(value);\
            switch(reg)\
            {\
                case Region::None:\
                    {\
                        auto idx0 = ToIndex(args[0]);\
                        Guard();\
                        if (pOtherArray)\
                        {\
                            auto& otherArray = *pOtherArray;\
                            array(idx0) Op otherArray;\
                        }\
                        else if (value->IsNumber())\
                        {\
                            double v = value->NumberValue();\
                            if (floor(v) == v)\
                            {\
                                array(idx0) Op value->Int32Value();\
                            }\
                            else if (isDouble)\
                            {\
                                array(idx0) Op v;\
                            }\
                            else\
                            {\
                                array(idx0) Op (float)v;\
                            }\
                        }\
                        else if (value->IsObject())\
                        {\
                            if (isDouble)\
                            {\
                                auto v = ToDComplex(value);\
                                array(idx0) Op v;\
                            }\
                            else\
                            {\
                                auto v = ToFComplex(value);\
                                array(idx0) Op v;\
                            }\
                        }\
                        else if (value->IsString())\
                        {\
                            String::Utf8Value str(value);\
                            long long v = strtoll(*str, nullptr, 10);\
                            array(idx0) Op v;\
                        }\
                        else\
                        {\
                            return NAN_THROW_INVALID_ARGS();\
                        }\
                    }\
                    break;\
                case Region::Row:\
                    {\
                        Guard();\
                        if (pOtherArray)\
                        {\
                            auto& otherArray = *pOtherArray;\
                            array.row(get<1>(regIdx)) Op otherArray;\
                        }\
                        else if (value->IsNumber())\
                        {\
                            double v = value->NumberValue();\
                            if (floor(v) == v)\
                            {\
                                array.row(get<1>(regIdx)) Op value->Int32Value();\
                            }\
                            else if (isDouble)\
                            {\
                                array.row(get<1>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                array.row(get<1>(regIdx)) Op (float)v;\
                            }\
                        }\
                        else if (value->IsObject())\
                        {\
                            if (isDouble)\
                            {\
                                auto v = ToDComplex(value);\
                                array.row(get<1>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                auto v = ToFComplex(value);\
                                array.row(get<1>(regIdx)) Op v;\
                            }\
                        }\
                        else if (value->IsString())\
                        {\
                            String::Utf8Value str(value);\
                            long long v = strtoll(*str, nullptr, 10);\
                            array.row(get<1>(regIdx)) Op v;\
                        }\
                        else\
                        {\
                            return NAN_THROW_INVALID_ARGS();\
                        }\
                    }\
                    break;\
                case Region::Rows:\
                    {\
                        Guard();\
                        if (pOtherArray)\
                        {\
                            auto& otherArray = *pOtherArray;\
                            array.rows(get<1>(regIdx), get<2>(regIdx)) Op otherArray;\
                        }\
                        else if (value->IsNumber())\
                        {\
                            double v = value->NumberValue();\
                            if (floor(v) == v)\
                            {\
                                array.rows(get<1>(regIdx), get<2>(regIdx)) Op value->Int32Value();\
                            }\
                            else if (isDouble)\
                            {\
                                array.rows(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                array.rows(get<1>(regIdx), get<2>(regIdx)) Op (float)v;\
                            }\
                        }\
                        else if (value->IsObject())\
                        {\
                            if (isDouble)\
                            {\
                                auto v = ToDComplex(value);\
                                array.rows(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                auto v = ToFComplex(value);\
                                array.rows(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                        }\
                        else if (value->IsString())\
                        {\
                            String::Utf8Value str(value);\
                            long long v = strtoll(*str, nullptr, 10);\
                            array.rows(get<1>(regIdx), get<2>(regIdx)) Op v;\
                        }\
                        else\
                        {\
                            return NAN_THROW_INVALID_ARGS();\
                        }\
                    }\
                    break;\
                case Region::Col:\
                    {\
                        Guard();\
                        if (pOtherArray)\
                        {\
                            auto& otherArray = *pOtherArray;\
                            array.col(get<1>(regIdx)) Op otherArray;\
                        }\
                        else if (value->IsNumber())\
                        {\
                            double v = value->NumberValue();\
                            if (floor(v) == v)\
                            {\
                                array.col(get<1>(regIdx)) Op value->Int32Value();\
                            }\
                            else if (isDouble)\
                            {\
                                array.col(get<1>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                array.col(get<1>(regIdx)) Op (float)v;\
                            }\
                        }\
                        else if (value->IsObject())\
                        {\
                            if (isDouble)\
                            {\
                                auto v = ToDComplex(value);\
                                array.col(get<1>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                auto v = ToFComplex(value);\
                                array.col(get<1>(regIdx)) Op v;\
                            }\
                        }\
                        else if (value->IsString())\
                        {\
                            String::Utf8Value str(value);\
                            long long v = strtoll(*str, nullptr, 10);\
                            array.col(get<1>(regIdx)) Op v;\
                        }\
                        else\
                        {\
                            return NAN_THROW_INVALID_ARGS();\
                        }\
                    }\
                    break;\
                case Region::Cols:\
                    {\
                        Guard();\
                        if (pOtherArray)\
                        {\
                            auto& otherArray = *pOtherArray;\
                            array.cols(get<1>(regIdx), get<2>(regIdx)) Op otherArray;\
                        }\
                        else if (value->IsNumber())\
                        {\
                            double v = value->NumberValue();\
                            if (floor(v) == v)\
                            {\
                                array.cols(get<1>(regIdx), get<2>(regIdx)) Op value->Int32Value();\
                            }\
                            else if (isDouble)\
                            {\
                                array.cols(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                array.cols(get<1>(regIdx), get<2>(regIdx)) Op (float)v;\
                            }\
                        }\
                        else if (value->IsObject())\
                        {\
                            if (isDouble)\
                            {\
                                auto v = ToDComplex(value);\
                                array.cols(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                auto v = ToFComplex(value);\
                                array.cols(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                        }\
                        else if (value->IsString())\
                        {\
                            String::Utf8Value str(value);\
                            long long v = strtoll(*str, nullptr, 10);\
                            array.cols(get<1>(regIdx), get<2>(regIdx)) Op v;\
                        }\
                        else\
                        {\
                            return NAN_THROW_INVALID_ARGS();\
                        }\
                    }\
                    break;\
                case Region::Slice:\
                    {\
                        Guard();\
                        if (pOtherArray)\
                        {\
                            auto& otherArray = *pOtherArray;\
                            array.slice(get<1>(regIdx)) Op otherArray;\
                        }\
                        else if (value->IsNumber())\
                        {\
                            double v = value->NumberValue();\
                            if (floor(v) == v)\
                            {\
                                array.slice(get<1>(regIdx)) Op value->Int32Value();\
                            }\
                            else if (isDouble)\
                            {\
                                array.slice(get<1>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                array.slice(get<1>(regIdx)) Op (float)v;\
                            }\
                        }\
                        else if (value->IsObject())\
                        {\
                            if (isDouble)\
                            {\
                                auto v = ToDComplex(value);\
                                array.slice(get<1>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                auto v = ToFComplex(value);\
                                array.slice(get<1>(regIdx)) Op v;\
                            }\
                        }\
                        else if (value->IsString())\
                        {\
                            String::Utf8Value str(value);\
                            long long v = strtoll(*str, nullptr, 10);\
                            array.slice(get<1>(regIdx)) Op v;\
                        }\
                        else\
                        {\
                            return NAN_THROW_INVALID_ARGS();\
                        }\
                    }\
                    break;\
                case Region::Slices:\
                    {\
                        Guard();\
                        if (pOtherArray)\
                        {\
                            auto& otherArray = *pOtherArray;\
                            array.slices(get<1>(regIdx), get<2>(regIdx)) Op otherArray;\
                        }\
                        else if (value->IsNumber())\
                        {\
                            double v = value->NumberValue();\
                            if (floor(v) == v)\
                            {\
                                array.slices(get<1>(regIdx), get<2>(regIdx)) Op value->Int32Value();\
                            }\
                            else if (isDouble)\
                            {\
                                array.slices(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                array.slices(get<1>(regIdx), get<2>(regIdx)) Op (float)v;\
                            }\
                        }\
                        else if (value->IsObject())\
                        {\
                            if (isDouble)\
                            {\
                                auto v = ToDComplex(value);\
                                array.slices(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                            else\
                            {\
                                auto v = ToFComplex(value);\
                                array.slices(get<1>(regIdx), get<2>(regIdx)) Op v;\
                            }\
                        }\
                        else if (value->IsString())\
                        {\
                            String::Utf8Value str(value);\
                            long long v = strtoll(*str, nullptr, 10);\
                            array.slices(get<1>(regIdx), get<2>(regIdx)) Op v;\
                        }\
                        else\
                        {\
                            return NAN_THROW_INVALID_ARGS();\
                        }\
                    }\
                    break;\
            }\
        }\
        else if (args.Length() == 3)\
        {\
            auto idx0 = ToIndex(args[0]);\
            auto idx1 = ToIndex(args[1]);\
            auto value = args[2];\
            auto pOtherArray = TryGetArray(value);\
            Guard();\
            if (pOtherArray)\
            {\
                auto& otherArray = *pOtherArray;\
                array(idx0, idx1) Op otherArray;\
            }\
            else if (value->IsNumber())\
            {\
                double v = value->NumberValue();\
                if (floor(v) == v)\
                {\
                    array(idx0, idx1) Op value->Int32Value();\
                }\
                else if (isDouble)\
                {\
                    array(idx0, idx1) Op v;\
                }\
                else\
                {\
                    array(idx0, idx1) Op (float)v;\
                }\
            }\
            else if (value->IsObject())\
            {\
                if (isDouble)\
                {\
                    auto v = ToDComplex(value);\
                    array(idx0, idx1) Op v;\
                }\
                else\
                {\
                    auto v = ToFComplex(value);\
                    array(idx0, idx1) Op v;\
                }\
            }\
            else if (value->IsString())\
            {\
                String::Utf8Value str(value);\
                long long v = strtoll(*str, nullptr, 10);\
                array(idx0, idx1) Op v;\
            }\
            else\
            {\
                return NAN_THROW_INVALID_ARGS();\
            }\
        }\
        else if (args.Length() == 4)\
        {\
            auto idx0 = ToIndex(args[0]);\
            auto idx1 = ToIndex(args[1]);\
            auto idx2 = ToIndex(args[2]);\
            auto value = args[3];\
            auto pOtherArray = TryGetArray(value);\
            Guard();\
            if (pOtherArray)\
            {\
                auto& otherArray = *pOtherArray;\
                array(idx0, idx1, idx2) Op otherArray;\
            }\
            else if (value->IsNumber())\
            {\
                double v = value->NumberValue();\
                if (floor(v) == v)\
                {\
                    array(idx0, idx1, idx2) Op value->Int32Value();\
                }\
                else if (isDouble)\
                {\
                    array(idx0, idx1, idx2) Op v;\
                }\
                else\
                {\
                    array(idx0, idx1, idx2) Op (float)v;\
                }\
            }\
            else if (value->IsObject())\
            {\
                if (isDouble)\
                {\
                    auto v = ToDComplex(value);\
                    array(idx0, idx1, idx2) Op v;\
                }\
                else\
                {\
                    auto v = ToFComplex(value);\
                    array(idx0, idx1, idx2) Op v;\
                }\
            }\
            else if (value->IsString())\
            {\
                String::Utf8Value str(value);\
                long long v = strtoll(*str, nullptr, 10);\
                array(idx0, idx1, idx2) Op v;\
            }\
            else\
            {\
                return NAN_THROW_INVALID_ARGS();\
            }\
        }\
        else\
        {\
            auto idx0 = ToIndex(args[0]);\
            auto idx1 = ToIndex(args[1]);\
            auto idx2 = ToIndex(args[2]);\
            auto idx3 = ToIndex(args[3]);\
            auto value = args[4];\
            auto pOtherArray = TryGetArray(value);\
            Guard();\
            if (pOtherArray)\
            {\
                auto& otherArray = *pOtherArray;\
                array(idx0, idx1, idx2, idx3) Op otherArray;\
            }\
            else if (value->IsNumber())\
            {\
                double v = value->NumberValue();\
                if (floor(v) == v)\
                {\
                    array(idx0, idx1, idx2, idx3) Op value->Int32Value();\
                }\
                else if (isDouble)\
                {\
                    array(idx0, idx1, idx2, idx3) Op v;\
                }\
                else\
                {\
                    array(idx0, idx1, idx2, idx3) Op (float)v;\
                }\
            }\
            else if (value->IsObject())\
            {\
                if (isDouble)\
                {\
                    auto v = ToDComplex(value);\
                    array(idx0, idx1, idx2, idx3) Op v;\
                }\
                else\
                {\
                    auto v = ToFComplex(value);\
                    array(idx0, idx1, idx2, idx3) Op v;\
                }\
            }\
            else if (value->IsString())\
            {\
                String::Utf8Value str(value);\
                long long v = strtoll(*str, nullptr, 10);\
                array(idx0, idx1, idx2, idx3) Op v;\
            }\
            else\
            {\
                return NAN_THROW_INVALID_ARGS();\
            }\
        }\
        \
        NanReturnValue(args.This());\
    }\
    ARRAYFIRE_CATCH\
}

AFARRAY_IMPL_ASSIGN(Assign, =)
AFARRAY_IMPL_ASSIGN(AddAssign, +=)
AFARRAY_IMPL_ASSIGN(SubAssign, -=)
AFARRAY_IMPL_ASSIGN(MulAssign, *=)
AFARRAY_IMPL_ASSIGN(DivAssign, /=)
#undef AFARRAY_IMPL_ASSIGN

#define AFARRAY_IMPL_BINOP(F, Op)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    \
    try\
    {\
        auto& array = *GetArray(args.This());\
        bool isDouble = NeedsDouble(array);\
        ARGS_LEN(1)\
        auto value = args[0];\
        auto pOtherArray = TryGetArray(value);\
        af::array* result = nullptr;\
        Guard();\
        if (pOtherArray)\
        {\
            auto& otherArray = *pOtherArray;\
            result = new af::array(array Op otherArray);\
        }\
        else if (value->IsNumber())\
        {\
            double v = value->NumberValue();\
            if (floor(v) == v)\
            {\
                result = new af::array(array Op value->Int32Value());\
            }\
            else if (isDouble)\
            {\
                result = new af::array(array Op v);\
            }\
            else\
            {\
                result = new af::array(array Op (float)v);\
            }\
        }\
        else if (value->IsObject())\
        {\
            if (isDouble)\
            {\
                auto v = ToDComplex(value);\
                result = new af::array(array Op v);\
            }\
            else\
            {\
                auto v = ToFComplex(value);\
                result = new af::array(array Op v);\
            }\
        }\
        else if (value->IsString())\
        {\
            String::Utf8Value str(value);\
            intl v = strtoll(*str, nullptr, 10);\
            result = new af::array(array Op v);\
        }\
        else\
        {\
            return NAN_THROW_INVALID_ARGS();\
        }\
        \
        NanReturnValue(New(result));\
    }\
    ARRAYFIRE_CATCH\
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
#undef AFARRAY_IMPL_BINOP

#define AFARRAY_IMPL_BINOP(F, Op)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    \
    try\
    {\
        auto& array = *GetArray(args.This());\
        bool isDouble = NeedsDouble(array);\
        ARGS_LEN(1)\
        auto value = args[0];\
        auto pOtherArray = TryGetArray(value);\
        af::array* result = nullptr;\
        Guard();\
        if (pOtherArray)\
        {\
            auto& otherArray = *pOtherArray;\
            result = new af::array(otherArray Op array);\
        }\
        else if (value->IsNumber())\
        {\
            double v = value->NumberValue();\
            if (floor(v) == v)\
            {\
                result = new af::array(value->Int32Value() Op array);\
            }\
            else if (isDouble)\
            {\
                result = new af::array(v Op array);\
            }\
            else\
            {\
                result = new af::array((float)v Op array);\
            }\
        }\
        else if (value->IsObject())\
        {\
            if (isDouble)\
            {\
                auto v = ToDComplex(value);\
                result = new af::array(v Op array);\
            }\
            else\
            {\
                auto v = ToFComplex(value);\
                result = new af::array(v Op array);\
            }\
        }\
        else if (value->IsString())\
        {\
            String::Utf8Value str(value);\
            intl v = strtoll(*str, nullptr, 10);\
            result = new af::array(v Op array);\
        }\
        else\
        {\
            return NAN_THROW_INVALID_ARGS();\
        }\
        \
        NanReturnValue(New(result));\
    }\
    ARRAYFIRE_CATCH\
}

AFARRAY_IMPL_BINOP(RhsAdd, +)
AFARRAY_IMPL_BINOP(RhsSub, -)
AFARRAY_IMPL_BINOP(RhsMul, *)
AFARRAY_IMPL_BINOP(RhsDiv, /)
AFARRAY_IMPL_BINOP(RhsBitShiftL, <<)
AFARRAY_IMPL_BINOP(RhsBitShiftR, >>)

AFARRAY_IMPL_BINOP(RhsLt, <)
AFARRAY_IMPL_BINOP(RhsGt, >)
AFARRAY_IMPL_BINOP(RhsLe, <=)
AFARRAY_IMPL_BINOP(RhsGe, >=)
AFARRAY_IMPL_BINOP(RhsEq, ==)
AFARRAY_IMPL_BINOP(RhsNeq, !=)
AFARRAY_IMPL_BINOP(RhsAnd, &&)
AFARRAY_IMPL_BINOP(RhsOr, ||)
AFARRAY_IMPL_BINOP(RhsBitAnd, &)
AFARRAY_IMPL_BINOP(RhsBitOr, |)
AFARRAY_IMPL_BINOP(RhsBitXor, ^)
#undef AFARRAY_IMPL_BINOP

#define AFARRAY_IMPL_UNOP(F, Op)\
NAN_METHOD(ArrayWrapper::F)\
{\
    NanScope();\
    \
    try\
    {\
        auto& array = *GetArray(args.This());\
        Guard();\
        NanReturnValue(New(array.operator Op()));\
    }\
    ARRAYFIRE_CATCH\
}

AFARRAY_IMPL_UNOP(Neg, -)
AFARRAY_IMPL_UNOP(Not, !)
#undef AFARRAY_IMPL_UNOP
