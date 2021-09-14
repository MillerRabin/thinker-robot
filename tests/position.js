const fetch = require('node-fetch');
const config = require('./config.js');

exports.isEqual = (source, target, tolerance) => {
    return (source <= (target + tolerance)) &&
        (source >= (target - tolerance))
};

async function request(pos) {
    const res = await fetch(config.endpoint + '/position', {
        headers: { 'Content-Type': 'application/json' },
        method: 'POST',
        body: JSON.stringify(pos)
    });
    let text = '';
    try {
        text = await res.text();
        return JSON.parse(text);
    } catch (e) {
        console.log (text);
        throw e;
    }
}

function delay(time) {
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve();
        }, time);
    });
}

exports.set = async (pos) => {
    while(true) {
        const obj = await request(pos);
        const err = obj['error'];
        if (err == null) return obj;
        if (err.code == -25) {
            await delay(2000);
            continue;
        }
        throw err;
    }
};
