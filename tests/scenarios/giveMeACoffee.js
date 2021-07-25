const position = require("../position.js");
const config = require('../config.js');

const spoon = {
    x: -180,
    y: 0,
    z: 140
}


describe.only('Give me a coffee', function() {
    it ('take a spoon', async function () {
        //await position.set({ "claw-x": 95, "claw-y": 0, "claw-z": 25 });
        await position.set({ "claw-x": -180, "claw-y": 0, "claw-z": 170, "claw-angle-y": 0 });
        await position.set({ "claw-x": -180, "claw-y": -50, "claw-z": 170, "claw-angle-y": 0, "claw-angle-x": -90 });
    });
});
