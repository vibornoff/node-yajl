/*
 * Copyright 2010, Nikhil Marathe <nsm.nikhil@gmail.com> All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Nikhil Marathe nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <v8.h>
#include <node.h>
#include <node/node_events.h>
#include <yajl/yajl_parse.h>

#include "handle.h"

using namespace v8;
using namespace node;

/*
 * Macro from the sqlite3 bindings
 * http://github.com/grumdrig/node-sqlite/blob/master/sqlite3_bindings.cc
 * by Eric Fredricksen
 */
#define REQ_STR_ARG(I, VAR)                                                         \
    if( args.Length() <= (I) || !args[I]->IsString() )                              \
        return ThrowException(                                                      \
            Exception::TypeError( String::New("Argument " #I " must be a string"))  \
        );                                                                          \
    String::Utf8Value VAR(args[I]->ToString());

#define CONFIG_OPT(OPT, MASK) OPT, ( ( OPT & MASK ) && 1 )

#define INIT_CB( ctx )                          \
    HandleScope scope;                          \
    Handle *yh = static_cast<Handle *>( ctx )

namespace yajljs
{
    void Handle::Initialize ( v8::Handle<v8::Object> target )
    {
        v8::HandleScope scope;

        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);

        t->Inherit( EventEmitter::constructor_template );
        t->InstanceTemplate()->SetInternalFieldCount(1);
        t->SetClassName(String::NewSymbol("Handle"));

        NODE_SET_PROTOTYPE_METHOD( t, "parse", Parse );
        NODE_SET_PROTOTYPE_METHOD( t, "completeParse", CompleteParse );
        NODE_SET_PROTOTYPE_METHOD( t, "getBytesConsumed", GetBytesConsumed );

        target->Set( v8::String::NewSymbol( "Handle"), t->GetFunction() );
    }

    v8::Handle<v8::Value> Handle::New (const v8::Arguments& args)
    {
        v8::HandleScope scope;

        int opt = 0;

        if( args.Length() >= 1 && args[0]->IsObject() )
        {
            Local<Object> obj = args[0]->ToObject();

            opt |= ( obj->Get( String::New( "allowComments" ) )->ToInteger()->Value()       ) ? yajl_allow_comments         : 0;
            opt |= ( obj->Get( String::New( "dontValidateStrings" ) )->ToInteger()->Value() ) ? yajl_dont_validate_strings  : 0;
            opt |= ( obj->Get( String::New( "allowTrailingGarbage" ) )->ToInteger()->Value()) ? yajl_allow_trailing_garbage : 0;
            opt |= ( obj->Get( String::New( "allowMultipleValues" ) )->ToInteger()->Value() ) ? yajl_allow_multiple_values  : 0;
            opt |= ( obj->Get( String::New( "allowPartialValues" ) )->ToInteger()->Value()  ) ? yajl_allow_partial_values   : 0;
        }

        Handle *handle = new Handle( (yajl_option)opt );
        handle->Wrap( args.This() );

        return args.This();
    }

    Handle::Handle( yajl_option opt ) : EventEmitter()
    {
        callbacks.yajl_null = OnNull;
        callbacks.yajl_boolean = OnBoolean;
        callbacks.yajl_integer = OnInteger;
        callbacks.yajl_double = OnDouble;
        callbacks.yajl_number = OnNumber;
        callbacks.yajl_string = OnString;
        callbacks.yajl_start_map = OnStartMap;
        callbacks.yajl_map_key = OnMapKey;
        callbacks.yajl_end_map = OnEndMap;
        callbacks.yajl_start_array = OnStartArray;
        callbacks.yajl_end_array = OnEndArray;
        
        yc_handle = yajl_alloc( &callbacks, NULL, this );

        yajl_config( yc_handle, CONFIG_OPT(yajl_allow_comments,opt) );
        yajl_config( yc_handle, CONFIG_OPT(yajl_dont_validate_strings,opt) );
        yajl_config( yc_handle, CONFIG_OPT(yajl_allow_trailing_garbage,opt) );
        yajl_config( yc_handle, CONFIG_OPT(yajl_allow_multiple_values,opt) );
        yajl_config( yc_handle, CONFIG_OPT(yajl_allow_partial_values,opt) );
    }

    Handle::~Handle()
    {
        yajl_free( yc_handle );
        yc_handle = NULL;
    }

    v8::Handle<Value> Handle::Parse( const Arguments& args )
    {
        HandleScope scope;

        REQ_STR_ARG(0, parseString);

        Handle *yh = Unwrap<Handle>( args.This() );
        int parsed = yh->Parse( const_cast<unsigned char*>( (unsigned char *) *parseString ), parseString.length() );

        return Integer::New(parsed);
    }

    v8::Handle<Value> Handle::CompleteParse( const Arguments& args )
    {
        HandleScope scope;

        Handle *yh = Unwrap<Handle>( args.This() );
        yh->CompleteParse();

        return Null();
    }

    v8::Handle<Value> Handle::GetBytesConsumed( const Arguments& args )
    {
        HandleScope scope;

        Handle *yh = Unwrap<Handle>( args.This() );
        int b = yajl_get_bytes_consumed( yh->yc_handle );

        return Integer::New(b);
    }

    int Handle::Parse( unsigned char* str, int len )
    {
        int status = yajl_parse( yc_handle, str, len );
        if( status != yajl_status_ok )
        {
            unsigned char *errorMsg = yajl_get_error( yc_handle, 1, str, len );
            Local<Value> args[1] = { String::New( (char *)errorMsg ) };
            Emit( String::New( "error" ), 1, args );
            yajl_free_error( yc_handle, errorMsg );
        }
        
        return (int)yajl_get_bytes_consumed( yc_handle );
    }

    void Handle::CompleteParse()
    {
        yajl_status status = yajl_complete_parse( yc_handle );
        if( status != yajl_status_ok )
        {
            unsigned char *errorMsg = yajl_get_error( yc_handle, 1, (unsigned char *)"", 0 );
            Local<Value> args[1] = { String::New( (char *)errorMsg ) };
            Emit( String::New( "error" ), 1, args );
            yajl_free_error( yc_handle, errorMsg );
        }
    }

    /**
     * Callbacks
     */

    int OnNull( void *ctx )
    {
        INIT_CB( ctx );
        v8::Handle<Value> args[1] = { Null() };
        yh->Emit( String::New("null"), 1, args );
        return 1;
    }

    int OnBoolean( void *ctx, int b )
    {
        INIT_CB( ctx );
        v8::Handle<Value> args[1] = { b ? True() : False() };
        yh->Emit( String::New("boolean"), 1, args );
        return 1;
    }

    int OnInteger( void *ctx, long long b )
    {
        INIT_CB( ctx );

        Local<Value> args[1] = { Integer::New( b ) };
        yh->Emit( String::New("integer"), 1, args );
        return 1;
    }

    int OnDouble( void *ctx, double b )
    {
        INIT_CB( ctx );

        Local<Value> args[1] = { Number::New( b ) };
        yh->Emit( String::New("double"), 1, args );
        return 1;
    }

    int OnNumber( void *ctx, const char *numberVal, size_t numberLen )
    {
        INIT_CB( ctx );

        Local<Value> args[1] = { String::New( numberVal, numberLen ) };
        yh->Emit( String::New("number"), 1, args );
        return 1;
    }

    int OnString( void *ctx, const unsigned char *stringVal, size_t stringLen )
    {
        INIT_CB( ctx );
        Local<Value> args[1] = { String::New( (char *)stringVal, stringLen ) };
        yh->Emit( String::New("string"), 1, args );
        return 1;
    }

    int OnStartMap( void *ctx )
    {
        INIT_CB( ctx );
        yh->Emit( String::New("startMap"), 0, NULL );
        return 1;
    }

    int OnMapKey( void *ctx, const unsigned char *key, size_t stringLen )
    {
        INIT_CB( ctx );
        Local<Value> args[1] = { String::New( (char *)key, stringLen ) };
        yh->Emit( String::New("mapKey"), 1, args );
        return 1;
    }

    int OnEndMap( void *ctx )
    {
        INIT_CB( ctx );
        yh->Emit( String::New("endMap"), 0, NULL );
        return 1;
    }

    int OnStartArray( void *ctx )
    {
        INIT_CB( ctx );
        yh->Emit( String::New("startArray"), 0, NULL );
        return 1;
    }

    int OnEndArray( void *ctx )
    {
        INIT_CB( ctx );
        yh->Emit( String::New("endArray"), 0, NULL );
        return 1;
    }
}
