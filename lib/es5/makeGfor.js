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

"use strict";

var _ = require("lodash");
var assert = require("better-assert");
var Seq = require("./seq");

function makeGfor(af) {
    return function gfor() {
        var fPos = -1;
        for (var i = 0; i < arguments.length; i++) {
            if (_.isFunction(arguments[i])) {
                fPos = i;
                break;
            }
        }
        if (fPos === -1) {
            throw new Error("Body function argument expected.");
        }
        if (fPos === 0) {
            throw new Error("Seq arguments expected.");
        }
        var thisArg = arguments[fPos];
        if (arguments.length > fPos + 1 && _.isObject(fPos + 1)) {
            thisArg = arguments[fPos + 1];
        }
        var seq = undefined;
        if (fPos === 1) {
            seq = new Seq(arguments[0]);
        } else if (fPos === 2) {
            seq = new Seq(arguments[0], arguments[1]);
        } else {
            throw new Error("Invalid number of Seq arguments.");
        }
        seq.isGFor = true;
        af._gforToggle();
        try {
            arguments[fPos].call(thisArg, seq);
        } finally {
            af._gforToggle();
        }
    };
}

module.exports = makeGfor;
//# sourceMappingURL=makeGfor.js.map
