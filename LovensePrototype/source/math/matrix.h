#pragma once

#include "Vector.h"

namespace math {
	struct matrix3 final {
		float data[ 9 ];

		/////////////////////////////////////////////////////////

		__forceinline static matrix3 lerp( const matrix3& start, const matrix3& end, float t ) noexcept {
			t = std::clamp( t, 0.0f, 1.0f );

			matrix3 result;

			for ( int i = 0; i < 9; ++i )
				result.data[ i ] = start.data[ i ] + ( end.data[ i ] - start.data[ i ] ) * t;

			return result;
		}

		__forceinline matrix3 lerp( const matrix3& target, float t ) const noexcept {
			return lerp( *this, target, t );
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 column( int index ) const noexcept {
			return vec3{ data[ index ], data[ index + 3 ], data[ index + 6 ] };
		}

		__forceinline vec3 row( int index ) const noexcept {
			return vec3{ data[ index ], data[ index + 1 ], data[ index + 2 ] };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 forward_Vector() const noexcept {
			return row( 6 );
		}

		__forceinline vec3 look_Vector() const noexcept {
			return column( 2 );
		}

		__forceinline vec3 right_Vector() const noexcept {
			return column( 0 );
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 operator*( const vec3& l ) {
			return vec3{
				data[ 0 ] * l.x + data[ 3 ] * l.y + data[ 6 ] * l.z,
				data[ 1 ] * l.x + data[ 4 ] * l.y + data[ 7 ] * l.z,
				data[ 2 ] * l.x + data[ 5 ] * l.y + data[ 8 ] * l.z
			};
		}

		__forceinline vec3 operator*( vec3 l ) {
			return vec3{
				data[ 0 ] * l.x + data[ 3 ] * l.y + data[ 6 ] * l.z,
				data[ 1 ] * l.x + data[ 4 ] * l.y + data[ 7 ] * l.z,
				data[ 2 ] * l.x + data[ 5 ] * l.y + data[ 8 ] * l.z
			};
		}

		/////////////////////////////////////////////////////////

		__forceinline float operator[]( int index ) {
			return ( float )data[ index ];
		}

		__forceinline const float operator[]( const int& index ) {
			return ( const float )data[ index ];
		}

		/////////////////////////////////////////////////////////
	};

	struct matrix4 final {
		float data[ 16 ];

		/////////////////////////////////////////////////////////

		__forceinline static matrix4 lerp( const matrix4& start, const matrix4& end, float t ) noexcept {
			t = std::clamp( t, 0.0f, 1.0f );

			matrix4 result;

			for ( int i = 0; i < 16; ++i )
				result.data[ i ] = start.data[ i ] + ( end.data[ i ] - start.data[ i ] ) * t;

			return result;
		}

		__forceinline matrix4 lerp( const matrix4& target, float t ) const noexcept {
			return lerp( *this, target, t );
		}

		/////////////////////////////////////////////////////////

		__forceinline Vector4 column( const int& idx ) const noexcept {
			return Vector4{ data[ idx ], data[ idx + 3 ], data[ idx + 6 ], data[ idx + 9 ] };
		}

		__forceinline Vector4 row( const int& idx ) const noexcept {
			return Vector4{ data[ idx ], data[ idx + 1 ], data[ idx + 2 ], data[ idx + 3 ] };
		}

		/////////////////////////////////////////////////////////

		Vector4 operator*( const vec3& l ) const noexcept {
			return Vector4{
			  data[ 0 ] * l.x + data[ 1 ] * l.y + data[ 2 ] * l.z + data[ 3 ],
			  data[ 4 ] * l.x + data[ 5 ] * l.y + data[ 6 ] * l.z + data[ 7 ],
			  data[ 8 ] * l.x + data[ 9 ] * l.y + data[ 10 ] * l.z + data[ 11 ],
			  data[ 12 ] * l.x + data[ 13 ] * l.y + data[ 14 ] * l.z + data[ 15 ]
			};
		}
	};
}