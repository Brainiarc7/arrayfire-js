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
#include "arrayhelperfunctions.h"
#include "helpers.h"
#include "arraywrapper.h"
#include "errors.h"
#include "guard.h"

using namespace v8;
using namespace std;
using namespace node;

ARRAYFIRE_SYNC_METHOD_ARR(IsZero, iszero)
ARRAYFIRE_SYNC_METHOD_ARR(IsInf, isInf)
ARRAYFIRE_SYNC_METHOD_ARR(IsNaN, isNaN)

NAN_METHOD(Print)
{
    NanScope();
    try
    {
        ARGS_LEN(1);
        Guard();
        af_print(*ArrayWrapper::GetArrayAt(args, 0));
        NanReturnUndefined();
    }
    ARRAYFIRE_CATCH;
}

void InitArrayHelperFunctions(v8::Handle<v8::Object> exports)
{
    exports->Set(NanNew("iszero"), NanNew<FunctionTemplate>(IsZero)->GetFunction());
    exports->Set(NanNew("isZero"), NanNew<FunctionTemplate>(IsZero)->GetFunction());
    exports->Set(NanNew("isInf"), NanNew<FunctionTemplate>(IsInf)->GetFunction());
    exports->Set(NanNew("isNaN"), NanNew<FunctionTemplate>(IsNaN)->GetFunction());
    exports->Set(NanNew("print"), NanNew<FunctionTemplate>(Print)->GetFunction());
}
