const position = require("../position.js");


describe.only('Fixed claw Z Angle', function() {
    it('move to home', async function () {
        await position.set({  "claw-angle": 0, "claw-angle-z": 0, "claw-angle-y": 0, "claw-x": 360, "claw-y": 0, "claw-z": 270, "iterations": 40 });
    });

    it('move to left', async function () {
        await position.set({  "claw-angle": 0, "claw-angle-z": 0, "claw-angle-y": 0, "claw-x": 360, "claw-y": -120, "claw-z": 270, "iterations": 40 });
    });

    it('move to right', async function () {
        await position.set({  "claw-angle": 0, "claw-angle-z": 0, "claw-angle-y": 0, "claw-x": 360, "claw-y": 120, "claw-z": 270, "iterations": 40 });
    });

    it('move to home', async function () {
        await position.set({  "claw-angle": 0, "claw-angle-z": 0, "claw-angle-y": 0, "claw-x": 360, "claw-y": 0, "claw-z": 270, "iterations": 40 });
    });

});
