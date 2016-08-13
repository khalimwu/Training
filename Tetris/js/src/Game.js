define(function()
{
    var Game = Class.extend({
        run: function(){
            if (this._running) return;
            this._running =true;
            var self = this;
            function loop(){
                self._reqframe = window.requestAnimationFrame(loop);
                self.tick();
                canvas.flip();
            }
            this._reqframe = window.requestAnimationFrame(loop);
        },

        stop: function()
        {
            if (this._reqframe)
            {
                window.cancelAnimationFrame(this._reqframe);
            }
            this._reqframe = null;
            this._running = false;
        },

        tick: function(){

        }
    });
    return Game;
});