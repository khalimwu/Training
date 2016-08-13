var canvas,
    ctx,
    height = 300,
    width = 600,
    s_cannon,
    s_playerBullet,
    s_BouncingBullet,
    states = {bouncing: 0, onCannon: 1, stopped: 2},
    yOrientation,
    fixedDelta = 1/60,

    cannon = {
        impulse: 10,
        angle: -30,
        x: 0,
        y: height - 40,
        barrel_x : 0.0,
        barrel_y : 0.0,

        shoot: function()
        {
            bullet.shoot(this.angle,this.impulse, this.barrel_x, this.barrel_y);
        },

        update: function()
        {
            this.barrel_x = this.x + Math.cos((Math.PI)/180*this.angle)*40+10;
            this.barrel_y = this.y + Math.sin((Math.PI)/180*this.angle)*40+20;
        },

        draw: function()
        {
            ctx.save();
            ctx.translate(this.x+40,this.y+20);
            ctx.rotate((Math.PI)/180*this.angle);
            ctx.translate(-this.x-40,-this.y-20);
            s_cannon.draw(ctx,this.x,this.y);
            ctx.restore();
        }
    }

    bullet = {
        _bullets: [],
        gravity : 9.8,
        mass : 10,
        velocity_x : 5.0,
        velocity_y : 2.0,
        radius: 10,
        x:20,
        y:0,
        COR:0.75,
        currentstate : states.onCannon,

        shoot: function(angle, speed, barrel_x, barrel_y)
        {
            this.velocity_x = Math.cos((Math.PI)/180*angle)*speed;
            this.velocity_y = Math.sin((Math.PI)/180*angle)*speed;
            this.currentstate = states.bouncing;
            this._bullets.push({
                x:barrel_x,
                y:barrel_y,
                velocity_x : this.velocity_x,
                velocity_y : this.velocity_y,
                currentstate : this.currentstate,
            });
        },

        update: function(){
            for (var i=0, len=this._bullets.length;i<len;i++)
            {                
                //vf = v0 +a*t
                // integral(vf) = yf
                //yf = y0 + v0t + a*t^2/2

                var _b = this._bullets[i];
                if (_b.currentstate === states.bouncing)
                {
                    _b.velocity_y += this.gravity*fixedDelta;

                    if (((_b.y + this.radius + _b.velocity_y) > canvas.height ) || (_b.y + _b.velocity_y < 0) )
                    {
                        _b.velocity_y = -this.COR*_b.velocity_y;
                        _b.velocity_x = this.COR*_b.velocity_x;
                    } 
                    else
                    {                      
                        if ((_b.x < 0) || ((_b.x + this.radius + _b.velocity_x) > width))
                        {
                            _b.velocity_x = -this.COR*_b.velocity_x;
                        }   
                    }

                    _b.x += _b.velocity_x;  

                    _b.y += _b.velocity_y;





                    if (Math.abs(_b.velocity_y) < 0.001)
                    {
                       _b.currentstate = states.stopped;
                    }    
            
                }
            }
        },

        destroy: function(){

        },

        draw: function(ctx){
            ctx.save();
            ctx.translate(this.x,this.y);
            for (var i=0, len=this._bullets.length;i<len;i++)
            {
                var _b = this._bullets[i];
                s_playerBullet.draw(ctx,_b.x,_b.y);
                ctx.beginPath();
                ctx.arc(_b.x,_b.y, this.radius, 0, Math.PI*2);
                ctx.stroke();
            }
            ctx.restore();
        }
    },

window.onkeydown = function (e) {
    var code = e.keyCode ? e.keyCode : e.which;
    if (code === 38){
        cannon.angle -= 1;
        cannon.angle = cannon.angle.clamp(-90,0);
    }
    if (code === 40){
        cannon.angle += 1;
        cannon.angle = cannon.angle.clamp(-90,0);
    }
    if (code === 32){
        cannon.shoot();
    }
}

function main() {

    canvas = document.createElement("canvas");
    canvas.height = height;
    canvas.width = width;
    ctx = canvas.getContext("2d");
    canvas.style.border = "1px solid #000";
    document.body.appendChild(canvas);
    var img = new Image();
    img.src = "res/sheet.png"

    img.onload = function()
    {
        initImg(this);
        setInterval(tick,fixedDelta);
    }

}

window.onload=function(){
     main();
}();

function tick(){
    update();
    render();
}

function update(){
    bullet.update();
    cannon.update();
}

function render(){
    ctx.clearRect(0,0,width,height);
    bullet.draw(ctx);
    cannon.draw(ctx);
}

function initImg(img){
    s_cannon = new Sprite(img, 0, 6, 80, 40);
    s_playerBullet = new Sprite(img, 88,0,20, 20);
    s_BouncingBullet = new Sprite(img, 88,24,20, 20);
}
