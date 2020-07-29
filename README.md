# Gomoku & Paint

Description: this project has two seperate applications which are Gomoku, and Paint in one project. The project implementing a combination of 'Facade Design Pattern' and 'Observer Design Pattern'.
The inner façade interacts with the Windows Console API and it's the only object that interacts with the API.
The outer façade only interact with the inner façade and will provide the value-added features like creating, restoring, and resizing the console.
Please refer to the UML diagram that I attached on

Paint:
The paint program created a 40x40 window with matching buffer, and paint RGB block using the mouse.  
Right clicking the mouse will rotate the draw colors through red, green, and blue.  
Hided the cursor while the program is running.

Gomoku:
The Gomoku program allows two players to place pieces on alternating turns, and indicates that a player has won the match.  
Check out Wikipedia for the instructions on Gomoku.
https://en.wikipedia.org/wiki/Gomoku

