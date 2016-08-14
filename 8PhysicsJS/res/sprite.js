var s_cannon, s_bullet,
    s_cannon_width = 255,
    s_cannon_height = 128;
    s_bullet_diameter = 128;


function initSprites(img)
{
    s_cannon = new Sprite(img,0,0,s_cannon_width,s_cannon_height);
    s_bullet = new Sprite(img,256,0,s_bullet_diameter,s_bullet_diameter);
}