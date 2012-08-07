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

#ifndef YAJLJS_HANDLE_HH
#define YAJLJS_HANDLE_HH

#include <v8.h>
#include <node.h>
#include <node/node_object_wrap.h>
#include <yajl/yajl_parse.h>

#include <string>
#include <map>

namespace yajljs
{
    class Handle : public node::ObjectWrap
    {
      public:
        static void Initialize( v8::Handle<v8::Object> target );

        static v8::Handle<v8::Value> Parse( const v8::Arguments& args );
        static v8::Handle<v8::Value> Complete( const v8::Arguments& args );
        static v8::Handle<v8::Value> GetBytesConsumed( const v8::Arguments& args );
/*
        static v8::Handle<v8::Value> AddListener(const v8::Arguments& args);
        static v8::Handle<v8::Value> RemoveListener(const v8::Arguments& args);
        static v8::Handle<v8::Value> RemoveAllListeners(const v8::Arguments& args);
*/
      protected:
        static v8::Handle<v8::Value> New( const v8::Arguments& args );

        Handle( yajl_option opt );
        ~Handle();

        int Parse( const char*, size_t );
        void Complete();

      private:
        yajl_handle handle;
/*
        std::map<std::string,int*> listener_counters;

        int on_null_count;
        int on_boolean_count;
        int on_integer_count;
        int on_double_count;
        int on_number_count;
        int on_string_count;
        int on_startMap_count;
        int on_mapKey_count;
        int on_endMap_count;
        int on_startArray_count;
        int on_endArray_count;
*/
      private:
        class EventName : public v8::String::ExternalAsciiStringResource
        {
          public:
            EventName( const char * s );
            EventName( const std::string& s );

            void Dispose ();

            const char* data() const;
            size_t length() const;

            operator const std::string& () const;

          private:
            std::string str;
        };

        static EventName NullEvent;
        static EventName BooleanEvent;
        static EventName IntegerEvent;
        static EventName DoubleEvent;
        static EventName NumberEvent;
        static EventName StringEvent;
        static EventName StartMapEvent;
        static EventName MapKeyEvent;
        static EventName EndMapEvent;
        static EventName StartArrayEvent;
        static EventName EndArrayEvent;

        static int OnNull( void *ctx );
        static int OnBoolean( void *ctx, int b );
        static int OnInteger( void *ctx, long long b );
        static int OnDouble( void *ctx, double b );
        static int OnNumber( void *ctx, const char *numberVal, size_t numberLen );
        static int OnString( void *ctx, const unsigned char *stringVal, size_t stringLen );
        static int OnStartMap( void *ctx );
        static int OnMapKey( void *ctx, const unsigned char *key, size_t stringLen );
        static int OnEndMap( void *ctx );
        static int OnStartArray( void *ctx );
        static int OnEndArray( void *ctx );

        static yajl_callbacks callbacks;

        void Emit(int nargs, v8::Handle<v8::Value> *args);
    };
}

#endif
