const position = require("../position.js");

const spoon = {
    x: -180,
    y: 0,
    z: 150
};

const sugarCup = {
    x: 180,
    y: -90,
    z: 120
};


const coffeeCup = {
    x: 180,
    y: 40,
    z: 120
};


const waterCup = {
    x: 160,
    y: 140,
    z: 170,
    mix: 15,
    iterations: 2,
    postDelay: 50,
    iterationDelay: 20
};


describe.skip('Give me a coffee', function() {
    it('move to home', async function () {
        await position.set({ "claw-x": 95,  "claw-y": 0,    "claw-z": 25,       "iterations": 60, "claw-angle-x": 0, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('take a spoon', async function () {
        const sp = spoon.x + 10;
        const spe = spoon.x - 20;
        const spzs = spoon.z + 30;
        await position.set({ "claw-x": sp,  "claw-y": 0,    "claw-z": spzs,     "claw-angle-y": 0, "claw-angle-x": 0, "claw-angle": 0, "iterations": 60, "iteration-delay": 30 });
        await position.set({ "claw-x": sp,  "claw-y": -50,  "claw-z": spzs,     "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sp,  "claw-y": -50,  "claw-z": spoon.z,  "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 30, "iteration-delay": 30 });
        await position.set({ "claw-x": sp,  "claw-y": 0,    "claw-z": spoon.z,  "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 30, "iteration-delay": 30 });
        await position.set({ "claw-x": spe, "claw-y": 0,    "claw-z": spoon.z,  "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('get a sugar', async function () {
        await position.set({ "claw-x": sugarCup.x,          "claw-y": sugarCup.y,   "claw-z": sugarCup.z,        "claw-angle-y": -30, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sugarCup.x,          "claw-y": sugarCup.y,   "claw-z": sugarCup.z - 70,   "claw-angle-y": -60, "claw-angle-x": -45, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sugarCup.x + 20,     "claw-y": sugarCup.y,   "claw-z": sugarCup.z - 70,   "claw-angle-y": -60, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sugarCup.x + 20,     "claw-y": sugarCup.y,   "claw-z": sugarCup.z + 30,   "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('move to water cup', async function () {
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,   "claw-angle-y": 0, "claw-angle-x": -10, "claw-angle": 0, "iteration-delay": 30, "post-delay": 200 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('get a sugar', async function () {
        await position.set({ "claw-x": sugarCup.x,          "claw-y": sugarCup.y,   "claw-z": sugarCup.z,        "claw-angle-y": -30, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sugarCup.x,          "claw-y": sugarCup.y,   "claw-z": sugarCup.z - 70,   "claw-angle-y": -60, "claw-angle-x": -45, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sugarCup.x + 20,     "claw-y": sugarCup.y,   "claw-z": sugarCup.z - 70,   "claw-angle-y": -60, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sugarCup.x + 20,     "claw-y": sugarCup.y,   "claw-z": sugarCup.z + 30,   "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('move to water cup', async function () {
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,   "claw-angle-y": 0, "claw-angle-x": -10, "claw-angle": 0, "iteration-delay": 30, "post-delay": 200 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('move to coffee cup', async function () {
        await position.set({ "claw-x": coffeeCup.x,      "claw-y": coffeeCup.y, "claw-z": coffeeCup.z,      "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": coffeeCup.x, "claw-y": coffeeCup.y, "claw-z": coffeeCup.z - 50, "claw-angle-x": -45, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": coffeeCup.x, "claw-y": coffeeCup.y, "claw-z": coffeeCup.z - 50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": coffeeCup.x + 10, "claw-y": coffeeCup.y, "claw-z": coffeeCup.z + 30, "claw-angle-x": -90, "claw-angle-y": 0, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('move to water cup', async function () {
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,   "claw-angle-y": 0, "claw-angle-x": -10, "claw-angle": 0, "iteration-delay": 30, "post-delay": 200 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('move to coffee cup', async function () {
        await position.set({ "claw-x": coffeeCup.x,      "claw-y": coffeeCup.y, "claw-z": coffeeCup.z,      "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": coffeeCup.x, "claw-y": coffeeCup.y, "claw-z": coffeeCup.z - 50, "claw-angle-x": -45, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": coffeeCup.x, "claw-y": coffeeCup.y, "claw-z": coffeeCup.z - 50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": coffeeCup.x + 10, "claw-y": coffeeCup.y, "claw-z": coffeeCup.z + 30, "claw-angle-x": -90, "claw-angle-y": 0, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('move to water cup', async function () {
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,   "claw-angle-y": 0, "claw-angle-x": -10, "claw-angle": 0, "iteration-delay": 30, "post-delay": 200 });
        await position.set({ "claw-x": waterCup.x - 30, "claw-y": waterCup.y - 30,       "claw-z": waterCup.z,        "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
    });

    it('mix coffee', async function () {
        const mcz = waterCup.z - 60;
        await position.set({ "claw-x": waterCup.x - 40, "claw-y": waterCup.y - 40, "claw-z": waterCup.z, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x, "claw-y": waterCup.y + waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x + waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });
        await position.set({ "claw-x": waterCup.x - waterCup.mix, "claw-y": waterCup.y - waterCup.mix,               "claw-z": mcz, "claw-angle-y": -50, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay, "iterations": waterCup.iterations, "post-delay": waterCup.postDelay  });

        await position.set({ "claw-x": waterCup.x - 40, "claw-y": waterCup.y - 40,  "claw-z": waterCup.z, "claw-angle-y": 0, "claw-angle-x": -45, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay });
        await position.set({ "claw-x": waterCup.x - 40, "claw-y": waterCup.y - 40,  "claw-z": waterCup.z + 30, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": waterCup.iterationDelay });
    });

    it('return a spoon', async function () {
        const sp = spoon.x + 10;
        const spe = spoon.x - 20;
        const spzs = spoon.z + 30;
        await position.set({ "claw-x": sp, "claw-y": 0, "claw-z": spzs, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iterations": 60, "iteration-delay": 30 });
        await position.set({ "claw-x": sp, "claw-y": 0, "claw-z": spoon.z, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 0, "iteration-delay": 30 });
        await position.set({ "claw-x": sp + 10, "claw-y": 0, "claw-z": spoon.z, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 30, "iteration-delay": 30 });
        await position.set({ "claw-x": sp + 10, "claw-y": -70, "claw-z": spoon.z, "claw-angle-y": 0, "claw-angle-x": -90, "claw-angle": 30, "iteration-delay": 30 });
    });

    it('move to home', async function () {
        await position.set({ "claw-x": 95,  "claw-y": 0,    "claw-z": 25,       "iterations": 60, "claw-angle-x": 0, "claw-angle": 0, "iteration-delay": 30 });
    });
});
