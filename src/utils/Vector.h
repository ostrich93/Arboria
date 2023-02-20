#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "math.h"

namespace Arboria {

	template<typename T>
	class Vector2 {
	public:
		Vector2();
		Vector2(const T _x, const T _y) { set(_x, _y); }

		//operators
		T& operator[](int index);
		T operator[](int index) const;
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-() const;
		Vector2 operator-(const Vector2& other) const;
		T operator*(const Vector2& other) const;
		Vector2 operator*(const T factor) const;
		Vector2 operator/(const T factor) const;
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(const T factor);
		Vector2& operator/=(const T factor);
		Vector2& operator/=(const Vector2& other);
		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		//getters and setters
		T getX() const;
		T getY() const;
		void setX(const T _x) { x = _x; }
		void setY(const T _y) { y = _y; }
		void set(const T _x, const T _y);

		void clamp(const Vector2& min, const Vector2& max);
		bool compare(const Vector2& other) const;
		//requires sqrt function, which optimized version uses intrinsics
		T length() const;
		T lengthSquared() const;
		void lerp(const Vector2& v1, const Vector2& v2, const float l);
		float normalize();
		Vector2 scale(const Vector2& other) const;
		Vector2 truncate(float length) const;
		static T dotProduct(Vector2 v1, Vector2 v2);

	protected:
		T x;
		T y;
	};

	template<typename T>
	inline Vector2<T>::Vector2() {}

	template<typename T>
	inline T& Vector2<T>::operator[](int index) {
		return (&x)[index];
	}

	template<typename T>
	inline T Vector2<T>::operator[](int index) const {
		return (&x)[index];
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::operator-() const {
		return Vector2(-x, -y);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	template<typename T>
	inline T Vector2<T>::operator*(const Vector2<T>& other) const {
		return x * other.x + y * other.y;
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::operator*(const T factor) const {
		return Vector2<T>(x * factor, y * factor);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::operator/(const T factor) const {
		return Vector2<T>(x / factor, y / factor);
	}

	template<typename T>
	inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	template<typename T>
	inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	template<typename T>
	inline Vector2<T>& Vector2<T>::operator*=(const T factor) {
		x *= factor;
		y *= factor;

		return *this;
	}

	template<typename T>
	inline Vector2<T>& Vector2<T>::operator/=(const T factor) {
		x /= factor;
		y /= factor;

		return *this;
	}

	template<typename T>
	inline Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& other) {
		x /= other.x;
		y / -other.y;

		return *this;
	}

	template<typename T>
	inline bool Vector2<T>::operator==(const Vector2<T>& other) const {
		return compare(other);
	}

	template<typename T>
	inline bool Vector2<T>::operator!=(const Vector2<T>& other) const {
		return !compare(other);
	}

	template<typename T>
	inline T Vector2<T>::getX() const {
		return x;
	}

	template<typename T>
	inline T Vector2<T>::getY() const {
		return y;
	}

	template<typename T>
	inline void Vector2<T>::set(const T _x, const T _y) {
		x = _x;
		y = _y;
	}

	//type specializations
	template<typename T>
	inline void Vector2<T>::clamp(const Vector2<T>& min, const Vector2<T>& max) {
		x = Math::clampFloat(min.x, max.x, x);
		y = Math::clampFloat(min.y, max.y, y);
	}

	template<>
	inline void Vector2<int>::clamp(const Vector2<int>& min, const Vector2<int>& max) {
		x = Math::clampInt(min.x, max.x, x);
		y = Math::clampInt(min.y, max.y, y);
	}

	template<>
	inline void Vector2<float>::clamp(const Vector2<float>& min, const Vector2<float>& max) {
		x = Math::clampFloat(min.x, max.x, x);
		y = Math::clampFloat(min.y, max.y, y);
	}

	template<typename T>
	inline bool Vector2<T>::compare(const Vector2<T>& other) const {
		return ((x == other.x) && (y == other.y));
	}

	template<typename T>
	inline T Vector2<T>::lengthSquared() const {
		return (x * x + y * y);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::scale(const Vector2<T>& other) const {
		return Vector2<T>(x * other.x, y * other.y);
	}

	template<typename T>
	class Vector3 {
	public:
		Vector3();
		Vector3(const T _x) { set(_x, _x, _x); }
		Vector3(const T _x, const T _y, const T _z) { set(_x, _y, _z); }

		T getX() const;
		T getY() const;
		T getZ() const;
		void setX(const T _x) { x = _x; }
		void setY(const T _y) { y = _y; }
		void setZ(const T _z) { z = _z; }
		void set(const T _x, const T _y, const T _z);

		//operators
		T& operator[](int index);
		T operator[](const int index) const;
		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-() const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator*(const T factor) const;
		Vector3 operator/(const T divisor) const;
		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(const Vector3& other);
		Vector3& operator*=(const T factor);
		Vector3& operator/=(const T divisor);
		Vector3& operator/=(const Vector3& other);
		Vector3& operator=(const Vector3& other);
		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		void clamp(const Vector3& min, const Vector3& max);
		bool compare(const Vector3& other) const;
		T length() const;
		T lengthSquared() const;
		void lerp(const Vector3& v1, const Vector3& v2, const float l);
		float normalize() const;
		void projectOntoPlane(const Vector3& normal, const float bounce = 1.0f);
		Vector3 truncate(int length) const;
		Vector2<T>& toVector2();
		float toYaw() const;
		float toPitch() const;

		Vector3 cross(const Vector3& a) const;
		Vector3& cross(const Vector3& a, const Vector3& b);

		static T dotProduct(Vector3& a, Vector3& b);
		static Vector3& crossProduct(Vector3& a, Vector3& b);

	protected:
		T x;
		T y;
		T z;
	};

	template<typename T>
	inline Vector3<T>::Vector3<T>() {}

	template<typename T>
	inline T Vector3<T>::getX() const {
		return x;
	}

	template<typename T>
	inline T Vector3<T>::getY() const {
		return y;
	}

	template<typename T>
	inline T Vector3<T>::getZ() const {
		return z;
	}

	template<typename T>
	inline void Vector3<T>::set(const T _x, const T _y, const T _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	template<typename T>
	inline T& Vector3<T>::operator[](int index) {
		return (&x)[index];
	}

	template<typename T>
	inline T Vector3<T>::operator[](const int index) const {
		return (&x)[index];
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator-() const {
		return Vector3(-x, -y, -z);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator*(const Vector3& other) const {
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator*(const T factor) const {
		return Vector3(x * factor, y * factor, z * factor);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator/(const T factor) const {
		return Vector3(x / factor, y / factor, z / factor);
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator*=(const Vector3& other) {
		x *= other.x;
		y *= other.y;

		return *this;
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator*=(const T factor) {
		x *= factor;
		y *= factor;

		return *this;
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator/=(const T factor) {
		x /= factor;
		y /= factor;

		return *this;
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator/=(const Vector3& other) {
		x /= other.x;
		y /= other.y;

		return *this;
	}

	template<typename T>
	inline Vector3<T>& Vector3<T>::operator=(const Vector3& other) {
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	template<typename T>
	inline bool Vector3<T>::operator==(const Vector3& other) const {
		return compare(other);
	}

	template<typename T>
	inline bool Vector3<T>::operator!=(const Vector3& other) const {
		return !compare(other);
	}

	template<typename T>
	inline void Vector3<T>::clamp(const Vector3& min, const Vector3& max) {
		x = Math::clampFloat(min.x, max.x, x);
		y = Math::clampFloat(min.y, max.y, y);
		z = Math::clampFloat(min.z, max.z, z);
	}

	template<>
	inline void Vector3<int>::clamp(const Vector3<int>& min, const Vector3<int>& max) {
		x = Math::clampInt(min.x, max.x, x);
		y = Math::clampInt(min.y, max.y, y);
		z = Math::clampInt(min.z, max.z, z);
	}

	template<>
	inline void Vector3<float>::clamp(const Vector3<float>& min, const Vector3<float>& max) {
		x = Math::clampFloat(min.x, max.x, x);
		y = Math::clampFloat(min.y, max.y, y);
		z = Math::clampFloat(min.z, max.z, z);
	}

	template<typename T>
	inline bool Vector3<T>::compare(const Vector3& other) const {
		return ((x == other.x) && (y == other.y) && (z == other.z));
	}

	template<typename T>
	inline T Vector3<T>::lengthSquared() const {
		return (x * x + y * y + z * z);
	}

	template<typename T>
	class Vector4 {
	public:
		Vector4();
		Vector4(const T _x) { set(_x, _x, _x, _x); }
		Vector4(const T _x, const T _y, const T _z, const T _w) { set(_x, _y, _z, _w); }

		T getX() const;
		T getY() const;
		T getZ() const;
		T getW() const;
		void setX(const T _x) { x = _x; }
		void setY(const T _y) { y = _y; }
		void setZ(const T _z) { z = _z; }
		void setW(const T _w) { w = _w; }
		void set(const T _x, const T _y, const T _z, const T _w);

		//operators
		T& operator[](int index);
		T operator[](const int index) const;
		Vector4 operator+(const int index) const;
		Vector4 operator-() const;
		Vector4 operator-(const Vector4& other) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator*(const T factor) const;
		Vector4 operator/(const T divisor) const;
		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator*=(const Vector4& other);
		Vector4& operator*=(const T factor);
		Vector4& operator/=(const Vector4& other);
		Vector4& operator/=(const T factor);
		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4 other) const;

		void clamp(const Vector4& min, const Vector4& max);
		bool compare(const Vector4& other) const;
		T length() const;
		T lengthSquared() const;
		void lerp(const Vector4& v1, const Vector4& v2, const float l);
		float normalize() const;
		Vector4 multiply(const Vector4& other) const;
		const Vector2<T>& toVector2() const;
		Vector2<T>& toVector2();
		const Vector3& toVector3() const;
		Vector3 toVector3();

	protected:
		T x;
		T y;
		T z;
		T w;
	};

	template<typename T>
	class Vector5 {
	public:
		Vector5();
		Vector5(const Vector3& xyz, const Vector2<T>& st);
		Vector5(const T _x, const T _y, const T _z, const T _s, const T _t);

		T getX() const;
		T getY() const;
		T getZ() const;
		T getS() const;
		T getT() const;
		void setX(const T _x) { x = _x; }
		void setY(const T _y) { y = _y; }
		void setZ(const T _z) { z = _z; }
		void setS(const T _s) { s = _s; }
		void setT(const T _t) { t = _t; }
		void set(const T _x, const T _y, const T _z, const T _s, const T _t);

		T operator[](int index) const;
		T& operator[](int index);
		Vector5& operator=(const Vector3& a);

		const Vector3& toVector3() const;
		Vector3& toVector3();
		void lerp(const Vector4& v1, const Vector4& v2, const float l);

	protected:
		T x;
		T y;
		T z;
		T s;
		T t;
	};

	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;
	typedef Vector3<int> Vector3i;
	typedef Vector3<float> Vector3f;
};

#endif