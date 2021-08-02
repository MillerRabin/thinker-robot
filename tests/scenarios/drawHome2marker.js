const position = require("../position.js");

const redMarker = {
    x: -220,
    y: -70,
    z: 160
};

const home = {
    blCorner: {
        x: 200,
        y: 140,
        z: 140
    }
};

describe('draw home', function() {
    it('move to home', async function () {
        await position.set({ "claw-x": 95,  "claw-y": 0,    "claw-z": 25,       "iterations": 60, "claw-angle-x": 0, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('take a red marker', async function () {
        await position.set({ "claw-x": redMarker.x + 20,  "claw-y": redMarker.y - 70,    "claw-z": redMarker.z,     "claw-angle-y": 0, "claw-angle-x": 0, "claw-angle": 0, "iterations": 60, "iteration-delay": 30 });
        await position.set({ "claw-x": redMarker.x + 20,  "claw-y": redMarker.y - 70,    "claw-z": redMarker.z,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": redMarker.x + 20,  "claw-y": redMarker.y - 70,    "claw-z": redMarker.z,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 30, "iteration-delay": 30 });
        await position.set({ "claw-x": redMarker.x + 20,       "claw-y": redMarker.y,      "claw-z": redMarker.z,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 30, "iteration-delay": 30 });
        await position.set({ "claw-x": redMarker.x,  "claw-y": redMarker.y,      "claw-z": redMarker.z,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 30, "iteration-delay": 30 });
        await position.set({ "claw-x": redMarker.x,  "claw-y": redMarker.y,      "claw-z": redMarker.z,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 6, "iteration-delay": 30 });
    });

    it('draw a home', async function () {
        await position.set({ "claw-x": home.blCorner.x - 10,  "claw-y": home.blCorner.y - 10,    "claw-z": home.blCorner.z,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 6, "iterations": 60, "iteration-delay": 30 });
        await position.set({ "claw-x": home.blCorner.x,  "claw-y": home.blCorner.y,    "claw-z": home.blCorner.z,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 6, "iterations": 60, "iteration-delay": 30 });
    });

});
