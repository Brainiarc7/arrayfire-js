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
      var af = require("../..")(id);
      describe("join", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement join test for " + id + "\n"));
        });
      });
      describe("tile", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement tile test for " + id + "\n"));
        });
      });
      describe("reorder", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement iota reorder for " + id + "\n"));
        });
      });
      describe("shift", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement shift test for " + id + "\n"));
        });
      });
      describe("moddims", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement moddims test for " + id + "\n"));
        });
      });
      describe("flat", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement flat test for " + id + "\n"));
        });
      });
      describe("flip", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement flip test for " + id + "\n"));
        });
      });
      describe("transpose", function() {
        it("should be implemented", function() {
          console.log(("TODO: implement transpose test for " + id + "\n"));
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

//# sourceMappingURL=moveAndReorderArrayTests.js.map
