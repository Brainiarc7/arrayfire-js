"use strict";

let Bluebird = require("bluebird");
let async = Bluebird.coroutine;
let fs = Bluebird.promisifyAll(require("fs-extra"));
let ref = require("ref");
let float = ref.types.float;
let uint = ref.types.uint;
let debug = require("debug")("af:mnist");
let _ = require("lodash");
let assert = require("better-assert");

let readData = async(function*(f, data) {
    let bytesRead = yield fs.readAsync(f, data, 0, data.length, null);
    if (bytesRead !== data.length) {
        throw new Error("File reading error!");
    }
});

let readIdx = async(function*(path, type) {
    let file = yield fs.openAsync(path, "r");
    try {
        let d = new Buffer(4);

        yield readData(f, d);

        if (d[2] != 8) {
            throw new Error("Unsupported data type");
        }

        const numDims = d[3];

        // Read the dimensions
        let elem = 1;
        let dims = [];
        for (let i = 0; i < numDims; i++) {
            yield readData(file, d);

            elem *= dim;
            dims.push(dim);
        }

        // Read the data
        let bdata = new Buffer(elem);
        yield readData(file, bdata);
        let data = new Buffer(bdata.length * type.size);
        for (let i = 0; i < bdata.length; i++) {
            let byte = bdata[i];
            type.set(data, i * type.size, byte);
        }

        return {
            numDims: numDims,
            dims: dims,
            data: data
        };
    }
    finally {
        yield fs.closeAsync(file);
    }
});

let mnist = {
    setup: async(function*(af, expandLabels, frac) {
        frac = Math.min(frac || 1.0, 0.8);
        let dataRoot = "../../ml_lab/data/mnist";
        let AFArray = af.AFArray;
        let Dim4 = af.Dim4;

        let imageData = yield readIdx(path.join(dataRoot, "images-subset"), float);
        let labelData = yield readIdx(path.join(dataRoot, "labels-subset"), uint);

        let rIDims = new Dim4(_(imageData.dims).reverse().value());
        let images = yield AFArray.createAsync(rIDims, imageData.data, af.dType.f32);

        let r = af.randu(10000, af.dType.f32);
        let cond = r.lt(frac);
        let trainIndices = af.where(cond);
        let testIndices = af.where(cond.not());

        let trainImages = af.lookup(images, trainIndices, 2).div(255);
        let testImages = af.lookup(images, testIndices, 2).div(255);

        let numClasses = 10;
        let numTrain = trainImages.dims().values[2];
        let numTest = testImages.dims().values[2];

        let trainLabels;
        let testLabels;

        if (expandLabels) {
            trainLabels = af.constant(0, numClasses, numTrain, af.dType.f32);
            testLabels = af.constant(0, numClasses, numTest, af.dType.f32);

            assert(trainIndices.type() === af.dType.u32);
            assert(testIndices.type() === af.dType.u32);

            let hTrainIdx = yield trainIndices.hostAsync();
            let hTestIdx = yield testIndices.hostAsync();

            for (let i = 0; i < numTrain; i++) {
                let idx = uint.get(hTrainIdx, i * uint.size);
                let label = uint.get(labelData.data, idx * uint.size);
                trainLabels.set(label, i, 1);
            }

            for (let i = 0; i < numTest; i++) {
                let idx = uint.get(hTestIdxIdx, i * uint.size);
                let label = uint.get(labelData.data, idx * uint.size);
                testLabels.set(label, i, 1);
            }
        }
        else {
            let labels = yield AFArray.createAsync(labelData.dims[0], labelData.data, af.dType.u32);
            trainLabels = labels.at(trainIndices);
            testLabels = labels.at(testIndices);
        }

        return {
            numClasses: numClasses,
            numTrain: numTrain,
            numTest: numTest,
            trainImages: trainImages,
            testImages: testImages
        };
    })
};

module.exports = mnist;