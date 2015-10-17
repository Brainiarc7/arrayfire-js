"use strict";
var assert = require("better-assert");
var _ = require("lodash");
var Bluebird = require("bluebird");
function testPlatform(id) {
  if (process.env["TEST_" + id] === "1") {
    describe(id + " platform", function() {
      var af = require("../..")(id);
      it("should return available devices", function() {
        var deviceCount = af.getDeviceCount();
        assert(deviceCount > 0);
        var infos = af.getDevices();
        assert(_.isArray(infos));
        assert(infos.length === deviceCount);
        var $__3 = true;
        var $__4 = false;
        var $__5 = undefined;
        try {
          for (var $__1 = void 0,
              $__0 = (infos)[Symbol.iterator](); !($__3 = ($__1 = $__0.next()).done); $__3 = true) {
            var info = $__1.value;
            {
              assert(_.isString(info.name) && info.name);
              assert(_.isString(info.platform) && info.platform);
              assert(_.isString(info.compute) && info.compute);
              assert(_.isBoolean(info.isDoubleAvailable));
            }
          }
        } catch ($__6) {
          $__4 = true;
          $__5 = $__6;
        } finally {
          try {
            if (!$__3 && $__0.return != null) {
              $__0.return();
            }
          } finally {
            if ($__4) {
              throw $__5;
            }
          }
        }
      });
      it("should do sync with callback", function(done) {
        af.sync(done);
      });
      it("should do sync with promise", function(done) {
        af.syncAsync().nodeify(done);
      });
      it("sync should failed if device is out of range", function(done) {
        af.syncAsync(100).then(function() {
          done(new Error("This should fail!"));
        }, function() {
          done();
        });
      });
    });
  }
}
describe("device methods", function() {
  testPlatform("CPU");
  testPlatform("OpenCL");
  testPlatform("CUDA");
});
//# sourceMappingURL=data:application/json;base64,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RztBQUM5QyxTQUFDLEtBQUssQUFBQyxDQUFDLElBQUcsQ0FBQyxDQUFDO01BQ2pCLENBQUMsQ0FBQztBQUVGLE9BQUMsQUFBQyxDQUFDLDZCQUE0QixDQUFHLFVBQVMsSUFBRyxDQUFHO0FBQzdDLFNBQUMsVUFBVSxBQUFDLEVBQUMsUUFBUSxBQUFDLENBQUMsSUFBRyxDQUFDLENBQUM7TUFDaEMsQ0FBQyxDQUFDO0FBRUYsT0FBQyxBQUFDLENBQUMsOENBQTZDLENBQUcsVUFBUyxJQUFHLENBQUc7QUFDOUQsU0FBQyxVQUFVLEFBQUMsQ0FBQyxHQUFFLENBQUMsS0FDUixBQUFDLENBQUMsU0FBUyxBQUFELENBQUc7QUFDYixhQUFHLEFBQUMsQ0FBQyxHQUFJLE1BQUksQUFBQyxDQUFDLG1CQUFrQixDQUFDLENBQUMsQ0FBQztRQUN4QyxDQUNBLFVBQVMsQUFBRCxDQUFHO0FBQ1AsYUFBRyxBQUFDLEVBQUMsQ0FBQztRQUNWLENBQUMsQ0FBQztNQUNWLENBQUMsQ0FBQztJQUNOLENBQUMsQ0FBQztFQUNOO0FBQUEsQUFDSjtBQUVBLE9BQU8sQUFBQyxDQUFDLGdCQUFlLENBQUcsVUFBUyxBQUFELENBQUc7QUFDbEMsYUFBVyxBQUFDLENBQUMsS0FBSSxDQUFDLENBQUM7QUFDbkIsYUFBVyxBQUFDLENBQUMsUUFBTyxDQUFDLENBQUM7QUFDdEIsYUFBVyxBQUFDLENBQUMsTUFBSyxDQUFDLENBQUM7QUFDeEIsQ0FBQyxDQUFDO0FBQ0YiLCJmaWxlIjoiZGV2aWNlVGVzdHMuanMiLCJzb3VyY2VSb290IjoidGVzdHMvZXM2Iiwic291cmNlc0NvbnRlbnQiOlsiLypcbkNvcHlyaWdodCAoYykgMjAxNC0yMDE1LCBBcnJheUZpcmVcbkNvcHlyaWdodCAoYykgMjAxNSBHw6Fib3IgTWV6xZEgYWthIHVuYm9ybmNoaWtrZW4gKGdhYm9yLm1lem9Ab3V0bG9vay5jb20pXG5BbGwgcmlnaHRzIHJlc2VydmVkLlxuXG5SZWRpc3RyaWJ1dGlvbiBhbmQgdXNlIGluIHNvdXJjZSBhbmQgYmluYXJ5IGZvcm1zLCB3aXRoIG9yIHdpdGhvdXQgbW9kaWZpY2F0aW9uLFxuYXJlIHBlcm1pdHRlZCBwcm92aWRlZCB0aGF0IHRoZSBmb2xsb3dpbmcgY29uZGl0aW9ucyBhcmUgbWV0OlxuXG4gKiBSZWRpc3RyaWJ1dGlvbnMgb2Ygc291cmNlIGNvZGUgbXVzdCByZXRhaW4gdGhlIGFib3ZlIGNvcHlyaWdodCBub3RpY2UsIHRoaXNcbiAgbGlzdCBvZiBjb25kaXRpb25zIGFuZCB0aGUgZm9sbG93aW5nIGRpc2NsYWltZXIuXG5cbiAqIFJlZGlzdHJpYnV0aW9ucyBpbiBiaW5hcnkgZm9ybSBtdXN0IHJlcHJvZHVjZSB0aGUgYWJvdmUgY29weXJpZ2h0IG5vdGljZSwgdGhpc1xuICBsaXN0IG9mIGNvbmRpdGlvbnMgYW5kIHRoZSBmb2xsb3dpbmcgZGlzY2xhaW1lciBpbiB0aGUgZG9jdW1lbnRhdGlvbiBhbmQvb3JcbiAgb3RoZXIgbWF0ZXJpYWxzIHByb3ZpZGVkIHdpdGggdGhlIGRpc3RyaWJ1dGlvbi5cblxuICogTmVpdGhlciB0aGUgbmFtZSBvZiB0aGUgQXJyYXlGaXJlIG5vciB0aGUgbmFtZXMgb2YgaXRzXG4gIGNvbnRyaWJ1dG9ycyBtYXkgYmUgdXNlZCB0byBlbmRvcnNlIG9yIHByb21vdGUgcHJvZHVjdHMgZGVyaXZlZCBmcm9tXG4gIHRoaXMgc29mdHdhcmUgd2l0aG91dCBzcGVjaWZpYyBwcmlvciB3cml0dGVuIHBlcm1pc3Npb24uXG5cblRISVMgU09GVFdBUkUgSVMgUFJPVklERUQgQlkgVEhFIENPUFlSSUdIVCBIT0xERVJTIEFORCBDT05UUklCVVRPUlMgXCJBUyBJU1wiIEFORFxuQU5ZIEVYUFJFU1MgT1IgSU1QTElFRCBXQVJSQU5USUVTLCBJTkNMVURJTkcsIEJVVCBOT1QgTElNSVRFRCBUTywgVEhFIElNUExJRURcbldBUlJBTlRJRVMgT0YgTUVSQ0hBTlRBQklMSVRZIEFORCBGSVRORVNTIEZPUiBBIFBBUlRJQ1VMQVIgUFVSUE9TRSBBUkVcbkRJU0NMQUlNRUQuIElOIE5PIEVWRU5UIFNIQUxMIFRIRSBDT1BZUklHSFQgSE9MREVSIE9SIENPTlRSSUJVVE9SUyBCRSBMSUFCTEUgRk9SXG5BTlkgRElSRUNULCBJTkRJUkVDVCwgSU5DSURFTlRBTCwgU1BFQ0lBTCwgRVhFTVBMQVJZLCBPUiBDT05TRVFVRU5USUFMIERBTUFHRVNcbihJTkNMVURJTkcsIEJVVCBOT1QgTElNSVRFRCBUTywgUFJPQ1VSRU1FTlQgT0YgU1VCU1RJVFVURSBHT09EUyBPUiBTRVJWSUNFUztcbkxPU1MgT0YgVVNFLCBEQVRBLCBPUiBQUk9GSVRTOyBPUiBCVVNJTkVTUyBJTlRFUlJVUFRJT04pIEhPV0VWRVIgQ0FVU0VEIEFORCBPTlxuQU5ZIFRIRU9SWSBPRiBMSUFCSUxJVFksIFdIRVRIRVIgSU4gQ09OVFJBQ1QsIFNUUklDVCBMSUFCSUxJVFksIE9SIFRPUlRcbihJTkNMVURJTkcgTkVHTElHRU5DRSBPUiBPVEhFUldJU0UpIEFSSVNJTkcgSU4gQU5ZIFdBWSBPVVQgT0YgVEhFIFVTRSBPRiBUSElTXG5TT0ZUV0FSRSwgRVZFTiBJRiBBRFZJU0VEIE9GIFRIRSBQT1NTSUJJTElUWSBPRiBTVUNIIERBTUFHRS5cbiovXG5cblwidXNlIHN0cmljdFwiO1xuXG5sZXQgYXNzZXJ0ID0gcmVxdWlyZShcImJldHRlci1hc3NlcnRcIik7XG5sZXQgXyA9IHJlcXVpcmUoXCJsb2Rhc2hcIik7XG5sZXQgQmx1ZWJpcmQgPSByZXF1aXJlKFwiYmx1ZWJpcmRcIik7XG5cbmZ1bmN0aW9uIHRlc3RQbGF0Zm9ybShpZCkge1xuICAgIGlmIChwcm9jZXNzLmVudltcIlRFU1RfXCIgKyBpZF0gPT09IFwiMVwiKSB7XG4gICAgICAgIGRlc2NyaWJlKGlkICsgXCIgcGxhdGZvcm1cIiwgZnVuY3Rpb24gKCkge1xuICAgICAgICAgICAgbGV0IGFmID0gcmVxdWlyZShcIi4uLy4uXCIpKGlkKTtcblxuICAgICAgICAgICAgaXQoXCJzaG91bGQgcmV0dXJuIGF2YWlsYWJsZSBkZXZpY2VzXCIsIGZ1bmN0aW9uKCkge1xuICAgICAgICAgICAgICAgIGxldCBkZXZpY2VDb3VudCA9IGFmLmdldERldmljZUNvdW50KCk7XG4gICAgICAgICAgICAgICAgYXNzZXJ0KGRldmljZUNvdW50ID4gMCk7XG4gICAgICAgICAgICAgICAgbGV0IGluZm9zID0gYWYuZ2V0RGV2aWNlcygpO1xuICAgICAgICAgICAgICAgIGFzc2VydChfLmlzQXJyYXkoaW5mb3MpKTtcbiAgICAgICAgICAgICAgICBhc3NlcnQoaW5mb3MubGVuZ3RoID09PSBkZXZpY2VDb3VudCk7XG4gICAgICAgICAgICAgICAgZm9yIChsZXQgaW5mbyBvZiBpbmZvcykge1xuICAgICAgICAgICAgICAgICAgICBhc3NlcnQoXy5pc1N0cmluZyhpbmZvLm5hbWUpICYmIGluZm8ubmFtZSk7XG4gICAgICAgICAgICAgICAgICAgIGFzc2VydChfLmlzU3RyaW5nKGluZm8ucGxhdGZvcm0pICYmIGluZm8ucGxhdGZvcm0pO1xuICAgICAgICAgICAgICAgICAgICBhc3NlcnQoXy5pc1N0cmluZyhpbmZvLmNvbXB1dGUpICYmIGluZm8uY29tcHV0ZSk7XG4gICAgICAgICAgICAgICAgICAgIGFzc2VydChfLmlzQm9vbGVhbihpbmZvLmlzRG91YmxlQXZhaWxhYmxlKSk7XG4gICAgICAgICAgICAgICAgfVxuICAgICAgICAgICAgfSk7XG5cbiAgICAgICAgICAgIGl0KFwic2hvdWxkIGRvIHN5bmMgd2l0aCBjYWxsYmFja1wiLCBmdW5jdGlvbihkb25lKSB7XG4gICAgICAgICAgICAgICAgYWYuc3luYyhkb25lKTtcbiAgICAgICAgICAgIH0pO1xuXG4gICAgICAgICAgICBpdChcInNob3VsZCBkbyBzeW5jIHdpdGggcHJvbWlzZVwiLCBmdW5jdGlvbihkb25lKSB7XG4gICAgICAgICAgICAgICAgYWYuc3luY0FzeW5jKCkubm9kZWlmeShkb25lKTtcbiAgICAgICAgICAgIH0pO1xuXG4gICAgICAgICAgICBpdChcInN5bmMgc2hvdWxkIGZhaWxlZCBpZiBkZXZpY2UgaXMgb3V0IG9mIHJhbmdlXCIsIGZ1bmN0aW9uKGRvbmUpIHtcbiAgICAgICAgICAgICAgICBhZi5zeW5jQXN5bmMoMTAwKVxuICAgICAgICAgICAgICAgICAgICAudGhlbihmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgICAgICAgICAgIGRvbmUobmV3IEVycm9yKFwiVGhpcyBzaG91bGQgZmFpbCFcIikpO1xuICAgICAgICAgICAgICAgICAgICB9LFxuICAgICAgICAgICAgICAgICAgICBmdW5jdGlvbigpIHtcbiAgICAgICAgICAgICAgICAgICAgICAgIGRvbmUoKTtcbiAgICAgICAgICAgICAgICAgICAgfSk7XG4gICAgICAgICAgICB9KTtcbiAgICAgICAgfSk7XG4gICAgfVxufVxuXG5kZXNjcmliZShcImRldmljZSBtZXRob2RzXCIsIGZ1bmN0aW9uKCkge1xuICAgIHRlc3RQbGF0Zm9ybShcIkNQVVwiKTtcbiAgICB0ZXN0UGxhdGZvcm0oXCJPcGVuQ0xcIik7XG4gICAgdGVzdFBsYXRmb3JtKFwiQ1VEQVwiKTtcbn0pO1xuIl19
