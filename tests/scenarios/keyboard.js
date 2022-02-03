const position = require("../position.js");

const iterations = 30;
const high = 150;
const low = 85;
const baseX = 290;
const baseY = -25;
const clawAngle = 5;
const fAngleX = -15;
const fAngleY = 0;

const pen = {
    x: 199,
    y: -312,
    z: 124
}

const keys = {
    "WinKey":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 70, "claw-y": baseY + 155, "claw-z": high, "iterations": iterations + 30, "post-delay": 50 },
    "Enter":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": -10, "claw-angle-z": -45, "claw-x": baseX - 5, "claw-y": baseY - 65, "claw-z": high, "iterations": iterations , "post-delay": 50, low: low - 10 },
    "Caps":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": 0, "claw-x": baseX - 20, "claw-y": baseY + 215, "claw-z": high, "iterations": iterations + 20, "post-delay": 100 },
    "Space":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 75, "claw-y": baseY + 70, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "-":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": -10, "claw-angle-z": -45, "claw-x": baseX + 10, "claw-y": baseY - 15, "claw-z": high, "iterations": iterations , "post-delay": 25, low: low - 20 },
    "n":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 50, "claw-y": baseY + 60, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "а":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 45, "claw-y": baseY + 85, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "б":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 60, "claw-y": baseY + 10, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "в":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 30, "claw-y": baseY + 120, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "г":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 10, "claw-y": baseY + 50, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "д":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 40, "claw-y": baseY, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "н":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 10, "claw-y": baseY + 50, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "е":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 10, "claw-y": baseY + 85, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "ж":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 45, "claw-y": baseY + 150, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "з":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": -10, "claw-angle-z": -45, "claw-x": baseX - 10, "claw-y": baseY, "claw-z": high, "iterations": iterations , "post-delay": 50, low: low - 20 },
    "и":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 60, "claw-y": baseY + 65, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "й":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -20, "claw-x": baseX, "claw-y": baseY + 170, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "к":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 10, "claw-y": baseY + 105, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "л":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 40, "claw-y": baseY + 20, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "м":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 50, "claw-y": baseY + 90, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "н":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 10, "claw-y": baseY + 70, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "о":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 40, "claw-y": baseY + 35, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "п":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 40, "claw-y": baseY + 70, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "р":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 37, "claw-y": baseY + 55, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "с":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 50, "claw-y": baseY + 110, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "т":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 60, "claw-y": baseY + 40, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "у":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 15, "claw-y": baseY + 125, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "х":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": -10, "claw-angle-z": -45, "claw-x": baseX - 10, "claw-y": baseY - 25, "claw-z": high, "iterations": iterations , "post-delay": 50, low: low - 20 },
    "ц":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -20, "claw-x": baseX - 10, "claw-y": baseY + 160, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "щ":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": -10, "claw-angle-z": -45, "claw-x": baseX - 15, "claw-y": baseY + 15, "claw-z": high, "iterations": iterations , "post-delay": 25, low: low - 25 },
    "ы":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 30, "claw-y": baseY + 140, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "ь":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -45, "claw-x": baseX - 60, "claw-y": baseY + 25, "claw-z": high, "iterations": iterations , "post-delay": 50 },
    "э":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": -10, "claw-angle-z": -45, "claw-x": baseX - 35, "claw-y": baseY - 25, "claw-z": high, "iterations": iterations + 10, "post-delay": 25, low: low - 20 },
    "я":  { "claw-angle": clawAngle, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX - 48, "claw-y": baseY + 145, "claw-z": high, "iterations": iterations , "post-delay": 50 },
};

async function press(key) {
    const pos = keys[key];
    await position.set(pos);
    const cLow = (pos.low == null) ? low : pos.low;
    pos["claw-z"] = cLow;
    await position.set(pos);
    pos["claw-z"] = high;
    await position.set(pos);
}



describe.skip('Keyboard', function() {
    it.only('take a pen', async function () {
        await position.set({"claw-angle": 35, "claw-angle-x": fAngleX, "claw-angle-y": 10,"claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y + 60,"claw-z": pen.z + 50, "iterations": 60, "post-delay": 500 });
        await position.set({"claw-angle": 35, "claw-angle-x": fAngleX, "claw-angle-y": 10,"claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y,"claw-z": pen.z, "iterations": 60, "post-delay": 1000 });
        await position.set({"claw-angle": 5, "claw-angle-x": fAngleX, "claw-angle-y": 10,"claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y,"claw-z": pen.z, "iterations": 60});
        await position.set({"claw-angle": 5, "claw-angle-x": fAngleX, "claw-angle-y": 10,"claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y,"claw-z": pen.z + 50, "iterations": 60});
        await position.set({"claw-angle": 5, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX, "claw-y": baseY, "claw-z": high, "iterations": 100 });
    });

    it('open notepad', async function () {
        await press("WinKey");
        await press("б");
        await press("л");
        await press("Enter");
    });

    it('First line', async function () {
        await press("Caps");
        await press("п");
        await press("Caps");
        await press("р");
        await press("и");
        await press("в");
        await press("е");
        await press("т");
        await press("Space");
        await press("Caps");
        await press("я");
        await press("Space");
    });

    it('Name', async function () {
        await press("э");
        await press("Caps");
        await press("л");
        await press("и");
        await press("с");
        await press("Enter");
    });

    it('Second line', async function () {
        await press("Caps");
        await press("я");
        await press("Caps");
        await press("Space");
        await press("м");
        await press("о");
        await press("г");
        await press("у");
        await press("Space");
        await press("п");
        await press("и");
        await press("с");
        await press("а");
        await press("т");
        await press("ь");
        await press("Space");
        await press("к");
        await press("о");
        await press("д");
        await press("Enter");
    });

    it.only('throw a pen', async function () {
        await position.set({"claw-angle": 5, "claw-angle-x": fAngleX, "claw-angle-y": fAngleY, "claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y,"claw-z": pen.z + 60, "iterations": 60, "post-delay": 500 });
        //await position.set({"claw-angle": 5, "claw-angle-x": fAngleX, "claw-angle-y": fAngleY, "claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y,"claw-z": pen.z, "iterations": 60, "post-delay": 1000 });
        /*await position.set({"claw-angle": 35, "claw-angle-x": fAngleX, "claw-angle-y": fAngleY, "claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y,"claw-z": pen.z, "iterations": 60 });
        await position.set({"claw-angle": 35, "claw-angle-x": fAngleX, "claw-angle-y": fAngleY, "claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y + 60,"claw-z": pen.z + 20, "iterations": 60 });
        await position.set({"claw-angle": 35, "claw-angle-x": fAngleX, "claw-angle-y": 9, "claw-angle-z": -90, "claw-x": pen.x,"claw-y": pen.y + 60,"claw-z": pen.z + 140, "iterations": 60});
        await position.set({"claw-angle": 35, "claw-angle-x": 0, "claw-angle-y": 0, "claw-angle-z": -30, "claw-x": baseX, "claw-y": baseY, "claw-z": high, "iterations": 100 });*/
    });
});
