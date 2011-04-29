// Will be a complete reformatter in javascript similar to the Yajl reformatter

require.paths.unshift( __dirname + '/build/default' );

var yajl = require('yajl');

var n = 10000, v, start_t, end_t;

var t1 = '["char/1","enterBattle",["lib/maps/test2.json","SimplePVE",0,[{"acc_id":1,"char_id":1,"coord":[18,5],"id":1,"appearance_avatar":"lib/graphics/icons/icon_digits/00.png","exp":[0,9],"appearance_world":"lib/graphics/gfx/creature/a1.swf","effects":[{"icon":null,"ltt":1303999023731,"btl":null,"ttl":15,"name":"Амфетамин","id":1}],"hp":[105,105],"name":"Основной Юнит Васи","stats":{"favor":5,"damage":5,"evade":1,"fire_rate":25.09,"agile":5,"max_up":10,"up":0,"speed":5,"power":5,"hp":105,"penetrate":10,"armor":0,"level":0,"max_hp":105,"skill":5,"celerity":5,"ideal":1,"accuracy":109},"unit_id":1,"group_id":"1","hidden":0,"items":[{"icon":"lib/graphics/icons/icon_digits/02.png","count":1,"usemode":null,"stats":{"damage":5,"penetrate":10,"burst":3,"range":5,"base_fire_rate":24,"recoil":1,"base_accuracy":105,"dps":5,"damage_type":1},"max_up":10,"name":"Тестовый автомат","up":0,"proto_id":"test_weapon","slotsAvailable":[1],"id":1,"type":"Weapon","price":[{"money0":0}],"quality":null,"slot":1}],"units":[]},{"acc_id":null,"char_id":"OBJ#158942488","coord":[7,9],"id":"OBJ#158942488","appearance_avatar":"lib/graphics/icons/icon_digits/00.png","exp":[0,9],"appearance_world":"lib/graphics/gfx/creature/a1.swf","effects":[{"icon":null,"ltt":1303999023731,"btl":null,"ttl":15,"name":"Амфетамин","id":1}],"hp":[105,105],"name":"Основной Юнит Васи","stats":{"favor":5,"damage":5,"evade":1,"fire_rate":25.09,"agile":5,"max_up":10,"up":0,"speed":5,"power":5,"hp":105,"penetrate":10,"armor":0,"level":0,"max_hp":105,"skill":5,"celerity":5,"ideal":1,"accuracy":109},"unit_id":null,"group_id":"1","hidden":0,"items":[{"icon":"lib/graphics/icons/icon_digits/02.png","count":1,"usemode":null,"stats":{"damage":5,"penetrate":10,"burst":3,"range":5,"base_fire_rate":24,"recoil":1,"base_accuracy":105,"dps":5,"damage_type":1},"max_up":10,"name":"Тестовый автомат","up":0,"proto_id":"test_weapon","slotsAvailable":[1],"id":"test_weapon","type":"Weapon","price":[{"money0":0}],"quality":null,"slot":1}],"units":[]}],[{"icon":"lib/graphics/icons/icon_digits/04.png","count":6,"cd":1000,"cd_end_time":null,"group_id":5,"name":"Безоболочная граната","id":159046752,"enabled":1},{"icon":"lib/graphics/icons/icon_digits/03.png","count":4,"cd":1000,"cd_end_time":null,"group_id":3,"name":"Аптечка","id":159200792,"enabled":1},{"icon":"lib/graphics/icons/icon_digits/05.png","count":3,"cd":1000,"cd_end_time":null,"group_id":4,"name":"Бронебойные патроны","id":159364728,"enabled":1},{"icon":"lib/graphics/icons/icon_digits/01.png","count":7,"cd":5000,"cd_end_time":null,"group_id":1,"name":"Поддержка с воздуха","id":159199128,"enabled":1},{"icon":"lib/graphics/icons/icon_digits/16.png","count":8,"cd":1000,"cd_end_time":null,"group_id":3,"name":"Дефибриллятор","id":159104112,"enabled":1},{"icon":"lib/graphics/icons/icon_digits/16.png","count":5,"cd":1000,"cd_end_time":null,"group_id":2,"name":"Амфетамин","id":159201416,"enabled":1}]],158569712]';
console.log("Test t1:");
{
    var h = new yajl.Handle({ allowMultipleValues: true, dontValidateStrings: true });
    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        h.parse(t1);
    end_t = (new Date).getTime();
    console.log( "YAJL Handle parse: " + n + " t1 values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = new yajl.Tree(t1);
    end_t = (new Date).getTime();
    console.log( "YAJL Tree parse: " + n + " t1 values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = JSON.parse(t1);
    end_t = (new Date).getTime();
    console.log( "JSON parse: " + n + " t1 values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );
}

var t2 = '["battleEvent",[1303999015862,[["action",["OBJ#158942488","shot",[4,1,0,300],[10,6]],300,[["damage",[1,-6],0,[["action",["OBJ#158942488","shot",[4,1,0,300],[10,6]],300,[["damage",[1,-6],0,[["action",["OBJ#158942488","shot",[4,1,0,300],[10,6]],300,[["damage",[1,-6],0,[]]]]]]]]]]]]]]]';
console.log("Test t2:");
{
    var h = new yajl.Handle({ allowMultipleValues: true, dontValidateStrings: true });
    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        h.parse(t2);
    end_t = (new Date).getTime();
    console.log( "YAJL Handle parse: " + n + " t1 values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = new yajl.Tree(t2);
    end_t = (new Date).getTime();
    console.log( "YAJL Tree parse: " + n + " t2 values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = JSON.parse(t2);
    end_t = (new Date).getTime();
    console.log( "JSON parse: " + n + " t2 values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );
}

var t3 = '["battleEvent",[1303999015862,[["action",["OBJ#15';
console.log("Test t3:");
try {
    v = new yajl.Tree(t3);
    v.get();
}
catch (err) {
    if ( err instanceof SyntaxError ) {
        console.log( "OK: caught SyntaxError when parsing bad data" );
    }
    else {
        throw err;
    }
}
