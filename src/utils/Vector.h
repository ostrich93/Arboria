#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "math.h"

namespace Arboria {
	class Vector2 {
	public:
		Vector2();
		Vector2(const float _x, const float _y) { set(_x, _y); }

		//operators
		float& operator[](int index);
		float operator[](int index) const;
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-() const;
		Vector2 operator-(const Vector2& other) const;
		int operator*(const Vector2& other) const;
		Vector2 operator*(const float factor) const;
		Vector2 operator/(const float factor) const;
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(const float factor);
		Vector2& operator/=(const float factor);
		Vector2& operator/=(const Vector2& other);
		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		//getters and setters
		float getX() const;
		float getY() const;
		void setX(const float _x) { x = _x; }
		void setY(const float _y) { y = _y; }
		void set(const float _x, const float _y);
		void zero() { set(0.0f, 0.0f); }

		void clamp(const Vector2& min, const Vector2& max);
		bool compare(const Vector2& other) const;
		//requires sqrt function, which optimized version uses intrinsics
		float length() const;
		float lengthSquared() const;
		void lerp(const Vector2& v1, const Vector2& v2, const float l);
		float normalize();
		Vector2 scale(const Vector2& other) const;
		void snap();
		void snapInt();
		Vector2 truncate(float length) const;
		static float dotProduct(Vector2 v1, Vector2 v2);

	protected:
		float x;
		float y;
	};

	inline Vector2::Vector2() {}

	inline float& Vector2::operator[](int index) {
		return (&x)[index];
	}

	inline float Vector2::operator[](int index) const {
		return (&x)[index];
	}

	inline Vector2 Vector2::operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	inline Vector2 Vector2::operator-() const {
		return Vector2(-x, -y);
	}

	inline Vector2 Vector2::operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	inline int Vector2::operator*(const Vector2& other) const {
		return x * other.x + y * other.y;
	}

	inline Vector2 Vector2::operator*(const float factor) const {
		return Vector2(x * factor, y * factor);
	}

	inline Vector2 Vector2::operator/(const float factor) const {
		return Vector2(x / factor, y / factor);
	}

	inline Vector2& Vector2::operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	inline Vector2& Vector2::operator*=(const float factor) {
		x *= factor;
		y *= factor;

		return *this;
	}

	inline Vector2& Vector2::operator/=(const float factor) {
		x /= factor;
		y /= factor;

		return *this;
	}

	inline Vector2& Vector2::operator/=(const Vector2& other) {
		x /= other.x;
		y / -other.y;

		return *this;
	}

	inline bool Vector2::operator==(const Vector2& other) const {
		return compare(other);
	}

	inline bool Vector2::operator!=(const Vector2& other) const {
		return !compare(other);
	}

	inline float Vector2::getX() const {
		return x;
	}

	inline float Vector2::getY() const {
		return y;
	}

	inline void Vector2::set(const float _x, const float _y) {
		x = _x;
		y = _y;
	}

	inline void Vector2::clamp(const Vector2& min, const Vector2& max) {
		x = Math::clampFloat(min.x, max.x, x);
		y = Math::clampFloat(min.y, max.y, y);
	}

	inline bool Vector2::compare(const Vector2& other) const {
		return ((x == other.x) && (y == other.y));
	}

	inline float Vector2::lengthSquared() const {
		return (x * x + y * y);
	}

	inline Vector2 Vector2::scale(const Vector2& other) const {
		return Vector2(x * other.x, y * other.y);
	}

	inline void Vector2::snap() {
		x = Math::floor(x + 0.5f);
		y = Math::floor(y + 0.5f);
	}

	inline void Vector2::snapInt() {
		x = float(int(x));
		y = float(int(y));
	}

	class Vector3 {
	public:
		Vector3();
		Vector3(const float _x) { set(_x, _x, _x); }
		Vector3(const float _x, const float _y, const float _z) { set(_x, _y, _z); }

		float getX() const;
		float getY() const;
		float getZ() const;
		void setX(const float _x) { x = _x; }
		void setY(const float _y) { y = _y; }
		void setZ(const float _z) { z = _z; }
		void set(const float _x, const float _y, const float _z);
		void zero() { set(0.0f, 0.0f, 0.0f); }

		//operators
		float& operator[](int index);
		float operator[](const int index) const;
		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-() const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator*(const float factor) const;
		Vector3 operator/(const float divisor) const;
		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(const Vector3& other);
		Vector3& operator*=(const float factor);
		Vector3& operator/=(const float divisor);
		Vector3& operator/=(const Vector3& other);
		Vector3& operator=(const Vector3& other);
		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		void clamp(const Vector3& min, const Vector3& max);
		bool compare(const Vector3& other) const;
		float length() const;
		float lengthSquared() const;
		void lerp(const Vector3& v1, const Vector3& v2, const float l);
		float normalize() const;
		void projectOntoPlane(const Vector3& normal, const float bounce = 1.0f);
		void snap();
		void snapInt();
		Vector3 truncate(int length) const;
		Vector2& toVector2();
		float toYaw() const;
		float toPitch() const;

		Vector3 cross(const Vector3& a) const;
		Vector3& cross(const Vector3& a, const Vector3& b);

		static int dotProduct(Vector3& a, Vector3& b);
		static Vector3& crossProduct(Vector3& a, Vector3& b);

	protected:
		float x;
		float y;
		float z;
	};

	inline Vector3::Vector3() {}

	inline float Vector3::getX() const {
		return x;
	}

	inline float Vector3::getY() const {
		return y;
	}

	inline float Vector3::getZ() const {
		return z;
	}

	inline void Vector3::set(const float _x, const float _y, const float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	inline float& Vector3::operator[](int index) {
		return (&x)[index];
	}

	inline float Vector3::operator[](const int index) const {
		return (&x)[index];
	}

	inline Vector3 Vector3::operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	inline Vector3 Vector3::operator-() const {
		return Vector3(-x, -y, -z);
	}

	inline Vector3 Vector3::operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	inline Vector3 Vector3::operator*(const Vector3& other) const {
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	inline Vector3 Vector3::operator*(const float factor) const {
		return Vector3(x * factor, y * factor, z * factor);
	}

	inline Vector3 Vector3::operator/(const float factor) const {
		return Vector3(x / factor, y / factor, z / factor);
	}

	inline Vector3& Vector3::operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	inline Vector3& Vector3::operator*=(const Vector3& other) {
		x *= other.x;
		y *= other.y;

		return *this;
	}

	inline Vector3& Vector3::operator*=(const float factor) {
		x *= factor;
		y *= factor;

		return *this;
	}

	inline Vector3& Vector3::operator/=(const float factor) {
		x /= factor;
		y /= factor;

		return *this;
	}

	inline Vector3& Vector3::operator/=(const Vector3& other) {
		x /= other.x;
		y /= other.y;

		return *this;
	}

	inline Vector3& Vector3::operator=(const Vector3& other) {
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	inline bool Vector3::operator==(const Vector3& other) const {
		return compare(other);
	}

	inline bool Vector3::operator!=(const Vector3& other) const {
		return !compare(other);
	}

	inline void Vector3::clamp(const Vector3& min, const Vector3& max) {
		x = Math::clampFloat(min.x, max.x, x);
		y = Math::clampFloat(min.y, max.y, y);
		z = Math::clampFloat(min.z, max.z, z);
	}

	inline bool Vector3::compare(const Vector3& other) const {
		return ((x == other.x) && (y == other.y) && (z == other.z));
	}

	inline float Vector3::lengthSquared() const {
		return (x * x + y * y + z * z);
	}

	inline void Vector3::snap() {
		x = Math::floor(x + 0.5f);
		y = Math::floor(y + 0.5f);
		z = Math::floor(z + 0.5f);
	}

	inline void Vector3::snapInt() {
		x = float(int(x));
		y = float(int(y));
		z = float(int(z));
	}

	class Vector4 {
	public:
		Vector4();
		Vector4(const float _x) { set(_x, _x, _x, _x); }
		Vector4(const float _x, const float _y, const float _z, const float _w) { set(_x, _y, _z, _w); }

		float getX() const;
		float getY() const;
		float getZ() const;
		float getW() const;
		void setX(const float _x) { x = _x; }
		void setY(const float _y) { y = _y; }
		void setZ(const float _z) { z = _z; }
		void setW(const float _w) { w = _w; }
		void set(const float _x, const float _y, const float _z, const float _w);
		void zero() { set(0.0f, 0.0f, 0.0f, 0.0f); }

		//operators
		float& operator[](int index);
		float operator[](const int index) const;
		Vector4 operator+(const int index) const;
		Vector4 operator-() const;
		Vector4 operator-(const Vector4& other) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator*(const float factor) const;
		Vector4 operator/(const float divisor) const;
		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator*=(const Vector4& other);
		Vector4& operator*=(const float factor);
		Vector4& operator/=(const Vector4& other);
		Vector4& operator/=(const float factor);
		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4 other) const;

		void clamp(const Vector4& min, const Vector4& max);
		bool compare(const Vector4& other) const;
		float length() const;
		float lengthSquared() const;
		void lerp(const Vector4& v1, const Vector4& v2, const float l);
		float normalize() const;
		Vector4 multiply(const Vector4& other) const;
		const Vector2& toVector2() const;
		Vector2& toVector2();
		const Vector3& toVector3() const;
		Vector3 toVector3();

	protected:
		float x;
		float y;
		float z;
		float w;
	};

	class Vector5 {
	public:
		Vector5();
		Vector5(const Vector3& xyz, const Vector2& st);
		Vector5(const float _x, const float _y, const float _z, const float _s, const float _t);

		float getX() const;
		float getY() const;
		float getZ() const;
		float getS() const;
		float getT() const;
		void setX(const float _x) { x = _x; }
		void setY(const float _y) { y = _y; }
		void setZ(const float _z) { z = _z; }
		void setS(const float _s) { s = _s; }
		void setT(const float _t) { t = _t; }
		void set(const float _x, const float _y, const float _z, const float _s, const float _t);
		void zero() { set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }

		int operator[](int index) const;
		int& operator[](int index);
		Vector5& operator=(const Vector3& a);

		const Vector3& toVector3() const;
		Vector3& toVector3();
		void lerp(const Vector4& v1, const Vector4& v2, const float l);

	protected:
		float x;
		float y;
		float z;
		float s;
		float t;
	};
}

#endif