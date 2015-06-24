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
#include "mathfunctions.h"
#include "helpers.h"
#include "arraywrapper.h"
#include "errors.h"
#include "guard.h"

using namespace v8;
using namespace std;
using namespace node;

FIRE_SYNC_METHOD_ARR(Abs, abs)
FIRE_SYNC_METHOD_ARR(Arg, arg)
FIRE_SYNC_METHOD_ARR(Ceil, ceil)
FIRE_SYNC_METHOD_ARR(Floor, floor)
FIRE_SYNC_METHOD_ARR_ARR(Hypot, hypot)
FIRE_SYNC_METHOD_ARR_ARR(Max, max)
FIRE_SYNC_METHOD_ARR_ARR(Min, min)
FIRE_SYNC_METHOD_ARR_ARR(Mod, mod)
FIRE_SYNC_METHOD_ARR(Round, round)
FIRE_SYNC_METHOD_ARR(Sign, sign)
FIRE_SYNC_METHOD_ARR(Trunc, trunc)
FIRE_SYNC_METHOD_ARR_DOUBLE_COMB(Rem, rem)

FIRE_SYNC_METHOD_ARR(ACos, acos)
FIRE_SYNC_METHOD_ARR(ASin, asin)
FIRE_SYNC_METHOD_ARR(ATan, atan)
FIRE_SYNC_METHOD_ARR_DOUBLE_COMB(ATan2, atan2)
FIRE_SYNC_METHOD_ARR(Cos, cos)
FIRE_SYNC_METHOD_ARR(Sin, sin)
FIRE_SYNC_METHOD_ARR(Tan, tan)

FIRE_SYNC_METHOD_ARR(Cbrt, cbrt)
FIRE_SYNC_METHOD_ARR(Erf, erf)
FIRE_SYNC_METHOD_ARR(Erfc, erfc)
FIRE_SYNC_METHOD_ARR(Exp, exp)
FIRE_SYNC_METHOD_ARR(ExpM1, expm1)
FIRE_SYNC_METHOD_ARR(Factorial, factorial)
FIRE_SYNC_METHOD_ARR(LGamma, lgamma)
FIRE_SYNC_METHOD_ARR(TGamma, tgamma)
FIRE_SYNC_METHOD_ARR(Log, log)
FIRE_SYNC_METHOD_ARR(Log10, log10)
FIRE_SYNC_METHOD_ARR(Log1P, log1p)
FIRE_SYNC_METHOD_ARR_ARR(Pow, pow)
FIRE_SYNC_METHOD_ARR(Pow2, pow2)
FIRE_SYNC_METHOD_ARR_DOUBLE_COMB(Root, root)
FIRE_SYNC_METHOD_ARR(Sqrt, sqrt)

FIRE_SYNC_METHOD_ARR(ACosH, acosh)
FIRE_SYNC_METHOD_ARR(ASinH, asinh)
FIRE_SYNC_METHOD_ARR(ATanH, atanh)
FIRE_SYNC_METHOD_ARR(CosH, cosh)
FIRE_SYNC_METHOD_ARR(SinH, sinh)
FIRE_SYNC_METHOD_ARR(TanH, tanh)

FIRE_SYNC_METHOD_ARR(Complex, complex)
FIRE_SYNC_METHOD_ARR(Conjg, conjg)
FIRE_SYNC_METHOD_ARR(Imag, imag)
FIRE_SYNC_METHOD_ARR(Real, real)

void InitMathFunctions(v8::Handle<v8::Object> exports)
{
    exports->Set(NanNew("abs"), NanNew<FunctionTemplate>(Abs)->GetFunction());
    exports->Set(NanNew("arg"), NanNew<FunctionTemplate>(Arg)->GetFunction());
    exports->Set(NanNew("ceil"), NanNew<FunctionTemplate>(Ceil)->GetFunction());
    exports->Set(NanNew("floor"), NanNew<FunctionTemplate>(Floor)->GetFunction());
    exports->Set(NanNew("hypot"), NanNew<FunctionTemplate>(Hypot)->GetFunction());
    exports->Set(NanNew("max"), NanNew<FunctionTemplate>(Max)->GetFunction());
    exports->Set(NanNew("min"), NanNew<FunctionTemplate>(Min)->GetFunction());
    exports->Set(NanNew("round"), NanNew<FunctionTemplate>(Round)->GetFunction());
    exports->Set(NanNew("sign"), NanNew<FunctionTemplate>(Sign)->GetFunction());
    exports->Set(NanNew("trunc"), NanNew<FunctionTemplate>(Trunc)->GetFunction());
    exports->Set(NanNew("rem"), NanNew<FunctionTemplate>(Rem)->GetFunction());

    exports->Set(NanNew("acos"), NanNew<FunctionTemplate>(ACos)->GetFunction());
    exports->Set(NanNew("aCos"), NanNew<FunctionTemplate>(ACos)->GetFunction());
    exports->Set(NanNew("asin"), NanNew<FunctionTemplate>(ASin)->GetFunction());
    exports->Set(NanNew("aSin"), NanNew<FunctionTemplate>(ASin)->GetFunction());
    exports->Set(NanNew("atan"), NanNew<FunctionTemplate>(ATan)->GetFunction());
    exports->Set(NanNew("aTan"), NanNew<FunctionTemplate>(ATan)->GetFunction());
    exports->Set(NanNew("atan2"), NanNew<FunctionTemplate>(ATan2)->GetFunction());
    exports->Set(NanNew("aTan2"), NanNew<FunctionTemplate>(ATan2)->GetFunction());
    exports->Set(NanNew("cos"), NanNew<FunctionTemplate>(Cos)->GetFunction());
    exports->Set(NanNew("sin"), NanNew<FunctionTemplate>(Sin)->GetFunction());
    exports->Set(NanNew("tan"), NanNew<FunctionTemplate>(Tan)->GetFunction());

    exports->Set(NanNew("cbrt"), NanNew<FunctionTemplate>(Cbrt)->GetFunction());
    exports->Set(NanNew("erf"), NanNew<FunctionTemplate>(Erf)->GetFunction());
    exports->Set(NanNew("erfc"), NanNew<FunctionTemplate>(Erfc)->GetFunction());
    exports->Set(NanNew("exp"), NanNew<FunctionTemplate>(Exp)->GetFunction());
    exports->Set(NanNew("expm1"), NanNew<FunctionTemplate>(ExpM1)->GetFunction());
    exports->Set(NanNew("expM1"), NanNew<FunctionTemplate>(ExpM1)->GetFunction());
    exports->Set(NanNew("factorial"), NanNew<FunctionTemplate>(Factorial)->GetFunction());
    exports->Set(NanNew("lgamma"), NanNew<FunctionTemplate>(LGamma)->GetFunction());
    exports->Set(NanNew("lGamma"), NanNew<FunctionTemplate>(LGamma)->GetFunction());
    exports->Set(NanNew("tgamma"), NanNew<FunctionTemplate>(TGamma)->GetFunction());
    exports->Set(NanNew("tGamma"), NanNew<FunctionTemplate>(TGamma)->GetFunction());
    exports->Set(NanNew("log"), NanNew<FunctionTemplate>(Log)->GetFunction());
    exports->Set(NanNew("log10"), NanNew<FunctionTemplate>(Log10)->GetFunction());
    exports->Set(NanNew("log1p"), NanNew<FunctionTemplate>(Log1P)->GetFunction());
    exports->Set(NanNew("log1P"), NanNew<FunctionTemplate>(Log1P)->GetFunction());
    exports->Set(NanNew("pow"), NanNew<FunctionTemplate>(Pow)->GetFunction());
    exports->Set(NanNew("pow2"), NanNew<FunctionTemplate>(Pow2)->GetFunction());
    exports->Set(NanNew("root"), NanNew<FunctionTemplate>(Root)->GetFunction());
    exports->Set(NanNew("sqrt"), NanNew<FunctionTemplate>(Sqrt)->GetFunction());

    exports->Set(NanNew("acosh"), NanNew<FunctionTemplate>(ACosH)->GetFunction());
    exports->Set(NanNew("aCosH"), NanNew<FunctionTemplate>(ACosH)->GetFunction());
    exports->Set(NanNew("asinh"), NanNew<FunctionTemplate>(ASinH)->GetFunction());
    exports->Set(NanNew("aSinH"), NanNew<FunctionTemplate>(ASinH)->GetFunction());
    exports->Set(NanNew("atanh"), NanNew<FunctionTemplate>(ATanH)->GetFunction());
    exports->Set(NanNew("aTanH"), NanNew<FunctionTemplate>(ATanH)->GetFunction());

    exports->Set(NanNew("complex"), NanNew<FunctionTemplate>(Complex)->GetFunction());
    exports->Set(NanNew("conjg"), NanNew<FunctionTemplate>(Conjg)->GetFunction());
    exports->Set(NanNew("imag"), NanNew<FunctionTemplate>(Imag)->GetFunction());
    exports->Set(NanNew("real"), NanNew<FunctionTemplate>(Real)->GetFunction());
}
