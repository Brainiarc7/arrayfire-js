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
#include "fire.h"
#include "device.h"
#include "arraywrapper.h"
#include "createarray.h"
#include "moveandreorderarray.h"
#include "arrayhelperfunctions.h"
#include "mathfunctions.h"
#include "vectoralgorithms.h"
#include "statistics.h"
#include "symbols.h"
#include "computervision.h"
#include "imageprocessing.h"
#include "linearalgebra.h"
#include "signalprocessing.h"

using namespace v8;
using namespace std;

NAN_METHOD(_DoEvents)
{
    NanScope();
    uv_run(uv_default_loop(), UV_RUN_ONCE);
    NanReturnUndefined();
}

NAN_METHOD(_GforToggle)
{
    NanScope();
    af::gforToggle();
    NanReturnUndefined();
}

NAN_METHOD(GC)
{
    NanScope();
    unsigned ms = args.Length() ? args[0]->Uint32Value() : 1000;
    NanIdleNotification(ms);
    uv_run(uv_default_loop(), UV_RUN_ONCE);
    af::deviceGC();
    NanReturnUndefined();
}

void Init(v8::Handle<v8::Object> exports)
{

    Symbols::Init();
    InitDevice(exports);
    ArrayWrapper::Init(exports);
    InitCreateArray(exports);
    InitMoveAndReorderArray(exports);
    InitArrayHelperFunctions(exports);
    InitMathFunctions(exports);
    InitVectorAlgorithms(exports);
    InitStatistics(exports);
    InitComputerVision(exports);
    InitImageProcessing(exports);
    InitLinearAlgebra(exports);
    InitSignalProcessing(exports);

    // Helpers:
    exports->Set(NanNew("_doEvents"), NanNew<FunctionTemplate>(_DoEvents)->GetFunction());
    exports->Set(NanNew("_gforToggle"), NanNew<FunctionTemplate>(_GforToggle)->GetFunction());
    exports->Set(NanNew("gc"), NanNew<FunctionTemplate>(::GC)->GetFunction());
}
