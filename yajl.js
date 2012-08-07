var yajl = require("./build/Release/yajl"),
    EventEmitter = require("events").EventEmitter;

yajl.Handle.prototype.__proto__ = EventEmitter.prototype;

module.exports = yajl;
