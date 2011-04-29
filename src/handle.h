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

#ifndef YAJLJS_HANDLE_H
#define YAJLJS_HANDLE_H

#include <v8.h>
#include <node.h>
#include <node/node_events.h>
#include <yajl/yajl_parse.h>

namespace yajljs
{
    class Handle : public node::EventEmitter
    {
      public:
        static void Initialize ( v8::Handle<v8::Object> target );

        static v8::Handle<v8::Value> Parse( const v8::Arguments& args );
        static v8::Handle<v8::Value> CompleteParse( const v8::Arguments& args );
        static v8::Handle<v8::Value> GetBytesConsumed( const v8::Arguments& args );

      protected:
        static v8::Handle<v8::Value> New (const v8::Arguments& args);

        Handle( yajl_option opt );
        ~Handle();

      private:
        int Parse( unsigned char*, int );
        void CompleteParse();

        yajl_handle yc_handle;
        yajl_callbacks callbacks;
    };

    int OnNull( void *ctx );
    int OnBoolean( void *ctx, int b );
    int OnInteger( void *ctx, long long b );
    int OnDouble( void *ctx, double b );
    int OnNumber( void *ctx, const char *numberVal, size_t numberLen );
    int OnString( void *ctx, const unsigned char *stringVal, size_t stringLen );
    int OnStartMap( void *ctx );
    int OnMapKey( void *ctx, const unsigned char *key, size_t stringLen );
    int OnEndMap( void *ctx );
    int OnStartArray( void *ctx );
    int OnEndArray( void *ctx );
}

#endif
