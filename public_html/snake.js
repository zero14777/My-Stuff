    //Holds variables for the game.
    function Game(){
        var canvas;
        var ctx;
        var tileSize;
        var snake;
        var prev;
        var speed;
        var badFood;
        var food;
        var direction;
        var play;
        var dead;
        var score;
        var turned;
        var highscore;
        var start;
        var hard;
    }

    //Creates a screen shown when the code is first run.
    var startScreen = function(){
        Game.start = true;
        
        Game.canvas = document.getElementById("canvas");
        Game.ctx = canvas.getContext("2d");
        
        Game.ctx.fillStyle="#023F0F";
        Game.ctx.fillRect(0, 0, canvas.width, canvas.height);
        
        Game.ctx.fillStyle="#FF0000";
        Game.ctx.textAlign = "center";
        Game.ctx.font="30px sans-serif";
        Game.ctx.fillText("Snake", 
                          (Game.canvas.width/2), 30);
        Game.ctx.fillStyle="#251321";
        Game.ctx.fillText("Press Enter to start", 
                          (Game.canvas.width/2), 60);
        Game.ctx.fillStyle="#000000";
        Game.ctx.font="20px sans-serif";
        Game.ctx.fillText("Use arrow keys to move", 
                          (Game.canvas.width/2), 90);
        Game.ctx.fillText("Collect red fruit to grow and get score", 
                          (Game.canvas.width/2), 120);
        Game.ctx.fillText("Avoid rotten green fruit,", 
                          (Game.canvas.width/2), 150);
        Game.ctx.fillText("they will shrink you and make you lose score", 
                          (Game.canvas.width/2), 180);
    }

    //Sets all the variables to the starting values, creates the snake,
    //creates the food, and creates the bad food. 
    var init = function(firstRun){
        Game.turned = false;
        Game.score = 0;
        Game.speed = 80;
        Game.dead = false;
        Game.canvas = document.getElementById("canvas");
        Game.ctx = canvas.getContext("2d");
        Game.tileSize = 10;
        Game.snake = [[30, 0], [20, 0], [10, 0], [0, 0]];
        Game.direction = "r";
        Game.prev = [];
        if(firstRun){
            Game.highscore = 0;
            Game.badFoodGen = setInterval(function(){createBadFood(false)},10000);
            Game.hard = false;
        }
        createFood(true);
        createBadFood();
        Game.play = setInterval(function(){update()}, Game.speed);
    };

    //Reacts to input on the keyboard. Works for the arrow keys or Enter.
    window.onkeydown = function(evt){
        evt = evt || window.event;
        if(evt.keyCode == 37 && 
           (Game.direction == "u" || Game.direction == "d") &&
                 Game.turned == false){
            Game.direction = "l";
            Game.turned = true;
        } else if(evt.keyCode == 38 && 
                  (Game.direction == "l" || Game.direction == "r") &&
                 Game.turned == false){
            Game.direction = "u";
            Game.turned = true;
        } else if(evt.keyCode == 39 && 
                  (Game.direction == "u" || Game.direction == "d") &&
                 Game.turned == false){
            Game.direction = "r";
            Game.turned = true;
        } else if(evt.keyCode == 40 && 
                  (Game.direction == "l" || Game.direction == "r") &&
                 Game.turned == false){
            Game.direction = "d";
            Game.turned = true;
        } else if(evt.keyCode == 13 && Game.dead){
            Game.hard = false;
            init(false);
        } else if(evt.keyCode == 13 && Game.start){
            Game.start = false;
            init(true);
        } else if(evt.keyCode == 16 && Game.dead){
            Game.start = false;
            Game.hard = true;
            init(false);
        }
    }
    
    //Creates a bad food tile on a random spot on the canvas that is not occupied.
    var createBadFood = function(){
        Game.badFood = [Math.floor((Math.random()*
                    (Game.canvas.width/Game.tileSize)))*Game.tileSize, 
                    Math.floor((Math.random()*
                    (Game.canvas.height/Game.tileSize)))*Game.tileSize];
        for(var i = 0; i < Game.snake.length-1; i++){
            if(Game.badFood[0] == Game.snake[i][0] &&
               Game.badFood[1] == Game.snake[i][1]){
                createBadFood();
            }
        }
        if(Game.badFood[0] == Game.food[0] &&
           Game.badFood[1] == Game.food[1]){
            createBadFood();
        }
    }

    //Creates a food tile on a random spot on the canvas that is not occupied.
    var createFood = function(firstRun){
        Game.food = [Math.floor((Math.random()*
                    (Game.canvas.width/Game.tileSize)))*Game.tileSize, 
                    Math.floor((Math.random()*
                    (Game.canvas.height/Game.tileSize)))*Game.tileSize];
        for(var i = 0; i < Game.snake.length-1; i++){
            if(Game.food[0] == Game.snake[i][0] &&
               Game.food[1] == Game.snake[i][1]){
                createFood();
            }
        }
        if(!firstRun){
            if(Game.badFood[0] == Game.food[0] &&
               Game.badFood[1] == Game.food[1]){
                createFood();
            }
        }
    }

    //Calls other functions to update the canvas one time. Displays a game 
    //over screen if the player is dead.
    var update = function(){
        move();
        outOfBounds();
        badFoodGet();
        foodGet();
        selfCollision();
        if(!Game.dead){
            draw();
        }
        if(Game.dead){
            gameOver();
        }
    }
    
    //Causes the player to lose if they leave the borders of the canvas.
    var outOfBounds = function(){
        if(Game.snake[0][0] < 0 || Game.snake[0][0] >= Game.canvas.width){
            clearInterval(Game.play);
            Game.dead = true;
        }
        
        if(Game.snake[0][1] < 0 || Game.snake[0][1] >= Game.canvas.height){
            clearInterval(Game.play);
            Game.dead = true;
        }        
    }

    //If the head collides with a bad food tile the snake shrinks, a new bad 
    //food tile is created, the players score is decremented, and if the players
    //score drops too low the player loses.
    var badFoodGet = function(){
        if(Game.snake[0][0] == Game.badFood[0] && 
           Game.snake[0][1] == Game.badFood[1]){
            if(Game.score == -3){
                clearInterval(Game.play);
                Game.dead = true;
                return;
            }
            Game.snake.pop();
            createBadFood();
            Game.score--;
        }
    }

    //If the head collides with a food tile the snake grows, a new food tile is
    //created, the players score is incremented, and a check is made to see if the
    //high score should be incremented.
    var foodGet = function(){
        if(Game.snake[0][0] == Game.food[0] && 
           Game.snake[0][1] == Game.food[1]){
            Game.snake.push(Game.prev);
            createFood(false);
            Game.score++;
            if(Game.hard && Game.speed > 30){
                Game.speed -= 2;
                clearInterval(Game.play);
                Game.play = setInterval(function(){update()}, Game.speed);
            }
            if(Game.score > Game.highscore){
                Game.highscore = Game.score;
            }
        }
    }

    //Player loses if the snakes head collieds with any other part of the snake.
    var selfCollision = function(){
        for(var i = 1; i < Game.snake.length; i++){
            if(Game.snake[0][0] == Game.snake[i][0] &&
               Game.snake[0][1] == Game.snake[i][1]){
                clearInterval(Game.play);
                Game.dead = true;
            }
        }
    }
    
    //Moves the snake one tile in the direction that it is currently moving.
    var move = function(){
        Game.prev = [Game.snake[Game.snake.length-1][0], 
                     Game.snake[Game.snake.length-1][1]];
        switch(Game.direction){
            case "r":
                for(var i = Game.snake.length-1; i > 0; i--){
                    Game.snake[i][0] = Game.snake[i-1][0];
                    Game.snake[i][1] = Game.snake[i-1][1];
                }
                Game.snake[0][0] += Game.tileSize;
                Game.turned = false;
                break;
            case "l":
                for(var i = Game.snake.length-1; i > 0; i--){
                    Game.snake[i][0] = Game.snake[i-1][0];
                    Game.snake[i][1] = Game.snake[i-1][1];
                }
                Game.snake[0][0] -= Game.tileSize;
                Game.turned = false;
                break;
            case "u":
                for(var i = Game.snake.length-1; i > 0; i--){
                    Game.snake[i][0] = Game.snake[i-1][0];
                    Game.snake[i][1] = Game.snake[i-1][1];
                }
                Game.snake[0][1] -= Game.tileSize;
                Game.turned = false;
                break;
            case "d":
                for(var i = Game.snake.length-1; i > 0; i--){
                    Game.snake[i][0] = Game.snake[i-1][0];
                    Game.snake[i][1] = Game.snake[i-1][1];
                }
                Game.snake[0][1] += Game.tileSize;
                Game.turned = false;
                break;
            default:
        }
    }

    //Draws the background, snake, food, bad food, and scores.
    var draw = function(){
        Game.ctx.fillStyle="#023F0F";
        Game.ctx.fillRect(0, 0, canvas.width, canvas.height);
        
        for(var i = 0; i < Game.snake.length; i++){
            Game.ctx.fillStyle="#000000";
            Game.ctx.fillRect(Game.snake[i][0], Game.snake[i][1],
                              Game.tileSize, Game.tileSize);
        }

        Game.ctx.fillStyle="#AF2010";
        Game.ctx.fillRect(Game.food[0], Game.food[1], 
                          Game.tileSize, Game.tileSize);
        Game.ctx.fillStyle="#34FF10";
        Game.ctx.fillRect(Game.badFood[0], Game.badFood[1], 
                          Game.tileSize, Game.tileSize);
        Game.ctx.fillStyle="#000000";
        Game.ctx.textAlign = "left";
        Game.ctx.font="10px sans-serif";
        Game.ctx.fillText("Score: " + Game.score + " High-Score: " + Game.highscore,
                          0, (Game.canvas.height-10));
    }
    
    //Displays a Game Over screen.
    var gameOver = function(){
        Game.ctx.fillStyle="#FF0000";
        Game.ctx.textAlign = "center";
        Game.ctx.font="30px sans-serif";
        Game.ctx.fillText("Game Over", 
                          (Game.canvas.width/2), (Game.canvas.height/2)-30);
        Game.ctx.fillStyle="#251321";
        Game.ctx.font="30px sans-serif";
        Game.ctx.fillText("Press Enter to start", 
                          (Game.canvas.width/2), ((Game.canvas.height/2)));
        Game.ctx.fillStyle="#251321";
        Game.ctx.font="30px sans-serif";
        Game.ctx.fillText("Press Shift to play Hard Mode", 
                          (Game.canvas.width/2), ((Game.canvas.height/2)+30));
        Game.ctx.fillStyle="#251321";
        Game.ctx.font="15px sans-serif";
        Game.ctx.fillText("In hard every time you get a red fruit", 
                          (Game.canvas.width/2), ((Game.canvas.height/2)+45));
        Game.ctx.fillStyle="#251321";
        Game.ctx.font="15px sans-serif";
        Game.ctx.fillText("your speed will increase to a cap", 
                          (Game.canvas.width/2), ((Game.canvas.height/2)+60));
    }
    
    startScreen();