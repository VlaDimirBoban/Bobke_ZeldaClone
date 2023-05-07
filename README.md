# Bobke 

![alt text]=(https://github.com/VlaDimirBoban/Bobke_ZeldaClone/blob/main/thumbs/Bobke.png)

Fun little zelda inspired clone. This game was a significant milestone for me, i wanted to make a pathfinding system and couldn't find any tutorials/material. This turned out to be a blessing in disguies, i researched old algorithms and built it from scatch. 

After that i wanted this to be a simple little 2D engine, that meant i had to build a couple of tools to ease my game creation. 
I made:

- enemy spawning too (only 3 types a s a proof of concept)
- static hitbox drawing tool

both of wich are saved into individual .csv files which allows me to save current state of the game world.

Currently i'm working on Z-buffering and map editor in a separate project (will probably post it as a separate project).

Useful keys:

F1 - rudimentary/fancy graphics switch
F2 - save world state (enemies && hitboxes)
TAB - toggle deboug layer (collision boxes, pathfinding grid & path)
SPACE - pause the gameplay (this enables you to add new enemies and hitboxes)



Raylib:

https://www.raylib.com/
