const assert = require('assert');
const position = require("./position.js");
const config = require('./config.js');

async function checkPosition(pos, tolerance = config.tolerance) {
    const obj = await position.set(pos);
    const ps = obj['target-position'];
    const lengthX = ps['claw-x'];
    const lengthY = ps['claw-y'];
    const lengthZ = ps['claw-z'];
    const tcax = pos['claw-angle-x'];
    const la = obj['target-logical-angles'];
    const cax = la['claw-angle-x'];
    if (tcax != null)
        assert.strictEqual(position.isEqual(cax, tcax, tolerance), true, `claw-angle-x ${cax} is incorrect`);

    assert.strictEqual(position.isEqual(lengthX, pos["claw-x"], tolerance), true, `X ${lengthX} is incorrect`);
    assert.strictEqual(position.isEqual(lengthY, pos["claw-y"], tolerance), true, `Y ${lengthY} is incorrect`);
    assert.strictEqual(position.isEqual(lengthZ, pos["claw-z"], tolerance), true, `Z ${lengthZ} is incorrect`);
}


describe('Positioning', function() {
    describe('Set position free angle', function () {
        it ('130 0 80', async function () {
            await checkPosition({ "claw-x": 130, "claw-y": 0, "claw-z": 80 });
        });

        it ('-130 0 80', async function () {
            await checkPosition({ "claw-x": -130, "claw-y": 0, "claw-z": 80, "iterations": 60, "iteration-delay": 60 });
        });

        it ('-130 80 80', async function () {
            await checkPosition({ "claw-x": -130, "claw-y": 80, "claw-z": 80 });
        });

        it ('0 130 80', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": 130, "claw-z": 80, "iterations": 60, "iteration-delay": 60 });
        });

        it ('0 -130 80', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": -130, "claw-z": 80, "iterations": 60, "iteration-delay": 60  });
        });

        it ('140 140 80', async function () {
            await checkPosition({ "claw-x": 140, "claw-y": 140, "claw-z": 80 });
        });

        it ('-140 140 80', async function () {
            await checkPosition({ "claw-x": -140, "claw-y": 140, "claw-z": 80 });
        });

        it ('140 -140 80', async function () {
            await checkPosition({ "claw-x": 140, "claw-y": -140, "claw-z": 80 });
        });

        it ('-140 -140 80', async function () {
            await checkPosition({ "claw-x": -140, "claw-y": -140, "claw-z": 80 });
        });

        it ('26 0 290', async function () {
            await checkPosition({ "claw-x": 26, "claw-y": 0, "claw-z": 290 });
        });

        it ('-26 0 290', async function () {
            await checkPosition({ "claw-x": -26, "claw-y": 0, "claw-z": 290, "iterations": 60, "iteration-delay": 60 });
        });

        it ('0 26 290', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": 26, "claw-z": 290 });
        });

        it ('0 -26 290', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": -26, "claw-z": 290 });
        });

        it ('26 26 290', async function () {
            await checkPosition({ "claw-x": 26, "claw-y": 26, "claw-z": 290 });
        });

        it ('-26 26 290', async function () {
            await checkPosition({ "claw-x": -26, "claw-y": 26, "claw-z": 290, "iterations": 60, "iteration-delay": 60  });
        });

        it ('95 0 80, 90', async function () {
            await checkPosition({ "claw-x": 130, "claw-y": 0, "claw-z": 80, "claw-angle-x": 90 });
        });

        it ('95 0 80, 0', async function () {
            await checkPosition({ "claw-x": 130, "claw-y": 0, "claw-z": 80, "claw-angle-x": 0 });
        });

    });

    describe('Set position angle is 0', function () {
        it ('250 0 80, 0', async function () {
            await checkPosition({ "claw-x": 400, "claw-y": 0, "claw-z": 100, "claw-angle-y": 0 });
        });

        it ('250 0 250, 0', async function () {
            await checkPosition({ "claw-x": 250, "claw-y": 0, "claw-z": 250, "claw-angle-y": 0 });
        });

        it ('150 0 250, 0', async function () {
            await checkPosition({ "claw-x": 250, "claw-y": 0, "claw-z": 250, "claw-angle-y": 0 });
        });
    });

    describe('Set position angle is 90', function () {
        it ('95 0 500, 90', async function () {
            await checkPosition({ "claw-x": 95, "claw-y": 0, "claw-z": 500, "claw-angle-y": 90 });
        });

        it ('150 0 420, 90', async function () {
            await checkPosition({ "claw-x": 150, "claw-y": 0, "claw-z": 420, "claw-angle-y": 90 });
        });
    });

    describe('Set to home', function () {
        it ('95 0 25', async function () {
            await checkPosition({ "claw-x": 95, "claw-y": 0, "claw-z": 25 });
        });
    });
});
