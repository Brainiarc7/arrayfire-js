"use strict";
"use strict";
var assert = require("better-assert");
var _ = require("lodash");
var ref = require("ref");
var Bluebird = require("bluebird");
var async = Bluebird.coroutine;
var int = ref.types.int;
var float = ref.types.float;
function testPlatform(id) {
  if (process.env["TEST_" + id] === "1") {
    describe(id + " platform", function() {
      var fire = Bluebird.promisifyAll(require("../..")(id));
      describe("randu", function() {
        it("should yield uniform random int array with 2 dimensions", function(done) {
          var f = async($traceurRuntime.initGeneratorFunction(function $__0() {
            var array,
                data,
                i,
                v;
            return $traceurRuntime.createGeneratorInstance(function($ctx) {
              while (true)
                switch ($ctx.state) {
                  case 0:
                    $ctx.state = 2;
                    return fire.randuAsync(2, 4, fire.types.dtype.s32);
                  case 2:
                    array = $ctx.sent;
                    $ctx.state = 4;
                    break;
                  case 4:
                    $ctx.state = 6;
                    return array.copyToHostAsync();
                  case 6:
                    data = $ctx.sent;
                    $ctx.state = 8;
                    break;
                  case 8:
                    assert(data instanceof Buffer);
                    assert(data.length == 2 * 4 * int.size);
                    for (i = 0; i < data.length / int.size; i++) {
                      v = int.get(data, i * int.size);
                      assert(v >= Number.MIN_SAFE_INTEGER && v <= Number.MAX_SAFE_INTEGER);
                      assert(Math.floor(v) === v);
                    }
                    $ctx.state = -2;
                    break;
                  default:
                    return $ctx.end();
                }
            }, $__0, this);
          }));
          f().nodeify(done);
        });
        it("should yield uniform random float array with 2 dimensions", function(done) {
          var f = async($traceurRuntime.initGeneratorFunction(function $__0() {
            var array,
                data,
                i,
                v;
            return $traceurRuntime.createGeneratorInstance(function($ctx) {
              while (true)
                switch ($ctx.state) {
                  case 0:
                    $ctx.state = 2;
                    return fire.randuAsync([2, 4], fire.types.dtype.f32);
                  case 2:
                    array = $ctx.sent;
                    $ctx.state = 4;
                    break;
                  case 4:
                    $ctx.state = 6;
                    return array.copyToHostAsync();
                  case 6:
                    data = $ctx.sent;
                    $ctx.state = 8;
                    break;
                  case 8:
                    assert(data instanceof Buffer);
                    assert(data.length == 2 * 4 * float.size);
                    for (i = 0; i < data.length / float.size; i++) {
                      v = float.get(data, i * float.size);
                      assert(v === 0 || v === 1.0 || (v > 0 && v < 1.0 && v % 1));
                    }
                    $ctx.state = -2;
                    break;
                  default:
                    return $ctx.end();
                }
            }, $__0, this);
          }));
          f().nodeify(done);
        });
      });
      describe("randf", function() {
        it("should throw error when invoking normal random int array with 2 dimensions", function(done) {
          var f = async($traceurRuntime.initGeneratorFunction(function $__0() {
            var array;
            return $traceurRuntime.createGeneratorInstance(function($ctx) {
              while (true)
                switch ($ctx.state) {
                  case 0:
                    $ctx.state = 2;
                    return fire.randnAsync(2, 4, fire.types.dtype.s32);
                  case 2:
                    array = $ctx.sent;
                    $ctx.state = -2;
                    break;
                  default:
                    return $ctx.end();
                }
            }, $__0, this);
          }));
          f().then(function() {
            done(new Error("This should throw."));
          }, function(e) {
            if (/invalid dtype argument/ig.test(e.message)) {
              done();
            } else {
              done(new Error("This should throw appropriate error."));
            }
          });
        });
        it("should yield normal random float array with 2 dimensions", function(done) {
          var f = async($traceurRuntime.initGeneratorFunction(function $__0() {
            var array,
                data,
                i,
                v;
            return $traceurRuntime.createGeneratorInstance(function($ctx) {
              while (true)
                switch ($ctx.state) {
                  case 0:
                    $ctx.state = 2;
                    return fire.randnAsync([2, 4], fire.types.dtype.f32);
                  case 2:
                    array = $ctx.sent;
                    $ctx.state = 4;
                    break;
                  case 4:
                    $ctx.state = 6;
                    return array.copyToHostAsync();
                  case 6:
                    data = $ctx.sent;
                    $ctx.state = 8;
                    break;
                  case 8:
                    assert(data instanceof Buffer);
                    assert(data.length == 2 * 4 * float.size);
                    for (i = 0; i < data.length / float.size; i++) {
                      v = float.get(data, i * float.size);
                      assert(v === 0 || (v > -4.0 && v < 4.0 && v % 1));
                    }
                    $ctx.state = -2;
                    break;
                  default:
                    return $ctx.end();
                }
            }, $__0, this);
          }));
          f().nodeify(done);
        });
      });
      describe("identity", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement identity test for " + id + "\n"));
        });
      });
      describe("range", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement range test for " + id + "\n"));
        });
      });
      describe("iota", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement iota test for " + id + "\n"));
        });
      });
      describe("diag", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement diag test for " + id + "\n"));
        });
      });
      describe("constant", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement constant test for " + id + "\n"));
        });
      });
    });
  }
}
describe("Functions to create arrays", function() {
  testPlatform("CPU");
  testPlatform("OpenCL");
  testPlatform("CUDA");
});
//# sourceMappingURL=data:application/json;base64,eyJ2ZXJzaW9uIjozLCJzb3VyY2VzIjpbImNyZWF0ZUFycmF5RnVuY3Rpb25UZXN0cy5qcyJdLCJuYW1lcyI6W10sIm1hcHBpbmdzIjoiQUFBQTtBQUFBLFdBQVcsQ0FBQztBQUVaLEFBQUksRUFBQSxDQUFBLE1BQUssRUFBSSxDQUFBLE9BQU0sQUFBQyxDQUFDLGVBQWMsQ0FBQyxDQUFDO0FBQ3JDLEFBQUksRUFBQSxDQUFBLENBQUEsRUFBSSxDQUFBLE9BQU0sQUFBQyxDQUFDLFFBQU8sQ0FBQyxDQUFDO0FBQ3pCLEFBQUksRUFBQSxDQUFBLEdBQUUsRUFBSSxDQUFBLE9BQU0sQUFBQyxDQUFDLEtBQUksQ0FBQyxDQUFDO0FBQ3hCLEFBQUksRUFBQSxDQUFBLFFBQU8sRUFBSSxDQUFBLE9BQU0sQUFBQyxDQUFDLFVBQVMsQ0FBQyxDQUFDO0FBQ2xDLEFBQUksRUFBQSxDQUFBLEtBQUksRUFBSSxDQUFBLFFBQU8sVUFBVSxDQUFDO0FBQzlCLEFBQUksRUFBQSxDQUFBLEdBQUUsRUFBSSxDQUFBLEdBQUUsTUFBTSxJQUFJLENBQUM7QUFDdkIsQUFBSSxFQUFBLENBQUEsS0FBSSxFQUFJLENBQUEsR0FBRSxNQUFNLE1BQU0sQ0FBQztBQUUzQixPQUFTLGFBQVcsQ0FBRyxFQUFDO0FBQ3BCLEtBQUksT0FBTSxJQUFJLENBQUUsT0FBTSxFQUFJLEdBQUMsQ0FBQyxJQUFNLElBQUUsQ0FBRztBQUNuQyxXQUFPLEFBQUMsQ0FBQyxFQUFDLEVBQUksWUFBVSxDQUFHLFVBQVUsQUFBRDtBQUNoQyxBQUFJLFFBQUEsQ0FBQSxJQUFHLEVBQUksQ0FBQSxRQUFPLGFBQWEsQUFBQyxDQUFDLE9BQU0sQUFBQyxDQUFDLE9BQU0sQ0FBQyxBQUFDLENBQUMsRUFBQyxDQUFDLENBQUMsQ0FBQztBQUV0RCxhQUFPLEFBQUMsQ0FBQyxPQUFNLENBQUcsVUFBVSxBQUFEO0FBQ3ZCLFNBQUMsQUFBQyxDQUFDLHlEQUF3RCxDQUFHLFVBQVUsSUFBRztBQUN2RSxBQUFJLFlBQUEsQ0FBQSxDQUFBLEVBQUksQ0FBQSxLQUFJLEFB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QUFBQyxDQUFDLHVCQUFzQixDQUFHLFVBQVMsQUFBRCxDQUFHO0FBQ25DLGdCQUFNLElBQUksQUFBQyxFQUFDLGdDQUFnQyxFQUFDLEdBQUMsRUFBQyxLQUFHLEVBQUMsQ0FBQztRQUN4RCxDQUFDLENBQUM7TUFDTixDQUFDLENBQUM7QUFFRixhQUFPLEFBQUMsQ0FBQyxVQUFTLENBQUcsVUFBVSxBQUFELENBQUc7QUFDN0IsU0FBQyxBQUFDLENBQUMsdUJBQXNCLENBQUcsVUFBUyxBQUFELENBQUc7QUFDbkMsZ0JBQU0sSUFBSSxBQUFDLEVBQUMsb0NBQW9DLEVBQUMsR0FBQyxFQUFDLEtBQUcsRUFBQyxDQUFDO1FBQzVELENBQUMsQ0FBQztNQUNOLENBQUMsQ0FBQztJQUNOLENBQUMsQ0FBQztFQUNOO0FBQUEsQUFDSjtBQUVBLE9BQU8sQUFBQyxDQUFDLDRCQUEyQixDQUFHLFVBQVUsQUFBRCxDQUFHO0FBQy9DLGFBQVcsQUFBQyxDQUFDLEtBQUksQ0FBQyxDQUFDO0FBQ25CLGFBQVcsQUFBQyxDQUFDLFFBQU8sQ0FBQyxDQUFDO0FBQ3RCLGFBQVcsQUFBQyxDQUFDLE1BQUssQ0FBQyxDQUFDO0FBQ3hCLENBQUMsQ0FBQztBQUFBIiwiZmlsZSI6ImNyZWF0ZUFycmF5RnVuY3Rpb25UZXN0cy5qcyIsInNvdXJjZVJvb3QiOiJ0ZXN0cy9lczYiLCJzb3VyY2VzQ29udGVudCI6WyJcInVzZSBzdHJpY3RcIjtcclxuXHJcbmxldCBhc3NlcnQgPSByZXF1aXJlKFwiYmV0dGVyLWFzc2VydFwiKTtcclxubGV0IF8gPSByZXF1aXJlKFwibG9kYXNoXCIpO1xyXG5sZXQgcmVmID0gcmVxdWlyZShcInJlZlwiKTtcclxubGV0IEJsdWViaXJkID0gcmVxdWlyZShcImJsdWViaXJkXCIpO1xyXG5sZXQgYXN5bmMgPSBCbHVlYmlyZC5jb3JvdXRpbmU7XHJcbmxldCBpbnQgPSByZWYudHlwZXMuaW50O1xyXG5sZXQgZmxvYXQgPSByZWYudHlwZXMuZmxvYXQ7XHJcblxyXG5mdW5jdGlvbiB0ZXN0UGxhdGZvcm0gKGlkKSB7XHJcbiAgICBpZiAocHJvY2Vzcy5lbnZbXCJURVNUX1wiICsgaWRdID09PSBcIjFcIikge1xyXG4gICAgICAgIGRlc2NyaWJlKGlkICsgXCIgcGxhdGZvcm1cIiwgZnVuY3Rpb24gKCkge1xyXG4gICAgICAgICAgICBsZXQgZmlyZSA9IEJsdWViaXJkLnByb21pc2lmeUFsbChyZXF1aXJlKFwiLi4vLi5cIikoaWQpKTtcclxuXHJcbiAgICAgICAgICAgIGRlc2NyaWJlKFwicmFuZHVcIiwgZnVuY3Rpb24gKCkge1xyXG4gICAgICAgICAgICAgICAgaXQoXCJzaG91bGQgeWllbGQgdW5pZm9ybSByYW5kb20gaW50IGFycmF5IHdpdGggMiBkaW1lbnNpb25zXCIsIGZ1bmN0aW9uIChkb25lKSB7XHJcbiAgICAgICAgICAgICAgICAgICAgbGV0IGYgPSBhc3luYyhmdW5jdGlvbiooKSB7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgIGxldCBhcnJheSA9IHlpZWxkIGZpcmUucmFuZHVBc3luYygyLCA0LCBmaXJlLnR5cGVzLmR0eXBlLnMzMik7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgIGxldCBkYXRhID0geWllbGQgYXJyYXkuY29weVRvSG9zdEFzeW5jKCk7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgIGFzc2VydChkYXRhIGluc3RhbmNlb2YgQnVmZmVyKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgYXNzZXJ0KGRhdGEubGVuZ3RoID09IDIgKiA0ICogaW50LnNpemUpO1xyXG5cclxuICAgICAgICAgICAgICAgICAgICAgICAgZm9yIChsZXQgaSA9IDA7IGkgPCBkYXRhLmxlbmd0aCAvIGludC5zaXplOyBpKyspIHtcclxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIGNvbnN0IHYgPSBpbnQuZ2V0KGRhdGEsIGkgKiBpbnQuc2l6ZSk7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgICAgICBhc3NlcnQodiA+PSBOdW1iZXIuTUlOX1NBRkVfSU5URUdFUiAmJiB2IDw9IE51bWJlci5NQVhfU0FGRV9JTlRFR0VSKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIGFzc2VydChNYXRoLmZsb29yKHYpID09PSB2KTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgfVxyXG4gICAgICAgICAgICAgICAgICAgIH0pO1xyXG4gICAgICAgICAgICAgICAgICAgIGYoKS5ub2RlaWZ5KGRvbmUpO1xyXG4gICAgICAgICAgICAgICAgfSk7XHJcbiAgICAgICAgICAgICAgICBpdChcInNob3VsZCB5aWVsZCB1bmlmb3JtIHJhbmRvbSBmbG9hdCBhcnJheSB3aXRoIDIgZGltZW5zaW9uc1wiLCBmdW5jdGlvbiAoZG9uZSkge1xyXG4gICAgICAgICAgICAgICAgICAgIGxldCBmID0gYXN5bmMoZnVuY3Rpb24qKCkge1xyXG4gICAgICAgICAgICAgICAgICAgICAgICBsZXQgYXJyYXkgPSB5aWVsZCBmaXJlLnJhbmR1QXN5bmMoWzIsIDRdLCBmaXJlLnR5cGVzLmR0eXBlLmYzMik7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgIGxldCBkYXRhID0geWllbGQgYXJyYXkuY29weVRvSG9zdEFzeW5jKCk7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgIGFzc2VydChkYXRhIGluc3RhbmNlb2YgQnVmZmVyKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgYXNzZXJ0KGRhdGEubGVuZ3RoID09IDIgKiA0ICogZmxvYXQuc2l6ZSk7XHJcblxyXG4gICAgICAgICAgICAgICAgICAgICAgICBmb3IgKGxldCBpID0gMDsgaSA8IGRhdGEubGVuZ3RoIC8gZmxvYXQuc2l6ZTsgaSsrKSB7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgICAgICBjb25zdCB2ID0gZmxvYXQuZ2V0KGRhdGEsIGkgKiBmbG9hdC5zaXplKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIGFzc2VydCh2ID09PSAwIHx8IHYgPT09IDEuMCB8fCAodiA+IDAgJiYgdiA8IDEuMCAmJiB2ICUgMSkpO1xyXG4gICAgICAgICAgICAgICAgICAgICAgICB9XHJcbiAgICAgICAgICAgICAgICAgICAgfSk7XHJcbiAgICAgICAgICAgICAgICAgICAgZigpLm5vZGVpZnkoZG9uZSk7XHJcbiAgICAgICAgICAgICAgICB9KTtcclxuICAgICAgICAgICAgfSk7XHJcblxyXG4gICAgICAgICAgICBkZXNjcmliZShcInJhbmRmXCIsIGZ1bmN0aW9uICgpIHtcclxuICAgICAgICAgICAgICAgIGl0KFwic2hvdWxkIHRocm93IGVycm9yIHdoZW4gaW52b2tpbmcgbm9ybWFsIHJhbmRvbSBpbnQgYXJyYXkgd2l0aCAyIGRpbWVuc2lvbnNcIiwgZnVuY3Rpb24gKGRvbmUpIHtcclxuICAgICAgICAgICAgICAgICAgICBsZXQgZiA9IGFzeW5jKGZ1bmN0aW9uKigpIHtcclxuICAgICAgICAgICAgICAgICAgICAgICAgbGV0IGFycmF5ID0geWllbGQgZmlyZS5yYW5kbkFzeW5jKDIsIDQsIGZpcmUudHlwZXMuZHR5cGUuczMyKTtcclxuICAgICAgICAgICAgICAgICAgICB9KTtcclxuICAgICAgICAgICAgICAgICAgICBmKClcclxuICAgICAgICAgICAgICAgICAgICAgICAgLnRoZW4oZnVuY3Rpb24gKCkge1xyXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgZG9uZShuZXcgRXJyb3IoXCJUaGlzIHNob3VsZCB0aHJvdy5cIikpO1xyXG4gICAgICAgICAgICAgICAgICAgICAgICB9LFxyXG4gICAgICAgICAgICAgICAgICAgICAgICBmdW5jdGlvbiAoZSkge1xyXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgaWYgKC9pbnZhbGlkIGR0eXBlIGFyZ3VtZW50L2lnLnRlc3QoZS5tZXNzYWdlKSkge1xyXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIGRvbmUoKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIH1cclxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIGVsc2Uge1xyXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIGRvbmUobmV3IEVycm9yKFwiVGhpcyBzaG91bGQgdGhyb3cgYXBwcm9wcmlhdGUgZXJyb3IuXCIpKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIH1cclxuICAgICAgICAgICAgICAgICAgICAgICAgfSk7XHJcblxyXG4gICAgICAgICAgICAgICAgfSk7XHJcbiAgICAgICAgICAgICAgICBpdChcInNob3VsZCB5aWVsZCBub3JtYWwgcmFuZG9tIGZsb2F0IGFycmF5IHdpdGggMiBkaW1lbnNpb25zXCIsIGZ1bmN0aW9uIChkb25lKSB7XHJcbiAgICAgICAgICAgICAgICAgICAgbGV0IGYgPSBhc3luYyhmdW5jdGlvbiooKSB7XHJcbiAgICAgICAgICAgICAgICAgICAgICAgIGxldCBhcnJheSA9IHlpZWxkIGZpcmUucmFuZG5Bc3luYyhbMiwgNF0sIGZpcmUudHlwZXMuZHR5cGUuZjMyKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgbGV0IGRhdGEgPSB5aWVsZCBhcnJheS5jb3B5VG9Ib3N0QXN5bmMoKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgYXNzZXJ0KGRhdGEgaW5zdGFuY2VvZiBCdWZmZXIpO1xyXG4gICAgICAgICAgICAgICAgICAgICAgICBhc3NlcnQoZGF0YS5sZW5ndGggPT0gMiAqIDQgKiBmbG9hdC5zaXplKTtcclxuXHJcbiAgICAgICAgICAgICAgICAgICAgICAgIGZvciAobGV0IGkgPSAwOyBpIDwgZGF0YS5sZW5ndGggLyBmbG9hdC5zaXplOyBpKyspIHtcclxuICAgICAgICAgICAgICAgICAgICAgICAgICAgIGNvbnN0IHYgPSBmbG9hdC5nZXQoZGF0YSwgaSAqIGZsb2F0LnNpemUpO1xyXG4gICAgICAgICAgICAgICAgICAgICAgICAgICAgYXNzZXJ0KHYgPT09IDAgfHwgKHYgPiAtNC4wICYmIHYgPCA0LjAgJiYgdiAlIDEpKTtcclxuICAgICAgICAgICAgICAgICAgICAgICAgfVxyXG4gICAgICAgICAgICAgICAgICAgIH0pO1xyXG4gICAgICAgICAgICAgICAgICAgIGYoKS5ub2RlaWZ5KGRvbmUpO1xyXG4gICAgICAgICAgICAgICAgfSk7XHJcbiAgICAgICAgICAgIH0pO1xyXG5cclxuICAgICAgICAgICAgZGVzY3JpYmUoXCJpZGVudGl0eVwiLCBmdW5jdGlvbiAoKSB7XHJcbiAgICAgICAgICAgICAgICBpdChcInNob3VsZCBiZSBpbXBsZW1lbnRlZFwiLCBmdW5jdGlvbigpIHtcclxuICAgICAgICAgICAgICAgICAgICBjb25zb2xlLmxvZyhgVE9ETzogaW1wbGVtZW50IGlkZW50aXR5IHRlc3QgZm9yICR7aWR9XFxuYCk7XHJcbiAgICAgICAgICAgICAgICB9KTtcclxuICAgICAgICAgICAgfSk7XHJcblxyXG4gICAgICAgICAgICBkZXNjcmliZShcInJhbmdlXCIsIGZ1bmN0aW9uICgpIHtcclxuICAgICAgICAgICAgICAgIGl0KFwic2hvdWxkIGJlIGltcGxlbWVudGVkXCIsIGZ1bmN0aW9uKCkge1xyXG4gICAgICAgICAgICAgICAgICAgIGNvbnNvbGUubG9nKGBUT0RPOiBpbXBsZW1lbnQgcmFuZ2UgdGVzdCBmb3IgJHtpZH1cXG5gKTtcclxuICAgICAgICAgICAgICAgIH0pO1xyXG4gICAgICAgICAgICB9KTtcclxuXHJcbiAgICAgICAgICAgIGRlc2NyaWJlKFwiaW90YVwiLCBmdW5jdGlvbiAoKSB7XHJcbiAgICAgICAgICAgICAgICBpdChcInNob3VsZCBiZSBpbXBsZW1lbnRlZFwiLCBmdW5jdGlvbigpIHtcclxuICAgICAgICAgICAgICAgICAgICBjb25zb2xlLmxvZyhgVE9ETzogaW1wbGVtZW50IGlvdGEgdGVzdCBmb3IgJHtpZH1cXG5gKTtcclxuICAgICAgICAgICAgICAgIH0pO1xyXG4gICAgICAgICAgICB9KTtcclxuXHJcbiAgICAgICAgICAgIGRlc2NyaWJlKFwiZGlhZ1wiLCBmdW5jdGlvbiAoKSB7XHJcbiAgICAgICAgICAgICAgICBpdChcInNob3VsZCBiZSBpbXBsZW1lbnRlZFwiLCBmdW5jdGlvbigpIHtcclxuICAgICAgICAgICAgICAgICAgICBjb25zb2xlLmxvZyhgVE9ETzogaW1wbGVtZW50IGRpYWcgdGVzdCBmb3IgJHtpZH1cXG5gKTtcclxuICAgICAgICAgICAgICAgIH0pO1xyXG4gICAgICAgICAgICB9KTtcclxuXHJcbiAgICAgICAgICAgIGRlc2NyaWJlKFwiY29uc3RhbnRcIiwgZnVuY3Rpb24gKCkge1xyXG4gICAgICAgICAgICAgICAgaXQoXCJzaG91bGQgYmUgaW1wbGVtZW50ZWRcIiwgZnVuY3Rpb24oKSB7XHJcbiAgICAgICAgICAgICAgICAgICAgY29uc29sZS5sb2coYFRPRE86IGltcGxlbWVudCBjb25zdGFudCB0ZXN0IGZvciAke2lkfVxcbmApO1xyXG4gICAgICAgICAgICAgICAgfSk7XHJcbiAgICAgICAgICAgIH0pO1xyXG4gICAgICAgIH0pO1xyXG4gICAgfVxyXG59XHJcblxyXG5kZXNjcmliZShcIkZ1bmN0aW9ucyB0byBjcmVhdGUgYXJyYXlzXCIsIGZ1bmN0aW9uICgpIHtcclxuICAgIHRlc3RQbGF0Zm9ybShcIkNQVVwiKTtcclxuICAgIHRlc3RQbGF0Zm9ybShcIk9wZW5DTFwiKTtcclxuICAgIHRlc3RQbGF0Zm9ybShcIkNVREFcIik7XHJcbn0pOyJdfQ==
