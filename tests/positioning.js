const assert = require('assert');
const fetch = require('node-fetch');
const config = require('./config.js');

function isEqual(source, target, tolerance) {
    return (source <= (target + tolerance)) &&
           (source >= (target - tolerance))
}

async function checkPosition(pos, tolerance = config.tolerance) {
    const res = await fetch(config.endpoint + '/position', {
        headers: { 'Content-Type': 'application/json' },
        method: 'POST',
        body: JSON.stringify(pos)
    });
    const obj = await res.json();
    const lengthX = obj['claw-x'];
    const lengthY = obj['claw-y'];
    const lengthZ = obj['claw-z'];
    assert.strictEqual(isEqual(lengthX, pos["claw-x"], tolerance), true, `X ${lengthX} is incorrect`);
    assert.strictEqual(isEqual(lengthY, pos["claw-y"], tolerance), true, `Y ${lengthY} is incorrect`);
    assert.strictEqual(isEqual(lengthZ, pos["claw-z"], tolerance), true, `Z ${lengthZ} is incorrect`);
}


describe('Positioning', function() {
    describe('Set position free angle', function () {
        it ('95 0 80', async function () {
            await checkPosition({ "claw-x": 95, "claw-y": 0, "claw-z": 80 });
        });

        it ('-95 0 80', async function () {
            await checkPosition({ "claw-x": -95, "claw-y": 0, "claw-z": 80 });
        });

        it ('-95 80 80', async function () {
            await checkPosition({ "claw-x": -95, "claw-y": 80, "claw-z": 80 });
        });

        it ('0 95 80', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": 95, "claw-z": 80 });
        });

        it ('0 -95 80', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": -95, "claw-z": 80 });
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

        it ('26 0 130', async function () {
            await checkPosition({ "claw-x": 26, "claw-y": 0, "claw-z": 130 });
        });

        it ('-26 0 130', async function () {
            await checkPosition({ "claw-x": -26, "claw-y": 0, "claw-z": 130 });
        });

        it ('0 26 130', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": 26, "claw-z": 130 });
        });

        it ('0 -26 130', async function () {
            await checkPosition({ "claw-x": 0, "claw-y": -26, "claw-z": 130 });
        });

        it ('26 26 130', async function () {
            await checkPosition({ "claw-x": 26, "claw-y": 26, "claw-z": 130 });
        });

        it ('-26 26 130', async function () {
            await checkPosition({ "claw-x": -26, "claw-y": 26, "claw-z": 130 });
        });
    });

    describe('Set position angle is 0', function () {
        it ('250 0 80, 0', async function () {
            await checkPosition({ "claw-x": 250, "claw-y": 0, "claw-z": 80, "claw-angle-y": 0 });
        });

        it ('250 0 250, 0', async function () {
            await checkPosition({ "claw-x": 250, "claw-y": 0, "claw-z": 250, "claw-angle-y": 0 });
        });

        it ('150 0 250, 0', async function () {
            await checkPosition({ "claw-x": 150, "claw-y": 0, "claw-z": 250, "claw-angle-y": 0 });
        });

        it ('60 0 250, 0', async function () {
            await checkPosition({ "claw-x": 60, "claw-y": 0, "claw-z": 250, "claw-angle-y": 0 });
        });

        it ('20 0 180, 0', async function () {
            await checkPosition({ "claw-x": 60, "claw-y": 0, "claw-z": 180, "claw-angle-y": 0 });
        });
    });

    describe('Set position angle is 90', function () {
        it ('95 0 300, 90', async function () {
            await checkPosition({ "claw-x": 95, "claw-y": 0, "claw-z": 300, "claw-angle-y": 90 });
        });

        it ('150 0 300, 90', async function () {
            await checkPosition({ "claw-x": 150, "claw-y": 0, "claw-z": 300, "claw-angle-y": 90 });
        });
    });

    describe('Set position angle is 270', function () {
        it ('120 0 60, 270', async function () {
            await checkPosition({ "claw-x": 120, "claw-y": 0, "claw-z": 60, "claw-angle-y": 270 });
        });

        it ('125 10 60, 270', async function () {
            await checkPosition({ "claw-x": 125, "claw-y": 10, "claw-z": 60, "claw-angle-y": 270 });
        });

        it ('130 0 60, 270', async function () {
            await checkPosition({ "claw-x": 130, "claw-y": 0, "claw-z": 60, "claw-angle-y": 270 });
        });

        it ('125 -10 300, 270', async function () {
            await checkPosition({ "claw-x": 125, "claw-y": -10, "claw-z": 60, "claw-angle-y": 270 });
        });
    });
});
