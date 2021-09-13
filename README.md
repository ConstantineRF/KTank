# KTank
## Description
KTank is an arcade game, developed by Konstantin Kalinchenko in 2021 for Windows 7+. Konstantin was inspired by the 1993 console game, 
where one or two players take control over their tanks to defend a fort with eagle against an enemy swarm of tanks on a single screen map.
Despite some differences with the original game, Konstantin aimed to replicate the chaotic behavior of the enemy tanks, while mixing it
with a deliberate progression toward the eagle using modified Dijkstra algorithm. There is also a twist in the game: the slowest and most 
armored enemy tanks, generally spawning closer to the end of each level, have minimal amount of randomness in their pathfinding, hance requiring
player to quickly adjust strategy. It might be even possible that earlier destructions of map objects will make the task of eagle defence
nearly impossible, so plan ahead!
## Controls
### Solo Mode
Tank is operated by arrow keys and left Ctrl.
### Bros Mode
Player 1 tank is operated by C,V,B,F and left Ctrl. Player 2 tank is operated by arrow keys and right Ctrl.
### Other buttons
At any point in the game, Esc terminates the program. P pauses the game. Menu navigation is facilitated by arrow keys, Backspace, and Enter.
## About Development
In essence, this project has been a C++ learning experience. The game also utilizes SSML library for OpenGL. Earlier version of the game 
utilized Chili framework https://wiki.planetchili.net/index.php/Chili_Framework . Although none of that code is used here, Chili still deserves 
credit for excellent C++ tutorials, that helped a lot in learning.


