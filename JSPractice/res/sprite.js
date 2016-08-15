var
    s_plane,
    s_bullet,
    s_BigBrownMeteor,
    s_BigGreyMeteor,
    s_SmallBrownMeteor,
    s_SmallGreyMeteor,    
    s_enemy,
    s_meteor,
    i_vector,
    i_warning,


    plane_img,
    bullet_img,
    sheet_img,

    i_vector = new Image();
    i_vector.src = "res/vectorArrow.png";

    i_warning = new Image();
    i_warning.src = "res/impact.png";

    bigMeteorRadius = 97/2,
    smallMeteorRadius = 39/2;

    sheet_img = new Image();
    sheet_img.src = "res/sheet.png";

    bullet_img = new Image();
    bullet_img.src = "res/bullets.png";

    plane_img = new Image();
    plane_img.src = "res/planeSheet.png";
    s_plane = [
        new Sprite(plane_img,0,0,64,63),
        new Sprite(plane_img,64,0,64,63),
        new Sprite(plane_img,0,64,64,63),
        new Sprite(plane_img,64,64,64,63),
        new Sprite(plane_img,0,128,64,63)
    ];

    //0 red, 1 yellow, 2 purple
    s_bullet = [
        new Sprite(bullet_img,0,0,8,8),
        new Sprite(bullet_img,9,0,8,8),
        new Sprite(bullet_img,17,0,8,8)
    ];

    s_enemy = [
        new Sprite(sheet_img,142,156,102,83)
    ];

    s_meteor = [
        //0 type
        new Sprite(sheet_img,0,520,120,97),
        //1
        new Sprite(sheet_img,0,617,120,97),
        //2
        new Sprite(sheet_img,237,453,45,39),
        //3
        new Sprite(sheet_img,282,453,45,39)
    ];

    s_BigBrownMeteor = [
        new Sprite(sheet_img,0,520,120,97)
    ];

    s_BigGreyMeteor = [
        new Sprite(sheet_img,0,617,120,97)
    ];
    
    s_SmallBrownMeteor = [
        new Sprite(sheet_img,237,453,45,39)
    ];

    s_SmallGreyMeteor = [
        new Sprite(sheet_img,282,453,45,39)
    ]; 