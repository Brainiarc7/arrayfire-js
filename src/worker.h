#ifndef ARRAY_FIRE_JS_WORKER_H
#define ARRAY_FIRE_JS_WORKER_H

#include <functional>
#include <nan.h>
#include <exception>

NAN_METHOD(noop)
{
    NanScope();
    NanReturnUndefined();
}

template<typename T>
struct Worker : public NanAsyncWorker
{
    typedef std::function<T()> ExecuteFunc;
    typedef std::function<v8::Handle<v8::Value>(T)> ResultConvFunc;

    Worker(NanCallback *callback, const ExecuteFunc& executeFunc, const ResultConvFunc& resultConvFunc) :
        NanAsyncWorker(callback ? callback : new NanCallback(NanNew<v8::FunctionTemplate>(noop)->GetFunction())),
        executeFunc(executeFunc),
        resultConvFunc(resultConvFunc)
    {
    }

    void Execute() override
    {
        using namespace std;
        try
        {
            result = executeFunc();
        }
        catch(exception& ex)
        {
            SetErrorMessage(ex.what());
        }
    }

protected:
    void HandleOKCallback() override
    {
        using namespace v8;
        NanScope();
        Local<Value> args[] = { NanNull(), resultConvFunc(result) };
        callback->Call(2, args);
    }

private:
    ExecuteFunc executeFunc;
    ResultConvFunc resultConvFunc;
    T result;
};

template<>
struct Worker<void> : public NanAsyncWorker
{
    typedef std::function<void()> ExecuteFunc;

    Worker(NanCallback *callback, const ExecuteFunc& executeFunc) :
        NanAsyncWorker(callback ? callback : new NanCallback(NanNew<v8::FunctionTemplate>(noop)->GetFunction())),
        executeFunc(executeFunc)
    {
    }

    void Execute() override
    {
        using namespace std;
        try
        {
            executeFunc();
        }
        catch(exception& ex)
        {
            SetErrorMessage(ex.what());
        }
    }

private:
    ExecuteFunc executeFunc;
};

#endif
