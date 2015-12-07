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

let Bluebird = require("bluebird");
let _ = require("lodash");
let async = Bluebird.coroutine;

const retryCount = 5;
const gcTime = 1000;

function isOutOfMemoryError(e) {
    return e.message.indexOf("out of memory") > 0;
}

function invokeGC(af) {
    af.gc(gcTime);
}

function synchronify(af, f) {
    return function () {
        var err;
        var res;
        var done;
        let cb = function (e, r) {
            err = e;
            res = r;
            done = true;
        };

        let args = _.toArray(arguments).concat(cb);

        for (let i = 0; i < retryCount; i++) {
            done = false;
            f.apply(this, args);
            while (!done) af._doEvents();

            if (err) {
                if (!isOutOfMemoryError(err)) {
                    throw err;
                }
                else {
                    invokeGC(af);
                }
            }
        }

        if (err) throw err;

        return res;
    };
}

function installAsyncAndSync(af, obj, name) {
    if (_.isUndefined(name)) {
        for (let key of _.keys(obj)) {
            installAsyncAndSync(af, obj, key);
        }
        return;
    }

    if (name !== "AFArray") {
        let f = obj[name];
        if (_.isFunction(f)) {
            if (!_.isFunction(obj[name + "Async"])) {
                obj[name + "Async"] = async(function* () {
                    let self = this;
                    let args = _.toArray(arguments);
                    let call = function () {
                        return new Bluebird(function (resolve, reject) {
                            let cb = function (e, r) {
                                if (e) {
                                    reject(e);
                                }
                                else {
                                    resolve(r);
                                }
                            };
                            args.push(cb);
                            f.apply(self, args);
                        })
                    };

                    let err = null;
                    for (let i = 0; i < retryCount; i++) {
                        try {
                            return yield call();
                        }
                        catch (e) {
                            if (!isOutOfMemoryError(e)) {
                                throw e;
                            }
                            invokeGC(af);
                            err = e;
                        }
                    }
                    throw err;
                });
            }
            if (!_.isFunction(obj[name + "Sync"])) {
                obj[name + "Sync"] = synchronify(af, f);
            }
            obj[name] = function () {
                let err = null;
                for (let i = 0; i < retryCount; i++) {
                    try {
                        return f.apply(this, arguments);
                    }
                    catch (e) {
                        if (!isOutOfMemoryError(e)) {
                            throw e;
                        }
                        invokeGC(af);
                        err = e;
                    }
                }
                throw err;
            };
        }
    }
}

function ext(af) {
    installAsyncAndSync(af, af);
    installAsyncAndSync(af, af.AFArray);
    installAsyncAndSync(af, af.AFArray.prototype);

    let scope = af.AFArray.scope =  require("./scope");

    _.extend(af, {
        end: -1,
        span: null,
        all: -1,
        dtype: require("./dtype"),
        dType: require("./dtype"),
        source: require("./source"),
        matchType: require("./matchType"),
        cSpace: require("./cSpace"),
        CSpace: require("./cSpace"),
        connectivity: require("./connectivity"),
        borderType: require("./borderType"),
        interpType: require("./interpType"),
        matProp: require("./matProp"),
        normType: require("./normType"),
        convMode: require("./convMode"),
        convDomain: require("./convDomain"),
        Dim4: require("./dim4"),
        Seq: require("./seq"),
        Complex: require("./complex"),
        Row: require("./row"),
        Col: require("./col"),
        Rows: require("./rows"),
        Cols: require("./cols"),
        getDevices: function () {
            let current = this.getDevice();
            try {
                let count = this.getDeviceCount();
                let result = [];
                for (let i = 0; i < count; i++) {
                    this.setDevice(i);
                    let info = this.deviceInfo();
                    info.id = i;
                    result.push(info);
                }
                return result;
            }
            finally {
                this.setDevice(current);
            }
        },
        gfor: require("./makeGfor")(af),
        scope: scope
    });
}

module.exports = ext;