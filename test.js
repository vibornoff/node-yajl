// Will be a complete reformatter in javascript similar to the Yajl reformatter

var yajl = require('yajl'),
    fs  = require('fs');

var n = 10000, v, start_t, end_t;

var s1 = fs.readFileSync('test1.json','utf8');
var b1 = fs.readFileSync('test1.json');
console.log("test1.json:");
{
    var h = new yajl.Handle({ allowMultipleValues: true, dontValidateStrings: true });
    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        h.parse(s1);
    end_t = (new Date).getTime();
    console.log( "YAJL Handle parse: " + n + " string values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        h.parse(b1);
    end_t = (new Date).getTime();
    console.log( "YAJL Handle parse: " + n + " buffer values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = new yajl.Tree(s1);
    end_t = (new Date).getTime();
    console.log( "YAJL Tree parse: " + n + " string values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s ): " + JSON.stringify(v) );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = JSON.parse(s1);
    end_t = (new Date).getTime();
    console.log( "JSON parse: " + n + " string values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );
}

var s2 = fs.readFileSync('test2.json','utf8');
var b2 = fs.readFileSync('test2.json');
console.log("test2.json:");
{
    var h = new yajl.Handle({ allowMultipleValues: true, dontValidateStrings: true });
    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        h.parse(s2);
    end_t = (new Date).getTime();
    console.log( "YAJL Handle parse: " + n + " string values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        h.parse(b2);
    end_t = (new Date).getTime();
    console.log( "YAJL Handle parse: " + n + " buffer values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = new yajl.Tree(s2);
    end_t = (new Date).getTime();
    console.log( "YAJL Tree parse: " + n + " string values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );

    start_t = (new Date).getTime();
    for ( var i = 0; i < n; i++ )
        v = JSON.parse(s2);
    end_t = (new Date).getTime();
    console.log( "JSON parse: " + n + " string values: " + ( end_t - start_t ) + " ms ( ~" + Math.round( 1000 * n / ( end_t - start_t ) ) + "/s )" );
}

var s3 = '["battleEvent",[1303999015862,[["action",["OBJ#15';
console.log("test3:");
try {
    v = new yajl.Tree(s3);
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
