// Copyright (c) 2019-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

namespace tao::pegtl
{
   // clang-format off
   struct A : one< 'a' > {};
   struct B : one< 'b' > {};
   struct AB : seq< A, B > {};
   // clang-format on

   template< typename >
   struct my_action
   {};

   template<>
   struct my_action< A >
   {
      static void apply0( int& c )
      {
         TAO_PEGTL_TEST_ASSERT( c == 0 );
         c = 1;
      }
   };

   struct S
   {
      int v = 0;

      template< typename ParseInput >
      explicit S( const ParseInput& /*unused*/, int& c )
      {
         if( c == 5 ) {
            v = 6;
         }
         else {
            TAO_PEGTL_TEST_ASSERT( c == 1 );
            v = 2;
         }
      }

      template< typename ParseInput >
      void success( const ParseInput& /*unused*/, int& c )
      {
         TAO_PEGTL_TEST_ASSERT( v == 3 );
         c = 4;
      }
   };

   template<>
   struct my_action< B >
      : change_state< S >
   {
      static void apply0( S& s )
      {
         TAO_PEGTL_TEST_ASSERT( s.v == 2 );
         s.v = 3;
      }
   };

   void unit_test()
   {
      {
         memory_input in( "ab", "" );
         int c = 0;
         const auto result = parse< AB, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( c == 4 );
      }
      {
         memory_input in( "a", "" );
         int c = 0;
         const auto result = parse< AB, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( c == 1 );
      }
      {
         memory_input in( "b", "" );
         int c = 0;
         const auto result = parse< AB, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( !result );
         TAO_PEGTL_TEST_ASSERT( c == 0 );
      }
      {
         memory_input in( "ab", "" );
         int c = 5;
         const auto result = parse< disable< AB >, my_action >( in, c );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( c == 5 );
      }
   }

}  // namespace tao::pegtl

#include "main.hpp"
