/*
 * Copyright (c) 2011, Artem S Vybornov <vybornov@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <v8.h>
#include <node.h>
#include <node/node_object_wrap.h>
#include <yajl/yajl_tree.h>

#include "tree.h"

using namespace v8;
using namespace node;

namespace yajljs
{
    Tree::Tree ( yajl_val t ) : ObjectWrap()
    {
        tree = t;
    }

    Tree::~Tree ()
    {
        yajl_tree_free( tree );
    }

    Handle<Value> Tree::New ( const Arguments& args )
    {
        HandleScope scope;

        if( args.Length() <= 0 || !args[0]->IsString() ) {
            return ThrowException( Exception::TypeError(
                String::New("Argument 0 must be a string")
            ));
        }

        String::Utf8Value str(args[0]);
        char errmsg[512]; errmsg[511] = 0;
        yajl_val t = yajl_tree_parse( *str, errmsg, 511 );
        if( t == NULL ) {
            return ThrowException( Exception::SyntaxError(
                String::New(errmsg)
            ));
        }

        Tree* self = new Tree( t );
        self->Wrap( args.This() );

        return args.This();
    }

    Handle<Value> Tree::Get ( const Arguments& args )
    {
        HandleScope scope;
        yajl_val t;

        Tree *self = Unwrap<Tree>( args.This() );
        if ( !self || !( t = self->tree ) )
            return ThrowException( Exception::TypeError(
                String::New("Illegal invocation")
            ));

        return tree_get(t);
    }

    Handle<Value> Tree::tree_get ( const yajl_val v )
    {
        if( v == NULL )
            return Undefined();

        if( YAJL_IS_NULL(v) )
            return Null();

        if( YAJL_IS_FALSE(v) )
            return False();

        if( YAJL_IS_TRUE(v) )
            return True();

        if( YAJL_IS_STRING(v) )
            return String::New( v->u.string );

        if( YAJL_IS_INTEGER(v) )
            return Integer::New( v->u.number.i );

        if( YAJL_IS_DOUBLE(v) )
            return Number::New( v->u.number.d );

        if( YAJL_IS_ARRAY(v) ) {
            Local<Array> a = Array::New( v->u.array.len );
            for ( int i = 0; i < v->u.array.len; i++ ) {
                a->Set( i, tree_get( (v->u.array.values)[i] ));
            }
            return a;
        }

        if( YAJL_IS_OBJECT(v) ) {
            Local<Object> o = Object::New();
            for ( int i = 0; i < v->u.object.len; i++ ) {
                o->Set( String::New((v->u.object.keys)[i]), tree_get( (v->u.array.values)[i] ));
            }
            return o;
        }

        return ThrowException( Exception::Error(
            String::New("Bad value type")
        ));
    }

    void Tree::Initialize ( Handle<Object> target )
    {
        HandleScope scope;

        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        t->InstanceTemplate()->SetInternalFieldCount(1);
        t->SetClassName(String::NewSymbol("Tree"));

        NODE_SET_PROTOTYPE_METHOD( t, "get", Get );

        target->Set( String::NewSymbol("Tree"), t->GetFunction() );
    }
}
