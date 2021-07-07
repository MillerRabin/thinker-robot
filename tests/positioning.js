const assert = require('assert');
const fetch = require('node-fetch');

const endpoint = 'http://192.168.1.19';
const tolerance = 2;

function isEqual(source, target, tolerance) {
    return (source <= (target + tolerance)) &&
           (source >= (target - tolerance))
}

async function checkPosition(pos) {
    const res = await fetch(endpoint + '/position', {
        headers: { 'Content-Type': 'application/json' },
        method: 'POST',
        body: JSON.stringify(pos)
    });
    const obj = await res.json();
    const lengthX = obj['length-x'];
    const lengthY = obj['length-y'];
    const lengthZ = obj['length-z'];
    assert.strictEqual(isEqual(lengthX, pos.x, tolerance), true, 'X is incorrect');
    assert.strictEqual(isEqual(lengthY, pos.y, tolerance), true, 'Y is incorrect');
    assert.strictEqual(isEqual(lengthZ, pos.z, tolerance), true, 'Z is incorrect');
}


describe('Positioning', function() {
    describe('Set position', function () {
        it ('95 0 80', async function () {
            await checkPosition({ x: 95, y: 0, z: 80 });
        });
    });

    describe('Set position', function () {
        it ('-95 0 80', async function () {
            await checkPosition({ x: -95, y: 0, z: 80 });
        });
    });
});
