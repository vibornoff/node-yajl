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
#include <node/node_object_wrap.h>
#include <node/node_buffer.h>
#include <yajl/yajl_parse.h>

#include <string>
#include <map>

#include "handle.hh"

using namespace v8;
using namespace node;
using namespace std;

/*
 * Adapted macro from the sqlite3 bindings
 * http://github.com/grumdrig/node-sqlite/blob/master/sqlite3_bindings.cc
 * by Eric Fredricksen
 */
#define REQ_STR_ARG(ARGS, I, VAR)                                                   \
    if( (ARGS).Length() <= (I) || !(ARGS)[I]->IsString() )                          \
        return ThrowException(                                                      \
            Exception::TypeError( String::New("Argument " #I " must be a string") ) \
        );                                                                          \
    String::Utf8Value VAR((ARGS)[I]);

#define CONFIG_OPT(OPT, MASK) OPT, ( ( OPT & MASK ) && 1 )

static Persistent<String> emit_symbol;

namespace yajljs
{
    /* Static members initialization */

    yajl_callbacks Handle::callbacks = {
        OnNull, OnBoolean, OnInteger, OnDouble, OnNumber, OnString,
        OnStartMap, OnMapKey, OnEndMap, OnStartArray, OnEndArray
    };

    Handle::EventName Handle::NullEvent("null");
    Handle::EventName Handle::BooleanEvent("boolean");
    Handle::EventName Handle::IntegerEvent("integer");
    Handle::EventName Handle::DoubleEvent("double");
    Handle::EventName Handle::NumberEvent("number");
    Handle::EventName Handle::StringEvent("string");
    Handle::EventName Handle::StartMapEvent("startMap");
    Handle::EventName Handle::MapKeyEvent("mapKey");
    Handle::EventName Handle::EndMapEvent("endMap");
    Handle::EventName Handle::StartArrayEvent("startArray");
    Handle::EventName Handle::EndArrayEvent("endArray");

    /* Library initialization */

    void Handle::Initialize( v8::Handle<v8::Object> target )
    {
        v8::HandleScope scope;

        v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(New);

        t->InstanceTemplate()->SetInternalFieldCount(1);
        t->SetClassName(String::NewSymbol("Handle"));

        NODE_SET_PROTOTYPE_METHOD( t, "parse", Parse );
        NODE_SET_PROTOTYPE_METHOD( t, "completeParse", Complete );
        NODE_SET_PROTOTYPE_METHOD( t, "getBytesConsumed", GetBytesConsumed );

        emit_symbol = NODE_PSYMBOL("emit");
        target->Set( v8::String::NewSymbol( "Handle"), t->GetFunction() );
    }

    v8::Handle<v8::Value> Handle::New( const v8::Arguments& args )
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

        Handle *self = new Handle( (yajl_option)opt );
        self->Wrap( args.This() );

        return args.This();
    }

    Handle::Handle( yajl_option opt ) : ObjectWrap()
    {
        handle = yajl_alloc( &callbacks, NULL, this );

        yajl_config( handle, CONFIG_OPT(yajl_allow_comments,opt) );
        yajl_config( handle, CONFIG_OPT(yajl_dont_validate_strings,opt) );
        yajl_config( handle, CONFIG_OPT(yajl_allow_trailing_garbage,opt) );
        yajl_config( handle, CONFIG_OPT(yajl_allow_multiple_values,opt) );
        yajl_config( handle, CONFIG_OPT(yajl_allow_partial_values,opt) );
    }

    Handle::~Handle()
    {
        yajl_free( handle );
        handle = NULL;
    }

    v8::Handle<Value> Handle::Parse( const Arguments& args )
    {
        HandleScope scope;
        Handle *self = Unwrap<Handle>( args.This() );

        if( args.Length() < 1 )
            return ThrowException( Exception::TypeError( String::New("Argument required") ) );

        int parsed = 0;

        if( Buffer::HasInstance(args[0]) ) {
            Local<Object> b = args[0].As<Object>();
            parsed = self->Parse( Buffer::Data(b), Buffer::Length(b) );
        }
        else if( args[0]->IsString() ) {
            String::Utf8Value str(args[0]);
            parsed = self->Parse( *str, str.length() );
        }
        else {
            return ThrowException( Exception::TypeError( String::New("Argument must be a string or buffer") ) );
        }

        return Integer::New(parsed);
    }

    v8::Handle<Value> Handle::Complete( const Arguments& args )
    {
        HandleScope scope;

        Handle *self = Unwrap<Handle>( args.This() );
        self->Complete();

        return Null();
    }

    v8::Handle<Value> Handle::GetBytesConsumed( const Arguments& args )
    {
        HandleScope scope;

        Handle *self = Unwrap<Handle>( args.This() );
        int b = yajl_get_bytes_consumed( self->handle );

        return Integer::New(b);
    }

    int Handle::Parse( const char* str, size_t len )
    {
        int status = yajl_parse( handle, (const unsigned char*)str, len );
        if( status != yajl_status_ok )
        {
            unsigned char *errorMsg = yajl_get_error( handle, 1, (const unsigned char*)str, len );
            Local<Value> args[2] = { String::New( "error" ),
                                     String::New( (char *)errorMsg ) };
            Emit( 2, args );
            yajl_free_error( handle, errorMsg );
        }
        
        return (int)yajl_get_bytes_consumed( handle );
    }

    void Handle::Complete()
    {
        yajl_status status = yajl_complete_parse( handle );
        if( status != yajl_status_ok )
        {
            unsigned char *errorMsg = yajl_get_error( handle, 1, (const unsigned char *)"", 0 );
            Local<Value> args[2] = { String::New( "error" ),
                                     String::New( (char *)errorMsg ) };
            Emit( 2, args );
            yajl_free_error( handle, errorMsg );
        }
    }

    /* Callbacks */

    int Handle::OnNull( void *ctx )
    {
        Handle *self = static_cast<Handle *>( ctx );

        v8::Handle<Value> args[2] = { String::NewExternal(&NullEvent),
                                      Null() };
        self->Emit( 2, args );

        return 1;
    }

    int Handle::OnBoolean( void *ctx, int b )
    {
        Handle *self = static_cast<Handle *>( ctx );

        v8::Handle<Value> args[2] = { String::NewExternal(&BooleanEvent),
                                      b ? True() : False() };
        self->Emit( 2, args );

        return 1;
    }

    int Handle::OnInteger( void *ctx, long long b )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[2] = { String::NewExternal(&IntegerEvent),
                                 Integer::New( b ) };
        self->Emit( 2, args );

        return 1;
    }

    int Handle::OnDouble( void *ctx, double b )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[2] = { String::NewExternal(&DoubleEvent),
                                 Number::New( b ) };
        self->Emit( 2, args );

        return 1;
    }

    int Handle::OnNumber( void *ctx, const char *numberVal, size_t numberLen )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[2] = { String::NewExternal(&NumberEvent),
                                 String::New( numberVal, numberLen ) };
        self->Emit( 2, args );

        return 1;
    }

    int Handle::OnString( void *ctx, const unsigned char *stringVal, size_t stringLen )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[2] = { String::NewExternal(&StringEvent),
                                 String::New( (char *)stringVal, stringLen ) };
        self->Emit( 2, args );

        return 1;
    }

    int Handle::OnStartMap( void *ctx )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[1] = { String::NewExternal(&StartMapEvent) };
        self->Emit( 1, args );

        return 1;
    }

    int Handle::OnMapKey( void *ctx, const unsigned char *key, size_t stringLen )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[2] = { String::NewExternal(&MapKeyEvent),
                                 String::New( (char *)key, stringLen ) };
        self->Emit( 2, args );

        return 1;
    }

    int Handle::OnEndMap( void *ctx )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[1] = { String::NewExternal(&EndMapEvent) };
        self->Emit( 1, args );

        return 1;
    }

    int Handle::OnStartArray( void *ctx )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[1] = { String::NewExternal(&StartArrayEvent) };
        self->Emit( 1, args );

        return 1;
    }

    int Handle::OnEndArray( void *ctx )
    {
        Handle *self = static_cast<Handle *>( ctx );

        Local<Value> args[1] = { String::NewExternal(&EndArrayEvent) };
        self->Emit( 1, args );

        return 1;
    }

    /* v8::String::ExternalAsciiStringResource wrapper around std::string */

    Handle::EventName::EventName( const char* s )
        : String::ExternalAsciiStringResource()
    {
        str = s;
    }

    Handle::EventName::EventName( const string& s )
        : String::ExternalAsciiStringResource()
    {
        str = s;
    }

    void Handle::EventName::Dispose()
    {
        // do nothing
    }

    const char* Handle::EventName::data() const
    {
        return str.c_str();
    }

    size_t Handle::EventName::length() const
    {
        return str.length();
    }

    Handle::EventName::operator const string& () const
    {
        return str;
    }

    /* Event emitting */
    /* http://permalink.gmane.org/gmane.comp.lang.javascript.nodejs/28298 */
    /* http://groups.google.com/group/nodejs/browse_thread/thread/53cd7a16a6a39f4b */

    void Handle::Emit(int nargs, v8::Handle<v8::Value> *args)
    {
        Local<Value> emit_v = handle_->Get(emit_symbol);
        Local<Function> emit = Local<Function>::Cast(emit_v);
        TryCatch tc;
        emit->Call(handle_, nargs, args);
        if (tc.HasCaught()) {
            node::FatalException(tc);
        }
    }
}
