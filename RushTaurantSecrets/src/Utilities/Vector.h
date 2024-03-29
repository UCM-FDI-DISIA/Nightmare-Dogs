#pragma once
#include <math.h>
constexpr float PI = 3.14159265358979323846264338327950288f;

class Vector {
private:
	float x, y;
public:
	static const Vector zero, right, down, left, up;

	Vector(float x = 0, float y = 0) : x(x), y(y) { }
	Vector(Vector const& v) : x(v.x), y(v.y) { }

	inline float getX() const { return x; }
	inline float getY() const { return y; }

	inline void setX(float x) { this->x = x; }
	inline void setY(float y) { this->y = y; }

	inline Vector operator+(Vector const& v) const { return Vector(x + v.x, y + v.y); }
	inline Vector operator-() const { return Vector(-x, -y); }
	inline Vector operator-(Vector const& v) const { return *this + -v; }
	inline Vector operator*(float num) const { return Vector(x * num, y * num); }
	inline Vector operator/(float num) const { return Vector(x / num, y / num); }

	inline Vector& operator+=(Vector const& v) { return *this = *this + v; }
	inline Vector& operator-=(Vector const& v) { return *this = *this - v; }
	inline Vector& operator*=(float num) { return *this = *this * num; }
	inline Vector& operator/=(float num) { return *this = *this / num; }
	inline bool operator==(Vector const& v) { return x==v.x && y==v.y; }

	inline float magnitude() const { return sqrtf(powf(x, 2) + powf(y, 2)); }
	inline Vector getNormalized() const {
		float mag = magnitude();
		return mag == 0 ? Vector() : *this / mag;
	}
	inline void normalize() { *this = getNormalized(); }
	// Rotar en sentido horario
	inline Vector getRotated(float degrees) const {
		float rad = degrees * PI / 180;
		return Vector(x * cosf(rad) - y * sinf(rad), x * sinf(rad) + y * cosf(rad));
	}
	// Rotar en sentido horario
	inline void rotate(float degrees) { *this = getRotated(degrees); }
	inline float angle() { return atanf(y / x) * 180 / PI; }
	inline float dot(Vector const& v) { return v.x * x + v.y * y; }
};