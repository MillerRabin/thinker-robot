const assert = require('assert');
const fetch = require('node-fetch');
const config = require('./config.js');

describe('Version', function() {
    describe('check version', function () {
        it ('request version', async function () {
            const res = await fetch(`${config.endpoint}/version`, {
                method: "POST",
                headers: { 'Content-Type': 'application/json' }
            })
            const obj = await res.json();
            assert.strictEqual(obj.version != null, true, "Version must be defined");
        });

        it ('check free heap', async function () {
            const res = await fetch(`${config.endpoint}/version`, {
                method: "POST",
                headers: { 'Content-Type': 'application/json' }
            })
            const obj = await res.json();
            assert.strictEqual(obj['free-heap'] != null, true, "Free heap must be defined");
        });
    });
});
