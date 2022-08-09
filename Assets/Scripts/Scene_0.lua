newTime = 0
prevTime = 0
gameTime = 0

Scene_0 = {
    assets = {
        [0] = {type="texture", id="terrain-day", file="./Assets/Tilemaps/Jungle.png"},
        [1] = {type="texture", id="tank-big-right", file="./Assets/Images/Tank-Big-Right.png"},
        [2] = {type="texture", id="chopper-spritesheet", file="./Assets/Images/Chopper-Spritesheet.png"},
        [3] = {type="texture", id="player-bullet", file="./Assets/Images/Bullet-Friendly.png"},
        [4] = {type="texture", id="enemy-bullet", file="./Assets/Images/Bullet-Enemy.png"},
        [5] = {type="texture", id="ui-radar", file="./Assets/Images/Radar.png"},
        [6] = {type="font", id="charriot-20-font", file="./Assets/Fonts/Charriot.ttf", fontSize = 20}
    },
    map = {
        textureId = "terrain-day",
        file = "./Assets/Tilemaps/Jungle.scene",
        scale = 2,
        tileSize = 32,
        mapSizeX = 25,
        mapSizeY = 20
    },
    gameObjects = {
        [0] = 
        {
            name = "LuaGameData",
            layer = 5,
            position = {
                x = 0,
                y = 0
            },
            scale = {
                x = 1, 
                y = 1
            },
            components = {
                luaBehaviour = {
                    onAwake = function()
                        gameTime = 0
                        newTime = 0
                    end,
                    onUpdate = function(deltaTime)
                        gameTime = gameTime + deltaTime
                        newTime = math.floor(gameTime)
                    end
                }
            }
        },
        [1] = {
            name = "Player",
            layer = 3,
            position = {
                x = 240,
                y = 106
            },
            scale = {
                x = 1, 
                y = 1
            },
            components = {
                sprite = {
                    isAnimated = true,
                    textureId = "chopper-spritesheet",
                    width = 32,
                    height = 32,
                    frameCount = 2,
                    animationSpeed = 16,
                    hasDirections = true,
                    isFixed = false
                },
                rigidBody = {
                    velX = 0,
                    velY = 0,
                    gravityScale = 0
                },
                keyboardInput = {
                    up = "w",
                    down = "s",
                    left = "a",
                    right = "d",
                    shoot = " "
                },
                collider = {
                    tag = 2,
                    width = 32,
                    height = 32,
                    offsetX = 0,
                    offsetY = 0,
                    onCollision = function (tag)
                        if(tag == 3)then
                            GameOver("Kamikaze")
                        end
                        if(tag == 4) then
                            GameOver("Ouch")
                        end
                    end
                }
            }
        },
        [2] = {
            name = "Tank",
            layer = 2,
            position = {
                x = 650,
                y = 405
            },
            scale = {
                x = 1, 
                y = 1
            },
            components = {
                sprite = {
                    isAnimated = false,
                    textureId = "tank-big-right",
                    sourceX = 0,
                    sourceY = 0,
                    width = 32,
                    height = 32,
                    isFixed = false
                },
                collider = {
                    tag = 3,
                    width = 32,
                    height = 32,
                    offsetX = 0,
                    offsetY = 0,
                    onCollision = function (tag)
                        if(tag == 5)then
                            DestroyObject("Tank", 2);
                            DestroyObject("ProjectileEmitter_Tank", 4);
                        end
                    end
                }
            }
        },
        [3] = {
            name = "ProjectileEmitter_Tank",
            layer = 4,
            position = {
                x = 650,
                y = 405
            },
            scale = {
                x = 1, 
                y = 1
            },
            components = {
                sprite = {
                    isAnimated = false,
                    textureId = "enemy-bullet",
                    sourceX = 0,
                    sourceY = 0,
                    width = 4,
                    height = 4,
                    isFixed = false
                },
                collider = {
                    tag = 4,
                    width = 4,
                    height = 4,
                    offsetX = 0,
                    offsetY = 0,
                    onCollision = null
                },
                projectileEmitter = {
                    refObjectLayer = 2,
                    refObjectName = "Tank",
                    offsetX = 16,
                    offsetY = 16,
                    speed = 100,
                    angleDeg = 270,
                    range = 200,
                    isLooping = true
                },
                luaBehaviour = {
                    onAwake = function ()
                        print("@Lua->OnAwake")
                    end,
                    onUpdate = function (deltaTime)
                        if(newTime ~= prevTime) then
                            print("@Lua->OnUpdate " .. newTime)
                            prevTime = newTime
                        end
                    end,
                    onDestroy = function ()
                        print("@Lua->OnDestroy")
                    end
                }
            }
        },
        [4] = {
            name = "ui-radar",
            layer = 5,
            position = {
                x = 1200 - 16 - (64 * 2),
                y = 16
            },
            scale = {
                x = 2, 
                y = 2
            },
            components = {
                sprite = {
                    isAnimated = true,
                    textureId = "ui-radar",
                    width = 64,
                    height = 64,
                    frameCount = 8,
                    animationSpeed = 8,
                    hasDirections = false,
                    isFixed = true
                },
                luaBehaviour = {
                    onUpdate = function(deltaTime)
                        if(gameTime > 5) then
                            DestroyObject("ui-radar", 5);
                        end
                    end 
                }
            }
        },
        [5] = {
            name = "label-level-name",
            layer = 5,
            position = {
                x = 32,
                y = 16
            },
            scale = {
                x = 100, 
                y = 50
            },
            components = {
                textLabel = {
                    text = "Level 0",
                    fontAssetId = "charriot-20-font",
                    fontColor = 
                    {
                        r = 255,
                        g = 255,
                        b = 255,
                        a = 255
                    }
                },
                luaBehaviour = {
                    onUpdate = function(deltaTime)
                        UpdateLabelColor("label-level-name", math.random(255), math.random(255), math.random(255), 255)
                        UpdateLabelText("label-level-name", "Time: " .. newTime)
                    end
                }
            }
        }
    }
}
