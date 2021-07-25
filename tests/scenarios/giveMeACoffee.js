const position = require("../position.js");
const config = require('../config.js');

const spoon = {
    x: -180,
    y: 0,
    z: 140
}


describe.only('Give me a coffee', function() {
    it ('take a spoon', async function () {
        await position.set({ "claw-x": 95, "claw-y": 0, "claw-z": 25, "iterations": 60, "claw-angle-x": 0, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": -170, "claw-y": 0, "claw-z": 180, "claw-angle-y": 0, "claw-angle-x": 0, "claw-angle": 0, "iterations": 60, "iteration-delay": 30 });
        await position.set({ "claw-x": -170, "claw-y": -50, "claw-z": 180, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": -170, "claw-y": -50, "claw-z": 150, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 40, "iteration-delay": 30 });
        await position.set({ "claw-x": -170, "claw-y": 0, "claw-z": 150, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 40, "iteration-delay": 30 });
        await position.set({ "claw-x": -190, "claw-y": 0, "claw-z": 150, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": 200, "claw-y": 0, "claw-z": 150, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });
});
