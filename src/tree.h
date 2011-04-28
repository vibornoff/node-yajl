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

#ifndef YAJLJS_TREE_H
#define YAJLJS_TREE_H

#include <v8.h>
#include <node.h>
#include <yajl/yajl_tree.h>

namespace yajljs
{
    class Tree : public node::ObjectWrap
    {
    public:
        static void Initialize ( v8::Handle<v8::Object> target );

        static v8::Handle<v8::Value> New( const v8::Arguments& args );
        static v8::Handle<v8::Value> Get( const v8::Arguments& args );

    protected:
        Tree( yajl_val t );
        ~Tree();

        static v8::Handle<v8::Value> tree_get ( const yajl_val );

    private:
        yajl_val tree;

    };
}

#endif
