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
/* global describe,it */

var assert = require("better-assert");
var _ = require("lodash");
var ref = require("ref");
var Bluebird = require("bluebird");
var async = Bluebird.coroutine;
var testExec = require("./testExec");
var float = ref.types.float;

describe("AFArray class and methods", function () {
    testExec.run(function (af) {
        var AFArray = af.AFArray;

        it("should export AFArray constructor", function () {
            assert(_.isFunction(AFArray));
        });

        it("should create empty", function () {
            var array = new AFArray();
            assert(_.isObject(array));
            assert(array.bytes() === 0);
            assert(array.elements() === 0);
            assert(array.isempty());
            assert(_.isFunction(array.host));
            assert(_.isFunction(array.hostAsync));
            assert(_.isUndefined(array.hostAsyncAsync));
            assert(_.isFunction(array.hostSync));
            assert(_.isUndefined(array.hostSyncAsync));
            assert(_.isUndefined(array.hostSyncSync));
            assert(_.isUndefined(array.hostAsyncSync));
        });

        it("should fail with one number argument", function () {
            try {
                var array = new AFArray(1);
                assert(false);
            } catch (e) {}
        });

        it("should create new one dimensional", function () {
            var array = new AFArray(10, af.dType.s32);
            assert(_.isObject(array));
            assert(array.bytes() === 10 * 4);
            assert(array.elements() === 10);
            assert(array.type() === af.dType.s32);
            assert(array.numdims() === 1);
            assert(array.dims(0) === 10);
            assert(array.dims(1) === 1);
            assert(array.dims(2) === 1);
            assert(array.dims(3) === 1);
            var dims = array.dims();
            assert(_.isObject(dims));
            assert(dims.elements === 10);
            assert(dims.ndims === 1);
            assert(dims.values[0] === 10);
            assert(dims.values[1] === 1);
            assert(dims.values[2] === 1);
            assert(dims.values[3] === 1);
            assert(array.isempty() === false);
            assert(array.isscalar() === false);
            assert(array.isvector() === true);
            assert(array.isrow() === false);
            assert(array.iscolumn() === true);
            assert(array.iscomplex() === false);
            assert(array.isreal() === true);
            assert(array.isdouble() === false);
            assert(array.issingle() === false);
            assert(array.isrealfloating() === false);
            assert(array.isfloating() === false);
            assert(array.isinteger() === true);
            assert(array.isbool() === false);
        });

        it("should create new two dimensional", function () {
            var array = new AFArray(10, 20, af.dType.f32);
            assert(_.isObject(array));
            assert(array.bytes() === 10 * 20 * 4);
            assert(array.elements() === 10 * 20);
            assert(array.type() === af.dType.f32);
            assert(array.numdims() === 2);
            assert(array.dims(0) === 10);
            assert(array.dims(1) === 20);
            assert(array.dims(2) === 1);
            assert(array.dims(3) === 1);
            var dims = array.dims();
            assert(_.isObject(dims));
            assert(dims.elements === 10 * 20);
            assert(dims.ndims === 2);
            assert(dims.values[0] === 10);
            assert(dims.values[1] === 20);
            assert(dims.values[2] === 1);
            assert(dims.values[3] === 1);
            assert(array.isempty() === false);
            assert(array.isscalar() === false);
            assert(array.isvector() === false);
            assert(array.isrow() === false);
            assert(array.iscolumn() === false);
            assert(array.iscomplex() === false);
            assert(array.isreal() === true);
            assert(array.isdouble() === false);
            assert(array.issingle() === true);
            assert(array.isrealfloating() === true);
            assert(array.isfloating() === true);
            assert(array.isinteger() === false);
            assert(array.isbool() === false);
        });

        it("should create new three dimensional", function () {
            var array = new AFArray(10, 20, 30, af.dType.f32);
            assert(_.isObject(array));
            assert(array.bytes() === 10 * 20 * 30 * 4);
            assert(array.elements() === 10 * 20 * 30);
            assert(array.type() === af.dType.f32);
            assert(array.numdims() === 3);
            assert(array.dims(0) === 10);
            assert(array.dims(1) === 20);
            assert(array.dims(2) === 30);
            assert(array.dims(3) === 1);
            var dims = array.dims();
            assert(_.isObject(dims));
            assert(dims.elements === 10 * 20 * 30);
            assert(dims.ndims === 3);
            assert(dims.values[0] === 10);
            assert(dims.values[1] === 20);
            assert(dims.values[2] === 30);
            assert(dims.values[3] === 1);
            assert(array.isempty() === false);
            assert(array.isscalar() === false);
            assert(array.isvector() === false);
            assert(array.isrow() === false);
            assert(array.iscolumn() === false);
            assert(array.iscomplex() === false);
            assert(array.isreal() === true);
            assert(array.isdouble() === false);
            assert(array.issingle() === true);
            assert(array.isrealfloating() === true);
            assert(array.isfloating() === true);
            assert(array.isinteger() === false);
            assert(array.isbool() === false);
        });

        function verify4(array) {
            assert(_.isObject(array));
            assert(array.bytes() === 10 * 20 * 30 * 40 * 4);
            assert(array.elements() === 10 * 20 * 30 * 40);
            assert(array.type() === af.dType.f32);
            assert(array.numdims() === 4);
            assert(array.dims(0) === 10);
            assert(array.dims(1) === 20);
            assert(array.dims(2) === 30);
            assert(array.dims(3) === 40);
            var dims = array.dims();
            assert(_.isObject(dims));
            assert(dims.elements === 10 * 20 * 30 * 40);
            assert(dims.ndims === 4);
            assert(dims.values[0] === 10);
            assert(dims.values[1] === 20);
            assert(dims.values[2] === 30);
            assert(dims.values[3] === 40);
            assert(array.isempty() === false);
            assert(array.isscalar() === false);
            assert(array.isvector() === false);
            assert(array.isrow() === false);
            assert(array.iscolumn() === false);
            assert(array.iscomplex() === false);
            assert(array.isreal() === true);
            assert(array.isdouble() === false);
            assert(array.issingle() === true);
            assert(array.isrealfloating() === true);
            assert(array.isfloating() === true);
            assert(array.isinteger() === false);
            assert(array.isbool() === false);
        }

        it("should create new four dimensional", function () {
            var array = new AFArray(10, 20, 30, 40, af.dType.f32);
            verify4(array);
        });

        it("should create new four dimensional from dim4", function () {
            var array = new AFArray({ values: [10, 20, 30, 40] }, af.dType.f32);
            verify4(array);
        });

        it("should create new four dimensional from dim4 array", function () {
            var array = new AFArray([10, 20, 30, 40], af.dType.f32);
            verify4(array);
        });

        it("should initialize from buffer, copyable, and readable - asynchronously w/ generators", function (done) {
            var f = async(regeneratorRuntime.mark(function _callee() {
                var int, count, buff, _v, array, buff2, _v2, v1, v2, array2, buff3, _v3, v;

                return regeneratorRuntime.wrap(function _callee$(_context) {
                    while (1) {
                        switch (_context.prev = _context.next) {
                            case 0:
                                int = ref.types.int;
                                count = 10;
                                buff = new Buffer(int.size * count);

                                for (_v = 0; _v < count; _v++) {
                                    int.set(buff, _v * int.size, _v * _v);
                                }

                                assert(_.isFunction(AFArray.create));
                                assert(_.isFunction(AFArray.createAsync));

                                _context.next = 8;
                                return AFArray.createAsync(count, af.dType.s32, buff);

                            case 8:
                                array = _context.sent;

                                assert(array.bytes() === count * int.size);
                                assert(array.type() === af.dType.s32);

                                buff2 = new Buffer(int.size * count);
                                _context.next = 14;
                                return array.hostAsync(buff2);

                            case 14:
                                for (_v2 = 0; _v2 < count; _v2++) {
                                    v1 = int.get(buff, _v2 * int.size);
                                    v2 = int.get(buff2, _v2 * int.size);

                                    assert(v1 === v2);
                                    assert(v1 === _v2 * _v2);
                                }

                                array2 = array.copy();

                                assert(array2 instanceof AFArray);
                                assert(array2.bytes() === array.bytes());
                                _context.next = 20;
                                return array2.hostAsync();

                            case 20:
                                buff3 = _context.sent;

                                assert(buff3 instanceof Buffer);
                                assert(buff3.length === int.size * count);
                                for (_v3 = 0; _v3 < count; _v3++) {
                                    v1 = int.get(buff, _v3 * int.size);
                                    v2 = int.get(buff3, _v3 * int.size);

                                    assert(v1 === v2);
                                    assert(v1 === _v3 * _v3);
                                }

                                // Let's do some indexing:
                                v = array2.value();

                                assert(v === 0.0);

                                v = array2.at(1).value();
                                assert(v === 1.0);

                                v = array2.at(2).scalar();
                                assert(v === 4.0);

                                v = array2.at("end").scalar();
                                assert(v === 9.0 * 9.0);

                                v = array2.at(af.end - 1).scalar();
                                assert(v === 8.0 * 8.0);

                            case 34:
                            case "end":
                                return _context.stop();
                        }
                    }
                }, _callee, this);
            }));
            f().nodeify(done);
        });

        it("should initialize from buffer, copyable, and readable - synchronously (blocking)", function () {
            var int = ref.types.int;
            var count = 10;
            var buff = new Buffer(int.size * count);
            for (var _v4 = 0; _v4 < count; _v4++) {
                int.set(buff, _v4 * int.size, _v4 * _v4);
            }

            assert(_.isFunction(AFArray.create));
            assert(_.isFunction(AFArray.createSync));

            var array = AFArray.createSync(count, af.dType.s32, buff);
            assert(array.bytes() === count * int.size);
            assert(array.type() === af.dType.s32);

            var buff2 = new Buffer(int.size * count);
            array.hostSync(buff2);
            for (var _v5 = 0; _v5 < count; _v5++) {
                var _v6 = int.get(buff, _v5 * int.size);
                var _v7 = int.get(buff2, _v5 * int.size);
                assert(_v6 === _v7);
                assert(_v6 === _v5 * _v5);
            }

            var array2 = array.copy();
            assert(array2 instanceof AFArray);
            assert(array2.bytes() === array.bytes());
            var buff3 = array2.hostSync();
            assert(buff3 instanceof Buffer);
            assert(buff3.length === int.size * count);
            for (var _v8 = 0; _v8 < count; _v8++) {
                var _v9 = int.get(buff, _v8 * int.size);
                var _v10 = int.get(buff3, _v8 * int.size);
                assert(_v9 === _v10);
                assert(_v9 === _v8 * _v8);
            }

            // Let's do some indexing:
            var v = array2.value();
            assert(v === 0.0);

            v = array.at(1).value();
            assert(v === 1.0);

            v = array2.at(2).scalar();
            assert(v === 4.0);

            v = array2.at("end").scalar();
            assert(v === 9.0 * 9.0);

            v = array2.at(af.end - 1).scalar();
            assert(v === 8.0 * 8.0);
        });

        it("should be created of a part of another with new dimensions", function (done) {
            async(regeneratorRuntime.mark(function _callee2() {
                var arr, sub, sub2, buff;
                return regeneratorRuntime.wrap(function _callee2$(_context2) {
                    while (1) {
                        switch (_context2.prev = _context2.next) {
                            case 0:
                                arr = new af.AFArray(10, af.dType.f32);

                                arr.set(new af.Col(0), 0);
                                arr.set(3, 1);
                                arr.set(4, 2);

                                sub = arr.at(new af.Seq(3, 6));
                                sub2 = new af.AFArray(sub, new af.Dim4(2, 2));
                                _context2.next = 8;
                                return sub2.hostAsync();

                            case 8:
                                buff = _context2.sent;

                                assert(float.get(buff, 0 * float.size) === 1);
                                assert(float.get(buff, 1 * float.size) === 2);

                                arr.set(3, 2);
                                arr.set(4, 3);

                                _context2.next = 15;
                                return sub2.hostAsync();

                            case 15:
                                buff = _context2.sent;

                                assert(float.get(buff, 0 * float.size) === 1);
                                assert(float.get(buff, 1 * float.size) === 2);

                            case 18:
                            case "end":
                                return _context2.stop();
                        }
                    }
                }, _callee2, this);
            }))().nodeify(done);
        });

        describe("RAII", function () {
            describe("scope", function () {
                it("should exported as a function", function () {
                    assert(_.isFunction(af.scope));
                });

                it("should support RAII interface", function () {
                    assert(_.isFunction(af.scope.begin));
                    assert(_.isFunction(af.scope.end));
                    assert(_.isFunction(af.scope.result));
                    assert(_.isFunction(af.scope.register));
                });

                it("should destroy temporaries (sync)", function () {
                    var arr = undefined,
                        sub = undefined;
                    var x = af.scope(function () {
                        assert(this === af.scope);
                        arr = new af.AFArray(10, af.dType.f32);
                        arr.set(new af.Col(0), 0);
                        arr.set(3, 1);
                        arr.set(4, 2);

                        sub = arr.at(new af.Seq(3, 6));

                        this.result(arr);

                        return 1;
                    });

                    assert(x === 1);

                    arr.set(3, 2);

                    try {
                        sub.set(0, 2);
                        assert(false);
                    } catch (e) {
                        if (!/free\(\)/.test(e.message)) {
                            throw e;
                        }
                    }
                });

                it("should destroy registered arrays", function () {
                    var arr = new af.AFArray(10, af.dType.f32);
                    arr.set(new af.Col(0), 0);
                    arr.set(3, 1);
                    arr.set(4, 2);

                    var sub = arr.at(new af.Seq(3, 6));

                    af.scope(function () {
                        assert(this === af.scope);

                        sub.set(0, 0);

                        // Part of the scope, hence will be destroyed.
                        this.register(sub);
                    });

                    arr.set(3, 2);

                    try {
                        sub.set(0, 2);
                        assert(false);
                    } catch (e) {
                        if (!/free\(\)/.test(e.message)) {
                            throw e;
                        }
                    }
                });
            });
        });
    });
});
//# sourceMappingURL=afArrayTests.js.map
