# YAJL (Yet Another JSON Library) binding for Node.js

[YAJL](http://lloyd.github.com/yajl/) is a low-level sax-like JSON parser
developed and maintained by Lloyd Hilaiel.

[Node-yajl](http://github.com/vibornoff/node-yajl) provides a glue that brings
YAJL into [Node.js](http://github.com/joyent/node) ecosystem. Main feature
of this project is a stream JSON parser allowing you to parse data
without explicitly delimitting data stream into valid-value chunks.

[Node-yajl](http://github.com/vibornoff/node-yajl) is forked from
[yajl-js](http://bitbucket.org/nikhilm/yajl-js) originally developed
by Nikhil Marathe and seems to be no longer maintained.

## Synopsis

```js
var yajl = require('yajl');

var h = new yajl.Handle();
h.on( 'error',      function (_) { console.debug(_)     });

h.on( 'startMap',   function ( ) { console.debug('{')   });
h.on( 'endMap',     function ( ) { console.debug('}')   });
h.on( 'startArray', function ( ) { console.debug('[')   });
h.on( 'endArray',   function ( ) { console.debug(']')   });
h.on( 'mapKey',     function (_) { console.debug(_+':') });
h.on( 'null',       function (_) { console.debug('null')});
h.on( 'boolean',    function (_) { console.debug(_)     });
h.on( 'string',     function (_) { console.debug(_)     });
h.on( 'integer',    function (_) { console.debug(_)     });
h.on( 'double',     function (_) { console.debug(_)     });

var data = [
    '{"some"',
    ':["JSON',
    '"],"data',
    '":[-3.5]}'
];

for ( var i in data ) {
    h.parse( data[i] );
}

h.completeParse();
```

## Build & install

Easy mode:

    npm install yajl

Build and install manually:

    node-waf configure build
    node ./test.js
    npm install .

Note that this package requires C++ building environment and 'libyajl' at least
of version 2 and its dev headers to be installed on target system.

See [yajl homepage](http://lloyd.github.com/yajl/) for details about building
and installing 'libyajl'.

## API

### yajl.Handle(opts)

`Handle` object constructor.
Class `Handle` itself is a subclass of `events.EventEmitter`.

Optional argument `opts` can be passed to constructor.
When `opts` is ommited or `null`, these defaults are used:

```js
{
    allowComments         : false,
    dontValidateStrings   : false,
    allowTrailingGarbage  : false,
    allowMultipleValues   : false,
    allowPartialValues    : false
}
```

#### handle.parse(string)

Parse a data chunk possibly raising number of events.

#### handle.parseComplete()

Parse any ramaining buffered data.

Because of `yajl`'s stream nature it can't sometimes determine when the recently
parsed data contains a valid JSON value or there need more data (e.g. when parsing numeric values).
So it's a good idea to call this method every time you reach the end of data stream.

#### handle.getBytesConsumed()

Get the amount of data consumed from the last chunk passed to `handle`.

In the case of a successful parse this can help you understand
if the entire buffer was consumed or not.

#### Events
 * 'startMap'
 * 'endMap'
 * 'startArray'
 * 'endArray'
 * 'mapKey'
 * 'null'
 * 'boolean'
 * 'integer'
 * 'double'
 * 'number'
 * 'string'

### yajl.Tree(string)

`Tree` object constructor.

Parses `string` into an in-memory JSON-tree.
Throws `SyntaxError` in case when `string` contains bad data.

Resulting object can be retrieved using `get` method.

#### tree.get()

Returns parsed JSON value.

## License

Modified BSD Lincense.
See ['LICENSE' file](http://github.com/vibornoff/node-yajl/blob/master/LICENSE)
for details.
