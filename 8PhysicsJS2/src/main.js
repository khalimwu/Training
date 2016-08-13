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

    cannon = {
        x:20,
        y:c_height - 128,
        angle:-30,
        impulse:20,
        center_x : 20 + s_cannon_width/2,
        center_y : c_height - 128 + s_cannon_height/2,

        update: function(dt){
        },

        shoot: function(){
            var barrelx = Math.cos(toRads*this.angle)*(s_cannon_width/2);
            var barrely = Math.sqrt((s_cannon_width/2)*(s_cannon_width/2) - barrelx*barrelx);
            bullet.addNewBullet(this.center_x+barrelx,this.center_y-barrely,this.angle,this.impulse);
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
        //lastHighesY : c_height,

        addNewBullet: function(barrelx, barrely, angle, impulse){
            this._bullets.push({
                x:barrelx - s_bullet_diameter/2,
                y:barrely - s_bullet_diameter/2,
                velocity_x:impulse*Math.cos(toRads*angle),
                velocity_y:impulse*Math.sin(toRads*angle),
                currentstate: bulletStates.bouncing,
                lastHighesY : c_height,
            })
        },

        update: function(dt){
            for (var i=0, len = this._bullets.length;i<len;i++)
            {
                var _b = this._bullets[i];
                //vf = v0 +a*t
                //yf = y0 + vf
                if (_b.currentstate===bulletStates.bouncing)
                {
                    //x,y are the center of sphere

                    if (Math.abs(_b.velocity_y) < 0.5)
                    {
                        _b.lastHighesY = c_height-_b.y;
                    }

                    _b.velocity_y += gravity*dt;

                    

                    if ((_b.y + s_bullet_diameter + _b.velocity_y >= c_height) || (_b.y <=0))
                    {
                         _b.velocity_y = -this.bouncingReduction*_b.velocity_y;
                    } 

                    if (Math.abs(_b.y + s_bullet_diameter - c_height) >= 0.01)
                    {
                        _b.velocity_x -= _b.velocity_x*this.friction;

                    }

                    if ((_b.x + s_bullet_diameter >= c_width) || (_b.x <= 0))
                    {
                        _b.velocity_x = -this.bouncingReduction*_b.velocity_x;
                    }                      
                              

                    if (Math.abs(_b.velocity_y*this.bouncingReduction) > 0.4)
                    {
                        _b.y += _b.velocity_y; 
                    } 
                    _b.x += _b.velocity_x;                 
                }
            }
        },

        draw: function(){
            ctx.save();
            for (var i=0, len = this._bullets.length;i<len;i++)
            {
                var _b = this._bullets[i];
                s_bullet.draw(ctx, _b.x, _b.y);
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
}

function tick(){
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

    if (e.keyCode === 32){
        cannon.shoot();
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



window.onload = function(){
    main();
}();