var 
    canvas,
    ctx,
    img,
    c_height = 600,
    c_width = 1080,
    fixedDelta = 1/60,
    gravity = 9.8,
    toRads = Math.PI/180,
    bulletStates = {bouncing:0,onCannon:1,stop:2},
    maxTimeAlive = 20,
    cannon_charge = 0.0,
    space_pressed = false,
    cannon_bar,

    cannon = {
        x:20,
        y:c_height - 128,
        angle:-30,
        impulse:40,
        center_x : 20 + s_cannon_width/2,
        center_y : c_height - 128 + s_cannon_height/2,
        barrelx: 0.0,
        barrely: 0.0,

        update: function(dt){
            this.barrelx = Math.cos(toRads*this.angle)*(s_cannon_width/2);
            this.barrely = Math.sqrt((s_cannon_width/2)*(s_cannon_width/2) - this.barrelx*this.barrelx);            
        },

        shoot: function(){
            bullet.addNewBullet(this.center_x+this.barrelx,this.center_y-this.barrely,this.angle,this.impulse*cannon_charge);
        },


        draw: function(){
            ctx.save();
            ctx.translate(this.x+s_cannon_width/2,this.y+s_cannon_height/2);
            ctx.rotate(toRads*this.angle);
            ctx.translate(-(this.x+s_cannon_width/2),-(this.y+s_cannon_height/2));
            s_cannon.draw(ctx,this.x,this.y);
            ctx.restore();
        }

    };

    bullet = {
        _bullets: [],
        x : 0,
        y : 0,
        velocity_x : 0.0,
        velocity_y : 0.0,
        bouncingReduction:0.75,
        friction:0.005,
        currentstate : bulletStates.onCannon,
        timer:0.0,

        addNewBullet: function(barrelx, barrely, angle, impulse){
            this._bullets.push({
                //b.x,b.y are the middle of the sphere
                //if b.x, b.y are used as the corner of the sphere, we need to take out the offset s_bullet_diameter/2 
                //for collisions between bullets, we use center of each bullet for easier calculation
                x:barrelx,
                y:barrely,
                velocity_x:impulse*Math.cos(toRads*angle),
                velocity_y:impulse*Math.sin(toRads*angle),
                currentstate: bulletStates.bouncing,
                timer:0.0,
            })
        },

        update: function(dt){
            var indexesToRemove = [];
            for (var i=0, len = this._bullets.length;i<len;i++)
            {
                var _b = this._bullets[i];
                //vf = v0 +a*t
                //yf = y0 + vf
                if (_b.currentstate===bulletStates.bouncing)
                {

                    _b.velocity_y += gravity*dt;

                    _b.x += _b.velocity_x; 

                    if (Math.abs(_b.velocity_y*this.bouncingReduction) > 0.4)
                    {
                        _b.y += _b.velocity_y; 
                    } 
                    else
                    {
                        //timer if not moving
                        if (Math.abs(_b.velocity_x)< 0.1){
                            _b.timer += dt;
                            if (_b.timer>=maxTimeAlive)
                            {
                                indexesToRemove.push(i);
                            }
                        }
                        else
                        {   
                            //reset timer if starts moving                         
                            _b.timer = 0;
                        }
                    } 

                    if ((_b.y + s_bullet_diameter/2 + _b.velocity_y >= c_height) || (_b.y - s_bullet_diameter/2 + _b.velocity_y<=0))
                    {
                         _b.velocity_y = -this.bouncingReduction*_b.velocity_y;
                    } 

                    if (Math.abs(_b.y + s_bullet_diameter - c_height) >= 0.01)
                    {
                        _b.velocity_x -= _b.velocity_x*this.friction;

                    }

                    if ((_b.x + s_bullet_diameter/2 + _b.velocity_x  >= c_width) || (_b.x + _b.velocity_x - s_bullet_diameter/2 <= 0))
                    {
                        _b.velocity_x = -this.bouncingReduction*_b.velocity_x;
                    } 

                    for (var j=0, len = this._bullets.length;j<len;j++)
                    {
                        if (j!=i)
                        {
                            //if distance from middle of each bullet is lower than s_bullet_diameter, there is a collision
                            var _otherBullet = this._bullets[j];
                            var distanceBetween = Math.sqrt(Math.pow((_otherBullet.x + _otherBullet.velocity_x)-(_b.x + _b.velocity_x),2)
                                                            +Math.pow((_otherBullet.y + _otherBullet.velocity_y + gravity*dt)-(_b.y + _b.velocity_y),2));

                            if (distanceBetween <= s_bullet_diameter)
                            {
                                //r = distanceBetween
                                //x = _otherbullet.x - _b.x
                                //collisionpoint = (_b.x+_otherBullet.x)/2,(_b.y+_otherBullet.y)/2,
                                //anglebetween = atan((_b.y-collisionpoint.y)/(_b.x-collisionpoint.x))
                                var collisionpoint_x = ((_otherBullet.x + _otherBullet.velocity_x)+(_b.x + _b.velocity_x))/2;
                                var collisionpoint_y = ((_otherBullet.y + _otherBullet.velocity_y)+(_b.y + _b.velocity_y))/2;
                                var anglebetween = Math.atan((_b.y-collisionpoint_y)/(_b.x-collisionpoint_x));
                                var v1, v2, angle1, angle2;

    //v1'x = v2cos(angle2 - anglebetween)*cos(anglebetween)+v1sin(angle1-anglebetween)*cos(anglebetween+pi/2);
    //v1'y = v2cos(angle2 - anglebetween)*sin(anglebetween)+v1sin(angle1-anglebetween)*sin(anglebetween+pi/2);

                                angle1 = Math.atan(_b.velocity_x/_b.velocity_y);
                                if (_otherBullet.velocity_y != 0)
                                {
                                    angle2 = Math.atan(_otherBullet.velocity_x/_otherBullet.velocity_y);
                                }
                                else
                                {
                                    angle2 = 0;
                                }
                                v1 = Math.sqrt(Math.pow(_b.velocity_x,2)+Math.pow(_b.velocity_y,2));
                                v2 = Math.sqrt(Math.pow(_otherBullet.velocity_x,2)+Math.pow(_otherBullet.velocity_y,2));

                                _b.velocity_x = v2*Math.cos(angle2 - anglebetween)*Math.cos(anglebetween)+
                                                v1*Math.sin(angle1-anglebetween)*Math.cos(anglebetween+Math.PI/2);
                                _b.velocity_y = v2*Math.cos(angle2 - anglebetween)*Math.sin(anglebetween)+
                                                v1*Math.sin(angle1-anglebetween)*Math.sin(anglebetween+Math.PI/2);

                                _otherBullet.velocity_x = v1*Math.cos(angle1 - anglebetween)*Math.cos(anglebetween)+
                                                v2*Math.sin(angle2-anglebetween)*Math.cos(anglebetween+Math.PI/2);
                                _otherBullet.velocity_y = v1*Math.cos(angle1 - anglebetween)*Math.sin(anglebetween)+
                                                v2*Math.sin(angle2-anglebetween)*Math.sin(anglebetween+Math.PI/2);


                            }                            
                        }
                    } 
                    
                }
            }
            for (var i=0,len = indexesToRemove.length; i<len;i++)
            {
                this._bullets.splice(indexesToRemove[i],1);
            }
            indexesToRemove = []
        },

        draw: function(){
            ctx.save();
            for (var i=0, len = this._bullets.length;i<len;i++)
            {
                var _b = this._bullets[i];
                s_bullet.draw(ctx, _b.x-s_bullet_diameter/2, _b.y - s_bullet_diameter/2);
            }
            ctx.restore();
        }

    };


function update(dt){
    cannon.update(dt);
    bullet.update(dt);
}

function render(){
    ctx.clearRect(0,0,c_width,c_height);
    cannon.draw();
    bullet.draw();
    pointsRectCalculation();
    drawProgressRect();
    drawBarRect(cannon_charge);
}

function tick(){
    if (space_pressed)
    {
        cannon_charge += fixedDelta/10;
        cannon_charge = cannon_charge.clamp(0,1);
    }
    else
    {
        if (cannon_charge>0)
        {
            cannon_charge -= fixedDelta/10;
            cannon_charge = cannon_charge.clamp(0,1);
        }
    }
    update(fixedDelta);
    render();
}

window.onkeydown = function(e) {
    if (e.keyCode === 38){
        cannon.angle -= 1;
    }

    if (e.keyCode === 40){
        cannon.angle += 1;
    }

    cannon.angle = cannon.angle.clamp(-90,0);

    if ((e.keyCode === 32) && !space_pressed){
        space_pressed = true;
    }
}

window.onkeyup = function(e) {
    if (e.keyCode === 32){
        cannon.shoot();
        space_pressed = false;
    }
}


function main(){
    canvas = document.createElement("canvas");
    canvas.style.border = "1px solid #000";
    canvas.height = c_height;
    canvas.width = c_width;
    ctx = canvas.getContext("2d");
    document.body.appendChild(canvas);



    var img = new Image();
    img.src = "res/Sheet.png";
    img.onload = function()
    {
        initSprites(img); 
        setInterval(tick, 0);     
    }

}

var d, theta, newX, newY, alpha, newX2, newY2;

function pointsRectCalculation(){


    theta = Math.atan(s_cannon_height/s_cannon_width);
    newX = Math.cos(toRads*cannon.angle+theta)*d;
    newY = Math.sin(toRads*cannon.angle+theta)*d;
    alpha = Math.atan((s_cannon_height-s_cannon_width*(Math.tan(toRads*cannon.angle)))/
                            (s_cannon_width+s_cannon_height*(Math.tan(toRads*cannon.angle))));
    newX2 = d*Math.cos(alpha);
    newY2 = d*Math.sin(alpha);
}

function drawProgressRect(){
    d = Math.sqrt(Math.pow(s_cannon_width/2*0.95,2)+Math.pow(s_cannon_height/2*0.9,2));
    pointsRectCalculation();
    ctx.beginPath();

    //p0 = cannon.center_x-newX, cannon.center_y-newY
    //p1 = cannon.center_x+newX2, cannon.center_y-newY2
    //p2 = cannon.center_x+newX, cannon.center_y+newY
    //p3 = cannon.center_x-newX2, cannon.center_y+newY2

    ctx.moveTo(cannon.center_x-newX, cannon.center_y-newY);

    ctx.lineTo(cannon.center_x+newX2,cannon.center_y-newY2);
    ctx.lineTo(cannon.center_x+newX, cannon.center_y+newY);
    ctx.lineTo(cannon.center_x-newX2, cannon.center_y+newY2);
    ctx.lineTo(cannon.center_x-newX, cannon.center_y-newY);


    ctx.closePath();
    ctx.fillStyle = '#ddd';
    ctx.fill();
}

function drawBarRect(percentage){
    d = Math.sqrt(Math.pow(s_cannon_width/2*0.95*percentage,2)+Math.pow(s_cannon_height/2*0.9*percentage,2));
    pointsRectCalculation();
    ctx.beginPath();
    ctx.moveTo(cannon.center_x-newX, cannon.center_y-newY);

    ctx.lineTo(cannon.center_x+newX2,cannon.center_y-newY2);
    ctx.lineTo(cannon.center_x+newX, cannon.center_y+newY);
    ctx.lineTo(cannon.center_x-newX2, cannon.center_y+newY2);
    ctx.lineTo(cannon.center_x-newX, cannon.center_y-newY);
    ctx.closePath();
    ctx.fillStyle = '#4CAF50';
    ctx.fill();
}

window.onload = function(){
    main();
}