Number.prototype.clamp = function(min, max) {
  return Math.min(Math.max(this, min), max);
};


Sprite.prototype.draw = function (ctx, x, y) {
    ctx.drawImage(this.img, this.x, this.y, this.width, this.height, x, y, this.width, this.height);
};


function Sprite(img, x, y, width, height) {
    this.img = img;
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
};
