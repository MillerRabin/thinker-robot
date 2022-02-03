const position = require("../position.js");

describe.only('Play chess', function() {
    this.timeout(0);
    it('move to home', async function () {
        await position.wait(15000);
        await position.move({ "shoulder-y": 135, "shoulder-z": 130, "elbow-y": 75, "wrist-y": 75, "claw": 15, "claw-x": 135, "claw-y": 0, "claw-z": 135, "speed": 1});
    }); 

    it('take black pawn', async function () {
        await position.move({ "shoulder-y": 145,"shoulder-z": 130,"elbow-y": 25,  "wrist-y": 103,"claw": 15, "claw-x": 135,"claw-y": 3,"claw-z": 135,"speed": 1});
        await position.move({ "shoulder-y": 145,"shoulder-z": 130,"elbow-y": 25,  "wrist-y": 103,"claw": 7, "claw-x": 135,"claw-y": 3,"claw-z": 135,"speed": 1});
        await position.move({"shoulder-y": 145,"shoulder-z": 130,"elbow-y": 35,  "wrist-y": 100,"claw": 7,"claw-x": 135,"claw-y": 0,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 100,"shoulder-z": 130,"elbow-y": 70,  "wrist-y": 105,"claw": 7,"claw-x": 135,"claw-y": 5,"claw-z": 135,"speed": 1});
        await position.wait(500);
        await position.move({"shoulder-y": 97,"shoulder-z": 130,"elbow-y": 64,  "wrist-y": 105,"claw": 10,"claw-x": 135,"claw-y": 7,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 97,"shoulder-z": 130,"elbow-y": 64,  "wrist-y": 105,"claw": 15,"claw-x": 135,"claw-y": 7,"claw-z": 135,"speed": 1}); 
        await position.move({ "shoulder-y": 135, "shoulder-z": 130, "elbow-y": 75, "wrist-y": 75, "claw": 15, "claw-x": 135, "claw-y": 0, "claw-z": 135, "speed": 1});
    }); 

    it('toggle clock', async function () {
        await position.move({ "shoulder-y": 100,"shoulder-z": 85,"elbow-y": 110,  "wrist-y": 70,"claw": 0,"claw-x": 135,"claw-y": 30,"claw-z": 135,"speed": 1 });        
        await position.move({ "shoulder-y": 80,"shoulder-z": 85,"elbow-y": 110,  "wrist-y": 70,"claw": 0,"claw-x": 135,"claw-y": 30,"claw-z": 135,"speed": 1 });        
        await position.move({ "shoulder-y": 100,"shoulder-z": 85,"elbow-y": 110,  "wrist-y": 70,"claw": 0,"claw-x": 135,"claw-y": 30,"claw-z": 135,"speed": 1 });       
        await position.move({ "shoulder-y": 135, "shoulder-z": 130, "elbow-y": 75, "wrist-y": 75, "claw": 15, "claw-x": 135, "claw-y": 0, "claw-z": 135, "speed": 1}); 
    }); 

    it('take black horse', async function () {
        await position.wait(15000);
        await position.move({"shoulder-y": 130,"shoulder-z": 102,"elbow-y": 90,  "wrist-y": 50,"claw": 10,"claw-x": 135,"claw-y": 30,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 140,"shoulder-z": 102,"elbow-y": 80,  "wrist-y": 50,"claw": 15,"claw-x": 100,"claw-y": 30,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 150,"shoulder-z": 97,"elbow-y": 60,  "wrist-y": 50,"claw": 15,"claw-x": 100,"claw-y": 30,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 150,"shoulder-z": 97,"elbow-y": 60,  "wrist-y": 50,"claw": 5,"claw-x": 100,"claw-y": 30,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 150,"shoulder-z": 97,"elbow-y": 90,  "wrist-y": 50,"claw": 5,"claw-x": 100,"claw-y": 0,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 90,"shoulder-z": 113,"elbow-y": 150,  "wrist-y": 40,"claw": 5,"claw-x": 100,"claw-y": 0,"claw-z": 135,"speed": 1});        
        await position.move({"shoulder-y": 90,"shoulder-z": 113,"elbow-y": 105,  "wrist-y": 55,"claw": 5,"claw-x": 110,"claw-y": 25,"claw-z": 135,"speed": 1});
        await position.move({"shoulder-y": 90,"shoulder-z": 113,"elbow-y": 105,  "wrist-y": 55,"claw": 15,"claw-x": 110,"claw-y": 25,"claw-z": 135,"speed": 1});
        await position.move({"shoulder-y": 110,"shoulder-z": 113,"elbow-y": 100,  "wrist-y": 75,"claw": 15,"claw-x": 110,"claw-y": 0,"claw-z": 135,"speed": 1}); 
    }); 

    it('toggle clock', async function () {
        await position.move({ "shoulder-y": 100,"shoulder-z": 85,"elbow-y": 110,  "wrist-y": 70,"claw": 0,"claw-x": 135,"claw-y": 30,"claw-z": 135,"speed": 1 });        
        await position.move({ "shoulder-y": 80,"shoulder-z": 85,"elbow-y": 110,  "wrist-y": 70,"claw": 0,"claw-x": 135,"claw-y": 30,"claw-z": 135,"speed": 1 });        
        await position.move({ "shoulder-y": 100,"shoulder-z": 85,"elbow-y": 110,  "wrist-y": 70,"claw": 0,"claw-x": 135,"claw-y": 30,"claw-z": 135,"speed": 1 });        
    }); 

    it('move to home', async function () {
        await position.move({ "shoulder-y": 135, "shoulder-z": 130, "elbow-y": 75, "wrist-y": 75, "claw": 15, "claw-x": 135, "claw-y": 0, "claw-z": 135, "speed": 1});
    }); 
});
