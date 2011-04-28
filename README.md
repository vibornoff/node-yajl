# YAJL (Yet Another JSON Library) binding for Node.js

[YAJL](http://lloyd.github.com/yajl/) is a low-level sax-like JSON parser
developed and maintained by Lloyd Hilaiel.

[Node-yajl](http://github.com/vibornoff/node-yajl) provides a glue that brings
YAJL into [Node.js](http://github.com/joyent/node) ecosystem. Main feature
of this project is a stream JSON parser allowing you to parse data
without explicitly delimitting data stream into valid-value chunks.

Node-yajl is forked from [yajl-js](http://bitbucket.org/nikhilm/yajl-js)
originally developed by Nikhil Marathe and seems to be no longer maintained.

## Synopsis

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
        "],"data',
        ":[-3.5]}'
    ];
    
    for ( var i in data ) {
        h.parse( data[i] );
    }

## Build & install

Build:
    node-waf configure build

Run test:
    node ./test.js

Install
    npm install .
    -or-
    npm install yajl

Note this package requires 'libyajl' and its dev headers to be installed on target system.
See [yajl homepage](http://lloyd.github.com/yajl/) for details about building and installing it.

## API

### yajl.Handle(opts)

`Handle` object constructor.
Class `Handle` itself is a subclass of `events.EventEmitter`.

Optional argument `opts` can be passed to constructor.
When `opts` is ommited or `null`, these defaults are used:
    {
        allowComments         : false,
        dontValidateStrings   : false,
        allowTrailingGarbage  : false,
        allowMultipleValues   : false,
        allowPartialValues    : false
    }

#### handle.parse(string)

Parse a data chunk possibly raising number of events.

#### handle.parseComplete()

#### handle.getBytesConsumed()

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

    Copyright (c) 2010, Nikhil Marathe <nsm.nikhil@gmail.com>
    Copyright (c) 2011, Artem S Vybornov <vybornov@gmail.com>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of the Nikhil Marathe nor the name of the Artem S Vybornov
          nor the names of its contributors may be used to endorse or promote products
          derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

