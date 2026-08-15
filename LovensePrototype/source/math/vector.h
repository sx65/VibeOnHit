#pragma once

#include <cmath>
#include <algorithm>

namespace math {
	struct vec2 final {
		float x, y;

		vec2() : x( 0.0f ), y( 0.0f ) {}
		vec2( float x, float y ) : x( x ), y( y ) {}

		__forceinline float length() const noexcept {
			return sqrt( x * x + y * y );
		}

		__forceinline vec2 normalize() const noexcept {
			float len = length();

			if ( len == 0 )
				return { 0, 0 };

			return { x / len, y / len };
		}

		__forceinline float dot( const vec2& l ) const noexcept {
			return x * l.x + y * l.y;
		}

		__forceinline static vec2 lerp( const vec2& start, const vec2& end, float t ) noexcept {
			t = std::clamp( t, 0.0f, 1.0f );

			return {
				start.x + ( end.x - start.x ) * t,
				start.y + ( end.y - start.y ) * t
			};
		}

		__forceinline vec2 lerp( const vec2& target, float t ) noexcept {
			return lerp( *this, target, t );
		}
		
		__forceinline float magnitude(const vec2& target) const noexcept {
			float _x = x - target.x;
			float _y = y - target.y;

			return sqrt(_x * _x + _y * _y);
		}

		/////////////////////////////////////////////////////////

		__forceinline bool empty() const noexcept {
			return x == 0.0f && y == 0.0f;
		}

		/////////////////////////////////////////////////////////

		__forceinline bool operator==( const vec2& l ) const noexcept {
			return x == l.x && y == l.y;
		}

		__forceinline bool operator==( vec2& l ) const noexcept {
			return x == l.x && y == l.y;
		}

		__forceinline bool operator==( float l ) const noexcept {
			return x == l && y == l;
		}

		/////////////////////////////////////////////////////////

		__forceinline vec2 operator*( const vec2& l ) const noexcept {
			return vec2{ x * l.x, y * l.y };
		}

		__forceinline vec2 operator*( vec2& l ) const noexcept {
			return vec2{ x * l.x, y * l.y };
		}

		__forceinline vec2 operator*( float l ) const noexcept {
			return vec2{ x * l, y * l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec2 operator/( const vec2& l ) const noexcept {
			return vec2{ x / l.x, y / l.y };
		}

		__forceinline vec2 operator/( vec2& l ) const noexcept {
			return vec2{ x / l.x, y / l.y };
		}

		__forceinline vec2 operator/( float l ) const noexcept {
			return vec2{ x / l, y / l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec2 operator-( const vec2& l ) const noexcept {
			return vec2{ x - l.x, y - l.y };
		}

		__forceinline vec2 operator-( vec2& l ) const noexcept {
			return vec2{ x - l.x, y - l.y };
		}

		__forceinline vec2 operator-( float l ) const noexcept {
			return vec2{ x - l, y - l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec2 operator+( const vec2& l ) const noexcept {
			return vec2{ x + l.x, y + l.y };
		}

		__forceinline vec2 operator+( vec2& l ) const noexcept {
			return vec2{ x + l.x, y + l.y };
		}

		__forceinline vec2 operator+( float l ) const noexcept {
			return vec2{ x + l, y + l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec2 operator-() const noexcept {
			return { -x, -y };
		}

		/////////////////////////////////////////////////////////

		__forceinline const float& operator[]( int i ) const noexcept {
			return ( ( float* )this )[ i ];
		}

		__forceinline float& operator[]( int i ) noexcept {
			return ( ( float* )this )[ i ];
		}
	};

	struct vec3 final {
		float x, y, z;

		vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
		vec3( float x, float y, float z ) : x( x ), y( y ), z( z ) {}

		__forceinline float length() const noexcept {
			return sqrt( x * x + y * y + z * z );
		}

		__forceinline vec3 normalize() const noexcept {
			float len = length();

			if ( len == 0 )
				return { 0, 0, 0 };

			return { x / len, y / len, z / len };
		}

		__forceinline float dot( const vec3& l ) const noexcept {
			return x * l.x + y * l.y + z * l.z;
		}

		__forceinline static vec3 lerp( const vec3& start, const vec3& end, float t ) noexcept {
			t = std::clamp( t, 0.0f, 1.0f );

			return {
				start.x + ( end.x - start.x ) * t,
				start.y + ( end.y - start.y ) * t,
				start.z + ( end.z - start.z ) * t
			};
		}

		__forceinline vec3 lerp( const vec3& target, float t ) noexcept {
			return lerp( *this, target, t );
		}

		__forceinline vec3 cross_product( const vec3& l ) const noexcept {
			return {
				y * l.z - z * l.y,
				z * l.x - x * l.z,
				x * l.y - y * l.x
			};
		}

		/////////////////////////////////////////////////////////

		__forceinline bool empty() const noexcept {
			return x == 0.0f && y == 0.0f && z == 0.0f;
		}

		/////////////////////////////////////////////////////////

		__forceinline bool operator==( const vec3& l ) const noexcept {
			return x == l.x && y == l.y && z == l.z;
		}

		__forceinline bool operator==( vec3& l ) const noexcept {
			return x == l.x && y == l.y && z == l.z;
		}

		__forceinline bool operator==( float l ) const noexcept {
			return x == l && y == l && z == l;
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 operator*( const vec3& l ) const noexcept {
			return vec3{ x * l.x, y * l.y, z * l.z };
		}

		__forceinline vec3 operator*( vec3& l ) const noexcept {
			return vec3{ x * l.x, y * l.y, z * l.z };
		}

		__forceinline vec3 operator*( float l ) const noexcept {
			return vec3{ x * l, y * l, z * l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 operator/( const vec3& l ) const noexcept {
			return vec3{ x / l.x, y / l.y, z / l.z };
		}

		__forceinline vec3 operator/( vec3& l ) const noexcept {
			return vec3{ x / l.x, y / l.y, z / l.z };
		}

		__forceinline vec3 operator/( float l ) const noexcept {
			return vec3{ x / l, y / l, z / l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 operator-( const vec3& l ) const noexcept {
			return vec3{ x - l.x, y - l.y, z - l.z };
		}

		__forceinline vec3 operator-( vec3& l ) const noexcept {
			return vec3{ x - l.x, y - l.y, z - l.z };
		}

		__forceinline vec3 operator-( float l ) const noexcept {
			return vec3{ x - l, y - l, z - l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 operator+( const vec3& l ) const noexcept {
			return vec3{ x + l.x, y + l.y, z + l.z };
		}

		__forceinline vec3 operator+( vec3& l ) const noexcept {
			return vec3{ x + l.x, y + l.y, z + l.z };
		}

		__forceinline vec3 operator+( float l ) const noexcept {
			return vec3{ x + l, y + l, z + l };
		}

		/////////////////////////////////////////////////////////

		__forceinline vec3 operator-() const noexcept {
			return { -x, -y, -z };
		}

		/////////////////////////////////////////////////////////

		__forceinline const float& operator[]( int i ) const noexcept {
			return ( ( float* )this )[ i ];
		}

		__forceinline float& operator[]( int i ) noexcept {
			return ( ( float* )this )[ i ];
		}
	};

	struct Vector4 final {
		float x, y, z, w;

		Vector4() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) {}
		Vector4( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ) {}

		__forceinline float length() const noexcept {
			return sqrt( x * x + y * y + z * z + w * w );
		}

		__forceinline Vector4 normalize() const noexcept {
			float len = length();

			if ( len == 0 )
				return { 0, 0, 0, 0 };

			return { x / len, y / len, z / len, w / len };
		}

		__forceinline float dot( const Vector4& l ) const noexcept {
			return x * l.x + y * l.y + z * l.z + w * l.w;
		}

		__forceinline static Vector4 lerp( const Vector4& start, const Vector4& end, float t ) noexcept {
			t = std::clamp( t, 0.0f, 1.0f );

			return {
				start.x + ( end.x - start.x ) * t,
				start.y + ( end.y - start.y ) * t,
				start.z + ( end.z - start.z ) * t,
				start.w + ( end.w - start.w ) * t
			};
		}

		__forceinline Vector4 lerp( const Vector4& target, float t ) noexcept {
			return lerp( *this, target, t );
		}

		/////////////////////////////////////////////////////////

		__forceinline bool empty() const noexcept {
			return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
		}

		/////////////////////////////////////////////////////////

		__forceinline bool operator==( const Vector4& l ) const noexcept {
			return x == l.x && y == l.y && z == l.z && w == l.w;
		}

		__forceinline bool operator==( Vector4& l ) const noexcept {
			return x == l.x && y == l.y && z == l.z && w == l.w;
		}

		__forceinline bool operator==( float l ) const noexcept {
			return x == l && y == l && z == l && w == l;
		}

		/////////////////////////////////////////////////////////

		__forceinline Vector4 operator*( const Vector4& l ) const noexcept {
			return Vector4{ x * l.x, y * l.y, z * l.z, w * l.w };
		}

		__forceinline Vector4 operator*( Vector4& l ) const noexcept {
			return Vector4{ x * l.x, y * l.y, z * l.z, w * l.w };
		}

		__forceinline Vector4 operator*( float l ) const noexcept {
			return Vector4{ x * l, y * l, z * l, w * l };
		}

		/////////////////////////////////////////////////////////

		__forceinline Vector4 operator/( const Vector4& l ) const noexcept {
			return Vector4{ x / l.x, y / l.y, z / l.z, w / l.w };
		}

		__forceinline Vector4 operator/( Vector4& l ) const noexcept {
			return Vector4{ x / l.x, y / l.y, z / l.z, w / l.w };
		}

		__forceinline Vector4 operator/( float l ) const noexcept {
			return Vector4{ x / l, y / l, z / l, w / l };
		}

		/////////////////////////////////////////////////////////

		__forceinline Vector4 operator-( const Vector4& l ) const noexcept {
			return Vector4{ x - l.x, y - l.y, z - l.z, w - l.w };
		}

		__forceinline Vector4 operator-( Vector4& l ) const noexcept {
			return Vector4{ x - l.x, y - l.y, z - l.z, w - l.w };
		}

		__forceinline Vector4 operator-( float l ) const noexcept {
			return Vector4{ x - l, y - l, z - l, w - l };
		}

		/////////////////////////////////////////////////////////

		__forceinline Vector4 operator+( const Vector4& l ) const noexcept {
			return Vector4{ x + l.x, y + l.y, z + l.z, w + l.w };
		}

		__forceinline Vector4 operator+( Vector4& l ) const noexcept {
			return Vector4{ x + l.x, y + l.y, z + l.z, w + l.w };
		}

		__forceinline Vector4 operator+( float l ) const noexcept {
			return Vector4{ x + l, y + l, z + l, w + l };
		}

		/////////////////////////////////////////////////////////

		__forceinline Vector4 operator-() const noexcept {
			return { -x, -y, -z, -w };
		}

		/////////////////////////////////////////////////////////

		__forceinline const float& operator[]( int i ) const noexcept {
			return ( ( float* )this )[ i ];
		}

		__forceinline float& operator[]( int i ) noexcept {
			return ( ( float* )this )[ i ];
		}
	};
}