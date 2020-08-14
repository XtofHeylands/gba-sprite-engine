#Mario Party Advance
Clone of the game "Mario Party" for the Game Boy Advance. Originally created and published by Nintendo Co., 
Ltd. All rights reserved. This project is part of the course "Software ontwerp in C/C++". The backend/game-engine was
supplied by De heer Wouter Groeneveld.

> https://github.com/wgroeneveld/gba-sprite-engine

##Gameplay
The player is greeted by a stylised 'spash screen' and is given the choice between several playable characters. 
Thereafter the shosen character is put on the 'game board'. Navigating on this board is done by using the arrow keys 
(up, down, right, left). Interacting with a certain tile is done by pressing the A button. At the time of writing this
document, there are only two playable characters and one interactive tile.

> Mario and Luigi are playable // Only the M-tile is interactive!

Pressing A on one of these M-tiles will commence the build-in minigame. The goal of this minigame is to dodge the oncomming
projectiles for a certain amount of time. Each dodged bullet equals one point. If the game is won, the score is added to the
total. If the game is lost, the score is also added to the total but one life point is lost. When the player loses all 3
life points, the game is lost. Consequently the 'game over' screen will appear displaying the final score.