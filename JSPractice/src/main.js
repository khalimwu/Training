var canvas, ctx, c_width = 800, c_height = 1024, 
    currentTime = Date.now(),
    deltaTime = 0.0,
    key=[],
    backgroundSpeed = 250,
    chanceToSpawnMeteor = 0.9,
    chanceToSpawnEnemy = 0.5,
    spawnMeteorInterval = 0.5,
    spawnMeteortimer = 0.0,
    spawnEnemyInterval = 0.5,
    spawnEnemytimer = 0.0,
    states = {Splash:0, Playing: 1, GameOver:2},
    currentState = states.Splash,
    isImpact = false,
    dt = 0.0,

plane = {
    // 0 = still, 1 left, 2 ignition, 3 right
    currentFrame : 0,
    x: c_width/2,
    y:c_height-128,
    speed: 5,
    aspd:0.025,
    radius: 26,
    velocity_x: 0.0,
    velocity_y: 0.0,
    rotation: 0.0,

    currentTimer : 0.0,

    cannon1_x: 0.0,
    cannon2_x: 0.0,
    cannons_y: 0.0,
    center_x : 0.0,
    center_y : 0.0,

    shoot: function(){
        if (this.currentTimer >= this.aspd)
        {
            bullets.addBullet(this.cannon1_x, this.cannons_y, false, -1);
            bullets.addBullet(this.cannon2_x, this.cannons_y, false, -1);
            this.currentTimer = 0.0;
        }
    },

    update: function(dt){
        this.currentTimer += dt;
        this.cannon1_x = this.x + 16 - 4;
        this.cannon2_x = this.x + 48 - 4;
        this.cannons_y = this.y + 32;
        this.center_x = this.x + 32;
        this.center_y = this.y + 32;
        this.rotation   += dt;
        this.x += this.velocity_x;
        this.y += this.velocity_y;
        if ((this.x <=0) || (this.x >=c_width-64))
        {
            this.velocity_x = 0;
        }

        if ((this.y <=0) || (this.y >=c_height-64))
        {
            this.velocity_y = 0;
        }

        this.x = this.x.clamp(0,c_width-64);
        this.y = this.y.clamp(0,c_height-64);

        for (var j = meteors._meteors.length - 1; j >= 0; j--) {
            
            var _m = meteors._meteors[j], d;

            d = Math.sqrt(Math.pow(this.center_x-(_m.x+_m.radius),2)+Math.pow(this.center_y-(_m.y+_m.radius),2));

            if (d<=(_m.radius + this.radius))
            {
                healthBar.currentHealth -= _m.damage/1000;
                healthBar.currentHealth = healthBar.currentHealth.clamp(0,1);
                //this.x
            }
        }
    },

    draw: function(){
        ctx.save();
        // ctx.beginPath();
        // ctx.arc(this.x+32, this.y+26, this.radius, 0, Math.PI*2);
        // ctx.fill();
        //ctx.translate(this.x,this.y);
        //ctx.rotate(this.rotation);
       // var img = new Image();
        //img.src ="res/l0_Plane1.png";
        //ctx.drawImage(img, -s_plane[this.currentFrame].width/2, -s_plane[this.currentFrame].height/2);
        s_plane[this.currentFrame].draw(ctx,this.x, this.y);
        ctx.restore();
    }
}

bullets = {
    _bullets : [],
    x:0.0,
    y:0.0,
    speed: 450,
    damage:1,
    direction: 1,
    isEnemyBullet: false,

    addBullet: function(_x,_y,isEnemy,_direction)
    {


        this._bullets.push({
            x: _x,
            y: _y,
            damage: this.damage,
            direction: _direction,
            isEnemyBullet: isEnemy,
        })
    },



    update: function(dt)
    {
        for (var i = this._bullets.length - 1; i >= 0; i--) {
            var _b = this._bullets[i], center_x = _b.x + 4, center_y = _b.y + 4;
            _b.y += _b.direction*this.speed*dt;
            for (var j = meteors._meteors.length - 1; j >= 0; j--) {
                
                var _m = meteors._meteors[j], d;

                d = Math.sqrt(Math.pow(center_x -(_m.x+_m.radius),2)+Math.pow(center_y -(_m.y+_m.radius),2));

                if (d<=_m.radius)
                {
                    this._bullets.splice(i,1);
                    _m.health -= _b.damage;
                    if ((_m.health < 50) && (_m.type<2))
                    {
                        _m.type += 2;
                        _m.radius = smallMeteorRadius;
                    }
                }
                else
                {
                    if (_b.isEnemyBullet)
                    {
                        d = Math.sqrt(Math.pow(center_x - plane.center_x,2)+Math.pow(center_y - plane.center_y,2));

                        if (d<= plane.radius)
                        {
                            this._bullets.splice(i,1);
                            healthBar.currentHealth -=_b.damage/10;
                        }
                    }
                    else
                    {
                        for (var i = 0; i < enemies._enemies.length; i++) {
                            var _e = enemies._enemies[i];

                            d = Math.sqrt(Math.pow(center_x - _e.center_x,2)+Math.pow(center_y - _e.center_y,2));

                            if (d<= _e.radius)
                            {
                                this._bullets.splice(i,1);
                                _e.health -=_b.damage;
                                bExists = false;
                            }
                        }
                    }
                }
            }
            if (_b.y < 0)
            {
                this._bullets.splice(i,1);
            }
        } 
    },

    draw: function()
    {
        ctx.save();
        for (var i=0; i<this._bullets.length; i++)
        {
            var _b = this._bullets[i], bulletType;
            if (_b.isEnemyBullet)
            {
                bulletType = 2;
            }
            else
            {
                bulletType = 0;
            }
            s_bullet[bulletType].draw(ctx,_b.x,_b.y);
        }
        ctx.restore();
    }
}


enemies = {
    _enemies: [],
    x:0.0,
    y:0.0,
    center_x:0.0,
    center_y:0.0,
    aspd: 0.5,
    currentTimer: 0.0,
    velocity_x: 1,
    velocity_y: 1,
    health: 50,
    radius: 83/2,

    addEnemy: function(_x,_y) {
        var randomFloat = Math.random(), xDirection;

        if (randomFloat>0.5)
        {
            xDirection = 1;
        }
        else
        {
            xDirection = -1;
        }

        this._enemies.push({
            x:_x,
            y:_y,
            center_x:0.0,
            center_y:0.0,
            aspd:this.aspd,
            currentTimer:0.0,
            velocity_x: this.velocity_x*xDirection,
            velocity_y: this.velocity_y,
            health: this.health,
            radius:this.radius,
        })
    },

    update: function(dt) {
        for (var i = this._enemies.length - 1; i >= 0; i--) {
            var _e = this._enemies[i];
            _e.center_x = _e.x + 102/2;
            _e.center_y = _e.y + 83/2;
            _e.currentTimer += dt;
            if (_e.currentTimer>_e.aspd){
                _e.currentTimer = 0;
                bullets.addBullet(_e.center_x,_e.center_y,true,+1);
            }
            _e.y += _e.velocity_y;
            _e.x += _e.velocity_x;
            if ((_e.x <=0) || (_e.x + 102)>=c_width)
            {
                _e.velocity_x = -_e.velocity_x;
            }

            if (_e.y - 83/2>=c_height)
            {
                this._enemies.splice(i,1);
            }
        }
    },

    draw: function() {
        ctx.save();
        for (var i = this._enemies.length - 1; i >= 0; i--) {
            var _e = this._enemies[i];            
            s_enemy[0].draw(ctx,_e.x,_e.y);
        }
        ctx.restore();
    }

}

meteors = {
    _meteors : [],
    x:0.0,
    y:0.0,
    radius : 0.0,
    health:100,
    damage: 10,
    center_x: 0.0,
    center_y: 0.0,

    addMeteor: function(_x,_y, _type){
        var _radius, _health;
        switch(_type){
            case 0:
            case 1:
                _radius = bigMeteorRadius;
                _health = 100;
                break;
            case 2:
            case 3:
                _radius = smallMeteorRadius;
                _health = 20;
                break;
        }


        this._meteors.push({
            x:_x,
            y:_y,
            center_x: _x + _radius,
            center_y: _y + _radius,
            type: _type,
            radius : _radius,
            health: _health,
            damage: this.damage,
        })
    },

    update: function(dt){

        for (var i = this._meteors.length - 1; i >= 0; i--) {
            var _m = this._meteors[i];
            _m.y += dt*backgroundSpeed;
            _m.center_y += dt*backgroundSpeed;
            if ((_m.y - _m.radius >= c_height)|| (_m.health <= 0))
            {
                this._meteors.splice(i,1);
            }
            
        }
    },

    draw: function(){
        ctx.save();
        for (var i = this._meteors.length - 1; i >= 0; i--) {
            var _m = this._meteors[i];

                s_meteor[_m.type].draw(ctx,_m.x,_m.y);
            }
        ctx.restore();
    }
}

vectorArrow = {
    currentRotation: 0.0,
    x: c_width-64,
    y: 40,

    update: function()
    {
        if (plane.velocity_x != 0)
        {
            this.currentRotation = Math.atan(plane.velocity_y/plane.velocity_x) + plane.velocity_x.sign()*Math.PI/2;
        }
        else
        {
            if (plane.velocity_y != 0)
            {
                this.currentRotation = plane.velocity_y.sign()*Math.PI/2;                
            }
            else
            {
                this.currentRotation = 0.0;
            }
        }
    },

    draw: function()
    {
        ctx.save();
        ctx.translate(this.x, this.y);
        ctx.rotate(this.currentRotation);
        ctx.drawImage(i_vector, -i_vector.width/2, -i_vector.height/2);
        ctx.restore();
    }
}

healthBar = {
    x:20,
    y:20,
    width: 400,
    height: 50,
    currentHealth:1.0,

    update: function(){
        if (this.currentHealth <=0)
        {
            currentState = states.GameOver;
        }
    },

    draw: function(){
        ctx.save();
        ctx.beginPath();        
        ctx.fillStyle = '#c1c1c1';
        ctx.moveTo(this.x,this.y);
        ctx.lineTo(this.x+this.width,this.y);
        ctx.lineTo(this.x+this.width,this.y+this.height);
        ctx.lineTo(this.x,this.y+this.height);
        ctx.fill();

        ctx.beginPath();
        ctx.fillStyle = '#06ff00';

        ctx.moveTo(this.x,this.y);
        ctx.lineTo(this.x+this.width*this.currentHealth,this.y);
        ctx.lineTo(this.x+this.width*this.currentHealth,this.y+this.height);
        ctx.lineTo(this.x,this.y+this.height);
        ctx.fill();

        ctx.restore();
    }
}

window.onkeydown = window.onkeyup = function(e){
    key[e.keyCode] = e.type == 'keydown';    
}

function detectCollisions(){

}


function keyinputCheck(dt)
{
    var atLeastOne = false;
    if (key[65] || key[37])
    {
        plane.currentFrame = 1;
        plane.velocity_x -= plane.speed*dt;
        atLeastOne = true;
    }
    if (key[68] || key[39])
    {                
        plane.currentFrame = 3;
        plane.velocity_x += plane.speed*dt;
        atLeastOne = true;
    }
    if (key[87] || key[38])
    {    
        plane.currentFrame = 2;
        plane.velocity_y -= plane.speed*dt;
        atLeastOne = true;
    }
    if (key[83] || key[40])
    {  
        plane.currentFrame = 4;
        plane.velocity_y += plane.speed*dt;
        atLeastOne = true;
    }
    if (key[32])
    {
        plane.shoot();
    }
    if (!atLeastOne)
    {
        plane.currentFrame = 0;
    }
}



function drawWarning(){
    ctx.save();
    ctx.translate(600,40);
    ctx.drawImage(i_warning,-64,-64);
    ctx.restore();
}

function proximitySensor(dt){
    for (var i = meteors._meteors.length - 1; i >= 0; i--) {
        var _m = meteors._meteors[i];
        //plane trayectory: x = plane.center_x + plane.velocity_x*t
        //                  t = (_m.x-plane.center_x)/plane.velocity_x
        //calculate t for x_plane = x_meteor
        //t for the plane to be on meteor's X
        var meteorYafterT = 0.0, planeYafterT = 0.0;
        if (plane.velocity_x!=0)
        {
            t = (_m.x-plane.center_x)/plane.velocity_x;
            //plane.y = plane.center_y + plane.velocity_y*t
            //_m.y = _m.center_y+backgroundspeed*dt*t;
            planeYafterT = plane.center_y + plane.velocity_y*t;
            meteorYafterT = _m.center_y+ backgroundSpeed*dt*t; 
            if ((Math.abs(planeYafterT - meteorYafterT) < (_m.radius+plane.radius))
                && (planeYafterT >= 0) && (planeYafterT <= c_height-64))    
                {
                   drawWarning();   
                   console.log('impact');               
                } 
        }
        else
        {
            if ((Math.abs(plane.center_x - _m.center_x) < (_m.radius+plane.radius))
            && (planeYafterT >= 0) && (planeYafterT <= c_height-64))
            {
                drawWarning();
                   console.log('impact');   
            } 
        }         
    }
}

function createEnviroment(dt){
    if (spawnMeteortimer >= spawnMeteorInterval)
    {        
        spawnMeteortimer = 0.0;
        var randomX = 0.0, randomFloat = 0.0;

        randomFloat = Math.random();

        if (randomFloat<=chanceToSpawnMeteor)
        {        
            randomFloat = Math.random();
            var randomType = parseInt(Math.random()*4),
                radius;

            switch (randomType){
                case 0:
                case 1:
                    radius = bigMeteorRadius;
                    break;
                case 2:
                case 3:
                    radius = smallMeteorRadius;
                    break;
            }
            var x = (c_width.clamp(radius,c_width-radius)*randomFloat);
            meteors.addMeteor(x, 0,randomType);
        }
    }    
    if (spawnEnemytimer >= spawnEnemyInterval)
    {        
        spawnEnemytimer = 0.0;
        var randomX = 0.0, randomFloat = 0.0;

        randomFloat = Math.random();
        if (randomFloat<=chanceToSpawnEnemy)
        {
            randomFloat = Math.random();
            var x = (c_width.clamp(83,c_width-83)*randomFloat);
            enemies.addEnemy(x, 0);
        }
    }

}


function update(dt){
    if (currentState == states.Playing)
    {
        spawnMeteortimer += dt;
        spawnEnemytimer += dt;
        keyinputCheck(dt);
        plane.update(dt);
        bullets.update(dt);
        createEnviroment(dt);
        meteors.update(dt);
        healthBar.update(dt);
        vectorArrow.update();
        enemies.update(dt);
    }
}

function render(dt){
    ctx.clearRect(0,0,c_width,c_height);
    plane.draw();
    bullets.draw();
    meteors.draw();
    healthBar.draw();
    vectorArrow.draw();
    proximitySensor(dt);
    enemies.draw();
}

function tick(){
    var lastTime = currentTime;
    currentTime = Date.now();
    ///dt as milisecs
    dt = (currentTime-lastTime)/1000;

    update(dt);
    render(dt);
}

function onpress(){
    currentState = states.Playing;
}

function main(){
    canvas = document.createElement("canvas");
    canvas.width = c_width;
    canvas.height = c_height;
    canvas.style.border = "1px solid #000";
    ctx = canvas.getContext("2d");
    document.body.appendChild(canvas);
    var evt = "mousedown";
    document.addEventListener(evt,onpress);


    setInterval(tick,dt);

}

window.onload = function(){
    main();
};